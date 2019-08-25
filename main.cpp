#include "CircularList.h"
#include <random>
#include <iostream>
#include <thread>

static constexpr size_t MAX_ELEMENTS = 200;
CircularList<unsigned long> list;
void push_front()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 100);

    for (size_t i = 0; i < MAX_ELEMENTS; ++i)
    {
        try
        {
            auto num = dist(rng);
            std::cout << "Push front : " << num << std::endl;
            list.push_front(num);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void push_back()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 100);

    for (size_t i = 0; i < MAX_ELEMENTS; ++i)
    {
        try
        {
            auto num = dist(rng);
            std::cout << "Push back : " << num << std::endl;
            list.push_back(num);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void pop_front()
{
    try
    {
        for (size_t i = 0; i < MAX_ELEMENTS; ++i)
        {
            std::cout << "Pop front : " << list.pop_front() << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void pop_back()
{
    try
    {
        for (size_t i = 0; i < MAX_ELEMENTS; ++i)
        {
            std::cout << "Pop back : " << list.pop_back() << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

int main()
{
    CircularList<int>  secondIntList;
    CircularList<char> firstCharList;
    std::thread th1(push_back);
    std::thread th2(push_front);

    th1.join();
    th2.join();
    list.print();
    std::thread th3(pop_back);
    std::thread th4(pop_front);
    th3.join();
    th4.join();
    list.print();
    return 0;
}
