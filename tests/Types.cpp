#include "../Types.h"

#include "gtest/gtest.h"

TEST(Number, numberOperationsTests) {
    Number n1{._v = 5}, n2{._v = static_cast<float>(-5.1)};

    EXPECT_EQ(std::get<float>(Number::abs(n2)._v), static_cast<float>(5.1));
    EXPECT_NEAR(std::get<float>((n1 + n2)._v), static_cast<float>(-0.1), 0.01);
    EXPECT_NEAR(std::get<float>((n1 - n2)._v), static_cast<float>(10.1), 0.01);
    EXPECT_NEAR(std::get<float>((n1 * n2)._v), static_cast<float>(-25.5), 0.01);
    EXPECT_NEAR(std::get<float>((n1 / n2)._v), static_cast<float>(-0.98), 0.01);

    n1 += n2;
    EXPECT_NEAR(std::get<float>((n1)._v), static_cast<float>(-0.1), 0.01);
    n1 -= n2;
    EXPECT_NEAR(std::get<float>((n1)._v), static_cast<float>(5), 0.01);
    n1 *= n2;
    EXPECT_NEAR(std::get<float>((n1)._v), static_cast<float>(-25.5), 0.01);
    n1 /= n2;
    EXPECT_NEAR(std::get<float>((n1)._v), static_cast<float>(5.0), 0.01);

    EXPECT_EQ(std::get<int>((n1 > n2)._v), 1);
    EXPECT_EQ(std::get<int>((n1 >= n2)._v), 1);
    EXPECT_EQ(std::get<int>((n1 < n2)._v), 0);
    EXPECT_EQ(std::get<int>((n1 <= n2)._v), 0);
}

