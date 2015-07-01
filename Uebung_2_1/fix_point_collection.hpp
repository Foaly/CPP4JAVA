#ifndef FIX_POINT_COLLECTION_HPP
#define FIX_POINT_COLLECTION_HPP

#include "fix_point.hpp"

#include <memory>


class fix_point_collection
{
public:

    fix_point_collection();
    ~fix_point_collection();

    void push_back(fix_point element);
    fix_point pop_back();
    void print() const;
    std::size_t size() const;
    fix_point sum() const;
    std::size_t count_value(fix_point value) const;

    const fix_point& operator[] (unsigned int index) const;
    fix_point& operator[] (unsigned int index);
    fix_point operator[] (const fix_point& index) const;

private:

    std::unique_ptr<fix_point []> m_collection;
    std::size_t m_size;
};


fix_point sum (fix_point_collection &collection);

std::size_t count_value(fix_point_collection &collection, fix_point value);


#endif // FIX_POINT_COLLECTION_HPP

