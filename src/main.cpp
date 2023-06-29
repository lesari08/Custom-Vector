
#include <iostream>
#include <vector>
#include "CustomVector.h"

int main()
{
    std::allocator<int> alloc;

   // int* start = alloc.allocate(5);
   // std::uninitialized_fill(start, start + 5, 7);


   std::vector<int> vec2;
   vec2.push_back(4);
    custom::Vector<int> vec(3,7);
   vec.push_back(5);
     vec.push_back(9);

    //int* ptr = start;
    for(int i = 0; i < 6; ++i)
    {
        std::cout<<vec.at(i)<<"\n";
       
    }

    custom::Vector<int>::Iterator itr(vec.begin());
    ++itr;
   std::cout<<"itr:"<<*itr<<"\n";

    
    
}