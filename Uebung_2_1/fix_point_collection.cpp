#include "fix_point_collection.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>

fix_point_collection::fix_point_collection() :
    m_size(0)
{

}


fix_point_collection::~fix_point_collection()
{

}


void fix_point_collection::push_back(fix_point element)
{
    // create a temporary pointer to a fix_point array that
    // is one element bigger than our collection
    std::unique_ptr<fix_point []> newCollection (new fix_point[m_size + 1]);

    // copy the content of our collection to the temporary array
    std::copy(m_collection.get(), m_collection.get() + m_size, newCollection.get());

    // copy the new element into the last element of the temporary array
    newCollection[m_size] = element;

    // move the temporary array into the collection
    m_collection = std::move(newCollection);

    // increase the size
    m_size++;
}

fix_point fix_point_collection::pop_back()
{
    fix_point lastElement;

    if(m_size > 0)
    {
        // save the last element
        lastElement = m_collection[m_size];

        // decrease the size
        m_size--;

        // create a temporary pointer to a fix_point array that
        // is one element smaller than our collection
        std::unique_ptr<fix_point []> newCollection (new fix_point[m_size]);

        // copy the content of our collection - 1 to the temporary array
        std::copy(m_collection.get(), m_collection.get() + m_size, newCollection.get());

        // move the temporary array into the collection
        m_collection = std::move(newCollection);

    }
    return lastElement;
}


void fix_point_collection::print() const
{
    for (std::size_t i = 0; i < m_size; ++i)
        std::cout << static_cast<float>(m_collection[i]) << ' ';

    std::cout << std::endl;
}


std::size_t fix_point_collection::size() const
{
    return m_size;
}


fix_point fix_point_collection::sum() const
{
    return std::accumulate(m_collection.get(), m_collection.get() + m_size, fix_point(0.f));
}


std::size_t fix_point_collection::count_value(fix_point value) const
{
    return std::count(m_collection.get(), m_collection.get() + m_size, value);
}


const fix_point& fix_point_collection::operator[](unsigned int index) const
{
    if(index < m_size)
        return m_collection[index];
    fix_point fp (0.f);
    return fp;
}


fix_point& fix_point_collection::operator[](unsigned int index)
{
    if(index < m_size)
        return m_collection[index];
    fix_point fp (0.f);
    return fp;
}


fix_point fix_point_collection::operator[](const fix_point &index) const
{
    const float floatIndex = static_cast<float>(index);
    const int upperIndex = std::ceil(floatIndex);
    const int lowerIndex = std::floor(floatIndex);
    const fix_point t = floatIndex - lowerIndex;

    const fix_point upperFP = m_collection[upperIndex];
    const fix_point lowerFP = m_collection[lowerIndex];

    fix_point interpolation = (fix_point(1.f) - t) * lowerFP + t * upperFP;
    return interpolation;
}


fix_point sum(fix_point_collection &collection)
{
    return collection.sum();
}


std::size_t count_value(fix_point_collection &collection, fix_point value)
{
    return collection.count_value(value);
}
