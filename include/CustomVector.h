#include <memory>
#include <vector>

namespace custom
{

    /**
     *  @brief This struct is responsible for allocating and dellallocating
     *   memory for an associated Vector class
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
    template <typename Type, class AllocType = std::allocator<Type>>
    struct Vector_Memory_Manager
    {
        Type *mem_block_start;
        Type *uninitialized_block_start;
        Type *mem_block_end;
        AllocType alloc;

        Vector_Memory_Manager(const AllocType &_alloc, typename AllocType::size_type n)
            : alloc{_alloc},
              mem_block_start{alloc.allocate(n)},
              uninitialized_block_start{mem_block_start + n},
              mem_block_end{mem_block_start + n} {}

        Vector_Memory_Manager(Vector_Memory_Manager &&other)
            : alloc{other.alloc},
              mem_block_start{other.mem_block_start},
              mem_block_end{other.mem_block_end},
              uninitialized_block_start{other.uninitialized_block_start}
        {
            other.mem_block_start = other.mem_block_end =
                other.uninitialized_block_start = nullptr;
        }

        Vector_Memory_Manager &operator=(Vector_Memory_Manager &&other)
        {
            std::swap(*this, other);
            return *this;
        }

        ~Vector_Memory_Manager()
        {
            alloc.deallocate(mem_block_start,
                             mem_block_end - mem_block_start);
        }

        Vector_Memory_Manager() = delete;
        // no copy operations allowed
        Vector_Memory_Manager(const Vector_Memory_Manager &) = delete;
        Vector_Memory_Manager &operator=(const Vector_Memory_Manager &) = delete;
    };

    /**
     *  @brief A custom vector container which replicates the functionality
     * of std::vector
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
    template <typename Type, typename AllocType = std::allocator<Type>>
    class Vector
    {
    private:
        std::vector<int> *m_vec;
        Vector_Memory_Manager<Type, AllocType> memory_manager;

        void destroyElements();

    public:
        using size_type = size_t;

        explicit Vector(
            size_type n,
            const Type &val = Type(),
            const AllocType &alloc = AllocType());

        // Vector(size_type _size) : m_vec(_size, 0) {}

        // copy constructor
        Vector(const Vector &other)
            : memory_manager{other.alloc, other.size()}
        {
            uninitialized_copy(other.begin(),other.end(),memory_manager.mem_block_start);
        }

        // // copy assignment
        // Vector &operator=(const Vector &other) const
        // {
        //     Vector<Type, AllocType> vec;

        //     return vec;
        // }

        // // move
        // Vector(Vector &&other)
        // {
        // }
        // // move assignment
        Vector &operator=(Vector &&other)
        {
            Vector<Type, AllocType> vec;

            throw std::logic_error("Function Not Implemented");
            return vec;
        }

        ~Vector()
        {
            destroyElements();
        }



        // ---------------------
        // Capacity and Size
        //----------------------
        size_type const capacity()
        {
            return memory_manager.mem_block_end -
                   memory_manager.mem_block_start;
        }
        size_type const size()
        {
            return memory_manager.uninitialized_block_start -
                   memory_manager.mem_block_start;
        }
        size_type const maxSize()
        {
            throw std::logic_error("Function Not Implemented");
            return 0;
        }

        // increase capacity
        void reserve(size_type);

        // change the number of elements
        void resize(size_type, Type = {});

        // empty the vector
        void clear() { resize(0); }

        // // iterators
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

    template <typename Type, typename AllocType>
    Vector<Type, AllocType>::Vector(
        size_type n,
        const Type &val,
        const AllocType &alloc) : memory_manager{alloc, n}
    {
        // construct n copies of val (in-place)
        std::uninitialized_fill(memory_manager.mem_block_start,
                                memory_manager.mem_block_start + n, val);
    }




    template <typename Type, typename A>
    void Vector<Type, A>::destroyElements()
    {
        for (Type *ptr = memory_manager.mem_block_start;
             ptr != memory_manager.uninitialized_block_start; ++ptr)
        {
            // explicit destruction
            ptr->~Type();
        }

        memory_manager.uninitialized_block_start = memory_manager.mem_block_start;
    }

}