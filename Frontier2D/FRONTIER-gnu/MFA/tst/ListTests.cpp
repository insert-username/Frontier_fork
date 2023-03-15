#include <gtest/gtest.h>
#include "List.h"

TEST(ListTests, testLen)
{
    auto lst = List<int>();

    ASSERT_EQ(0, lst.returnLen());
}

TEST(ListTests, testLenWithElements)
{
    auto lst = List<int>();

    lst.append(0);
    ASSERT_EQ(1, lst.returnLen());

    lst.append(0);
    ASSERT_EQ(2, lst.returnLen());

    lst.append(0);
    ASSERT_EQ(3, lst.returnLen());
}

TEST(ListTests, testIndexFrom1)
{
    auto lst = List<int>();

    lst.append(10);
    lst.append(20);
    lst.append(30);

    ASSERT_EQ(10, lst.retrieve(1));
    ASSERT_EQ(20, lst.retrieve(2));
    ASSERT_EQ(30, lst.retrieve(3));
}
