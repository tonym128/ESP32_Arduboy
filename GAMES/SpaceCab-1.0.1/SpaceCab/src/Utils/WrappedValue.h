#pragma once

template<typename T, T Min, T Max>

class WrappedValue
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
    WrappedValue(void) = default;
    WrappedValue(uint8_t value) : value(value < Min ? Min : value > Max ? Max : value) {}
	
    operator T(void) const {
      return this->value;
    }

    WrappedValue & operator++(void) {
      if(this->value < MaxValue)
        ++this->value;
      else
        this->value = MinValue;
      return *this;
    }

    WrappedValue & operator--(void) {
      if(this->value > MinValue)
        --this->value;
      else
        this->value = MaxValue;
      return *this;
    }

    WrappedValue & operator++(int dummy) {
      (void)dummy; // In case the compiler complains
	  
//    const auto temp = *this;
      this->operator++();
      return *this;
    }

    WrappedValue & operator--(int dummy) {
      (void)dummy; // In case the compiler complains
	  
//    const auto temp = *this;
      this->operator--();
      return *this;
    }
};