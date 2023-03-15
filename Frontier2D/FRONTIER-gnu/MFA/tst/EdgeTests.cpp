#include <gtest/gtest.h>
#include "Edge.h"

TEST(EdgeTests, testInit)
{
    auto edge = Edge();

    ASSERT_EQ(0, edge.returnName());
    ASSERT_EQ(0, edge.returnType());
    ASSERT_EQ(0, edge.returnWeight());
    ASSERT_EQ(0.0f, edge.returnValue());
    ASSERT_EQ(0, edge.returnEnd1());
    ASSERT_EQ(0, edge.returnEnd2());
    ASSERT_EQ(0, edge.returnPart1());
    ASSERT_EQ(0, edge.returnPart2());
    ASSERT_EQ(0, edge.returnLabel());
    ASSERT_EQ(0, edge.returnScan());
    ASSERT_EQ(0, edge.returnFlow1());
    ASSERT_EQ(0, edge.returnFlow2());
    ASSERT_EQ(0, edge.returnPosFlow());
    ASSERT_EQ(0, edge.returnPredVer());
}