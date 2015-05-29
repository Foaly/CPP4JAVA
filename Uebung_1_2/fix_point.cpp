#include "fix_point.hpp"

#include <cmath>

fix_point::fix_point(float value) :
    m_data(toFixPointInt(value))
{

}


fix_point::fix_point(double value) :
    m_data(toFixPointInt(static_cast<float>(value)))
{

}


fix_point::fix_point(int32_t value) :
    m_data(value)
{

}


fix_point& fix_point::operator = (float value)
{
    m_data = toFixPointInt(value);
    return *this;
}


std::int32_t fix_point::getData() const
{
    return m_data;
}


float fix_point::frac() const
{
    // lower 16 bit are set to 1
    const int mask = 0x0000FFFF;

    // set the top 16 bit to 0 using the AND operator and the mask
    // so all that is left is the lower 16 bit (part after the comma)
    const std::int32_t trailingPart = m_data & mask;

    // convert Q16.16 int to float
    // taken from here: https://en.wikipedia.org/wiki/Q_%28number_format%29#Conversion
    float floatValue = static_cast<float>(trailingPart);
    floatValue = floatValue * std::pow(2, -16);

    return floatValue;
}


std::int32_t fix_point::toFixPointInt(float value)
{
    // taken from https://en.wikipedia.org/wiki/Q_%28number_format%29#Conversion
    value = value * std::pow(2, 16);
    value = std::round(value);
    return static_cast<std::int32_t>(value);
}


bool operator == (const fix_point& lhs, const fix_point& rhs)
{
    return (lhs.getData() == rhs.getData());
}


bool operator != (const fix_point& lhs, const fix_point& rhs)
{
    return !(lhs == rhs);
}


bool operator <= (const fix_point& lhs, const fix_point& rhs)
{
    return (lhs.getData() <= rhs.getData());
}


bool operator >= (const fix_point& lhs, const fix_point& rhs)
{
    return (lhs.getData() >= rhs.getData());
}


bool operator < (const fix_point& lhs, const fix_point& rhs)
{
    return (lhs.getData() < rhs.getData());
}


bool operator > (const fix_point& lhs, const fix_point& rhs)
{
    return (lhs.getData() > rhs.getData());
}


fix_point operator + (const fix_point& lhs, const fix_point& rhs)
{
    const std::int32_t sum = lhs.getData() + rhs.getData();
    return fix_point(sum);
}


fix_point operator - (const fix_point& lhs, const fix_point& rhs)
{
    const std::int32_t difference = lhs.getData() - rhs.getData();
    return fix_point(difference);
}


fix_point operator * (const fix_point& lhs, const fix_point& rhs)
{
    // https://en.wikipedia.org/wiki/Q_%28number_format%29#Multiplication

    //multiply the two values
    std::int64_t temp = static_cast<std::int64_t>(lhs.getData()) * static_cast<std::int64_t>(rhs.getData());

    // Rounding; mid values are rounded up
    temp += (1 << (16 - 1));

    // Correct by dividing by base
    const std::int32_t result = temp >> 16;

    return fix_point(result);
}


fix_point operator / (const fix_point& lhs, const fix_point& rhs)
{
    // https://en.wikipedia.org/wiki/Q_%28number_format%29#Division

    // pre-multiply by the base
    const std::int64_t temp = static_cast<std::int64_t>(lhs.getData()) << 16;

    // and divide
    const std::int32_t result = temp / rhs.getData();

    return fix_point(result);
}


fix_point& operator += (fix_point& lhs, const fix_point& rhs)
{
    return lhs = lhs + rhs;
}


fix_point& operator -= (fix_point& lhs, const fix_point& rhs)
{
    return lhs = lhs - rhs;
}


fix_point& operator *= (fix_point& lhs, const fix_point& rhs)
{
    return lhs = lhs * rhs;
}


fix_point& operator /= (fix_point& lhs, const fix_point& rhs)
{
    return lhs = lhs / rhs;
}



////////////////////
///  Free functions
////////////////////

float floor(fix_point value)
{
    // the part before the comma is in the top 16 bit, so simply
    // shift 16 bit to the left the rest is automatically padded with 0
    std::int32_t result = value.getData() >> 16;

    return static_cast<float>(result);
}


float frac(fix_point value)
{
    return value.frac();
}


