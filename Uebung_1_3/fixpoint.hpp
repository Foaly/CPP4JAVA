#ifndef FIX_POINT_H
#define FIX_POINT_H

#include <cstdint>
#include <cmath>
#include <iostream>


template< int Number >
struct make_type
{
};

template<> struct make_type<8>
{
   using type = std::int8_t;
};

template<> struct make_type<16>
{
   using type = std::int16_t;
};

template<> struct make_type<32>
{
   using type = std::int32_t;
};

template<> struct make_type<64>
{
   using type = std::int64_t;
};




template <int pre, int post>
class fix_point
{
public:

    static const int pre_point_bits = pre;
    static const int post_point_bits = post;
    static const int total_bits = pre + post;
    using IntegerType = typename make_type<total_bits>::type;
    using IntegerTypeDoublePrecision = typename make_type<total_bits * 2>::type;


    fix_point() :
        m_data(0)
    {

    }


    fix_point(float value) :
        m_data(toFixPointInt(value))
    {

    }


    fix_point(IntegerType value) :
        m_data(value)
    {

    }


    fix_point& operator = (float value)
    {
        m_data = toFixPointInt(value);
        return *this;
    }

    /**
     * Member operators
     */
    // prefix increment
    fix_point& operator ++ ()
    {
        m_data += toFixPointInt(1.f);
        return (*this);
    }


    // prefix decrement
    fix_point& operator -- ()
    {
        m_data -= toFixPointInt(1.f);
        return (*this);
    }


    // postfix increment
    fix_point operator ++ (int)
    {
        // make a copy
        fix_point result(m_data);

        // increment
        ++(*this);

        // return the copy
        return result;
    }


    // postfix decrement
    fix_point operator -- (int)
    {
        // make a copy
        fix_point result(m_data);

        // increment
        --(*this);

        // return the copy
        return result;
    }




    /**
     * Cast operators
     */
    operator float() const
    {
        return intToFloat(m_data);
    }


    IntegerType getData() const
    {
        return m_data;
    }


    float floor() const
    {
        return m_data >> post;
    }


    /**
     * @brief Get the leading part (part before the comma) of the fix_point number
     */
    float frac() const
    {
        // create a mask where the post bits are set to 1
        const IntegerType mask = std::pow(2, post) - 1;

        // set the pre bit to 0 using the AND operator and the mask
        // so all that is left is the post bits (part after the comma)
        const std::int32_t trailingPart = m_data & mask;

        // convert int to float
        return intToFloat(trailingPart);
    }

private:

    IntegerType m_data;


    static IntegerType toFixPointInt(float value)
    {
        // taken from https://en.wikipedia.org/wiki/Q_%28number_format%29#Conversion
        value = value * std::pow(2, post);
        value = std::round(value);
        return static_cast<IntegerType>(value);
    }


    static float intToFloat(IntegerType value)
    {
        // convert int to float
        // taken from here: https://en.wikipedia.org/wiki/Q_%28number_format%29#Conversion
        float floatValue = static_cast<float>(value);
        floatValue *= std::pow(2, -post);
        return floatValue;
    }
};




/**
 *   Comparision operators
 */
template <typename fixpointType>
bool operator == (const fixpointType& lhs, const fixpointType& rhs);

template <typename fixpointType>
bool operator != (const fixpointType& lhs, const fixpointType& rhs);

template <typename fixpointType>
bool operator <= (const fixpointType& lhs, const fixpointType& rhs);

template <typename fixpointType>
bool operator >= (const fixpointType& lhs, const fixpointType& rhs);

template <typename fixpointType>
bool operator < (const fixpointType& lhs, const fixpointType& rhs);

template <typename fixpointType>
bool operator > (const fixpointType& lhs, const fixpointType& rhs);


/**
 * Arithmetic operators
 */
template <typename fixpointType>
fixpointType operator + (const fixpointType& lhs, const fixpointType& rhs);

template <typename fixpointType>
fixpointType operator - (const fixpointType& lhs, const fixpointType& rhs);

template <typename fixpointType>
fixpointType operator * (const fixpointType& lhs, const fixpointType& rhs);

template <typename fixpointType>
fixpointType operator / (const fixpointType& lhs, const fixpointType& rhs);


/**
 * Compound assignment operators
 */
template <typename fixpointType>
fixpointType& operator += (fixpointType& lhs, const fixpointType& rhs);

template <typename fixpointType>
fixpointType& operator -= (fixpointType& lhs, const fixpointType& rhs);

template <typename fixpointType>
fixpointType& operator *= (fixpointType& lhs, const fixpointType& rhs);

template <typename fixpointType>
fixpointType& operator /= (fixpointType& lhs, const fixpointType& rhs);




/**
 * @brief Get the leading part (part before the comma) of the fix_point number
 * @param value The given fix_point number
 */
template <typename fixpointType>
float floor(fixpointType value);

/**
 * @brief Get the trailing part (part after the comma) of the fix_point number
 * @param value The given fix_point number
 */
template <typename fixpointType>
float frac(fixpointType value);


#include "fixpoint.inl"


//class fix_point
//{
//public:
//    /**
//     * Constructors
//     */
//    fix_point();
//    fix_point(float value);
//    fix_point(double value);
//    explicit fix_point(std::int32_t value); // takes an int in the format Q16.16

//    /**
//     * Cast operators
//     */
//    explicit operator float() const;
//    explicit operator int() const;


//};


///**
// * @brief sin Calculate the sin of the given value using the taylor series
// * @param value The given fix_point number
// */
//float sin(fix_point value);

///**
// * @brief cos Calculate the cos of the given value using the taylor series
// * @param value The given fix_point number
// */
//float cos(fix_point value);

#endif // FIX_POINT_H
