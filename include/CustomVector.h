/*******************************************************************************
 *  @file CustomVector.h
 *  @brief This file contains methods that define and implement a custom Vector
 *
 *  @author Leslie Aririguzo
 *******************************************************************************/

#ifndef CUSTOM_VECTOR_H
#define CUSTOM_VECTOR_H 1

#include <algorithm>
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
        Vector_Memory_Manager(const AllocType &_alloc, typename AllocType::size_type n);

        Vector_Memory_Manager(Vector_Memory_Manager &&other);
        Vector_Memory_Manager &operator=(Vector_Memory_Manager &&other);

        Vector_Memory_Manager() = delete;

        // no copy operations allowed
        Vector_Memory_Manager(const Vector_Memory_Manager &) = delete;
        Vector_Memory_Manager &operator=(const Vector_Memory_Manager &) = delete;

        ~Vector_Memory_Manager();

        typename AllocType::size_type max_size() const noexcept;

        friend void swap(Vector_Memory_Manager &a, Vector_Memory_Manager &b) noexcept
        {
            // enable ADL (argument dependent lookup)
            using namespace std;

            swap(a.block_start, b.block_start);
            swap(a.uninitialized_block_start, b.uninitialized_block_start);
            swap(a.block_end, b.block_end);
        }

        AllocType alloc;
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

        class Iterator
        {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;

            Iterator(T *ptr) : m_ptr(ptr) {}

            reference operator*() const { return *m_ptr; }
            pointer operator->() const { return m_ptr; }

            Iterator &operator++()
            {
                ++m_ptr;
                return *this;
            }

            Iterator operator++(int)
            {
                Iterator temp = *this;

                ++m_ptr;
                return temp;
            }
            Iterator &operator--()
            {
                --m_ptr;
                return *this;
            }

            Iterator operator--(int)
            {
                Iterator temp = *this;

                --m_ptr;
                return temp;
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

            reference operator[](difference_type n) const { return *(m_ptr + n); }

            friend bool operator==(const Iterator &a, const Iterator &b)  { return a.m_ptr == b.m_ptr; }
            friend bool operator!=(const Iterator &a, const Iterator &b)  { return a.m_ptr != b.m_ptr; }

        private:
            T *m_ptr;
        };

        Vector(const AllocType &alloc = AllocType());

        explicit Vector(size_type n, const T &val = T(),
                        const AllocType &alloc = AllocType());

        Vector(const Vector &other);
        Vector(Vector &&other);

        Vector &operator=(Vector &other);

        Vector &operator=(Vector &&other);

        ~Vector() { destroyElements(); }

        // Element Access
        T &at(size_type idx);
        constexpr T &at(size_type idx) const;

        T &operator[](size_type idx) { return at(idx); }
        const T &operator[](size_type idx) const { return at(idx); }
        T &front();
        T &back();
        constexpr T *data() noexcept { return mem_manager.block_start; }
        constexpr const T *data() const noexcept { return mem_manager.block_start; }

        // Modifiers
        void push_back(const T &val);
        void insert(Iterator index, const T &val);
        void erase(Iterator position);
        void pop_back();
        void clear() { resize(0); }
        void resize(size_type, T = {});

        // Size and Capacity
        void reserve(size_type);
        size_type capacity() const noexcept
        {
            return mem_manager.block_end - mem_manager.block_start;
        }
        bool empty() const noexcept
        {
            return mem_manager.uninitialized_block_start - mem_manager.block_start == 0;
        }
        size_type maxSize() const noexcept;

        size_type size() const noexcept
        {
            return mem_manager.uninitialized_block_start - mem_manager.block_start;
        }

        friend void swap(Vector &a, Vector &b) noexcept
        {
            swap(a.mem_manager, b.mem_manager);
        }
        //--------------------------------------------
        // Iterator Methods
        //--------------------------------------------
        using iterator = Iterator;
        using const_iterator = const Iterator;
        iterator begin() { return Iterator(mem_manager.block_start); }
        const_iterator begin() const { return const_iterator(mem_manager.block_start); }
        iterator end() { return Iterator(mem_manager.uninitialized_block_start); }
        const_iterator end() const { return const_iterator(mem_manager.uninitialized_block_start); }

        void destroyElements();

    protected:
    private:
        Vector_Memory_Manager<T, AllocType> mem_manager;
    };

    //--------------------------------------------------------------------------------------------
    //------------------   Vector_Memory_Manager Methods    --------------------------------------
    //--------------------------------------------------------------------------------------------

    /*******************************************************************************
     *  Vector_Memory_Manager::Constructor
     *
     *  @param _alloc allocation object
     *  @param n allocation size
     *
     *******************************************************************************/
    template <class T, class A>
    Vector_Memory_Manager<T, A>::Vector_Memory_Manager(
        const A &_alloc, typename A::size_type n)
        : alloc{_alloc}
    {
        using traits = std::allocator_traits<decltype(alloc)>;
        block_start = traits::allocate(alloc, n);
        uninitialized_block_start = block_start;
        block_end = block_start + n;
    }

    /*******************************************************************************
     *  Vector_Memory_Manager::Move Constructor
     *
     *  @param other Vector_Memory_Manager object
     *
     *******************************************************************************/
    template <class T, class A>
    Vector_Memory_Manager<T, A>::Vector_Memory_Manager(Vector_Memory_Manager &&other)
        : alloc{other.alloc},
          block_start{nullptr},
          uninitialized_block_start{nullptr},
          block_end{nullptr}
    {

        swap(*this, other);
    }

    /*******************************************************************************
     *  Vector_Memory_Manager:: move assignment operator
     *
     *  @param other Vector_Memory_Manager object
     *  @return reference
     *
     *  Note: Currently implemented without freeing the memory 'other' contains at
     *  the point of the return statement. That is because our assumption is that
     * 'other' go out of scope in due time and free the memory without intervention
     *******************************************************************************/
    template <class T, class A>
    Vector_Memory_Manager<T, A> &
    Vector_Memory_Manager<T, A>::operator=(Vector_Memory_Manager<T, A> &&other)
    {
        swap(*this, other);

        return *this;
    }

    /*******************************************************************************
     *  Vector_Memory_Manager:: destructor
     *******************************************************************************/
    template <class T, class A>
    Vector_Memory_Manager<T, A>::~Vector_Memory_Manager()
    {
        using traits = std::allocator_traits<decltype(alloc)>;

        traits::deallocate(alloc, block_start, block_end - block_start);

        block_end = uninitialized_block_start = block_start = nullptr;
    }

    /*******************************************************************************
     *  @brief Vector_Memory_Manager:: max_size
     *******************************************************************************/
    template <class T, class A>
    typename A::size_type Vector_Memory_Manager<T, A>::max_size() const noexcept
    {
        return std::allocator_traits<decltype(alloc)>::max_size(alloc);
    }

    //--------------------------------------------------------------------------------------------
    //-------------------------    VECTOR METHODS  -----------------------------------------------
    //--------------------------------------------------------------------------------------------

    /*******************************************************************************
     * @brief default constructor
     *
     * @param alloc allocator
     * @return n/a
     *******************************************************************************/
    template <class T, typename A>
    Vector<T, A>::Vector(const A &alloc)
        : mem_manager{alloc, 0}
    {
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

        mem_manager.uninitialized_block_start = mem_manager.block_start + n;
    }

    /*******************************************************************************
     * copy constructor
     *
     * @param other vector object
     * @return n/a
     *******************************************************************************/
    template <class T, class A>
    Vector<T, A>::Vector(const Vector &other)
        : mem_manager{other.mem_manager.alloc, other.size()}
    {
        int n = other.size();
        T *other_start = other.mem_manager.block_start;
        T *other_end = other.mem_manager.block_start + n;
        std::uninitialized_copy(other_start, other_end, mem_manager.block_start);

        mem_manager.uninitialized_block_start = mem_manager.block_start + n;
    }

    /*******************************************************************************
     * copy assignment operator
     *
     * @param other vector object
     * @return Vector reference
     *******************************************************************************/
    template <class T, typename A>
    Vector<T, A> &Vector<T, A>::operator=(Vector<T, A> &other)
    {
        // copy-and-swap
        Vector<T, A> temp(other);
        swap(*this, temp);

        return *this;
    }

    /*******************************************************************************
     * @brief move constructor
     *
     * @param other vector object
     * @return n/a
     *******************************************************************************/
    template <class T, typename A>
    Vector<T, A>::Vector(Vector &&other)
        : mem_manager{other.mem_manager.alloc, 0}
    {
        // call move assignment operator to avoid duplicate code
        *this = std::move(other);
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
        // TO-DO: Consider whether or not this simple swap suffices, or if after
        // the swap, 'other' should explicibly have its objects destroyed
        swap(*this, other);

        return *this;
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
        if (size_to_reserve <= capacity())
            return;

        Vector_Memory_Manager<T, A> next_mem_manager{
            mem_manager.alloc, size_to_reserve};

        std::uninitialized_move(mem_manager.block_start,
                                mem_manager.block_start + size(),
                                next_mem_manager.block_start);

        next_mem_manager.uninitialized_block_start =
            next_mem_manager.block_start + size();

        std::swap(next_mem_manager, mem_manager);
    }

    /*******************************************************************************
     * resize
     *
     * @brief resize vector based on given new_size
     * 
     * If the given size is smaller than the vector's current size, the vector
     * will be reduced.
     *
     * @param new_size number of objects in vector after the method completes

     * @param val the value that will be assigned to the uninitialized
     *  memory spaces if the new_size is greater than the current size.
     *  
     * @return void
     *******************************************************************************/
    template <class T, typename A>
    void Vector<T, A>::resize(size_type new_size, T val)
    {
        if(new_size == size())
            return;

        // ensure that there's enough allocated memory for the new_size
        reserve(new_size);

        if (size() < new_size)
        {
            // The vector is expanding
            // construct new elements in the uninitialized memory spaces
            std::uninitialized_fill(mem_manager.uninitialized_block_start,
                                    mem_manager.block_start + new_size, val);
        }
        else // shrink
        {
            // The vector size is shrinking
            // remove all elements at and after vector[new_size]
            T *remove_start = mem_manager.block_start + size();

            size_type num_to_destroy = mem_manager.uninitialized_block_start -
                                       remove_start;

            std::destroy_n(remove_start, num_to_destroy);
        }
        
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
    typename Vector<T, A>::size_type Vector<T, A>::maxSize() const noexcept
    {
        return mem_manager.max_size();
    }

    /*******************************************************************************
     * at
     *
     * @return value at index
     *******************************************************************************/
    template <class T, typename A>
    T &Vector<T, A>::at(size_type idx)
    {
        if (idx < 0 || idx >= size())
            throw std::out_of_range("Invalid index");

        return *(mem_manager.block_start + idx);
    }

    /*******************************************************************************
     * at
     *
     * @return const ref
     *******************************************************************************/
    template <class T, typename A>
    constexpr T &Vector<T, A>::at(size_type idx) const
    {
        if (idx < 0 || idx >= size())
            throw std::out_of_range("Invalid index");

        return *(mem_manager.block_start + idx);
    }

    /*******************************************************************************
     * front
     *
     * @return size_type
     *******************************************************************************/
    template <class T, typename A>
    T &Vector<T, A>::front()
    {
        if (empty())
            throw std::out_of_range("Vector is empty");

        return *mem_manager.block_start;
    }

    /*******************************************************************************
     * back
     *
     * @brief return the maximum size that can be allocated
     *
     * @return size_type
     *******************************************************************************/
    template <class T, typename A>
    T &Vector<T, A>::back()
    {
        if (empty())
            throw std::out_of_range("Vector is empty");

        return *(mem_manager.uninitialized_block_start - 1);
    }

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
            reserve(empty() ? 1 : size() << 1);

        std::construct_at(mem_manager.uninitialized_block_start, val);

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
    void Vector<T, A>::insert(Iterator pos, const T &val)
    {
        throw std::logic_error("insert::function not yet implemented");
    }

    /*******************************************************************************
     * erase
     *
     * @brief erase val at given iterator position
     * @param position position to erase
     * @return void
     *******************************************************************************/
    template <class T, typename A>
    void Vector<T, A>::erase(Iterator position)
    {
        throw std::logic_error(__PRETTY_FUNCTION__ + std::string("function not yet implemented"));

        //auto itr_end = this->end();

        //  if (empty() || position == itr_end())
        //      return;

        //  Iterator next = position;
        //  ++next;
        // while(position + 1 != itr_end)
        // {
        //     Iterator next =
        //     swap(*position, *(position+1));
        //     ++position;
        // }
        // the element to erase is now
        // the last element in the vector;
        //pop_back();
    }

    /*******************************************************************************
     * pop_back
     *
     * @brief destroy and remove the last element from the vector
     *
     * @return void
     *******************************************************************************/
    template <class T, typename A>
    void Vector<T, A>::pop_back()
    {
        if (empty())
            throw std::out_of_range(__PRETTY_FUNCTION__ + std::string(": Vector is empty"));

        T *to_remove = mem_manager.uninitialized_block_start - 1;
        std::destroy_at(to_remove);

        mem_manager.uninitialized_block_start -= 1;
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
        std::destroy_n(mem_manager.block_start, size());

        mem_manager.uninitialized_block_start = mem_manager.block_start;
    }
}

#endif // CUSTOM_VECTOR_H