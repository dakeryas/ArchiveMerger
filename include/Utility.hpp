#ifndef UTILITY_UTILITY_H
#define UTILITY_UTILITY_H

#include "cereal/archives/binary.hpp"

namespace Utility{
  
  template<class InputIterator, class OutputIterator>
  void naiveCopy(InputIterator first, InputIterator last, OutputIterator result){
    
    while(first!=last){
      
      *result = *first;
      ++first;
      
    }
    
  }
  
}

#endif