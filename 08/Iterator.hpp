#include <iterator>

template <class T>
class Iterator
    : public std::iterator<std::random_access_iterator_tag, T>
{
    T* ptr_;
public:
    
    Iterator();
    explicit Iterator(T* ptr);

    Iterator(Iterator&& other);
    Iterator(const Iterator& iter);

    ~Iterator();

    Iterator& operator=(const Iterator& other);
    Iterator& operator=(Iterator&& other);

    bool operator==(const Iterator<T>& other) const;
    bool operator!=(const Iterator<T>& other) const;

    bool operator <(const Iterator<T>& other) const;
    bool operator >(const Iterator<T>& other) const;
    bool operator<=(const Iterator<T>& other) const;
    bool operator>=(const Iterator<T>& other) const;
    
    T& operator*();//

    Iterator operator+(const size_t) const;

    template<class U>
    friend Iterator<U> operator+(size_t, const Iterator<U>&);

    Iterator  operator-(const size_t) const;
    ptrdiff_t operator-(const Iterator&) const;

    Iterator& operator++();
    Iterator  operator++(int /*dummyInt = 0*/);
    Iterator& operator--();
    Iterator  operator--(int /*dummyInt = 0*/);

    Iterator& operator+=(const size_t);
    Iterator& operator-=(const size_t);

    Iterator& operator[](size_t) const;
    
};

template<class T>
void swap(Iterator<T> r, Iterator<T> s);
    
template <class T>
Iterator<T>::Iterator()
    : ptr_(nullptr)
    {
    }

template <class T>
Iterator<T>::Iterator(T* ptr)
    : ptr_(ptr) 
    {
    }

template <class T>
Iterator<T>::Iterator(Iterator&& other)
{
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;    
}

template <class T>
Iterator<T>::Iterator(const Iterator& iter)
    : ptr_(iter.ptr_) 
    {
    }

template <class T>
Iterator<T>::~Iterator() 
    {
    }

template <class T>
Iterator<T>& Iterator<T>::operator=(const Iterator& other)
{
    if(other == *this)
        return *this;

    ptr_ = other.ptr_;
    return *this;
}

template <class T>
Iterator<T>& Iterator<T>::operator=(Iterator&& other)
{
    if(other == *this)
        return *this;

    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
    return *this;
}

template <class T>
bool Iterator<T>::operator==(const Iterator<T>& other) const
{
    return ptr_ == other.ptr_;
}

template <class T>
bool Iterator<T>::operator!=(const Iterator<T>& other) const
{
    return !(*this == other);
}

template <class T>
bool Iterator<T>::operator <(const Iterator<T>& other) const
{
    return (ptr_ < other.ptr_);
}

template <class T>
bool Iterator<T>::operator >(const Iterator<T>& other) const
{
    return (ptr_ > other.ptr_);
}

template <class T>
bool Iterator<T>::operator<=(const Iterator<T>& other) const
{
    return (ptr_ <= other.ptr_);
}

template <class T>
bool Iterator<T>::operator>=(const Iterator<T>& other) const
{
    return (ptr_ >= other.ptr_);
}

template <class T>
T& Iterator<T>::Iterator::operator*()
{
    return *ptr_;
}

template <class T>
Iterator<T> Iterator<T>::operator+(const size_t n) const
{
    Iterator<T> result(ptr_);
    result.ptr_ = result.ptr_ + n;
    return result;
}

template <class T>
Iterator<T> operator+(size_t n, const Iterator<T>& other)
{
    return other + n;
}

template <class T>
Iterator<T> Iterator<T>::operator-(const size_t n) const
{
    Iterator<T>   result(ptr_);
    result.ptr_ = result.ptr_ - n;
    return result;    
}

template <class T>
ptrdiff_t Iterator<T>::operator-(const Iterator& other) const
{
    return (this->ptr_ - other.ptr_);
}

template <class T>
Iterator<T>& Iterator<T>::operator++()
{
    ++ptr_;
    return *this;
}
    
template <class T>
Iterator<T> Iterator<T>::operator++(int /*dummyInt = 0*/)
{
    Iterator temp = *this;
    ++*this;
    return temp;
}

template <class T>
Iterator<T>& Iterator<T>::operator--()
{
    --ptr_;
    return *this;
}
    
template <class T>
Iterator<T> Iterator<T>::operator--(int /*dummyInt = 0*/)
{
    Iterator temp = *this;
    --*this;
    return temp;
}

template <class T>
Iterator<T>& Iterator<T>::operator+=(const size_t n)
{
    ptr_ = ptr_ + n;
    return *this;
}

template <class T>
Iterator<T>& Iterator<T>::operator-=(const size_t n)
{
    ptr_ = ptr_ - n;
    return *this;
}

template <class T>
Iterator<T>& Iterator<T>::operator[](size_t idx) const
{
    return (*this + idx);
}


template<class T>
void swap(Iterator<T> r, Iterator<T> s)
{
   Iterator<T> temp = r;
   r = s;
   s = temp;
   return;
}
