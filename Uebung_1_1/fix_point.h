#ifndef FIXPOINT_INCLUDE
#define FIXPOINT_INCLUDE

#include <cstdint>

struct fix_point {
    std::int32_t data;
};

/**
 * Conversion functions
 */
fix_point to_fix_point(float value);
float to_float(fix_point value);

/**
 * functions to get the leading to trailing part
 */
float floor(fix_point value);
float frac(fix_point value);

/**
 * Comparison functions
 */
bool equals(fix_point lhs, fix_point rhs);
bool less_than(fix_point lhs, fix_point rhs);

/**
 * Math functions
 */
fix_point add(fix_point lhs, fix_point rhs);
fix_point sub(fix_point lhs, fix_point rhs);
fix_point mul(fix_point lhs, fix_point rhs);
fix_point div(fix_point lhs, fix_point rhs);

#endif // FIXPOINT_INCLUDE
