#include "fixpoint.hpp"

#include <cmath>

fix_point::fix_point() :
    m_data(0)
{

}


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


fix_point& fix_point::operator ++ ()
{
    m_data += toFixPointInt(1.f);
    return (*this);
}


fix_point& fix_point::operator -- ()
{
    m_data -= toFixPointInt(1.f);
    return (*this);
}


fix_point fix_point::operator ++ (int)
{
    // make a copy
    fix_point result(m_data);

    // increment
    ++(*this);

    // return the copy
    return result;
}


fix_point fix_point::operator -- (int)
{
    // make a copy
    fix_point result(m_data);

    // decrement
    --(*this);

    // return the copy
    return result;
}


fix_point::operator float() const
{
    return intToFloat(m_data);
}


fix_point::operator int() const
{
    return static_cast<int>(floor());
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
    return intToFloat(trailingPart);
}


float fix_point::floor() const
{
    // the part before the comma is in the top 16 bit, so simply
    // shift 16 bit to the left the rest is automatically padded with 0
    const std::int32_t result = m_data >> 16;

    return static_cast<float>(result);
}


std::int32_t fix_point::toFixPointInt(float value)
{
    // taken from https://en.wikipedia.org/wiki/Q_%28number_format%29#Conversion
    value = value * std::pow(2, 16);
    value = std::round(value);
    return static_cast<std::int32_t>(value);
}



///////////////
///  Operators
///////////////

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
    return value.floor();
}


float frac(fix_point value)
{
    return value.frac();
}


float intToFloat(std::int32_t value)
{
    // convert Q16.16 int to float
    // taken from here: https://en.wikipedia.org/wiki/Q_%28number_format%29#Conversion
    float floatValue = static_cast<float>(value);
    floatValue *= std::pow(2, -16);
    return floatValue;
}


// anonymous namespace; the functions inside it are only available inside this file
namespace
{
    /**
     * @brief factorial Calculates the factorial of n
     */
    unsigned int factorial(unsigned int n)
    {
        if (n == 0)
           return 1;
        return n * factorial(n - 1);
    }
}


float sin(fix_point value)
{
    const float x = intToFloat(value.getData());

    /**
     * Something here is still fucky. If value is bigger than 2
     * the results go towards negative infinity. If I increase
     * n it results in nan.
     *
     * The whole revolution ( 2pi or 6.28) should be covert. Also
     * a modulo should be applyed to to x so it falls into that range
     */

    float sum = 0.f;
    for(int n = 0; n < 10; n++)
    {
        const int term = 2 * n + 1;
        sum += std::pow(-1, n) * std::pow(x, term) / factorial(term);
    }

    return sum;
}


float cos(fix_point value)
{
    const float x = intToFloat(value.getData());

    /**
     * Same precision issues as with sin
     */

    float sum = 0.f;
    for(int n = 0; n < 10; n++)
    {
        const int term = 2 * n;
        sum += std::pow(-1, n) * std::pow(x, term) / factorial(term);
    }

    return sum;
}
