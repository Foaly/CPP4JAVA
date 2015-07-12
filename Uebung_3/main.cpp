#include <iostream>

#include "fix_point.hpp"

int main()
{
    fix_point fp(1.0);

    std::cout << static_cast<float>(fp) << std::endl;

    return 0;
}
