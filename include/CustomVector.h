/*******************************************************************************
 *  @file CustomVector.h
 *  @brief This file contains methods that define and implement a custom Vector
 *
 *  @author Leslie Aririguzo
 *******************************************************************************/

#ifndef CUSTOM_VECTOR_H
#define CUSTOM_VECTOR_H 1

#include <memory>

namespace custom
{
    /*******************************************************************************
     * struct Vector_Memory_Manager
     *
     *   @brief responsible for allocating and
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
     *******************************************************************************/
    template <class T, class AllocType>
    struct Vector_Memory_Manager
    {
        Vector_Memory_Manager(const AllocType &_alloc, typename AllocType::size_type n)
            : alloc{_alloc}, block_start{alloc.allocate(n)},
              uninitialized_block_start{block_start + n},
              block_end{block_start + n}
        {
        }

        Vector_Memory_Manager(Vector_Memory_Manager &&other)
            : alloc{other.alloc}, block_start{other.block_start},
              uninitialized_block_start{other.uninitialized_block_start},
              block_end{other.block_end}
        {
            other.block_start = other.block_end =
                other.uninitialized_block_start = nullptr;
        }

        Vector_Memory_Manager &operator=(Vector_Memory_Manager &&other);

        Vector_Memory_Manager() = delete;
        // no copy operations allowed
        Vector_Memory_Manager(const Vector_Memory_Manager &) = delete;
        Vector_Memory_Manager &operator=(const Vector_Memory_Manager &) = delete;

        ~Vector_Memory_Manager()
        {
            alloc.deallocate(block_start,
                             block_end - block_start);
        }

        typename AllocType::size_type max_size()
        {
            return std::allocator_traits<decltype(alloc)>::max_size(alloc);
        }

        AllocType alloc; // TODO: replace the allocator with allocator_traits
        T *block_start;
        T *uninitialized_block_start;
        T *block_end;
    };

    /*******************************************************************************
     * class Vector
     * 
     *  @brief A custom vector container which replicates the
     *  functionality of std::vector
     *
     *  A vector can be described as a dynamic
     *  C-style array, providing quick, random access to any element.
     *  This container allow shields the user from
     *  memory and size allocation concerns
     *
     *  @tparam Type  Type of element.
     *  @tparam AllocType  Allocator type, default value is allocator<_Type>.
     *
     *******************************************************************************/
    template <class T, typename AllocType = std::allocator<T>>
    class Vector
    {

    public:
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;

        class Iterator
        {
        public:
            Iterator(T *ptr) : m_ptr(ptr) {}

            T &operator*() const { return *m_ptr; }
            T *operator->() const { return m_ptr; }

            Iterator &operator++()
            {
                ++m_ptr;
                return *this;
            }

            Iterator &operator--()
            {
                --m_ptr;
                return *this;
            }

            Iterator &operator+=(difference_type n)
            {
                m_ptr += n;
                return *this;
            }
            Iterator &operator-=(difference_type n)
            {
                m_ptr -= n;
                return *this;
            }

            Iterator operator+(difference_type n) const { return Iterator(m_ptr + n); }
            Iterator operator-(difference_type n) const { return Iterator(m_ptr - n); }

            difference_type operator-(const Iterator &other) const { return m_ptr - other.m_ptr; }

            T &operator[](difference_type n) const { return *(m_ptr + n); }

            bool operator==(const Iterator &other) const { return m_ptr == other.m_ptr; }
            bool operator!=(const Iterator &other) const { return m_ptr != other.m_ptr; }
            bool operator<(const Iterator &other) const { return m_ptr < other.m_ptr; }
            bool operator>(const Iterator &other) const { return m_ptr > other.m_ptr; }
            bool operator<=(const Iterator &other) const { return m_ptr <= other.m_ptr; }
            bool operator>=(const Iterator &other) const { return m_ptr >= other.m_ptr; }

        private:
            T *m_ptr;
        };

        Vector(const AllocType &alloc = AllocType());

        explicit Vector(size_type n, const T &val = T(),
                        const AllocType &alloc = AllocType());

        Vector(const Vector &other);
        Vector(Vector &&other);
        Vector &operator=(const Vector &other) const;
        Vector &operator=(Vector &&other);

        ~Vector() { destroyElements(); }

        T at(size_type idx) { return *(mem_manager.block_start + idx); }
        T operator[](size_type idx) { return at(idx); }

        void push_back(const T &val);
        void insert(size_type index, const T &val);

        size_type capacity() const
        {
            return mem_manager.block_end - mem_manager.block_start;
        }
        bool empty() const
        {
            return mem_manager.uninitialized_block_start - mem_manager.block_start == 0;
        }

        size_type maxSize() const;
        void reserve(size_type);
        void resize(size_type, T = {});
        void clear() { resize(0); }

        size_type size() const
        {
            return mem_manager.uninitialized_block_start - mem_manager.block_start;
        }

        //--------------------------------------------
        // Iterator Methods
        //--------------------------------------------
        using const_iterator = const Iterator;
        Iterator begin() { return Iterator(mem_manager.block_start); }
        const Iterator begin() const { return const_iterator(mem_manager.block_start); }
        Iterator end() { return Iterator(mem_manager.uninitialized_block_start); }
        const Iterator end() const { return const_iterator(mem_manager.uninitialized_block_start); }

    private:
        Vector_Memory_Manager<T, AllocType> mem_manager;
        void destroyElements();
    };

    /*******************************************************************************
     *  @brief Vector_Memory_Manager:: move assignment operator
     *
     *  @tparam Type  Type of element.
     *  @tparam AllocType  Allocator type, default value is allocator<_Type>.
     *  @param other Vector_Memory_Manager object
     *
     *******************************************************************************/
    template <class T, class A>
    Vector_Memory_Manager<T, A> &
    Vector_Memory_Manager<T, A>::operator=(Vector_Memory_Manager &&other)
    {
        if (this != &other)
        {
            alloc = other.alloc;
            block_start = other.block_start;
            uninitialized_block_start = other.uninitialized_block_start;
            block_end = other.block_end;

            other.block_start = other.block_end = other.uninitialized_block_start = nullptr;

            // std::swap(*this, other);
        }

        return *this;
    }


    /*******************************************************************************
     * @brief default constructor
     *
     * @param n size
     * @param val default value for constructed objects
     * @param alloc allocator
     * @return n/a
     *******************************************************************************/
    template <class T, typename A>
    Vector<T, A>::Vector(const A &alloc)
        : mem_manager{alloc, 0}
    {
        // construct n copies of val (in-place)
     //   std::uninitialized_fill(mem_manager.block_start,
      //                          mem_manager.block_start + n, val);
    }

    /*******************************************************************************
     * @brief parameter constructor
     *
     * @param n size
     * @param val default value for constructed objects
     * @param alloc allocator
     * @return n/a
     *******************************************************************************/
    template <class T, typename A>
    Vector<T, A>::Vector(size_type n, const T &val, const A &alloc)
        : mem_manager{alloc, n}
    {
        // construct n copies of val (in-place)
        std::uninitialized_fill(mem_manager.block_start,
                                mem_manager.block_start + n, val);
    }

    /*******************************************************************************
     * @brief copy constructor
     *
     * @param other vector object
     * @return n/a
     *******************************************************************************/
    template <class T, typename A>
    Vector<T, A>::Vector(const Vector &other)
        : mem_manager{other.alloc111, other.size()}
    {
        uninitialized_copy(other.begin(), other.end(), mem_manager.block_start);
    }

    /*******************************************************************************
     * @brief copy assignment operator
     *
     * @param other vector object
     * @return Vector reference
     *******************************************************************************/
    template <class T, typename A>
    Vector<T, A> &Vector<T, A>::operator=(const Vector &other) const
    {
        Vector<T, A> vec;

        // delete old items before copying vals

        throw std::logic_error("Function Not Implemented");
        return vec;
    }

    /*******************************************************************************
     * @brief move constructor
     *
     * @param other vector object
     * @return n/a
     *******************************************************************************/
    template <class T, typename A>
    Vector<T, A>::Vector(Vector &&other)
        : mem_manager{other.alloc, other.size()}
    {
        throw std::logic_error("Function Not Implemented");
    }

    /*******************************************************************************
     * @brief move assignment operator
     *
     * @param other vector object
     * @return Vector reference
     *******************************************************************************/
    template <class T, typename A>
    Vector<T, A> &Vector<T, A>::operator=(Vector &&other)
    {
        Vector<T, A> vec;
        throw std::logic_error("Function Not Implemented");

        return vec;
    }

    /*******************************************************************************
     * reserve
     *
     * @brief reserve specified amount of memory for vector
     *
     * @param size_to_reserve size of memory to allocate
     * @return n/a
     *******************************************************************************/
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

    /*******************************************************************************
     * resize
     *
     * @brief resize vector based on given new_size
     *
     * @param new_size number of objects in vector
     *  after the operation has completed
     * @param T val the value that will be assigned
     *  to uninitialized memory spaces
     * @return n/a
     *******************************************************************************/
    template <class T, typename A>
    void Vector<T, A>::resize(size_type new_size, T val)
    {
        reserve(new_size);
        if (size() < new_size)
            std::uninitialized_fill(mem_manager.block_start + size(),
                                    mem_manager.block_start + new_size, val);
        else // shrink
            destory(mem_manager.alloc, mem_manager.block_start + new_size,
                    mem_manager.block_start + size());

        mem_manager.uninitialized_block_start = mem_manager.block_start + new_size;
    }

    /*******************************************************************************
     * maxSize
     *
     * @brief return the maximum size that can be allocated
     *
     * @return size_type
     *******************************************************************************/
    template <class T, typename A>
    typename Vector<T, A>::size_type Vector<T, A>::maxSize() const
    {
        throw std::logic_error("Function Not Implemented");
        return 0;
    }

    //------------------------------------------------------------
    // Insertions
    //------------------------------------------------------------

    /*******************************************************************************
     * push_back
     *
     * @brief append element to end of vector
     *
     * @param val the value to add to vector
     * @return n/a
     *******************************************************************************/
    template <class T, typename A>
    void Vector<T, A>::push_back(const T &val)
    {
        if (size() == capacity())
            reserve(empty() ? 1 : size() * 2);

        std::allocator_traits<decltype(mem_manager.alloc)>::construct(
            mem_manager.alloc, mem_manager.uninitialized_block_start, val);
        ++mem_manager.uninitialized_block_start;
    }

    /*******************************************************************************
     * insert
     *
     * @brief insert val at given index
     *
     * @param index position to insert
     * @param val the value to add
     * @return void
     *******************************************************************************/
    template <class T, typename A>
    void Vector<T, A>::insert(size_type index, const T &val)
    {
        throw std::logic_error("Function Not Implemented");
    }

    /********************************************************************************
     * destroyElements
     *
     * @brief calls the destructor
     * for each element in Vector
     *
     * @return void
     ********************************************************************************/
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

#endif // CUSTOM_VECTOR_H