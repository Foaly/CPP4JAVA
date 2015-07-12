#include <iostream>
#include <vector>
#include <thread>       // std::thread
#include <mutex>        // std::mutex
#include <numeric>      // std::inner_product, std::accumulate
#include <cmath>        // std::floor

#include "fix_point.hpp"
#include "clock.hpp"


const unsigned int vectorSize = 65536;


float calculateDotProduct(const std::vector<fix_point>& vec1, const std::vector<fix_point>& vec2);
float calculateDotProductThreaded(const std::vector<fix_point>& vec1, const std::vector<fix_point>& vec2);


int main()
{
    // initialize first vector with 65536 elements of the value 0.5f
    std::vector<fix_point> vectorOne;
    vectorOne.insert(vectorOne.begin(), vectorSize, fix_point(0.5f));

    // initialize second vector with 65536 elements of the value 0.25f
    std::vector<fix_point> vectorTwo;
    vectorTwo.insert(vectorTwo.begin(), vectorSize, fix_point(0.25f));


    // calculate the dot product without threads
    Clock clock;
    std::cout << "Dot product: " << calculateDotProduct(vectorOne, vectorTwo) << std::endl;
    std::cout << "Time to calculate normal dot product: " << clock.getElapsedTime().count() << " microseconds." << std::endl << std::endl;

    // calculate the dot product using threads
    Clock threadClock;
    std::cout << "Thread dot products: " << calculateDotProductThreaded(vectorOne, vectorTwo) << std::endl;
    std::cout << "Time to calcualte threaded dot product: " << threadClock.getElapsedTime().count() << " microseconds." << std::endl;

    return 0;
}


float calculateDotProduct(const std::vector<fix_point>& vec1, const std::vector<fix_point>& vec2)
{
    // calculate the dot product without threads
    fix_point dotProduct = std::inner_product(vec1.begin(), vec1.end(), vec2.begin(), fix_point(0.f));
    return static_cast<float>(dotProduct);
}


std::vector<fix_point> threadResults;
std::mutex resultMutex;
using vectorIterator = std::vector<fix_point>::const_iterator;

// thread function
void dotProductThreadFunc(vectorIterator first1, vectorIterator last1, vectorIterator first2)
{
    // calculate dot product
    fix_point dotProduct = std::inner_product(first1, last1, first2, fix_point(0.f));

    std::lock_guard<std::mutex> lock (resultMutex);
    threadResults.push_back(dotProduct);
}


float calculateDotProductThreaded(const std::vector<fix_point>& vec1, const std::vector<fix_point>& vec2)
{
    // get the number of hardware supported threads
    const unsigned int numberOfThreads = std::thread::hardware_concurrency();
    std::cout << numberOfThreads << " concurrent threads are supported." << std::endl;

    // caculate how many elements per thread
    const unsigned int stepSize = std::floor(vectorSize / numberOfThreads);

    // create the threads
    std::vector<std::thread> threads;
    threads.reserve(numberOfThreads);
    for(unsigned int i = 0; i < numberOfThreads - 1; ++i)
    {
        //std::cout << "begin: " << stepSize * i << " end: " << stepSize * (i + 1) << std::endl;
        threads.emplace_back(std::thread(dotProductThreadFunc, vec1.begin() + stepSize * i, vec1.begin() + stepSize * (i + 1), vec2.begin()));
    }

    //std::cout << "begin: " << stepSize * (numberOfThreads - 1) << " end: " << vectorSize << std::endl;
    threads.emplace_back(std::thread(dotProductThreadFunc, vec1.begin() + stepSize * (numberOfThreads - 1), vec1.end(), vec2.begin()));

    // wait for the threads to finish
    for (auto& thread : threads)
        thread.join();

    // print thread results
    //for (auto& n: threadResults)
    //    std::cout << static_cast<float>(n) << std::endl;

    // calculate the sum of the thread results
    fix_point result = std::accumulate(threadResults.begin(), threadResults.end(), fix_point(0.f));

    return static_cast<float>(result);
}
