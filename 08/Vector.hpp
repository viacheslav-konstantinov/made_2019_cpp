#include "Allocator.hpp"
#include "Iterator.hpp"

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

    Vector(const std::initializer_list<value_type>& init)
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
            alloc_.construct(begin_ + used, std::move(value));
            ++used;
            return;
        }

        // если аллоцированной памяти недостаточно, аллоцироуем участок в 2 раза больше
        pointer buffer = alloc_.allocate(2*reserved);
        
        //копируем данные
        for(size_type idx = 0; idx < used; ++idx)
            alloc_.construct(buffer + idx, *(begin_ + idx));
        
        //вставляем новый элемент
        alloc_.construct(buffer + used, std::move(value));
        
        //деаллоцируем предыдущий участок памяти
        alloc_.deallocate(begin_, reserved);

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

        pointer buffer = alloc_.allocate(reserved*2);
        
        for(size_type idx = 0; idx < used; ++idx)
            alloc_.construct(buffer + idx, *(begin_ + idx));
        alloc_.construct(buffer + used, value);
        
        alloc_.deallocate(begin_, reserved);

        begin_ = buffer;
        reserved = reserved*2;
        ++used;
    }

    // вызываем конструктор у последнего элемента, если этот элемент существует
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
        return *(begin_ + pos);
    }

    const_reference operator[]( size_type pos ) const
    {
        return *(begin_ + pos);
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

            //копируем данные
            for(size_type idx = 0; idx < used; ++idx)
                alloc_.construct(buffer + idx, *(begin_ + idx));
            
            //деаллоцируем предыдущий участок памяти
            alloc_.deallocate(begin_, reserved);

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