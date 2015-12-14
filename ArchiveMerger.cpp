#include "boost/program_options.hpp"
#include "Merger.hpp"
#include "Cosmogenic/CandidateTree.hpp"

namespace bpo = boost::program_options;

namespace ArchiveMerger{
  
  template <class T, class K>
  void merge(const boost::filesystem::path& targetPath, const std::regex& regex, const boost::filesystem::path& outputPath, bool deleteMergedFiles){

    Merger<CosmogenicHunter::CandidateTree<T,K>> candidateTreeMerger(regex, deleteMergedFiles);
    candidateTreeMerger.mergeMatchingFiles(targetPath, outputPath);
    
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
  ("output,o", bpo::value<boost::filesystem::path>(&outputPath)->required(), "Output file where to save the archive")
  ("clean", "Remove the merged files");

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
    
    ArchiveMerger::merge<float, float>(targetPath, std::regex(regexString), outputPath, arguments.count("clean"));
    
  }
  
}
