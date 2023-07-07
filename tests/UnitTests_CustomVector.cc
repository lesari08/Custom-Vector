#include <gtest/gtest.h>
#include <numeric>

#include "CustomVector.h"

using namespace custom;
class VectorTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        int size = 5;
        vec1 = custom::Vector<int>(size);
        std::iota(vec1.begin(), vec1.end(), 1);
    }
    // TearDown()
    //   custom::Vector<int> vec;
    custom::Vector<int> vec1;
    custom::Vector<int> vec2;
    custom::Vector<int> vec3;
};

// Constructors
TEST(TestConstructors, DefaultConstructor)
{
    Vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

TEST(TestConstructors, ParamConstructor)
{
    int size = 4;
    int intitial_val = 13;
    Vector<int> vec(size, intitial_val);

    EXPECT_EQ(vec.size(), size);

    for (int num : vec)
        EXPECT_EQ(num, intitial_val);
}

TEST(TestConstructors, CopyConstructor)
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

TEST(TestConstructors, MoveCopyConstructor)
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

TEST(TestConstructors, AssignmentOperator)
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

TEST(TestConstructors, MoveAssignmentOperator)
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
TEST(VectorTestSuite, iteratorBegin)
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

    Vector<int>::Iterator constItr = cv.begin();
    EXPECT_EQ(-1, *constItr);
}

TEST_F(VectorTest, iteratorIncrement)
{

    Vector<int>::Iterator itr = vec1.begin();
    ASSERT_EQ(*itr, *vec1.begin());

    for(int i = 0; i < vec1.size(); ++i){
        ASSERT_EQ(*itr, vec1[i]);
        ++itr;
    }
}

// TEST(VectorTestSuite, iteratorEnd)
// {

// }

// TEST(VectorTestSuite, iteratorSequence)
// {

// }

// // Capacity
// TEST(VectorTestSuite, size)
// {
// 	Vector<double> v;

// 	// Size should be 0 upon creation
// 	EXPECT_EQ(0, v.size());

// 	v.push_back(3.14);
// 	EXPECT_EQ(1, v.size());

// 	v.push_back(0);
// 	EXPECT_EQ(2, v.size());
// }

// TEST(VectorTestSuite, capacity)
// {

// }

// // Modifiers
// TEST(VectorTestSuite, push_back_ints)
// {
// 	Vector<int> v;

// 	for (int i = 0; i < 10; ++i)
// 		v.push_back(i);

// 	for (int i = 0; i < 10; ++i)
// 		EXPECT_EQ(i, v[i]);

// 	v.push_back(1000);
// 	EXPECT_EQ(1000, v[v.size() - 1]);
// }

// // Accessors
// TEST(VectorTestSuite, accessAt)
// {
// 	// Non-const vector
// 	Vector<int> v;

// 	EXPECT_THROW(v.at(0), out_of_range);
// 	EXPECT_THROW(v.at(-97), out_of_range);
// 	EXPECT_THROW(v.at(1), out_of_range);

// 	v.push_back(4);
// 	v.push_back(8);
// 	v.push_back(10);
// 	v.push_back(-19);
// 	v.push_back(0);

// 	EXPECT_EQ(4, v.at(0));
// 	EXPECT_EQ(8, v.at(1));
// 	EXPECT_EQ(10, v.at(2));
// 	EXPECT_EQ(-19, v.at(3));
// 	EXPECT_EQ(0, v.at(4));

// 	EXPECT_THROW(v.at(-1), out_of_range);
// 	EXPECT_THROW(v.at(5), out_of_range);

// 	// Const vector
// 	const Vector<int> cv = v;

// 	EXPECT_EQ(4, cv.at(0));
// 	EXPECT_EQ(8, cv.at(1));
// 	EXPECT_EQ(10, cv.at(2));
// 	EXPECT_EQ(-19, cv.at(3));
// 	EXPECT_EQ(0, cv.at(4));

// 	EXPECT_THROW(cv.at(-1), out_of_range);
// 	EXPECT_THROW(cv.at(5), out_of_range);
// 	EXPECT_THROW(cv.at(-2), out_of_range);
// 	EXPECT_THROW(cv.at(8), out_of_range);
// }

// TEST(VectorTestSuite, accessBrackets)
// {
// 	const int NUM_ELEMENTS = 50;

// 	Vector<double> v;

// 	for (int i = 0; i < NUM_ELEMENTS; ++i)
// 		v.push_back(i);

// 	for (int i = 0; i < NUM_ELEMENTS; ++i)
// 		EXPECT_EQ(i, v[i]);

// 	const Vector<double> cv = v;

// 	for (int i = 0; i < NUM_ELEMENTS; ++i)
// 		EXPECT_EQ(i, cv[i]);
// }

// TEST(VectorTestSuite, accessFront)
// {
// 	Vector<int> v;

// 	v.push_back(9);
// 	EXPECT_EQ(9, v.front());

// 	v.push_back(4);
// 	EXPECT_EQ(9, v.front());
// }

// TEST(VectorTestSuite, accessBack)
// {
// 	Vector<int> v;

// 	v.push_back(9);
// 	EXPECT_EQ(9, v.back());

// 	v.push_back(4);
// 	EXPECT_EQ(4, v.back());

// 	v.push_back(1);
// 	EXPECT_EQ(1, v.back());
// }

TEST(VectorTestSuite, accessData)
{
	Vector<char> v;

	v.push_back('t');
	v.push_back('r');
	v.push_back('o');
	v.push_back('l');
	v.push_back('l');

	char* ptrCh = v.data();

	EXPECT_EQ(*ptrCh, v.front());
	EXPECT_EQ(*(ptrCh + 1), v.at(1));
	EXPECT_EQ(*(ptrCh + 2), v.at(2));
	EXPECT_EQ(*(ptrCh + 3), v.at(3));
	EXPECT_EQ(*(ptrCh + 4), v.at(4));
}

// TEST_F(VectorTest, DequeueWorks) {
//   int* n = vec1.Dequeue();
//   EXPECT_EQ(n, nullptr);

//   n = q1_.Dequeue();
//   ASSERT_NE(n, nullptr);
//   EXPECT_EQ(*n, 1);
//   EXPECT_EQ(q1_.size(), 0);
//   delete n;

//   n = q2_.Dequeue();
//   ASSERT_NE(n, nullptr);
//   EXPECT_EQ(*n, 2);
//   EXPECT_EQ(q2_.size(), 1);
//   delete n;
// }
