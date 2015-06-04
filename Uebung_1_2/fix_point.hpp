#ifndef FIX_POINT_H
#define FIX_POINT_H

#include <cstdint>


class fix_point
{
public:
    /**
     * Constructors
     */
    fix_point();
    fix_point(float value);
    fix_point(double value);
    explicit fix_point(std::int32_t value); // takes an int in the format Q16.16

    /**
     * Member operators
     */
    fix_point& operator = (float value);

    fix_point& operator ++ (); // prefix
    fix_point& operator -- (); // prefix

    fix_point operator ++ (int); // postfix
    fix_point operator -- (int); // postfix

    /**
     * Cast operators
     */
    explicit operator float() const;
    explicit operator int() const;

    /**
     * @brief Get the fix_point numbers data in the format Q16.16. The first 16 bit represent
     *        the part before the comma and the last 16 bit represent the part after the comma
     * @return A std::int32_t representing the fix point number
     */
    std::int32_t getData() const;

    /**
     * @brief Get the trailing part (part after the comma) of the fix_point number
     */
    float frac() const;

    /**
     * @brief Get the leading part (part before the comma) of the fix_point number
     */
    float floor() const;



private:
    /**
     * @brief Convert a float to an std::int32_t number in the Q16.16 format
     * @param value The float to convert
     */
    static std::int32_t toFixPointInt(float value);


    std::int32_t m_data;
};

/**
 *  Operators
 */
bool operator == (const fix_point& lhs, const fix_point& rhs);
bool operator != (const fix_point& lhs, const fix_point& rhs);
bool operator <= (const fix_point& lhs, const fix_point& rhs);
bool operator >= (const fix_point& lhs, const fix_point& rhs);
bool operator < (const fix_point& lhs, const fix_point& rhs);
bool operator > (const fix_point& lhs, const fix_point& rhs);

fix_point operator + (const fix_point& lhs, const fix_point& rhs);
fix_point operator - (const fix_point& lhs, const fix_point& rhs);
fix_point operator * (const fix_point& lhs, const fix_point& rhs);
fix_point operator / (const fix_point& lhs, const fix_point& rhs);

fix_point& operator += (fix_point& lhs, const fix_point& rhs);
fix_point& operator -= (fix_point& lhs, const fix_point& rhs);
fix_point& operator *= (fix_point& lhs, const fix_point& rhs);
fix_point& operator /= (fix_point& lhs, const fix_point& rhs);


/**
 * @brief Get the leading part (part before the comma) of the fix_point number
 * @param value The given fix_point number
 */
float floor(fix_point value);

/**
 * @brief Get the trailing part (part after the comma) of the fix_point number
 * @param value The given fix_point number
 */
float frac(fix_point value);

/**
 * @brief toFloat Convert a int in the format Q16.16 to a float
 */
float intToFloat(std::int32_t value);

/**
 * @brief sin Calculate the sin of the given value using the taylor series
 * @param value The given fix_point number
 */
float sin(fix_point value);

/**
 * @brief cos Calculate the cos of the given value using the taylor series
 * @param value The given fix_point number
 */
float cos(fix_point value);

#endif // FIX_POINT_H
