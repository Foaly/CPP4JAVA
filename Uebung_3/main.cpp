#include <iostream>
#include <vector>
#include <thread>       // std::thread
#include <mutex>        // std::mutex
#include <numeric>      // std::inner_product, std::accumulate
#include <iterator>     // std::input_iterator_tag
#include <cmath>        // std::floor

#include "fix_point.hpp"

std::vector<fix_point> threadResults;
std::mutex resultMutex;


// thread function
using inputIterator = std::vector<fix_point>::iterator;
void dotProductThreadFunc(inputIterator first1, inputIterator last1, inputIterator first2)
{
    // calculate dot product
    fix_point dotProduct = std::inner_product(first1, last1, first2, fix_point(0.f));

    std::lock_guard<std::mutex> lock (resultMutex);
    threadResults.push_back(dotProduct);
}


int main()
{
    const unsigned int vectorSize = 65536;

    // initialize first vector with 65536 elements of the value 0.5f
    std::vector<fix_point> vectorOne;
    vectorOne.insert(vectorOne.begin(), vectorSize, fix_point(0.5f));

    // initialize second vector with 65536 elements of the value 0.25f
    std::vector<fix_point> vectorTwo;
    vectorTwo.insert(vectorTwo.begin(), vectorSize, fix_point(0.25f));

    //dot product
    fix_point dotProduct = std::inner_product(vectorOne.begin(), vectorOne.end(), vectorTwo.begin(), fix_point(0.f));

    std::cout << "Dot product: " << static_cast<float>(dotProduct) << std::endl;

    const unsigned int numberOfThreads = std::thread::hardware_concurrency();
    std::cout << numberOfThreads << " concurrent threads are supported." << std::endl;

    const unsigned int stepSize = std::floor(vectorSize / numberOfThreads);

    std::vector<std::thread> threads;
    threads.reserve(numberOfThreads);

    for(unsigned int i = 0; i < numberOfThreads - 1; ++i)
    {
        //std::cout << "begin: " << stepSize * i << " end: " << stepSize * (i + 1) << std::endl;
        threads.push_back(std::thread(dotProductThreadFunc, vectorOne.begin() + stepSize * i, vectorOne.begin() + stepSize * (i + 1), vectorTwo.begin()));
    }

    //std::cout << "begin: " << stepSize * (numberOfThreads - 1) << " end: " << vectorSize << std::endl;
    threads.push_back(std::thread(dotProductThreadFunc, vectorOne.begin() + stepSize * (numberOfThreads - 1), vectorOne.end(), vectorTwo.begin()));

    // wait for the threads to finish
    for (auto& thread : threads)
        thread.join();

    // print thread results
    //for (auto& n: threadResults)
    //    std::cout << static_cast<float>(n) << std::endl;

    // calculate the sum of the thread results
    fix_point result = std::accumulate(threadResults.begin(), threadResults.end(), fix_point(0.f));
    std::cout << "Thread dot products: " << static_cast<float>(result) << std::endl;

    return 0;
}




