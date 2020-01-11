#include <iterator>

template <class T>
class Iterator
    : public std::iterator<std::forward_iterator_tag, T>
{
    T* ptr_;
public:
    
    Iterator()
    : ptr_(nullptr)
    {
    }

    explicit Iterator(T* ptr)
        : ptr_(ptr) {}

    Iterator(Iterator&& other)
    {
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;    
    }

    Iterator(const Iterator& iter)
        : ptr_(iter.ptr_) {}

    ~Iterator() {}

    Iterator& operator=(const Iterator& other)
    {
        if(other == *this)
            return *this;

        ptr_ = other.ptr_;
        return *this;
    }

    Iterator& operator=(Iterator&& other)
    {
        if(other == *this)
            return *this;

        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
        return *this;
    }

    bool operator==(const Iterator<T>& other) const
    {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator<T>& other) const
    {
        return !(*this == other);
    }

    T& operator*()
    {
        return *ptr_;
    }

    Iterator& operator++()
    {
        ++ptr_;
        return *this;
    }
    
    Iterator operator++(int)
    {
        Iterator temp = *this;
        ++*this;
        return temp;
    }
};


template<class T>
void swap(Iterator<T> r, Iterator<T> s)
{
   Iterator<T> temp = r;
   r = s;
   s = temp;
   return;
} 