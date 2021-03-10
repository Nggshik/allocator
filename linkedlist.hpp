#pragma once

template <typename T, typename Allocator = std::allocator<T>>
struct linkedlist
{
        struct node{
            T value;
            node* next;
            node(T&& val) : value(std::forward<T>(val)),next(nullptr){};
        };
        struct iter
        {
            iter(node* data) : data(data){};
            iter& operator++(){data = data->next; return *this;}
            iter operator++(int){data = data->next; return iter(data);}
            T& operator*() {if(data != nullptr) return data->value;}
            bool operator!=(const iter& it){return it.data != data;} 
            private:
                node* data;
        };
    
    linkedlist() { data = (node*)malloc(sizeof(node)); if(!data) throw std::bad_alloc(); data->next = nullptr;};
    ~linkedlist() 
    {
        node* ptr = data->next;
        free(data);
        while(ptr != nullptr)
        {
            data = ptr->next;
            alloc.destroy(ptr);
            alloc.deallocate(ptr, 1);
            ptr = data;
        }
    };
    void emplace(T&& val)
    {
        node* ptr = data;
        while(ptr->next != nullptr)
            ptr = ptr->next;
        ptr->next = alloc.allocate(1);
        alloc.construct(ptr->next,std::forward<T>(val));
    }

    iter begin() { return iter(data->next);}
    iter end() {return iter(nullptr);}

    private:     
        node* data;
        typename Allocator::template rebind<node>::other alloc;
};

