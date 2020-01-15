#include <limits>
#include <memory>

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

        return (pointer) malloc(sizeof(value_type) * n);
    }

    void deallocate(pointer p, size_type n)
    {
        /*if( p && (size_type*)p[-1] == n)*/ free(p);
    }

    size_type max_size() const
    {
        return std::numeric_limits<size_type>::max();
    }

    template<class... Args>
    void construct(pointer p, Args&&... args)
    {
        if(p)
            new (p) T(std::forward<Args>(args)...);
    }

    void destroy(pointer p)
    {
        if(p)
            p->~T();
    }

};