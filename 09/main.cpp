/*
Домашнее задание 9
Реализовать пул потоков со следующим интерфейсом:

class ThreadPool
{
public:
    explicit ThreadPool(size_t poolSize);

     // pass arguments by value
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>;
};

Использование пула потоков:

struct A {};

void foo(const A&) {}

ThreadPool pool(8);

auto task1 = pool.exec(foo, A());
task1.get();

auto task2 = pool.exec([]() { return 1; });
task2.get();

*/

#include <iostream>
#include <chrono>
#include "ThreadPool.hpp" // заголовочный фал с реализацией класса ThreadPool

using std::cout;
using std::cin;
using std::endl;
using namespace std::chrono_literals;


int main()
{
    cout << "This program demonstrates the functionality of the ThreadPool class.\n";
    cout << "As example a number of functions to calculate power of 3 for a given number.\n";
    
    int N, numberOfThreads;

    cout << "Enter number of threads in pool and max number to calculate its power of 3:\n";
    cin >> numberOfThreads >> N;

    using Function = std::function<long (int)>;
    
    Function func = [](int x)->long 
    {
        cout << "Providing calculations for x = " << x << endl;
        return x*x*x;
    };
    
    ThreadPool pool(numberOfThreads);

    vector<future<long>> results;

    for(int i = 0; i <= N; ++i)
        results.emplace_back(pool.exec(func, i));

    pool.printWorkerStates();

    while(!pool.isReady())
    {
        std::this_thread::sleep_for(10ns);
        cout << "Calculations are still in progress...\n";
        pool.printWorkerStates();
    }
        
    cout << "Results:\n";
    for(int i = 0; i <= N; ++i)
        cout << "i = " << i << ", result = " << results[i].get() << endl;

    return 0;
}