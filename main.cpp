#include <iostream>
#include <vector>
#include <map>

struct hard
{
    int iValue;
    double dValue;
    hard(int a, double b) : iValue{a}, dValue{b}{std::cout << __PRETTY_FUNCTION__ << std::endl;};
    ~hard(){std::cout << __PRETTY_FUNCTION__ << std::endl;}

    hard(const hard& cpy) = delete; //{std::cout << __PRETTY_FUNCTION__ << std::endl;}

    hard(hard&& ) noexcept {std::cout << __PRETTY_FUNCTION__ << std::endl;}
};

template <typename T, size_t sz>
struct logging_allocater
{
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind{
        using other = logging_allocater<U, sz>;
    };

    T* allocate(size_t n)
    {
        if(last_ptr == nullptr){
            ptr = malloc(sizeof(T)* sz);
            last_ptr = (T*)ptr;
        }
        if(!ptr)
            throw std::bad_alloc();
        if((last_ptr - (T*)ptr) == sz*sizeof(T))
        {
            //realoc
        }

        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return last_ptr++;
    };
    void deallocate(T* p, size_t n)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        if(--last_ptr == (T*)ptr)
            std::free(p);
    };

    template <typename U, typename... Args>
    void construct(U* p, Args... args)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        new(p) U(std::forward<Args>(args) ... );
    }

    void destroy(T* p)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        p->~T();
    }
    private:
        void* ptr = nullptr;
        T* last_ptr = nullptr;
};

int main()
{
    // std::vector<hard,logging_allocater<hard>> v;
    // for(size_t i = 0; i < 5; ++i)
    // {
    //     v.emplace_back(i,i);
    // }

    // auto  v = std::map<int,int,std::less<int>,logging_allocater<std::pair<const int, int>,10>>{};
    // for(size_t i = 0; i < 5; ++i)
    // {
    //     // v.emplace(std::piecewise_construct,std::forward_as_tuple(i),std::forward_as_tuple(i,i));
    //     v.emplace(i,i);
    // }
    auto  v = std::map<int,hard,std::less<int>>{};
    for(size_t i = 0; i < 5; ++i)
    {
        v.emplace(std::piecewise_construct,std::forward_as_tuple(i),std::forward_as_tuple(i,i));
        // v.emplace(i,i);
    }
}