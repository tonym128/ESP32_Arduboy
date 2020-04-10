#pragma once

#include <stddef.h>
#include <Print.h>


// Extract individual digits of a uint8_t -------------------------------------

template< size_t size > void extractDigits(uint8_t (&buffer)[size], uint8_t value) {

  for(uint8_t i = 0; i < size; ++i) {
    buffer[i] = value % 10;
    value /= 10;
  }

}

// Extract individual digits of a uint16_t
template< size_t size > void extractDigits(uint8_t (&buffer)[size], uint16_t value) {

  for(uint8_t i = 0; i < size; ++i) {
    buffer[i] = value % 10;
    value /= 10;
  }
  
}


// ----------------------------------------------------------------------------
//  A better absolute as it uses less memory than the standard one .. 
//
template<typename T> T absT(const T & v) {
  
  return (v < 0) ? -v : v;

}


// ----------------------------------------------------------------------------
// This gets the length of an array at compile time
//
template< typename T, size_t N > constexpr size_t ArrayLength(T (&)[N]) {
	return N;
}


