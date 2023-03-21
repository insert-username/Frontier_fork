#include <gtest/gtest.h>
#include "entities/Graph.h"
#include "ShapeTypes.h"
#include "ShapePartTypes.h"
#include "ConstraintTypes.h"

/**
 * From Sketcher_IO.doc
 *
 * Format of output to the UTU :
    1.	Flag for UTU
    2. ShapeTypeID
    3. ShapeID
    4. Point : - x-coordinate
                    y-coordinate
      Line : -  pt1 x-coordinate
                    Pt1 y-coordinate
                    Pt2 x-coordinate
                    Pt2 y-coordinate
      Circle : - center pt1 x-coordinate
                     center Pt1 y-coordinate
                     radius
      Arc : -     center pt1 x-coordinate
                     center Pt1 y-coordinate
                     radius
                     angle
     Image : - pt1 x-coordinate
                    Pt1 y-coordinate
                    Pt2 x-coordinate
                    Pt2 y-coordinate
    5. -1
    6. ConstraintTypeID
    7. ConstraintID
    8. Number of shapes
    9. ShapeID
    10. Subshape of the shape involved in the constraint
    11. –1
    12. group tree node ID
    13. Number of shapes involved
    14. Shapes ID’s
    15. Number of children groups
    16. –1
 */

TEST(GraphTests, testInit)
{
    auto graph = Graph();
}

TEST(GraphTests, testSketchInputOnePt)
{
    GlobalState globalState;

    const int pointID = 123;
    const double pointX = 10.5;
    const double pointY = 20.5;

    std::vector<int> intInput = {
            0,
            ShapeTypeIDs::POINT,
            pointID,
            -1,
            // constraints here
            -1
    };
    std::vector<double> dblInput = {
            pointX,
            pointY
    };

    auto graph = Graph();

    int idx = 1;
    int idd = 0;
    int groupID = graph.sketchInput(
            globalState,
            idx,
            intInput,
            idd,
            dblInput);

    ASSERT_EQ(pointID, globalState.getSingleVertex());
    ASSERT_EQ(pointID + 4, globalState.getNextVerName());
    ASSERT_EQ(1, globalState.getNextEdgeName());

    ASSERT_EQ(5, groupID);
    ASSERT_EQ(1, graph.getVertices().size());
    ASSERT_EQ(0,  graph.getEdges().size());
}


TEST(GraphTests, testSketchInputTwoPt)
{
    GlobalState globalState;

    const int point0ID = 123;
    const int point1ID = 456;

    std::vector<int> intInput = {
            0,
            ShapeTypeIDs::POINT,
            point0ID,
            ShapeTypeIDs::POINT,
            point1ID,
            -1,
            // constraints here
            -1
    };
    std::vector<double> dblInput = {
            123.1,
            123.2,
            456.1,
            456.2
    };

    auto graph = Graph();

    int idx = 1;
    int idd = 0;
    int groupID = graph.sketchInput(
            globalState,
            idx,
            intInput,
            idd,
            dblInput);

    ASSERT_EQ(7, groupID);

    ASSERT_EQ(point1ID, globalState.getSingleVertex());
    ASSERT_EQ(point1ID + 4, globalState.getNextVerName());
    ASSERT_EQ(1, globalState.getNextEdgeName());

    ASSERT_EQ(2, graph.getVertices().size());
    ASSERT_EQ(0,  graph.getEdges().size());
}

TEST(GraphTests, testSketchInputWConstraint)
{
    GlobalState globalState;

    const int point0ID = 123;
    const int point1ID = 456;

    const int constraint1ID = 789;

    std::vector<int> intInput = {
            0,
            ShapeTypeIDs::POINT,
            point0ID,
            ShapeTypeIDs::POINT,
            point1ID,
            -1,
            // constraints here
            ConstraintTypeIDs::INCIDENCE,
            constraint1ID,
            2, // 2 shapes involved in constraint
            point0ID,
            ShapePartTypeIDs::FIRST_POINT,
            point1ID,
            ShapePartTypeIDs::FIRST_POINT,
            -1
    };
    std::vector<double> dblInput = {
            123.1,
            123.2,
            456.1,
            456.2
    };

    auto graph = Graph();

    int idx = 1;
    int idd = 0;
    int lastIndex = graph.sketchInput(
            globalState,
            idx,
            intInput,
            idd,
            dblInput);

    ASSERT_EQ(intInput.size(), lastIndex);

    ASSERT_EQ(point1ID, globalState.getSingleVertex());
    ASSERT_EQ(point1ID + 4, globalState.getNextVerName());
    ASSERT_EQ(constraint1ID + 1, globalState.getNextEdgeName());

    ASSERT_EQ(2, graph.getVertices().size());
    ASSERT_EQ(1,  graph.getEdges().size());
}
