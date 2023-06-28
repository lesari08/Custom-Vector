/*
The MIT License (MIT)
Copyright (c) 2023 Leslie Aririguzo
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <memory>
#include <vector>

namespace custom
{

    /**
     *  @brief struct Vector_Memory_Manager is responsible for allocating and
     *   dellallocating memory for an associated Vector class
     *
     *   Instances of this struct are created during the creation of a vector
     *   class object and will be destroyed upon destruction of its associated
     *   vector object.
     *
     *
     *  @tparam Type  Type of element.
     *  @tparam AllocType  Allocator type, default value is allocator<Type>.
     *
     */
    template <class T, class AllocType>
    struct Vector_Memory_Manager
    {
        AllocType alloc;
        T *block_start;
        T *uninitialized_block_start;
        T *block_end;

        Vector_Memory_Manager(const AllocType &_alloc, typename AllocType::size_type n)
            : alloc{_alloc},
              block_start{alloc.allocate(n)},
              uninitialized_block_start{block_start + n},
              block_end{block_start + n}
        {
        }

        Vector_Memory_Manager(Vector_Memory_Manager &&other)
            : alloc{other.alloc},
              block_start{other.block_start},
              uninitialized_block_start{other.uninitialized_block_start},
              block_end{other.block_end}
        {
            other.block_start = other.block_end =
                other.uninitialized_block_start = nullptr;
        }

        Vector_Memory_Manager &operator=(Vector_Memory_Manager &&other);

        typename AllocType::size_type max_size()
        {
            return std::allocator_traits<decltype(alloc)>::max_size(alloc);
        }

        ~Vector_Memory_Manager()
        {
            alloc.deallocate(block_start,
                             block_end - block_start);
        }

        // Vector_Memory_Manager() = delete;
        // no copy operations allowed
        Vector_Memory_Manager(const Vector_Memory_Manager &) = delete;
        Vector_Memory_Manager &operator=(const Vector_Memory_Manager &) = delete;
    };


    /**
     *  @brief Vector_Memory_Manager's move assignment operator 
     *
     *  @tparam Type  Type of element.
     *  @tparam AllocType  Allocator type, default value is allocator<_Type>.
     *  @param other Vector_Memory_Manager object
     *
     */
    template <class T, class A>
    Vector_Memory_Manager<T, A> &Vector_Memory_Manager<T, A>::operator=(Vector_Memory_Manager &&other)
    {
        if (this != &other){
            alloc = other.alloc;
            block_start = other.block_start;
            uninitialized_block_start = other.uninitialized_block_start;
            block_end = other.block_end;

            other.block_start = other.block_end = other.uninitialized_block_start = nullptr;

            //std::swap(*this, other);
        }

        return *this;
    }

    /**

     *  @brief A custom vector container which replicates the 
     *  functionality of std::vector
     *
     *  A vector can be described as a dynamic
     *  C-style array, providing quick, random access to any element.
     *  This container allow shields the user from
     *  memory and size allocation concerns Subscripting ( @c [] ) access is
     *  also provided as with C-style arrays.
     *
     *  @tparam Type  Type of element.
     *  @tparam AllocType  Allocator type, default value is allocator<_Type>.
     *
     */
    template <class T, typename AllocType = std::allocator<T>>
    class Vector
    {
    private:
        Vector_Memory_Manager<T, AllocType> mem_manager;

        void destroyElements();

    public:
        using size_type = size_t;

        Vector(const AllocType &alloc = AllocType());

        explicit Vector(size_type n, const T &val = T(),
                        const AllocType &alloc = AllocType());

        // copy constructors
        Vector(const Vector &other);
        Vector &operator=(const Vector &other) const;

        // move constructors
        Vector(Vector &&other);
        Vector &operator=(Vector &&other);

        ~Vector() { destroyElements(); }

        //--------------------------------------------
        // Get
        //--------------------------------------------

        T at(size_type idx)
        {
            T *ptr = mem_manager.block_start;
            return ptr[idx];
        }

        //--------------------------------------------
        // Capacity and Size
        //--------------------------------------------
        size_type capacity() const
        {
            return mem_manager.block_end - mem_manager.block_start;
        }
        // empty the vector
        void clear() { resize(0); }

        bool empty() { return mem_manager.uninitialized_block_start - mem_manager.block_start == 0; }

        size_type maxSize() const
        {
            throw std::logic_error("Function Not Implemented");
            return 0;
        }

        // increase capacity
        void reserve(size_type);

        void resize(size_type, T = {});

        size_type size() const
        {
            return mem_manager.uninitialized_block_start - mem_manager.block_start;
        }

        void push_back(const T &val);

        //--------------------------------------------
        // Iterators
        //--------------------------------------------

        // //
        // begin();
        // end();
        // TO-DO
        /*
            rbegin()
            rend()
            cbegin()
            cend()
            crbegin()
            crend()
        */
    };

    //------------------------------------------------------------
    // Constuctors
    //------------------------------------------------------------

    template <class T, typename A>
    Vector<T, A>::Vector(size_type n, const T &val, const A &alloc)
        : mem_manager{alloc, n}
    {
        // construct n copies of val (in-place)
        std::uninitialized_fill(mem_manager.block_start,
                                mem_manager.block_start + n, val);
    }

    // copy constructor
    template <class T, typename A>
    Vector<T, A>::Vector(const Vector &other)
        : mem_manager{other.alloc111, other.size()}
    {
        uninitialized_copy(other.begin(), other.end(), mem_manager.block_start);
    }

    // copy assignment
    template <class T, typename A>
    Vector<T, A> &Vector<T, A>::operator=(const Vector &other) const
    {
        Vector<T, A> vec;

        // delete old items

        throw std::logic_error("Function Not Implemented");
        return vec;
    }

    // move constructor
    template <class T, typename A>
    Vector<T, A>::Vector(Vector &&other)
        : mem_manager{other.alloc, other.size()}
    {
        throw std::logic_error("Function Not Implemented");
    }

    // move assignment
    template <class T, typename A>
    Vector<T, A> &Vector<T, A>::operator=(Vector &&other)
    {
        Vector<T, A> vec;
        throw std::logic_error("Function Not Implemented");

        return vec;
    }

    //------------------------------------------------------------
    // Capacity and Size
    //------------------------------------------------------------
    template <class T, typename A>
    void Vector<T, A>::reserve(size_type size_to_reserve)
    {
        if (size_to_reserve < capacity())
            return;

        Vector_Memory_Manager<T, A> new_manager{mem_manager.alloc, size_to_reserve};

        std::uninitialized_copy(mem_manager.block_start,
                                mem_manager.block_start + size(), new_manager.block_start);

        new_manager.uninitialized_block_start = new_manager.block_start + size();
        std::swap(new_manager, mem_manager);
    }

    template <class T, typename A>
    void Vector<T, A>::resize(size_type new_size, T val)
    {
        reserve(new_size);
        if (size() < new_size)
            std::uninitialized_fill(mem_manager.block_start + size(), mem_manager.block_start + new_size, val);
        else // shrink
            destory(mem_manager.alloc, mem_manager.block_start + new_size, mem_manager.block_start + size());

        mem_manager.uninitialized_block_start = mem_manager.block_start + new_size;

        // TO-DO
        // Do we need to reassign mem_block_end as well or leave it as is?
        //  mem_manager.mem_block_end = mem_manager.mem_block_start + new_size;
    }

    /**
     * @brief calls the destructor for each element in Vector
     *
     * @param val the value to add to vector
     * @return void
     */
    template <class T, typename A>
    void Vector<T, A>::push_back(const T &val)
    {

        if (size() == capacity())
            reserve(empty() ? 1 : size() * 2);

        size_type siz = size();
        size_type cap = capacity();

        std::allocator_traits<decltype(mem_manager.alloc)>::construct(mem_manager.alloc, mem_manager.uninitialized_block_start, val);
        ++mem_manager.uninitialized_block_start;
    }

    /**
     * @brief calls the destructor for each element in Vector
     *
     * @return void
     */
    template <class T, typename A>
    void Vector<T, A>::destroyElements()
    {
        for (T *ptr = mem_manager.block_start;
             ptr != mem_manager.uninitialized_block_start; ++ptr)
        {
            std::destroy_at(ptr);
        }

        mem_manager.uninitialized_block_start = mem_manager.block_start;
    }
}