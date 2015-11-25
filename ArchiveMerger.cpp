#include <regex>
#include <fstream>
#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"
#include "InputIterator.hpp"
#include "OutputIterator.hpp"
#include "Cosmogenic/CandidateTree.hpp"
#include "Utility.hpp"

namespace bpo = boost::program_options;
namespace CsHt = CosmogenicHunter;

namespace ArchiveMerger{
  
  template <class T, class K>
  void merge(const boost::filesystem::path& targetPath, const std::regex& regex, const boost::filesystem::path& outputPath){

    std::ofstream outputStream(outputPath.string(), std::ios::binary);
    cereal::BinaryOutputArchive outputArchive(outputStream);
    
    cereal::Archive::OutputIterator<CsHt::CandidateTree<T,K>> outputIterator(outputArchive);
    
    for(auto directoryIterator = boost::filesystem::directory_iterator{targetPath}; directoryIterator != boost::filesystem::directory_iterator{}; ++directoryIterator){
    
      if(std::regex_search(directoryIterator->path().string(), regex)){
	
	std::ifstream inputStream(directoryIterator->path().string(), std::ios::binary);
	cereal::BinaryInputArchive inputArchive(inputStream);
	
	Utility::naiveCopy(cereal::Archive::InputIterator<CsHt::CandidateTree<T,K>>{inputArchive}, cereal::Archive::InputIterator<CsHt::CandidateTree<T,K>>{}, outputIterator);

      }
      
    }
    
  }
  
}

int main(int argc, char* argv[]){
 
  boost::filesystem::path targetPath, outputPath;
  std::string regexString;
  
  bpo::options_description optionDescription("ArchiveMerger usage");
  optionDescription.add_options()
  ("help,h", "Display this help message")
  ("target,t", bpo::value<boost::filesystem::path>(&targetPath)->required(), "Path of the directory containing the archives to merge")
  ("regex,r", bpo::value<std::string>(&regexString)->default_value(".*\\.bin"), "Regular expression to select files in 'target'")
  ("output,o", bpo::value<boost::filesystem::path>(&outputPath)->required(), "Output file where to save the archive");

  bpo::positional_options_description positionalOptions;//to use arguments without "--"
  positionalOptions.add("target", 1);
  
  bpo::variables_map arguments;
  try{
    
    bpo::store(bpo::command_line_parser(argc, argv).options(optionDescription).positional(positionalOptions).run(), arguments);
    
    if(arguments.count("help") || argc == 1){
      
      std::cout<<optionDescription<<std::endl;
      return 0;
      
    }
      
    bpo::notify(arguments);//the arguments are ready to be used
    
  }
  catch(bpo::error& e){
    
    std::cout<<e.what()<<std::endl;
    return 1;
    
  }
  
  if(!boost::filesystem::is_directory(targetPath)){
    
    std::cout<<"Error: "<<targetPath<<" is not a directory"<<std::endl;
    return 1;
    
  }
  else{
    
    ArchiveMerger::merge<float, float>(targetPath, std::regex(regexString), outputPath);
    
  }
  
}
