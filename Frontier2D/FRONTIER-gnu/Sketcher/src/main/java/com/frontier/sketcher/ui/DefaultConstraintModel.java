package com.frontier.sketcher.ui;

import com.frontier.sketcher.ui.app.ConstraintModel;
import com.frontier.sketcher.ui.app.ConstraintType;
import com.frontier.sketcher.ui.app.ItemSelectionModel;
import com.frontier.sketcher.ui.app.SKApplication;
import com.frontier.sketcher.ui.items.constraints.*;
import com.google.common.base.Function;
import com.google.common.base.Preconditions;
import com.google.common.collect.ImmutableMap;

import java.util.Map;
import java.util.concurrent.Callable;

public class DefaultConstraintModel implements ConstraintModel {

    private final SKApplication skApplication;

    public DefaultConstraintModel(SKApplication skApplication) {
        this.skApplication = Preconditions.checkNotNull(skApplication);
    }

    @Override
    public boolean canApplyConstraintType(ConstraintType constraintType) {
        final var selectionModel = skApplication.getItemSelectionModel();
        final var update = skApplication.getUpdateFlag();
        final var mode = skApplication.getModeFlag();

        if (update && (!((mode == 4) || (mode == 7))) && (SKOptions.byteOptions[SKOptions.onlineMode] == 0)) {
            return false;
        } else if (skApplication.getItemSelectionModel().selectedShapesCount() == 2) {
            return checkConstraintTypeAvailable(constraintType, selectionModel);
        } else {
            return false;
        }
    }

    private boolean checkConstraintTypeAvailable(final ConstraintType constraintType,
                                                 final ItemSelectionModel selectionModel) {
        switch (constraintType) {
            case ANGLE -> {
                return SKAngleConstraint.isAvailable(selectionModel);
            }
            case DISTANCE -> {
                return SKDistanceConstraint.isAvailable(selectionModel);
            }
            case INCIDENCE -> {
                return SKIncidenceConstraint.isAvailable(selectionModel);
            }
            case PARALLEL -> {
                return SKParallelConstraint.isAvailable(selectionModel);
            }
            case PERPENDICULAR -> {
                return SKPerpConstraint.isAvailable(selectionModel);
            }
            case TANGENT -> {
                return SKTangentConstraint.isAvailable(selectionModel);
            }
            default -> throw new IllegalArgumentException("Unknown constraint");
        }
    }
}
