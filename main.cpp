#include <iostream>
#include <vector>
#include <map>
#include <cstring>

#include "linkedlist.hpp"
struct hard
{
    int iValue;
    double dValue;
    hard(int a, double b) : iValue{a}, dValue{b}{std::cout << __PRETTY_FUNCTION__ << std::endl;};
    ~hard(){std::cout << __PRETTY_FUNCTION__ << std::endl;}

    hard(const hard& cpy) = delete; //{std::cout << __PRETTY_FUNCTION__ << std::endl;}

    hard(hard&& mh) noexcept {iValue = mh.iValue; dValue = mh.dValue;  std::cout << __PRETTY_FUNCTION__ << std::endl;}
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
        if(block_ptr == nullptr){
            block_ptr = (T*) malloc(sizeof(T)* sz);
        }
        if(!block_ptr)
            throw std::bad_alloc();
        if(cntr == sz)
        {
            auto ptr = malloc(sizeof(T) * sz);
            if(!ptr)
                throw std::bad_alloc();

            cntr = 0;
            block_ptr = (T*)ptr;
        }

        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return (block_ptr+cntr++);
    };
    void deallocate(T* p, size_t n)
    {
        static int d_cntr = 0;
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        if((d_cntr++ % sz) == 0)
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
        size_t cntr = 0;
        T* block_ptr = nullptr;
};

int main()
{
    //Custom container with std alloc
    //---------------------------------------------------------
    linkedlist<int> llint;
    for(size_t i = 0; i < 12; ++i)
    {
        llint.emplace(i);
    }
    for(auto it = llint.begin(); it != llint.end(); ++it)
    {
        std::cout << "Int value = " << (*it) << std::endl;
    }

    // Custom container with custom allocator<int>
    //----------------------------------------------------------
    linkedlist<int,logging_allocater<int,10>> llintCustom;
    for(size_t i = 0; i < 12; ++i)
    {
        llintCustom.emplace(i);
    }
    for(auto it = llintCustom.begin(); it != llintCustom.end(); ++it)
    {
        std::cout << "Int value = " << (*it) << std::endl;
    }

    // Custom container with custom allocator<hard>
    //-----------------------------------------------------------
    linkedlist<hard,logging_allocater<hard,10>> ll;
    for(size_t i = 0; i < 11; ++i)
    {
        ll.emplace(hard(i,i));
    }
    for(auto it = ll.begin(); it != ll.end(); ++it)
    {
        std::cout << "Hard value = (" << (*it).iValue << ", " << (*it).dValue << " )" << std::endl;
    }

    // Map with std::allocator<int>
    //------------------------------------------------------------
    auto  v_i = std::map<int,int>{};
    for(size_t i = 0; i < 11; ++i)
    {
        v_i.emplace(i,i);
    }

    // Map with custom allocator<int>
    //------------------------------------------------------------
    auto  v_iCustom = std::map<int,int,std::less<int>,logging_allocater<std::pair<const int, int>,10>>{};
    for(size_t i = 0; i < 11; ++i)
    {
        v_iCustom.emplace(i,i);
    }

    // Map with std::allocator<hard>
    //------------------------------------------------------------
    auto  v = std::map<int,hard>{};
    for(size_t i = 0; i < 11; ++i)
    {
        v.emplace(std::piecewise_construct,std::forward_as_tuple(i),std::forward_as_tuple(i,i));
    }

    //Standart map with custom allocator<hard>
    //----------------------------------------------------------------------------------------------
    auto  vCustom = std::map<int,hard,std::less<int>,logging_allocater<std::pair<const int, hard>,10>>{};
    for(size_t i = 0; i < 11; ++i)
    {
        vCustom.emplace(std::piecewise_construct,std::forward_as_tuple(i),std::forward_as_tuple(i,i));
    }

}