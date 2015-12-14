#ifndef ARCHIVE_MERGER_MERGER_H
#define ARCHIVE_MERGER_MERGER_H

#include <regex>
#include <fstream>
#include "boost/filesystem.hpp"
#include "InputIterator.hpp"
#include "OutputIterator.hpp"
#include "Utility.hpp"

namespace ArchiveMerger{
  
  template <class DataType>
  class Merger{
    
    std::regex regex;//match files from target
    bool clean;//remove matched files after merge
    boost::filesystem::path outputPath;
    
    void appendFile(const boost::filesystem::path& filePath, cereal::Archive::OutputIterator<DataType> outputIterator);
    void processFile(const boost::filesystem::path& filePath, cereal::Archive::OutputIterator<DataType> outputIterator);
    
  public:
    Merger(std::regex regex, bool clean);
    void mergeMatchingFiles(const boost::filesystem::path& targetPath, const boost::filesystem::path& outputPath);
    
  };
  
  template <class DataType>
  void Merger<DataType>::appendFile(const boost::filesystem::path& filePath, cereal::Archive::OutputIterator<DataType> outputIterator){
    
    std::ifstream inputStream(filePath.string(), std::ios::binary);
    cereal::BinaryInputArchive inputArchive(inputStream);
    Utility::naiveCopy(cereal::Archive::InputIterator<DataType>{inputArchive}, cereal::Archive::InputIterator<DataType>{}, outputIterator);
    
  }
  
  template <class DataType>
  void Merger<DataType>::processFile(const boost::filesystem::path& filePath, cereal::Archive::OutputIterator<DataType> outputIterator){
    
    if(!boost::filesystem::equivalent(filePath, outputPath) && std::regex_search(filePath.filename().string(), regex)){

      appendFile(filePath, outputIterator);
      if(clean) boost::filesystem::remove(filePath);

    }
    
  }
  
  template <class DataType>
  Merger<DataType>::Merger(std::regex regex, bool clean):regex(regex),clean(clean){
    
  }
  
  template <class DataType>
  void Merger<DataType>::mergeMatchingFiles(const boost::filesystem::path& targetPath, const boost::filesystem::path& outputPath){

    this->outputPath = outputPath;
    std::ofstream outputStream(outputPath.string(), std::ios::binary);
    cereal::BinaryOutputArchive outputArchive(outputStream);
    
    cereal::Archive::OutputIterator<DataType> outputIterator(outputArchive);
    std::for_each(boost::filesystem::directory_iterator{targetPath}, boost::filesystem::directory_iterator{}, [&](const auto& filePath){processFile(filePath, outputIterator);});
    
  }
  
}

#endif