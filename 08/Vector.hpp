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

    explicit Vector(size_type count);

    Vector(size_type count, const value_type& defaultValue);

    Vector(const std::initializer_list<value_type>& init);

    Vector();

    ~Vector();

    Vector& operator=(const Vector& other) = delete;
    Vector(Vector&& other) = delete;
    Vector& operator=(Vector&& other) = delete;


    iterator               begin()   noexcept;
    reverse_iterator       rbegin()  noexcept;
    const_iterator         cbegin()  const noexcept;
    const_reverse_iterator crbegin() const noexcept;

    iterator               end()   noexcept;
    reverse_iterator       rend()  noexcept;
    const_iterator         cend()  const noexcept;
    const_reverse_iterator crend() const noexcept;

    void push_back(value_type&& value);
    void push_back(const value_type& value);

    void pop_back();

    reference       operator[]( size_type pos );
    const_reference operator[]( size_type pos ) const;

    bool empty() const;

    size_type size() const;

    void reserve(size_type count);

    size_type capacity() const noexcept;

    void resize(size_type newSize);
    void resize(size_type newSize, const value_type& defaultValue);    

    void clear() noexcept;

private:
    Alloc alloc_; // аллокатор
    pointer begin_ = nullptr; // указатель на начало участка выделенной памяти
    size_type reserved = 0;   // вместимость
    size_type used = 0;       // текущий размер массива

    //вспомогательные методы для push_back и resize,
    //чтобы не копипастить
    void push_back_(value_type&& value);
    void resize_(size_type newSize, const value_type& value);
};


// аллоцируется всегда в 2 раза больше памяти, чем нужно для массива,
// чтобы уменьшить количество вызовов для допольнительного расширения памяти
template <class T, class Alloc>
Vector<T, Alloc>::Vector(size_type count)
{
    value_type randomT;
    begin_ = alloc_.allocate(2*count);
    reserved = 2*count;
    for(size_type i = 0; i < count; ++i)
        alloc_.construct(begin_ + i, randomT);
    used = count;
}

template <class T, class Alloc>
Vector<T, Alloc>::Vector(size_type count, const value_type& defaultValue)
    {
        begin_ = alloc_.allocate(2*count);
        reserved = 2*count;

        for(size_type i = 0; i < count; ++i)
            alloc_.construct(begin_ + i, defaultValue);

        used = count;
    }

template <class T, class Alloc>
Vector<T, Alloc>::Vector(const std::initializer_list<value_type>& init)
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

template <class T, class Alloc>
Vector<T, Alloc>::Vector() 
    {
    }

template <class T, class Alloc>
Vector<T, Alloc>::~Vector() 
{
    alloc_.deallocate(begin_, reserved);
}

template <class T, class Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::begin() noexcept
{
    return iterator(begin_);
}

template <class T, class Alloc>
typename Vector<T, Alloc>::reverse_iterator Vector<T, Alloc>::rbegin() noexcept
{
    return reverse_iterator(begin_);
}

template <class T, class Alloc>
typename Vector<T, Alloc>::const_iterator Vector<T, Alloc>::cbegin() const noexcept
{
    return const_iterator(begin_);
}

template <class T, class Alloc>
typename Vector<T, Alloc>::const_reverse_iterator Vector<T, Alloc>::crbegin() const noexcept
{
    return const_reverse_iterator(begin_);
}

template <class T, class Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::end() noexcept
{
    return iterator(begin_ + used);
}

template <class T, class Alloc>
typename Vector<T, Alloc>::reverse_iterator Vector<T, Alloc>::rend() noexcept
{
    return reverse_iterator(begin_ + used);
}

template <class T, class Alloc>
typename Vector<T, Alloc>::const_iterator Vector<T, Alloc>::cend() const noexcept
{
    return const_iterator(begin_ + used);
}

template <class T, class Alloc>
typename Vector<T, Alloc>::const_reverse_iterator Vector<T, Alloc>::crend() const noexcept
{
    return const_reverse_iterator(begin_ + used);
}

template <class T, class Alloc>
void Vector<T, Alloc>::push_back_(value_type&& value)
{
    //если аллоцированной памяти достаточно, просто вызываем конструктор
    // и увеличииваем used
    if(used != reserved)
    {
        alloc_.construct(begin_ + used, std::forward<value_type>(value));
        ++used;
        return;
    }

    // если аллоцированной памяти недостаточно, аллоцироуем участок в 2 раза больше
    pointer buffer = alloc_.allocate(2*reserved);
        
    //копируем данные
    for(size_type idx = 0; idx < used; ++idx)
        alloc_.construct(buffer + idx, *(begin_ + idx));
        
    //вставляем новый элемент
    alloc_.construct(buffer + used, std::forward<value_type>(value));
        
    //деаллоцируем предыдущий участок памяти
    alloc_.deallocate(begin_, reserved);

    begin_ = buffer;
    reserved = 2*reserved;
    ++used;
}


template <class T, class Alloc>
void Vector<T, Alloc>::push_back(value_type&& value)
{
    push_back_(std::move(value));
}

template <class T, class Alloc>
void Vector<T, Alloc>::push_back(const value_type& value)
{
    push_back_(value);
}

// вызываем конструктор у последнего элемента, если этот элемент существует
template <class T, class Alloc>
void Vector<T, Alloc>::pop_back()
{
    if(used > 0)
    {
        alloc_.destroy(begin_ + used - 1);
        --used;
        return;
    }
}

//проверка на выход за границы, как и в обычном std::vector, не производится
template <class T, class Alloc>
typename Vector<T, Alloc>::reference  Vector<T, Alloc>::operator[]( size_type pos )
{
    return *(begin_ + pos);
}

template <class T, class Alloc>
typename Vector<T, Alloc>::const_reference Vector<T, Alloc>::operator[]( size_type pos ) const
{
    return *(begin_ + pos);
}

template <class T, class Alloc>
bool Vector<T, Alloc>::empty() const
{
    return (used == 0);
}

template <class T, class Alloc>
typename Vector<T, Alloc>::size_type Vector<T, Alloc>::size() const
{
    return used;
}

template <class T, class Alloc>
void Vector<T, Alloc>::reserve(size_type count)
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

template <class T, class Alloc>
typename Vector<T, Alloc>::size_type Vector<T, Alloc>::capacity() const noexcept
{
    return reserved;
}

// старается увеличить used до newSize
// только если памяти не хватает для расширения, аллоцироуем дополнительно
template <class T, class Alloc>
void Vector<T, Alloc>::resize_(size_type newSize, const value_type& value)
{
    if(newSize > reserved)
        reserve(2*newSize);

    if(newSize > used)
    {
        // вставляем на новые места value
        for(size_type i = used; i < newSize; ++i)
            alloc_.construct(begin_ + i, value);

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

template <class T, class Alloc>
void Vector<T, Alloc>::resize(size_type newSize)
{
    value_type randomT;
    resize_(newSize, randomT);
}

template <class T, class Alloc>
void Vector<T, Alloc>::resize(size_type newSize, const value_type& defaultValue)
{
    resize_(newSize, defaultValue);
}

// вызываем деструктор для всех элементов массива
// память не деаллоциоруется
template <class T, class Alloc>
void Vector<T, Alloc>::clear() noexcept
{
    for(size_type i = 0; i < used; ++i)
        alloc_.destroy(begin_ + i);

    used = 0;
}
