// заголовочный фал с реализацией класса ThreadPool

#include <future>
#include <vector>
#include <deque>
#include <iostream>
#include <functional>

using std::mutex;
using std::condition_variable;
using std::deque;
using std::future;
using std::packaged_task;
using std::unique_lock;
using std::cout;
using std::endl;
using std::vector;

//функции, которые кладутся в пул потоков,
//приводятся к виду WorkerFunction, где R - тип возвращаемых
//функцией данных
template<class R>
using WorkerFunction = std::function<R()>;

//статус потока
enum workerState
{
    ready,
    occupied
};

class ThreadPool
{
private:

    //мютекс и условная переменная для организации работы потоков
    mutex m;
    condition_variable v;

    //перечень работ для выполнения
    deque<packaged_task<void()>> works;
    //потоки
    vector<future<void>> workingThreads;
    //статусы потоков
    vector<workerState> workerStatus;

    void worker(size_t idx) 
    {
        //номер потока
        size_t workerIdx = idx;
        while(true)
        {
            packaged_task<void()> f;
            {
                unique_lock<mutex> l(m);
                if (works.empty())
                {
                    //ждём нового задания
                    v.wait(l, [&]{return !works.empty();});
                }
                //если есть задание, то меняем статус на occupied
                workerStatus[idx] = workerState::occupied;
                f = std::move(works.front());
                works.pop_front();
            }
            
            // если задание некорректно, то меняем статус на ready
            // и ждём работу
            if (!f.valid()) 
            {
                workerStatus[idx] = workerState::ready;
                return;
            }

            // выполняем работу и после завершения меняем статус на ready
            cout << "Calling worker #" << workerIdx << endl;
            f();
            workerStatus[idx] = workerState::ready;
        }
    }

public:
    explicit ThreadPool(size_t poolSize)
    {
        for (size_t i = 0; i < poolSize; ++i)
        {
            //запускаем потоки и устанавливаем первоначальные статусы потоков
            workerStatus.push_back(workerState::ready);
            workingThreads.push_back(std::async(std::launch::async, [this, i]{ worker(i);}));
        }
            
    }
    
    ~ThreadPool() 
    {         
        {
            unique_lock<mutex> l(m);
            for(auto&&unused : workingThreads)
                works.push_back({});
        }
        
        v.notify_all();
        workingThreads.clear();
    }

    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool(ThreadPool&& other) = delete; 
    ThreadPool& operator=(const ThreadPool& other) = delete;
    ThreadPool& operator=(ThreadPool&& other) = delete;

    //функция и аргументы записываются в пул потоков по значению
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>
    {
        //двухуровневая обёртка вызываемой функции: сначала в WorkerFunction,
        //потом - в packaged_task
        WorkerFunction<decltype(func(args...))>   newWorker = std::bind(func, args...);
        packaged_task <decltype(func(args...))()> newTask(newWorker);

        auto result = newTask.get_future();
        {
            //кладём в очередь новое задание
            unique_lock<mutex> l(m);
            works.emplace_back(std::move(newTask));
        }
        
        //оповещаем, что поток освободился
        v.notify_one();
        return result;
    }
    
    bool isReady()
    {
        {
            unique_lock<mutex> l(m);
            for(auto& state : workerStatus)
            {
                if(state != workerState::ready) 
                    return false;
            }

            if (!works.empty()) return false;
        }
        return true;
    }

    void printWorkerStates()
    {
        {
            unique_lock<mutex> l(m);
            cout << "-------------------------------------------\n";
            cout << "Current states of the threads in the pool:\n";
            for(size_t i = 0; i < workerStatus.size(); ++i)
            {
                if(workerStatus[i] == workerState::ready) 
                    cout << "Thread #" << i << " is ready\n";
                else
                    cout << "Thread #" << i << " is occupied\n";
            }
            cout << endl;
            cout << "Number of workes to finish: " << works.size() << endl;
            cout << "-------------------------------------------\n";
        }
        return;
    }
};