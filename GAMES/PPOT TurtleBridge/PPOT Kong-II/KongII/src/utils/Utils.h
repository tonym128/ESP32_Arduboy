#pragma once

#include <stddef.h>
#include <Print.h>

// ----------------------------------------------------------------------------
//  Extract the size of an array .. 
//
template< typename T, size_t size > constexpr size_t getSize(T(&)[size]) {

  return size;

}

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
//  Swap two elements .. 
//
template <typename T> void swap(T& x,T& y) {
	T temp;
	temp = x;
	x = y;
	y = temp;
}


// ----------------------------------------------------------------------------
//  A better absolute as it uses less memory than the standard one .. 
//
template<typename T> T absT(const T & v) {
  
  return (v < 0) ? -v : v;

}


// ----------------------------------------------------------------------------
//  Get the difference between two values as a positive nubmer .. 
//
template<typename T> T Diff(const T &x, const T &y) {
  
  return (x < y) ? y - x : x - y;

}


// ----------------------------------------------------------------------------
//  Clamp the value within the range .. 
//
template <typename T> inline T clamp(const T& value, const T& low, const T& high) {

  return (value < low) ? low : (value > high) ? high : value; 

}


// ----------------------------------------------------------------------------
//  Wrap the range .. 
//
template <typename T> inline T wrapInc(const T& value, const T& low, const T& high) {

  return (value >= high) ? low : value + 1; 

}

template <typename T> inline T wrapDec(const T& value, const T& low, const T& high) {

  return (value > low) ? value - 1 : high; 

}

template <typename T> inline void decToZero(T& value) {

  if (value > 0) value--; 

}

template <typename T> inline void incToLimit(T& value, const T& high) {

  if (value < high) value++; 

}
