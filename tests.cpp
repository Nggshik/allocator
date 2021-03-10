#define BOOST_TEST_MODULE allocator

#include <boost/test/unit_test.hpp>
#include "linkedlist.hpp"

BOOST_AUTO_TEST_SUITE(Allocate_test)

BOOST_AUTO_TEST_CASE(custom_container_test)
{
    linkedlist<int> ll;
    for(size_t i = 0; i < 10; ++i)
    {
        ll.emplace(i);
    }
    
    size_t check_val = 0;
    for(auto& val : ll)
    { 
        BOOST_CHECK(val==check_val++);
    }
}

BOOST_AUTO_TEST_SUITE_END()