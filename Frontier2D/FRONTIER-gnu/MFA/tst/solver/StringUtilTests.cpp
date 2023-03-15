#include <gtest/gtest.h>
#include "solver/StringUtil.h"

template<typename T>
void assertToStringEquivalent(const T &value) {
    ASSERT_EQ(std::to_string(value), toString(value));
}

TEST(StringUtilTests, testToStringInt)
{
assertToStringEquivalent((int)10);
assertToStringEquivalent((int)-10);

assertToStringEquivalent((int)0);

assertToStringEquivalent(std::numeric_limits<int>::max());
assertToStringEquivalent(std::numeric_limits<int>::min());
}

TEST(StringUtilTests, testToStringLong)
{
assertToStringEquivalent((long)10);
assertToStringEquivalent((long)-10);
assertToStringEquivalent((long)0);

assertToStringEquivalent(std::numeric_limits<long>::max());
assertToStringEquivalent(std::numeric_limits<long>::min());
}

TEST(StringUtilTests, testToStringFloat)
{
assertToStringEquivalent((float)10);
assertToStringEquivalent((float)-10);
assertToStringEquivalent((float)0);

assertToStringEquivalent(std::numeric_limits<float>::max());
assertToStringEquivalent(std::numeric_limits<float>::min());
}

TEST(StringUtilTests, testToStringDouble)
{
    assertToStringEquivalent((double)10);
    assertToStringEquivalent((double)-10);
    assertToStringEquivalent((double)0);

    assertToStringEquivalent(std::numeric_limits<double>::max());
    assertToStringEquivalent(std::numeric_limits<double>::min());
}
