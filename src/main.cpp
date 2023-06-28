
#include <iostream>
#include <vector>
#include "CustomVector.h"

int main()
{
    std::allocator<int> alloc;

   // int* start = alloc.allocate(5);
   // std::uninitialized_fill(start, start + 5, 7);

   
    custom::Vector<int> vec(3,7);
   vec.push_back(5);
     vec.push_back(9);

    //int* ptr = start;
    for(int i = 0; i < 6; ++i)
    {
        std::cout<<vec.at(i)<<"\n";
       
    }
   // std::cout<<vec.at(1)<<"\n";

    
    
}