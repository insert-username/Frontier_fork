#include <gtest/gtest.h>
#include "Vertex.h"

TEST(VertexTests, testInit)
{
    auto vertex = Vertex();

    ASSERT_EQ(0, vertex.returnName());
    ASSERT_EQ(0, vertex.returnType());
    ASSERT_EQ(0, vertex.returnWeight());
    ASSERT_EQ(0, vertex.returnLabel());
    ASSERT_EQ(0, vertex.returnScan());
    ASSERT_EQ(0, vertex.isFrozen());
    ASSERT_EQ(0, vertex.returnPredEdg());
    ASSERT_EQ(0, vertex.returnDepth());
    ASSERT_EQ(0, vertex.numIncid());
}
