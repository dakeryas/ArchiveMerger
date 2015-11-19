#ifndef OUTPUT_ITERATOR_H
#define OUTPUT_ITERATOR_H

#include "cereal/archives/binary.hpp"

namespace cereal{
  
  namespace Archive{

    template <class T>
    class OutputIterator : public std::iterator<std::output_iterator_tag, void, void, void, void>{
      
      BinaryOutputArchive* outputArchive;

    public:
      OutputIterator(BinaryOutputArchive& outputArchive);
      OutputIterator<T>& operator*();//no operation but allows to write *it = value
      OutputIterator<T>& operator=(const T& value);
      OutputIterator<T>& operator++();//no operation but allows to modify the stream with *++it = value
      OutputIterator<T>& operator++(int);//no operation but allows to write *it++ = value
      bool isEqualTo(const OutputIterator<T>& other) const;
      
    };
    
    template<class T>
    OutputIterator<T>::OutputIterator(BinaryOutputArchive& outputArchive):outputArchive(&outputArchive){
      
      ++*this;

    }
    
    template<class T>
    OutputIterator<T>& OutputIterator<T>::operator*(){

      return *this;
      
    }
    
    template<class T>
    OutputIterator<T>& OutputIterator<T>::operator=(const T& value){

      (*outputArchive)(value);
      return *this;
      
    }
    
    template<class T>
    OutputIterator<T>& OutputIterator<T>::operator++(){

      return *this;
      
    }
    
    template<class T>
    OutputIterator<T>& OutputIterator<T>::operator++(int){
      
      return *this;
      
    }

    template<class T>
    bool OutputIterator<T>::isEqualTo(const OutputIterator<T>& other) const{

      return outputArchive == other.outputArchive;
      
    }

    template<class T>
    bool operator==(const OutputIterator<T>& iterator1, const OutputIterator<T>& iterator2){

      return iterator1.isEqualTo(iterator2);
      
    }
    
    template<class T>
    bool operator!=(const OutputIterator<T>& iterator1, const OutputIterator<T>& iterator2){

      return !(iterator1 == iterator2);
      
    }
    
  }

}

#endif