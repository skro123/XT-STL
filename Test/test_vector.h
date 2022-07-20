#include <gtest/gtest.h>

#include <vector>
#include "../Src/vector.h"

namespace my_stl
{
namespace stl_test
{
namespace vector_test
{
    // Demonstrate some basic assertions.
    TEST(VectorTest, BasicAssertions) {
        std::vector<int> stdv1;
        my_stl::vector<int> xtv1;
        stdv1.push_back(1);
        xtv1.push_back(1);
        EXPECT_EQ(*(stdv1.begin()), *(xtv1.begin()));
        
        my_stl::vector<int> xtv2();
        // Expect two strings not to be equal.
        EXPECT_STRNE("hello", "world");
        // Expect equality.
        EXPECT_EQ(7 * 6, 42);
    }
}
}
}


