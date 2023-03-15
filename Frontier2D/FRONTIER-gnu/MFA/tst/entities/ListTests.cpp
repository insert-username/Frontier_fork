#include <gtest/gtest.h>
#include "entities/List.h"

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


TEST(ListTests, testRetrieve)
{
    auto lst = List<int>();

    lst.append(10);
    lst.append(20);
    lst.append(30);

    // appears to be that retrieve on a position greater than len
    // should yield a default value
    ASSERT_EQ(0, lst.retrieve(4));
}

TEST(ListTests, testRemoveByIndex)
{
    auto lst = List<int>();

    lst.append(10);
    lst.append(20);
    lst.append(30);

    lst.deleteIndex(2);

    ASSERT_EQ(2, lst.returnLen());
    ASSERT_EQ(10, lst.retrieve(1));
    ASSERT_EQ(30, lst.retrieve(2));
}

TEST(ListTests, testRemoveByValue)
{
    auto lst = List<int>();

    lst.append(10);
    lst.append(20);
    lst.append(30);

    int value = 20;
    lst.deleteElem(value);

    ASSERT_EQ(2, lst.returnLen());
    ASSERT_EQ(10, lst.retrieve(1));
    ASSERT_EQ(30, lst.retrieve(2));

}

TEST(ListTests, testAddrByIndex)
{
    auto lst = List<int>();

    lst.append(10);
    lst.append(20);
    lst.append(30);

    ASSERT_EQ(20, *lst.addrByIndex(2));
}

TEST(ListTests, testHasElem)
{
    auto lst = List<int>();

    lst.append(10);
    lst.append(20);
    lst.append(30);

    int testValue = 0;
    ASSERT_FALSE(lst.hasElem(testValue));

    testValue = 10;
    ASSERT_TRUE(lst.hasElem(testValue));

    testValue = 20;
    ASSERT_TRUE(lst.hasElem(testValue));

    testValue = 30;
    ASSERT_TRUE(lst.hasElem(testValue));

    testValue = 40;
    ASSERT_FALSE(lst.hasElem(testValue));
}

TEST(ListTests, testPop)
{
    auto lst = List<int>();

    lst.append(10);
    lst.append(20);
    lst.append(30);

    ASSERT_EQ(10, lst.pop());

    ASSERT_EQ(2, lst.returnLen());
    ASSERT_EQ(20, lst.retrieve(1));
    ASSERT_EQ(30, lst.retrieve(2));

    ASSERT_EQ(20, lst.pop());

    ASSERT_EQ(1, lst.returnLen());
    ASSERT_EQ(30, lst.retrieve(1));

    ASSERT_EQ(30, lst.pop());
    ASSERT_EQ(0, lst.returnLen());

    // appears to be that a pop on an empty list yields a default
    // value
    ASSERT_EQ(0, lst.pop());
}

TEST(ListTests, testMakeEmpty)
{
    auto lst = List<int>();

    lst.append(10);
    lst.append(20);
    lst.append(30);

    lst.makeEmpty();
    ASSERT_EQ(0, lst.returnLen());
}

TEST(ListTests, testAssignmentOperator)
{
    auto lst0 = List<int>();

    lst0.append(10);
    lst0.append(20);
    lst0.append(30);

    auto lst1 = List<int>();
    lst1 = lst0;

    ASSERT_EQ(10, lst0.retrieve(1));
    ASSERT_EQ(20, lst0.retrieve(2));
    ASSERT_EQ(30, lst0.retrieve(3));

    ASSERT_EQ(10, lst1.retrieve(1));
    ASSERT_EQ(20, lst1.retrieve(2));
    ASSERT_EQ(30, lst1.retrieve(3));

    // check for mutability
    int newElemValue = 50;
    lst0.changeElem(2, newElemValue);
    ASSERT_EQ(10, lst0.retrieve(1));
    ASSERT_EQ(50, lst0.retrieve(2));
    ASSERT_EQ(30, lst0.retrieve(3));
    ASSERT_EQ(10, lst1.retrieve(1));
    ASSERT_EQ(20, lst1.retrieve(2));
    ASSERT_EQ(30, lst1.retrieve(3));


    newElemValue = 70;
    lst1.changeElem(2, newElemValue);
    ASSERT_EQ(10, lst0.retrieve(1));
    ASSERT_EQ(50, lst0.retrieve(2));
    ASSERT_EQ(30, lst0.retrieve(3));
    ASSERT_EQ(10, lst1.retrieve(1));
    ASSERT_EQ(70, lst1.retrieve(2));
    ASSERT_EQ(30, lst1.retrieve(3));

}
