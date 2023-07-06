#include <gtest/gtest.h>
#include "CustomVector.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
}

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions2) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hell", "hello");

}


// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions3) {
  // Expect two strings to be equal.
  EXPECT_STREQ("hell", "hell");

}

// Demonstrate some basic assertions.
TEST(MultTestSuite, MultTest) {
  // Expect equality.
  custom::Vector<int> vec(5,3);
 // vec.push_back(5);
  EXPECT_EQ(vec.at(0), 3);
}

// // Demonstrate some basic assertions.
// TEST(MultTestSuite, MultTest2) {
//   // Expect equality.
//   EXPECT_EQ(tt_mult(7), 5);
// }