#include <gtest/gtest.h>
#include <numeric>
#include <memory>
#include "CustomVector.h"

using namespace custom;

//--------------------------------------------------------------------------------------------
//---------------   class Vector_Memory_Manager tests    -------------------------------------
//--------------------------------------------------------------------------------------------

TEST(MemoryManger, verifyObjectEmpty)
{
    std::allocator<int> alloc;
    Vector_Memory_Manager<int, std::allocator<int>> empty_mgr(alloc, 0);
    ASSERT_EQ(empty_mgr.block_start, empty_mgr.block_end);
    EXPECT_EQ(empty_mgr.block_start, empty_mgr.uninitialized_block_start);
}

TEST(MemoryManger, constructor)
{
    std::allocator<int> alloc;
    int size = 7;
    Vector_Memory_Manager<int, std::allocator<int>> mgr(alloc, size);

    EXPECT_EQ(mgr.block_start, mgr.uninitialized_block_start);
    EXPECT_EQ(mgr.block_start + size, mgr.block_end);
}

TEST(MemoryManger, moveConstructor)
{
    using MemoryManger = Vector_Memory_Manager<int, std::allocator<int>>;
    std::allocator<int> alloc;
    int allocation_size = 7;

    MemoryManger mgr(alloc, allocation_size);
    ASSERT_EQ(mgr.block_start + allocation_size, mgr.block_end);

    MemoryManger copied_mgr(move(mgr));
    EXPECT_EQ(mgr.block_start, nullptr);
    EXPECT_EQ(copied_mgr.block_start + allocation_size, copied_mgr.block_end);
}

TEST(MemoryManger, moveAssignmentConstructor)
{
    using MemoryManger = Vector_Memory_Manager<int, std::allocator<int>>;
    std::allocator<int> alloc;
    int allocation_size = 5;

    MemoryManger mgr(alloc, allocation_size);
    ASSERT_EQ(mgr.block_start + allocation_size, mgr.block_end);

    MemoryManger copied_mgr = move(mgr);
    EXPECT_EQ(mgr.block_start, nullptr);
    EXPECT_EQ(copied_mgr.block_start + allocation_size, copied_mgr.block_end);
}

TEST(MemoryManger, swap)
{
    using MemoryManger = Vector_Memory_Manager<int, std::allocator<int>>;
    std::allocator<int> alloc;
    int size_a = 5;
    int size_b = 8;
    MemoryManger mgr_a(alloc, size_a);
    MemoryManger mgr_b(alloc, size_b);

    ASSERT_EQ(mgr_a.block_start + size_a, mgr_a.block_end);
    ASSERT_EQ(mgr_b.block_start + size_b, mgr_b.block_end);

    swap(mgr_a, mgr_b);

    // ensure that object a's new length equals b's old
    // length, and vice versa
    EXPECT_EQ(mgr_a.block_start + size_b, mgr_a.block_end);
    EXPECT_EQ(mgr_b.block_start + size_a, mgr_b.block_end);
}

TEST(MemoryManger, deletedConstructor)
{
    using MemoryManger = Vector_Memory_Manager<int, std::allocator<int>>;

    EXPECT_FALSE(std::is_default_constructible<MemoryManger>::value);
    EXPECT_FALSE(std::is_copy_constructible<MemoryManger>::value);
    EXPECT_FALSE(std::is_copy_assignable<MemoryManger>::value);
}

TEST(MemoryManger, max_size)
{
    using MemoryManger = Vector_Memory_Manager<int, std::allocator<int>>;
    std::allocator<int> alloc;
    MemoryManger mgr(alloc, 5);

    // There isnt a specific size we're expecting.
    // But we do know it should not be zero;
    EXPECT_TRUE(mgr.max_size() != 0);
}

//--------------------------------------------------------------------------------------------
//---------------   class Vector tests    ----------------------------------------------------
//--------------------------------------------------------------------------------------------

class VectorTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        int size = 5;
        vec_int = custom::Vector<int>(size);
        std::iota(vec_int.begin(), vec_int.end(), 1);

        vec_char.push_back('a');
        vec_char.push_back('b');
        vec_char.push_back('c');
        vec_char.push_back('d');
        vec_char.push_back('e');
    }

    //shared vectors 
    custom::Vector<int> vec_int;
    custom::Vector<char> vec_char;

    // TearDown()
};

// Constructors
TEST(ConstructorTests, DefaultConstructor)
{
    Vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

// Constructors
TEST(ConstructorTests, initializerListConstructor)
{
    Vector<int> v{11, 22, 33, 44};
    EXPECT_EQ(v[0], 11);
    EXPECT_EQ(v[1], 22);
    EXPECT_EQ(v[2], 33);
    EXPECT_EQ(v[3], 44);
}

TEST(ConstructorTests, ParamConstructor)
{
    int size = 3;
    int intitial_val = 13;
    Vector<int> vec(size, intitial_val);

    EXPECT_EQ(vec.size(), size);

    for (int num : vec)
        EXPECT_EQ(num, intitial_val);
}

TEST(ConstructorTests, MoveCopyConstructor)
{
    constexpr int size = 5;
    char chars[size] = {'c', 'v', 'q', 'r'};
    Vector<char> v;
    for (int i = 0; i < size; ++i)
        v.push_back(chars[i]);

    // ensure vector has the expected values
    for (int i = 0; i < size; ++i)
        ASSERT_EQ(v.at(i), chars[i]);

    Vector<char> copy_vec(std::move(v));

    EXPECT_EQ(v.size(), 0);
    ASSERT_EQ(copy_vec.size(), size);

    // ensure copied vector has the expected values
    for (int i = 0; i < size; ++i)
        ASSERT_EQ(copy_vec.at(i), chars[i]);
}

TEST(ConstructorTests, AssignmentOperator)
{
    Vector<char> v{'a','z','x','r','e'};

    Vector<char> copy = v;

    EXPECT_EQ(v.at(0), copy.at(0));
    EXPECT_EQ(v.at(1), copy.at(1));
    EXPECT_EQ(v.at(2), copy.at(2));
    EXPECT_EQ(v.at(3), copy.at(3));
    EXPECT_EQ(v.at(4), copy.at(4));
}

TEST(ConstructorTests, MoveAssignmentOperator)
{
    constexpr int size = 5;
    char chars[size] = {'c', 'v', 'q', 'r'};
    Vector<char> v;
    for (int i = 0; i < size; ++i)
        v.push_back(chars[i]);

    // ensure vector has the expected values
    for (int i = 0; i < size; ++i)
        ASSERT_EQ(v.at(i), chars[i]);

    Vector<char> copy_vec = std::move(v);

    EXPECT_EQ(v.size(), 0);
    ASSERT_EQ(copy_vec.size(), size);

    // ensure copied vector has the expected values
    for (int i = 0; i < size; ++i)
        ASSERT_EQ(copy_vec.at(i), chars[i]);
}

TEST_F(VectorTest, CopyConstructor)
{

    EXPECT_FALSE(vec_char.empty());
    Vector<char> copy(vec_char);

    EXPECT_EQ(vec_char.at(0), copy.at(0));
    EXPECT_EQ(vec_char.at(1), copy.at(1));
    EXPECT_EQ(vec_char.at(2), copy.at(2));
    EXPECT_EQ(vec_char.at(3), copy.at(3));
    EXPECT_EQ(vec_char.at(4), copy.at(4));
}

// Iterators
TEST(IteratorTests, iteratorBegin)
{
    Vector<int> v;
    int front_val = 4;
    v.push_back(front_val);
    EXPECT_EQ(front_val, *v.begin());

    // front value should remain unchanged
    v.push_back(9);
    EXPECT_EQ(front_val, *v.begin());

    // change front value
    front_val = -1;
    v[0] = front_val;
    Vector<int>::Iterator itr = v.begin();

    ASSERT_EQ(*itr, *v.begin());
    EXPECT_EQ(front_val, *itr);

    ++itr;
    EXPECT_EQ(9, *itr);
    // Constant vector
    const Vector<int> cv(v);

    Vector<int>::const_iterator constItr = cv.begin();
    EXPECT_EQ(-1, *constItr);
}

TEST_F(VectorTest, iteratorIncrement)
{
    Vector<int>::Iterator itr = vec_int.begin();
    ASSERT_EQ(*itr, *vec_int.begin());

    for (int i = 0; i < vec_int.size(); ++i)
    {
        ASSERT_EQ(*itr, vec_int[i]);
        ++itr;
    }
}

TEST_F(VectorTest, revItr)
{
    Vector<int> vec;
    int size = 5;
    vec.reserve(size);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);

    ASSERT_EQ(vec.size(), 5);

    // test the reverse_iterator post increment operator
    Vector<int>::reverse_iterator r_itr = vec.rbegin();
    for (int i = vec.size() - 1; i >= 0; --i)
    {
        EXPECT_EQ(*r_itr++, vec[i]);
    }

    // test the reverse_iterator post decrement operator
    Vector<int>::reverse_iterator r_end = --vec.rend();
    for (int i = 0; i < vec.size(); ++i)
    {
        EXPECT_EQ(*r_end--, vec[i]);
    }
}

TEST(IteratorTests, iteratorEnd)
{
    Vector<int> vec;
    vec.push_back(5);
    Vector<int>::Iterator itr = --vec.end();
    ASSERT_EQ(*itr, 5);

    vec.push_back(7);
    itr = --vec.end();
    ASSERT_EQ(*itr, 7);

    vec.push_back(15);
    itr = --vec.end();
    ASSERT_EQ(*itr, 15);

    // GTEST_SKIP() << "Not yet implemented";
}

TEST(IteratorTests, iteratorSequence)
{
    GTEST_SKIP() << "Not yet implemented";
}

TEST(IteratorTests, preFix)
{
    // + & -
    GTEST_SKIP() << "Not yet implemented";
}

TEST(IteratorTests, postFix)
{
    // + & -
    GTEST_SKIP() << "Not yet implemented";
}

TEST(IteratorTests, comparision)
{
    Vector<int> vec;
    vec.push_back(5);

    Vector<int>::Iterator a = vec.begin();
    Vector<int>::Iterator b = vec.begin();

    EXPECT_TRUE(a == b);
}

TEST(IteratorTests, TODO)
{
    /*
    Other operations to test
    +=
    -=
    itr += val
    itr + 5
    itr - 5

    itr1 - itr2
    itr1 == itr2
    itr1 != itr2

    const begin, end
    */
    GTEST_SKIP() << "Not yet implemented";
}



// Modifiers
TEST(ModifierTests, pushBack)
{
    Vector<int> v;

    for (int i = 0; i < 10; ++i)
        v.push_back(i);

    for (int i = 0; i < 10; ++i)
        EXPECT_EQ(i, v[i]);

    v.push_back(1000);
    EXPECT_EQ(1000, v[v.size() - 1]);
}

TEST(ModifierTests, popBack)
{
    Vector<int> v;

    for (int i = 0; i < 10; ++i)
        v.push_back(i);

    EXPECT_EQ(v.back(), 9);

    v.pop_back();
    EXPECT_EQ(v.back(), 8);

    v.pop_back();
    EXPECT_EQ(v.back(), 7);

    v.push_back(12);
    EXPECT_EQ(v.back(), 12);

    v.pop_back();
    EXPECT_EQ(v.back(), 7);
}

TEST(ModifierTests, resize)
{
    int org_size = 3;
    Vector<int> v(org_size);
    EXPECT_EQ(v.size(), org_size);
    for (int i = 0; i < org_size; ++i)
        EXPECT_EQ(v[i], 0);

    v.resize(5, 2);
    ASSERT_EQ(v.size(), 5);

    // ensure the original values remain unchanged
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 0);
    EXPECT_EQ(v[2], 0);
    // ensure new elements contain expected value
    EXPECT_EQ(v[3], 2);
    EXPECT_EQ(v[4], 2);
}

TEST_F(VectorTest, insert)
{
    auto insert_itr = vec_int.insert(vec_int.begin(), 40);

    EXPECT_TRUE(vec_int.begin() == insert_itr);
    EXPECT_EQ(*insert_itr, 40);
}

TEST_F(VectorTest, modifierErase)
{
    int original_size = vec_int.size();
    vec_int.erase(vec_int.begin());

    EXPECT_EQ(vec_int.size(), original_size - 1);

    EXPECT_EQ(vec_int[0], 2);
    EXPECT_EQ(vec_int[1], 3);
}

TEST_F(VectorTest, assign)
{
    Vector<int> vec;
    vec.assign(5, 12);

    ASSERT_TRUE(vec.size() == 5);
    for (int num : vec)
        EXPECT_EQ(num, 12);

    // re-assign values in a
    // vector that already has
    // initialized values
    vec_int.assign(30, 19);
    ASSERT_TRUE(vec_int.size() == 30);
    for (int num : vec_int)
        EXPECT_EQ(num, 19);
}

// Accessors
TEST(AccessorTests, accessAt)
{
    using namespace std;

    Vector<int> v;

    EXPECT_THROW(v.at(0), out_of_range);
    EXPECT_THROW(v.at(-97), out_of_range);
    EXPECT_THROW(v.at(1), out_of_range);

    v.push_back(4);
    v.push_back(8);
    v.push_back(10);
    v.push_back(-19);
    v.push_back(0);

    EXPECT_EQ(4, v.at(0));
    EXPECT_EQ(8, v.at(1));
    EXPECT_EQ(10, v.at(2));
    EXPECT_EQ(-19, v.at(3));
    EXPECT_EQ(0, v.at(4));

    EXPECT_THROW(v.at(-1), out_of_range);
    EXPECT_THROW(v.at(5), out_of_range);

    // Const vector
    const Vector<int> &cv = v;

    EXPECT_EQ(4, cv.at(0));
    EXPECT_EQ(8, cv.at(1));
    EXPECT_EQ(10, cv.at(2));
    EXPECT_EQ(-19, cv.at(3));
    EXPECT_EQ(0, cv.at(4));

    EXPECT_THROW(cv.at(-1), out_of_range);
    EXPECT_THROW(cv.at(5), out_of_range);
    EXPECT_THROW(cv.at(-2), out_of_range);
    EXPECT_THROW(cv.at(8), out_of_range);
}

TEST(AccessorTests, accessBrackets)
{
    const int NUM_ELEMENTS = 50;

    Vector<double> v;

    for (int i = 0; i < NUM_ELEMENTS; ++i)
        v.push_back(i);

    for (int i = 0; i < NUM_ELEMENTS; ++i)
        EXPECT_EQ(i, v[i]);

    const Vector<double> cv = v;

    for (int i = 0; i < NUM_ELEMENTS; ++i)
        EXPECT_EQ(i, cv[i]);
}

TEST(AccessorTests, accessFront)
{
    Vector<int> v;

    v.push_back(9);
    EXPECT_EQ(9, v.front());

    v.push_back(4);
    EXPECT_EQ(9, v.front());
}

TEST(AccessorTests, accessBack)
{
    Vector<int> v;

    v.push_back(9);
    EXPECT_EQ(9, v.back());

    v.push_back(4);
    EXPECT_EQ(4, v.back());

    v.push_back(1);
    EXPECT_EQ(1, v.back());
}

TEST(AccessorTests, accessData)
{
    Vector<char> v{'t','r','o','l','l'};

    char *ptrCh = v.data();

    EXPECT_EQ(*ptrCh, v.front());
    EXPECT_EQ(*(ptrCh + 1), v.at(1));
    EXPECT_EQ(*(ptrCh + 2), v.at(2));
    EXPECT_EQ(*(ptrCh + 3), v.at(3));
    EXPECT_EQ(*(ptrCh + 4), v.at(4));
}

TEST(AccessorTests, empty)
{
    Vector<int> v;
    EXPECT_EQ(v.size(), 0);

    v.push_back(5);
    EXPECT_NE(v.size(), 0);

    v.push_back(3);
    v.push_back(2);
    EXPECT_NE(v.size(), 0);
}

TEST(AccessorTests, maxSize)
{
    Vector<int> v;
    // Theres no set expected max value,
    // but we do know that the value
    // should be greater than zero
    ASSERT_NE(v.maxSize(), 0);
}

TEST(AccessorTests, other)
{
    /*
    back()
    front()
    clear()
    reserve
    capacity
    empty
    size
    swap

    */
}
