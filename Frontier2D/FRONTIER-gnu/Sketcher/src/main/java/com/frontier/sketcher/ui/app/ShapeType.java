package com.frontier.sketcher.ui.app;

/**
 *  This was the initial bgshapes ordering
 *          bgShapes.add(btnCursor);
 *          bgShapes.add(btnPoint);
 *          bgShapes.add(btnLine);
 *          bgShapes.add(btnCircle);
 *          bgShapes.add(btnArc);
 *          bgShapes.add(btnAngleConstraint);
 *          bgShapes.add(btnDistanceConstraint);
 *          bgShapes.add(btnIncidenceConstraint);
 *          bgShapes.add(btnParallelConstraint);
 *          bgShapes.add(btnPerpConstraint);
 *          bgShapes.add(btnTangentConstraint);
 *          bgShapes.add(btnImage);
 */

public enum ShapeType {

    POINT(0),
    LINE(3),
    CIRCLE(4),
    ARC(5),
    IMAGE(6);

    private final int id;

    ShapeType(final int id) {
        this.id = id;
    }

    public int getIdCode() {
        return this.id;
    }
}
