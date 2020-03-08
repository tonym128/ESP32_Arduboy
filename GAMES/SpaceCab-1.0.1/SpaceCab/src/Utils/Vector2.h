#pragma once

#include <FixedPoints.h>
#include <FixedPointsCommon.h>

class Vector2 {
	
  public:

    SQ7x8 x = 0;
    SQ7x8 y = 0;
    
  public:

    constexpr Vector2(void) = default;
    constexpr Vector2(SQ7x8 x, SQ7x8 y) : x(x), y(y) {}

};

inline constexpr Vector2 operator+(Vector2 left, Vector2 right) {
	return Vector2(left.x + right.x, left.y + right.y);
}

inline constexpr Vector2 operator-(Vector2 left, Vector2 right) {
	return Vector2(left.x - right.x, left.y - right.y);
}