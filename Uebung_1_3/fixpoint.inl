#include <cmath>



/////////////////
/////  Operators
/////////////////

template <typename fixpointType>
bool operator == (const fixpointType& lhs, const fixpointType& rhs)
{
    return (lhs.getData() == rhs.getData());
}


template <typename fixpointType>
bool operator != (const fixpointType& lhs, const fixpointType& rhs)
{
    return !(lhs == rhs);
}


template <typename fixpointType>
bool operator <= (const fixpointType& lhs, const fixpointType& rhs)
{
    return (lhs.getData() <= rhs.getData());
}


template <typename fixpointType>
bool operator >= (const fixpointType& lhs, const fixpointType& rhs)
{
    return (lhs.getData() >= rhs.getData());
}


template <typename fixpointType>
bool operator < (const fixpointType& lhs, const fixpointType& rhs)
{
    return (lhs.getData() < rhs.getData());
}


template <typename fixpointType>
bool operator > (const fixpointType& lhs, const fixpointType& rhs)
{
    return (lhs.getData() > rhs.getData());
}


template <typename fixpointType>
fixpointType operator + (const fixpointType& lhs, const fixpointType& rhs)
{
    const typename fixpointType::IntegerType sum = lhs.getData() + rhs.getData();
    return fixpointType(sum);
}


template <typename fixpointType>
fixpointType operator - (const fixpointType& lhs, const fixpointType& rhs)
{
    const typename fixpointType::IntegerType sum = lhs.getData() - rhs.getData();
    return fixpointType(sum);
}


template <typename fixpointType>
fixpointType operator * (const fixpointType& lhs, const fixpointType& rhs)
{
    // https://en.wikipedia.org/wiki/Q_%28number_format%29#Multiplication

    //multiply the two values
    using nextBiggerType = typename fixpointType::IntegerTypeDoublePrecision;
    nextBiggerType temp = static_cast<nextBiggerType>(lhs.getData()) * static_cast<nextBiggerType>(rhs.getData());

    // Rounding; mid values are rounded up
    temp += (1 << (fixpointType::post_point_bits - 1));

    // Correct by dividing by base
    const typename fixpointType::IntegerType result = temp >> fixpointType::post_point_bits;

    return fixpointType(result);
}


template <typename fixpointType>
fixpointType operator / (const fixpointType& lhs, const fixpointType& rhs)
{
    // https://en.wikipedia.org/wiki/Q_%28number_format%29#Division

    // pre-multiply by the base
    using nextBiggerType = typename fixpointType::IntegerTypeDoublePrecision;
    const nextBiggerType temp = static_cast<nextBiggerType>(lhs.getData()) << fixpointType::post_point_bits;

    // and divide
    const typename fixpointType::IntegerType result = temp / rhs.getData();

    return fixpointType(result);
}


template <typename fixpointType>
fixpointType& operator += (fixpointType& lhs, const fixpointType& rhs)
{
    return lhs = lhs + rhs;
}


template <typename fixpointType>
fixpointType& operator -= (fixpointType& lhs, const fixpointType& rhs)
{
    return lhs = lhs - rhs;
}


template <typename fixpointType>
fixpointType& operator *= (fixpointType& lhs, const fixpointType& rhs)
{
    return lhs = lhs * rhs;
}


template <typename fixpointType>
fixpointType& operator /= (fixpointType& lhs, const fixpointType& rhs)
{
    return lhs = lhs / rhs;
}




//////////////////////
/////  Free functions
//////////////////////

template <typename fixpointType>
float floor(fixpointType value)
{
    return value.floor();
}


template <typename fixpointType>
float frac(fixpointType value)
{
    return value.frac();
}
