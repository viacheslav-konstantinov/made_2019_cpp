/*
Практическая работа 8
Написать свой контейнер Vector аналогичный std::vector, аллокатор и итератор произвольного доступа для него. 
Из поддерживаемых методов достаточно operator[], push_back, pop_back, empty, size, clear, begin, end, rbegin, rend, resize, reserve.
*/


#include <iostream>
#include <cstring>
#include <limits>
#include <vector>

template <class T>
class Allocator
{
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

private:
    size_type reserved = 0;   // вместимость выделенного участка памяти
    pointer begin_ = nullptr; // указатель на начало аллоцированной памяти

public:
    
    pointer address(reference x) const noexcept
    {
        return std::addressof(x);    
    }
    
    const_pointer address(const_reference x) const noexcept
    {
        return std::addressof(x); 
    }
    
    pointer allocate(size_type n, const_pointer hint = 0)
    {
        if(!n) // если запросили нуль элементов, то ничего не делаем и возвращаем nullptr
            return nullptr;
        
        if(n <= reserved ) // если достаточное количество памяти уже выделено,
            return begin_; // тоже ничего не делаем и возвращаем begin_
        
        if(!begin_) // если память не выделена совсем, аллоцируем
        {
            pointer result = new T[n];
            reserved = n;
            begin_ = result;
            return result;
        }
        else // если память выделена, но её недостаточно, то выделяем участок большего
             // размера и копируем туда информацию из ранее выделенного участка
             // и деаллоцируем ранее выделенный участок памяти
        {
            pointer result = new T[n];
            std::memcpy(result, begin_, reserved*sizeof(T));
            delete[] begin_;
            reserved = n;
            begin_ = result;
            return result;            
        }
    }
    

    void deallocate(pointer p, size_type n)
    {
        if(p != begin_) // удалять из середины нельзя
            return;

        if(n >= reserved)
        {
            delete[] p;
            reserved = 0;
            begin_ = nullptr;
            return;
        }
        
        // если удаляются не все данные,
        // перекопируем оставшиеся в новый учаток памяти
        pointer buffer = new T[reserved - n];
        std::memcpy(buffer, begin_ + n, (reserved - n)*sizeof(T));

        delete[] p;
        begin_ = buffer;
        reserved -= n;
    }
    
    size_type max_size() const
    {
        return std::numeric_limits<size_type>::max();
    }
    
    template<class... Args>
    void construct(pointer p, Args&&... args)
    {
        if(begin_ && p >= begin_ && p < begin_+reserved)
            new (p) T(std::forward<Args>(args)...);
    }

    void destroy(pointer p)
    {
        if(begin_ && p >= begin_ && p < begin_+reserved)
            p->~T();
    }

    pointer get_begin() const
    {
        return begin_;
    }

    size_type size_reserved() const
    {
        return reserved;
    }

};


template <class T>
class Iterator
    : public std::iterator<std::forward_iterator_tag, T>
{
    T* ptr_;
public:
    explicit Iterator(T* ptr)
        : ptr_(ptr) {}

    Iterator(const Iterator& iter)
        : ptr_(iter.ptr_) {}
    
    ~Iterator() {}
    
    Iterator(Iterator&& other)
    {
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;    
    }
    
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
};


template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using size_type = size_t;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
	
    using allocator_type = Alloc;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    
    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // аллоцируется всегда в 2 раза больше памяти, чем нужно для массива,
    // чтобы уменьшить количество вызовов для допольнительного расширения памяти
    explicit Vector(size_type count)
    {
        value_type randomT;
        begin_ = alloc_.allocate(2*count);
        reserved = 2*count;
        for(size_type i = 0; i < count; ++i)
            alloc_.construct(begin_ + i, randomT);
        used = count;
    }

    Vector(size_type count, const value_type& defaultValue)
    {
        begin_ = alloc_.allocate(2*count);
        reserved = 2*count;
        
        for(size_type i = 0; i < count; ++i)
            alloc_.construct(begin_ + i, defaultValue);
        
        used = count;
    }

    Vector(std::initializer_list<value_type> init)
    {

        begin_ = alloc_.allocate(2*init.size());
        reserved = 2*init.size();
        
        size_type i = 0;
        for(auto& obj : init)
        {
            alloc_.construct(begin_ + i, obj);
            ++i;
        }
        used = init.size();      
    }

    Vector() {}
    
    ~Vector() 
    {
        alloc_.deallocate(begin_, reserved);
    }
    
    Vector& operator=(const Vector& other) = delete;
    Vector(Vector&& other) = delete;
    Vector& operator=(Vector&& other) = delete;

    
    iterator begin() noexcept
    {
        return iterator(begin_);
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(begin_);
    }

    const_iterator cbegin() const noexcept
    {
        return const_iterator(begin_);
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(begin_);
    }

    iterator end() noexcept
    {
        return iterator(begin_ + used);
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin_ + used);
    }

    const_iterator cend() const noexcept
    {
        return const_iterator(begin_ + used);
    }
    
    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin_ + used);
    }

    void push_back(value_type&& value)
    {
        //если аллоцированной памяти достаточно, просто вызываем конструктор
        // и увеличииваем used
        if(used != reserved)
        {
            alloc_.construct(begin_ + used, value);
            ++used;
            return;
        }

        // если аллоцированной памяти недостаточно, аллоцироуем участок в 2 раза больше
        // перерос данных произойдет внутри вызова alloc_.allocate
        pointer buffer = alloc_.allocate(2*reserved);
        alloc_.construct(buffer + used, value);
        
        begin_ = buffer;
        reserved = 2*reserved;
        ++used;
    }

    void push_back(const value_type& value)
    {
        if(used < reserved)
        {
            alloc_.construct(begin_ + used, value);
            ++reserved;
            return;
        }

        pointer buffer = alloc_.allocate(2*reserved);    
        alloc_.construct(buffer + used, value);
        
        begin_ = buffer;
        reserved = reserved*2;
        ++used;
    }
    
    // вызываем конструктор у последнего элемента, если он есть
    void pop_back()
    {
        if(used > 0)
        {
            alloc_.destroy(begin_ + used - 1);
            --used;
            return;
        }
    }
    
    //проверка на выход за границы, как и в обычном std::vector, не производится
    reference       operator[]( size_type pos )
    {
        return *iterator(begin_ + pos);
    }

    const_reference operator[]( size_type pos ) const
    {
        return *const_iterator(begin_ + pos);
    }
    
    bool empty() const
    {
        return (used == 0);
    }

    size_type size() const
    {
        return used;
    }

    void reserve(size_type count)
    {
        //аллоцируем только, если до этого не было выделено достаточно памяти
        if(reserved < count)
        {
            pointer buffer = alloc_.allocate(count);
            begin_ = buffer;
            reserved = count;
            return;
        }
    }

    size_type capacity() const noexcept
    {
        return reserved;
    }

    // старается увеличить used до newSize
    // только если памяти не хватает для расширения, аллоцироуем дополнительно
    void resize(size_type newSize)
    {
        if(newSize > reserved)
            reserve(2*newSize);
    
        if(newSize > used)
        {
            // вставляем на новые места объекты по умолчанию
            value_type randomT;
            for(size_type i = used; i < newSize; ++i)
                alloc_.construct(begin_ + i, randomT);
            
            used = newSize;
            return;
        }

        // если новый размер меньше used, вызываем деструктор для элементов
        // с индексами [newSize, used)
        if(newSize < used)
        {
            for(size_type i = newSize; i < used; ++i)
                alloc_.destroy(begin_ + i);
            
            used = newSize;
            return;
        }
    }

    void resize(size_type newSize, const value_type& defaultValue)
    {
        if(newSize > reserved)
            reserve(2*newSize);
    
        if(newSize > used)
        {
            for(size_type i = used; i < newSize; ++i)
                alloc_.construct(begin_ + i, defaultValue);
            
            used = newSize;
            return;
        }

        if(newSize < used)
        {
            for(size_type i = newSize; i < used; ++i)
                alloc_.destroy(begin_ + i);
            
            used = newSize;
            return;
        }
    }    

    // вызываем деструктор для всех элементов массива
    // память не деаллоциоруется
    void clear() noexcept
    {
        for(size_type i = 0; i < used; ++i)
            alloc_.destroy(begin_ + i);
        
        used = 0;
    }

private:
    Alloc alloc_; // аллокатор
    pointer begin_ = nullptr; // указатель на начало участка выделенной памяти
    size_type reserved = 0;   // вместимость
    size_type used = 0;       // текущий размер массива
};


int main()
{
    std::cout << "This program demostrates the realizations of the class Vector, similar to the std::vector one\n";
    std::cout << "and the auxiliary classes of Iterator and Allocator\n";
    
    std::cout << "Creation of a Vector with a list {0, 1, 2, 3, 4, 5}\n";
    Vector<int> v = {0, 1, 2, 3, 4, 5};

    std::cout << "Elements of the array:\n";
    size_t i = 0;    
    for(auto it = v.begin(); it != v.end(); ++it )
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << *it << std::endl;
        ++i;
    }

    std::cout << "size = " << v.size() << std::endl;
    std::cout << "capacity = " << v.capacity() << std::endl;
    std::cout << std::endl;
    
    std::cout << "Pushing back a new element 23\n";
    v.push_back(23);
    std::cout << "Calling v[6]\n";
    std::cout << v[6] << std::endl;
    std::cout << "Checking new size of the Vector:\n";
    std::cout << v.size() << std::endl;
    std::cout << std::endl;

    std::cout << "Resizing the Vector up to the size of 20 with elements = 100\n";
    v.resize(20, 100);

    std::cout << "Elements of the array:\n";
    i = 0;
    for(auto it = v.begin(); it != v.end(); ++it )
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << *it << std::endl;
        ++i;
    }

    std::cout << "size = " << v.size() << std::endl;
    std::cout << "capacity = " << v.capacity() << std::endl;
    std::cout << std::endl;

    std::cout << "Deleting the last element of the Vector\n";
    v.pop_back();

    std::cout << "Elements of the array:\n";
    for(i = 0; i < v.size(); ++i )
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << v[i] << std::endl;
    }
    std::cout << "size = " << v.size() << std::endl;
    std::cout << "capacity = " << v.capacity() << std::endl;
    std::cout << std::endl;

    std::cout << "Reserving memory for 100 elements\n";
    v.reserve(100);

    std::cout << "Elements of the array:\n";
    for(i = 0; i < v.size(); ++i)
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << v[i] << std::endl;
    }
    std::cout << "size = " << v.size() << std::endl;
    std::cout << "capacity = " << v.capacity() << std::endl;
    std::cout << std::endl;

    std::cout << "Comparing with the std::vector\n";
    std::cout << "Creation of a std::vector with a list {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}\n";
    std::vector<int> v2 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    

    std::cout << "Elements of the array:\n";
    i = 0;    
    for(auto it = v2.begin(); it != v2.end(); ++it )
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << *it << std::endl;
        ++i;
    }

    std::cout << "size = " << v2.size() << std::endl;
    std::cout << "capacity = " << v2.capacity() << std::endl;
    std::cout << std::endl;

    std::cout << "Pushing back a new element 23\n";
    v2.push_back(123);   
    std::cout << "Reserving memory for an array of the size 100\n";
    v2.reserve(100);

    std::cout << "Elements of the array:\n";
    i = 0;    
    for(auto it = v2.begin(); it != v2.end(); ++it )
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << *it << std::endl;
        ++i;
    }

    std::cout << "size = " << v2.size() << std::endl;
    std::cout << "capacity = " << v2.capacity() << std::endl;

    
    return 0;
}