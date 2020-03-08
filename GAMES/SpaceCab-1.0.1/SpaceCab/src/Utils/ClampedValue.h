#pragma once

template<typename T, T Min, T Max>

class ClampedValue
{
  public:
    static_assert(Min < Max, "Min must be less than Max");

  public:
    using ValueType = T;

  public:
    static constexpr ValueType MinValue = Min;
    static constexpr ValueType MaxValue = Max;

  private:
    ValueType value = MinValue;

  public:
    ClampedValue(void) = default;
    ClampedValue(uint8_t value) : value(value < Min ? Min : value > Max ? Max : value) {}
	
    operator T(void) const {
      return this->value;
    }

    ClampedValue & operator++(void) {
      if(this->value < MaxValue)
        ++this->value;
      return *this;
    }

    ClampedValue & operator--(void) {
      if(this->value > MinValue)
        --this->value;
      return *this;
    }

    ClampedValue & operator++(int dummy) {
      (void)dummy; // In case the compiler complains
	  
//      const auto temp = *this;
      this->operator++();
      return *this;
    }

    ClampedValue & operator--(int dummy) {
      (void)dummy; // In case the compiler complains
	  
//      const auto temp = *this;
      this->operator--();
      return *this;
    }
};