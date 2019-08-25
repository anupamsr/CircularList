#include "CircularList.h"
#include <random>
#include <iostream>

int main()
{
    constexpr size_t MAX_ELEMENTS = 5;
    CircularList<unsigned long> firstIntList;
    CircularList<int>  secondIntList;
    CircularList<char> firstCharList;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 100);

    for (size_t i = 0; i < MAX_ELEMENTS; ++i)
    {
        unsigned long num = dist(rng);
        std::cout << "Push back : " << num << std::endl;
        firstIntList.push_back(num);
        num = dist(rng);
        std::cout << "Push front : " << num << std::endl;
        firstIntList.push_front(num);
    }

    firstIntList.print();
    for (size_t i = 0; i < MAX_ELEMENTS; ++i)
    {
        std::cout << "Pop back : " << firstIntList.pop_back() << std::endl;
        std::cout << "Pop front : " << firstIntList.pop_front() << std::endl;
    }

    return 0;
}
