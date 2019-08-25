#include "CircularList.h"
#include <random>
#include <iostream>
#include <thread>

static std::mutex stdout_mutex;
void print_message(const int& thread_num, const std::string& message, const bool isError)
{
    std::lock_guard<std::mutex> guard(stdout_mutex);

    if (isError)
    {
        std::cerr << thread_num << " : ERROR : " << message << std::endl;
    }
    else
    {
        std::cout << thread_num << " : " << message << std::endl;
    }
}

static constexpr size_t MAX_ELEMENTS = 200;
int main()
{
    CircularList<char> charList;
    CircularList<unsigned long> ulList;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist_int(0, 100);
    std::uniform_int_distribution<std::mt19937::result_type> dist_char(0, 127 - 33);
    auto push_front = [&](const int& thread_num) {
                          for (size_t i = 0; i < MAX_ELEMENTS; ++i)
                          {
                              try
                              {
                                  auto value   = dist_int(rng);
                                  auto message = "Push front : " + std::to_string(value);
                                  print_message(thread_num, message, false);
                                  ulList.push_front(value);
                              }
                              catch (std::exception& e)
                              {
                                  print_message(thread_num, e.what(), true);
                                  break;
                              }
                          }
                      };
    auto push_back = [&](const int& thread_num) {
                         for (size_t i = 0; i < MAX_ELEMENTS; ++i)
                         {
                             try
                             {
                                 auto value   = dist_int(rng);
                                 auto message = "Push back : " + std::to_string(value);
                                 print_message(thread_num, message, false);
                                 ulList.push_back(value);
                             }
                             catch (std::exception& e)
                             {
                                 print_message(thread_num, e.what(), true);
                                 break;
                             }
                         }
                     };
    auto pop_front = [&](const int& thread_num) {
                         try
                         {
                             for (size_t i = 0; i < MAX_ELEMENTS; ++i)
                             {
                                 auto message = "Pop front : " + std::to_string(ulList.pop_front());
                                 print_message(thread_num, message, false);
                             }
                         }
                         catch (std::exception& e)
                         {
                             print_message(thread_num, e.what(), true);
                         }
                     };
    auto pop_back = [&](const int& thread_num) {
                        try
                        {
                            for (size_t i = 0; i < MAX_ELEMENTS; ++i)
                            {
                                auto message = "Pop back : " + std::to_string(ulList.pop_back());
                                print_message(thread_num, message, false);
                            }
                        }
                        catch (std::exception& e)
                        {
                            print_message(thread_num, e.what(), true);
                        }
                    };
    auto push_front_char = [&](const int& thread_num) {
                               for (size_t i = 0; i < MAX_ELEMENTS; ++i)
                               {
                                   try
                                   {
                                       auto value   = static_cast<char>(dist_char(rng) + 33);
                                       auto message = std::string("Push front : ") + value;
                                       print_message(thread_num, message, false);
                                       charList.push_front(value);
                                   }
                                   catch (std::exception& e)
                                   {
                                       print_message(thread_num, e.what(), true);
                                       break;
                                   }
                               }
                           };
    auto push_back_char = [&](const int& thread_num) {
                              for (size_t i = 0; i < MAX_ELEMENTS; ++i)
                              {
                                  try
                                  {
                                      auto value   = static_cast<char>(dist_char(rng) + 33);
                                      auto message = std::string("Push back : ") + value;
                                      print_message(thread_num, message, false);
                                      charList.push_back(value);
                                  }
                                  catch (std::exception& e)
                                  {
                                      print_message(thread_num, e.what(), true);
                                      break;
                                  }
                              }
                          };
    std::thread th1(push_back_char, 1);
    std::thread th2(push_back, 2);
    std::thread th3(push_front, 3);
    std::thread th4(push_front_char, 4);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    ulList.print();
    std::cout << "Size: " << ulList.size() << std::endl;
    charList.print();
    std::cout << "Size: " << charList.size() << std::endl;
    std::thread th5(pop_back, 5);
    std::thread th6(pop_front, 6);
    th5.join();
    th6.join();
    ulList.print();
    return 0;
}
