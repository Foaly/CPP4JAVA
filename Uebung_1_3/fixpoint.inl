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


//fix_point::fix_point(double value) :
//    m_data(toFixPointInt(static_cast<float>(value)))
//{

//}


//fix_point::operator int() const
//{
//    return static_cast<int>(floor());
//}



//// anonymous namespace; the functions inside it are only available inside this file
//namespace
//{
//    /**
//     * @brief factorial Calculates the factorial of n
//     */
//    unsigned int factorial(unsigned int n)
//    {
//        if (n == 0)
//           return 1;
//        return n * factorial(n - 1);
//    }
//}


//float sin(fix_point value)
//{
//    const float x = intToFloat(value.getData());

//    /**
//     * Something here is still fucky. If value is bigger than 2
//     * the results go towards negative infinity. If I increase
//     * n it results in nan.
//     *
//     * The whole revolution ( 2pi or 6.28) should be covert. Also
//     * a modulo should be applyed to to x so it falls into that range
//     */

//    float sum = 0.f;
//    for(int n = 0; n < 10; n++)
//    {
//        const int term = 2 * n + 1;
//        sum += std::pow(-1, n) * std::pow(x, term) / factorial(term);
//    }

//    return sum;
//}


//float cos(fix_point value)
//{
//    const float x = intToFloat(value.getData());

//    /**
//     * Same precision issues as with sin
//     */

//    float sum = 0.f;
//    for(int n = 0; n < 10; n++)
//    {
//        const int term = 2 * n;
//        sum += std::pow(-1, n) * std::pow(x, term) / factorial(term);
//    }

//    return sum;
//}
