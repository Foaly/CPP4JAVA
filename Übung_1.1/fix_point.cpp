#include "fix_point.h"

#include <cmath>

/**
 * @brief convert a float to a fix_point
 * @param value
 * @return value as float
 */
fix_point to_fix_point(float value)
{
    // taken from https://en.wikipedia.org/wiki/Q_%28number_format%29#Conversion
    value = value * std::pow(2, 16);
    value = std::round(value);
    return fix_point{static_cast<std::int32_t>(value)};
}


/**
 * @brief convert a fix_point to a float
 * @param value
 * @return value as float
 */
float to_float(fix_point value)
{
    float floatValue = static_cast<float>(value.data);
    floatValue = floatValue * std::pow(2, -16);
    return floatValue;
}


/**
 * @brief Get the leading part of the fix_point number (part before the comma)
 * @param value
 * @return
 */
float floor(fix_point value)
{
    // the part before the comma is in the top 16 bit, so simply
    // shift 16 bit to the left the rest is automatically padded with 0
    std::int32_t result = value.data >> 16;

    return static_cast<float>(result);
}


/**
 * @brief Get the trailing part of the fix_point number (part after the comma)
 * @param value
 * @return
 */
float frac(fix_point value)
{
    // lower 16 bit are set to 1
    const int mask = 0x0000FFFF;

    // set the top 16 bit to 0 using the AND operator and the mask
    // so all that is left is the lower 16 bit (part after the comma)
    const std::int32_t trailingPart = value.data & mask;

    // we can't convert directly to float, so we have to construct a
    // fix_point struct as a middle step
    const fix_point fp {trailingPart};

    // now convert to float
    return to_float(fp);
}


/**
 * @brief Checks if two fix_point numbers are equal
 * @param lhs
 * @param rhs
 * @return
 */
bool equals(fix_point lhs, fix_point rhs)
{
    return (lhs.data == rhs.data);
}


/**
 * @brief Check is the lhs fix_point number is less than the rhs
 * @param lhs
 * @param rhs
 * @return
 */
bool less_than(fix_point lhs, fix_point rhs)
{
    return (lhs.data < rhs.data);
}


/**
 * @brief Adds two fix_point numbers
 * @param lhs
 * @param rhs
 * @return
 */
fix_point add(fix_point lhs, fix_point rhs)
{
    const fix_point result {lhs.data + rhs.data};
    return result;
}


/**
 * @brief Subtracts two fix_point numbers
 * @param lhs
 * @param rhs
 * @return
 */
fix_point sub(fix_point lhs, fix_point rhs)
{
    const fix_point result {lhs.data - rhs.data};
    return result;
}


/**
 * @brief Multiplies two fix_point numbers
 * @param lhs
 * @param rhs
 * @return
 */
fix_point mul(fix_point lhs, fix_point rhs)
{
    // https://en.wikipedia.org/wiki/Q_%28number_format%29#Multiplication

    //multiply the two values
    std::int64_t temp = static_cast<std::int64_t>(lhs.data) * static_cast<std::int64_t>(rhs.data);

    // Rounding; mid values are rounded up
    temp += (1 << (16 - 1));

    // Correct by dividing by base
    const std::int32_t result = temp >> 16;

    return fix_point { result };
}


/**
 * @brief Divides two fix_point numbers
 * @param lhs
 * @param rhs
 * @return
 */
fix_point div(fix_point lhs, fix_point rhs)
{
    // https://en.wikipedia.org/wiki/Q_%28number_format%29#Division

    // pre-multiply by the base
    const std::int64_t temp = static_cast<std::int64_t>(lhs.data) << 16;

    // and divide
    const std::int32_t result = temp / rhs.data;

    return fix_point { result };
}
