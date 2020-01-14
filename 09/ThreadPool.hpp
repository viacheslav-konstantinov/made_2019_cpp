// заголовочный фал с реализацией класса ThreadPool

#include <atomic>
#include <future>
#include <thread>
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
using std::thread;

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
    mutex m_;
    condition_variable v_;

    //перечень работ для выполнения
    deque<packaged_task<void()>> works_;
    //потоки
    vector<thread> workingThreads_;
    //статусы потоков
    vector<workerState> workerStatus_;
    //переменная = нужно ли завершить работу потока
    //необходим для корректного удаления пула
    std::atomic_bool stopPool_;

    void worker(size_t idx) 
    {
        //номер потока
        size_t workerIdx = idx;

        //переменная, в которую записывается текущее значение 
        //stopPool_ при уведомлении потока
        bool doStop;

        while(true)
        {
            packaged_task<void()> f;
            {
                unique_lock<mutex> l(m_);
                if (works_.empty())
                {
                    //ждём нового задания или окончания работы пула
                    v_.wait(l, 
                    [&]
                    {
                        //чтобы лишний раз не проверять значение атомарной переменной,
                        //проверим, если работы в works_
                        if(!works_.empty())
                            return true;

                        //проверяем stopPool_ только если works_ пуст
                        doStop = stopPool_.load(); 
                        return doStop;
                    });
                }

                //если работа пула завершена, выходим из цикла
                if(doStop) 
                {
                    #ifdef DEBUG
                    cout << "Stopping worker #" << workerIdx << "..." << endl;
                    #endif
                    return;
                }

                //если есть задание, то меняем статус на occupied
                workerStatus_[idx] = workerState::occupied;
                f = std::move(works_.front());
                works_.pop_front();
                
            }
            
            // если задание некорректно, то меняем статус на ready
            // и ждём работу
            if (!f.valid()) 
            {
                #ifdef DEBUG
                cout << "Invalid work for worker #" << workerIdx << endl;
                #endif
                workerStatus_[idx] = workerState::ready;
                return;
            }

            // выполняем работу и после завершения меняем статус на ready
            #ifdef DEBUG
            cout << "Calling worker #" << workerIdx << endl;
            #endif
            f();
            workerStatus_[idx] = workerState::ready;
        }
    }

public:
    explicit ThreadPool(size_t poolSize)
    {

        //запускаем потоки и устанавливаем первоначальные статусы
        workerStatus_.resize(poolSize, workerState::ready);
        stopPool_.store(false);

        for(size_t i = 0; i < poolSize; ++i)
        {
            workingThreads_.push_back(thread( [this, i]{ worker(i);}));
        }
            
    }
    
    ~ThreadPool() 
    {   
        //завершаем работу пула, уведовляем потоки
        stopPool_.store(true);   
        v_.notify_all();

        //дожидаемся завершения работы всех потоков
        for(size_t idx = 0; idx < workingThreads_.size(); ++idx)
        {
            workingThreads_[idx].join();
        }
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
            unique_lock<mutex> l(m_);
            works_.emplace_back(std::move(newTask));
        }
        
        //оповещаем, что поток освободился
        v_.notify_one();
        return result;
    }
    
    bool isReady()
    {
        {
            unique_lock<mutex> l(m_);
            for(auto& state : workerStatus_)
            {
                if(state != workerState::ready) 
                    return false;
            }

            if (!works_.empty()) return false;
        }
        return true;
    }

    void printWorkerStates()
    {
        {
            unique_lock<mutex> l(m_);
            cout << "-------------------------------------------\n";
            cout << "Current states of the threads in the pool:\n";
            for(size_t i = 0; i < workerStatus_.size(); ++i)
            {
                if(workerStatus_[i] == workerState::ready) 
                    cout << "Thread #" << i << " is ready\n";
                else
                    cout << "Thread #" << i << " is occupied\n";
            }
            cout << endl;
            cout << "Number of workes to finish: " << works_.size() << endl;
            cout << "-------------------------------------------\n";
        }
        return;
    }
};
