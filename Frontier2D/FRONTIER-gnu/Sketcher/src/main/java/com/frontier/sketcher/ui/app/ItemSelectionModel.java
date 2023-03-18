package com.frontier.sketcher.ui.app;

import com.frontier.sketcher.ui.items.constraints.SKBaseConstraint;
import com.frontier.sketcher.ui.items.shapes.SKBaseShape;
import com.frontier.sketcher.ui.items.shapes.SKShapeArray;
import com.google.common.collect.ImmutableList;

import java.util.Optional;

public interface ItemSelectionModel {

    int selectedShapesCount();
    SKBaseShape selectedShapeAt(int i);
    ImmutableList<SKBaseShape> selectedShapes();

    /**
     * @deprecated "Use #selectedShapes instead"
     */
    @Deprecated
    SKShapeArray selectedShapesShapeArray();
    void deselectShape(SKBaseShape oldShape);
    void selectShape(SKBaseShape oldShape);
    void clearSelectedShapes();

    Optional<SKBaseConstraint> selectedConstraint();

    default void deselectConstraint() {
        deselectConstraint(false);
    }

    void deselectConstraint(final boolean ignoreAlreadyDeselected);
    void selectConstraint(SKBaseConstraint con);

    boolean isConstraintSelected(SKBaseConstraint con);
}
