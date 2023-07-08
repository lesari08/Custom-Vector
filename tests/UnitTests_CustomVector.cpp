#include <gtest/gtest.h>
#include <numeric>
#include <memory>
#include "CustomVector.h"

using namespace custom;

//--------------------------------------------------------------------------------------------
//---------------   class Vector_Memory_Manager tests    -------------------------------------
//--------------------------------------------------------------------------------------------

TEST(MemoryManger, emptCtor)
{
    std::allocator<int> alloc;
    Vector_Memory_Manager<int, std::allocator<int>> empty_mgr(alloc, 0);
    EXPECT_EQ(empty_mgr.block_start, empty_mgr.block_end);
    EXPECT_EQ(empty_mgr.block_start, empty_mgr.uninitialized_block_start);
}

TEST(MemoryManger, ctor)
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

//--------------------------------------------------------------------------------------------
//---------------   class Vector tests    ----------------------------------------------------
//--------------------------------------------------------------------------------------------

class VectorTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        int size = 5;
        custom::Vector<int> vec1(size);
        std::iota(vec1.begin(), vec1.end(), 1);
    }
    // TearDown()
    custom::Vector<int> vec1;
    custom::Vector<int> vec2;
    custom::Vector<int> vec3;
};

// Constructors
TEST(ConstructorTests, DefaultConstructor)
{
    Vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
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

TEST(ConstructorTests, CopyConstructor)
{
    Vector<char> v;
    v.push_back('a');
    v.push_back('z');
    v.push_back('x');
    v.push_back('r');
    v.push_back('e');

    Vector<char> copy(v);

    EXPECT_EQ(v.at(0), copy.at(0));
    EXPECT_EQ(v.at(1), copy.at(1));
    EXPECT_EQ(v.at(2), copy.at(2));
    EXPECT_EQ(v.at(3), copy.at(3));
    EXPECT_EQ(v.at(4), copy.at(4));
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
    Vector<char> v;
    v.push_back('a');
    v.push_back('z');
    v.push_back('x');
    v.push_back('r');
    v.push_back('e');

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
    Vector<int>::Iterator itr = vec1.begin();
    ASSERT_EQ(*itr, *vec1.begin());

    for (int i = 0; i < vec1.size(); ++i)
    {
        ASSERT_EQ(*itr, vec1[i]);
        ++itr;
    }
}

TEST(IteratorTests, iteratorEnd)
{
    GTEST_SKIP() << "Not yet implemented";
}

TEST(IteratorTests, iteratorSequence)
{
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
TEST(ModifierTests, destroy)
{
    Vector<const char *> vec;
    vec.push_back("aaa");
    vec.push_back("bbb");
    vec.push_back("ccc");

    ASSERT_EQ(vec.size(), 3);
    ASSERT_STREQ(vec.front(), "aaa");
    vec.destroyElements();

    EXPECT_TRUE(vec.empty());
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
    Vector<char> v;

    v.push_back('t');
    v.push_back('r');
    v.push_back('o');
    v.push_back('l');
    v.push_back('l');

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