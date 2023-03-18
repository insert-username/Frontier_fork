package com.frontier.sketcher.ui.app;

import com.frontier.sketcher.ui.items.constraints.SKBaseConstraint;
import com.frontier.sketcher.ui.items.shapes.SKBaseShape;
import com.frontier.sketcher.ui.items.shapes.SKShapeArray;
import com.google.common.base.Preconditions;
import com.google.common.collect.ImmutableList;

import java.util.Optional;

public class DefaultItemSelectionModel implements ItemSelectionModel {

    private final SKShapeArray selectedShapes = new SKShapeArray(100);

    private SKBaseConstraint selectedConstraint = null;

    @Override
    public int selectedShapesCount() {
        return selectedShapes.size();
    }

    @Override
    public SKBaseShape selectedShapeAt(int i) {
        Preconditions.checkElementIndex(i, selectedShapes.size());

        return selectedShapes.get(i);
    }

    @Override
    public ImmutableList<SKBaseShape> selectedShapes() {
        return selectedShapes.asList();
    }

    @Override
    public SKShapeArray selectedShapesShapeArray() {
        return selectedShapes;
    }

    @Override
    public void deselectShape(final SKBaseShape oldShape) {
        Preconditions.checkNotNull(oldShape);

        if (!selectedShapes.removeShape(oldShape)) {
            throw new IllegalArgumentException("Shape cannot be removed: not added");
        }
    }

    @Override
    public void selectShape(SKBaseShape oldShape) {
        Preconditions.checkNotNull(oldShape);

        if (selectedShapes.asList().contains(oldShape)) {
            // todo: maybe this is not necessary. However be strict for now.
            throw new IllegalArgumentException("Attempt to add duplicate shape.");
        }

        selectedShapes.add(oldShape);
    }

    @Override
    public void clearSelectedShapes() {
        selectedShapes.clear();
    }

    @Override
    public Optional<SKBaseConstraint> selectedConstraint() {
        return Optional.ofNullable(selectedConstraint);
    }

    @Override
    public void deselectConstraint(final boolean ignoreAlreadyDeselected) {
        if (!ignoreAlreadyDeselected && selectedConstraint().isEmpty()) {
            throw new IllegalStateException("Constraint already deselected.");
        }

        this.selectedConstraint = null;
    }

    @Override
    public void selectConstraint(final SKBaseConstraint con) {
        Preconditions.checkNotNull(con);

        if (selectedConstraint().isPresent()) {
            throw new IllegalArgumentException("A constraint is already selected.");
        }

        this.selectedConstraint = con;
    }

    @Override
    public boolean isConstraintSelected(SKBaseConstraint con) {
        Preconditions.checkNotNull(con);
        return this.selectedConstraint == con;
    }
}
