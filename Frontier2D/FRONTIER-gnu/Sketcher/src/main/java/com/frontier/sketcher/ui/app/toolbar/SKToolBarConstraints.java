package com.frontier.sketcher.ui.app.toolbar;

import com.frontier.sketcher.ui.SKOptions;
import com.frontier.sketcher.ui.app.ConstraintType;
import com.frontier.sketcher.ui.app.SKApplication;
import com.frontier.sketcher.ui.items.constraints.*;
import com.frontier.sketcher.utils.ResourceLoading;
import com.google.common.base.Preconditions;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;

public class SKToolBarConstraints extends JToolBar {

    private final JToggleButton btnAngleConstraint = new JToggleButton();
    private final JToggleButton btnDistanceConstraint = new JToggleButton();
    private final JToggleButton btnTangentConstraint = new JToggleButton();
    private final JToggleButton btnPerpConstraint = new JToggleButton();
    private final JToggleButton btnParallelConstraint = new JToggleButton();
    private final JToggleButton btnIncidenceConstraint = new JToggleButton();

    private final SKApplication skApplication;

    private final ButtonGroup buttonGroup;

    public SKToolBarConstraints(SKApplication skApplication, ButtonGroup buttonGroup) throws IOException {
        this.skApplication = Preconditions.checkNotNull(skApplication);
        this.buttonGroup = Preconditions.checkNotNull(buttonGroup);

        init();
    }

    private void init() throws IOException {
        btnAngleConstraint.setIcon(ResourceLoading.loadImageIcon("angle.gif"));
        btnAngleConstraint.setToolTipText("Angle Constraint");
        btnAngleConstraint.setPreferredSize(new Dimension(30, 27));
        btnAngleConstraint.setMaximumSize(new Dimension(30, 27));
        btnAngleConstraint.setEnabled(false);
        btnAngleConstraint.addActionListener(skApplication::mniAngleConstr_actionPerformed);

        btnDistanceConstraint.setMinimumSize(new Dimension(30, 27));
        btnDistanceConstraint.addActionListener(skApplication::mniDistanceConstr_actionPerformed);
        btnDistanceConstraint.setMaximumSize(new Dimension(30, 27));
        btnDistanceConstraint.setEnabled(false);
        btnDistanceConstraint.setPreferredSize(new Dimension(30, 27));
        btnDistanceConstraint.setToolTipText("Distance Constraint");
        btnDistanceConstraint.setIcon(ResourceLoading.loadImageIcon("distance.gif"));

        btnTangentConstraint.setMinimumSize(new Dimension(30, 27));
        btnTangentConstraint.addActionListener(skApplication::mniTangentConstraint_actionPerformed);
        btnTangentConstraint.setMaximumSize(new Dimension(30, 27));
        btnTangentConstraint.setEnabled(false);
        btnTangentConstraint.setPreferredSize(new Dimension(30, 27));
        btnTangentConstraint.setToolTipText("Tangent Constraint");
        btnTangentConstraint.setIcon(ResourceLoading.loadImageIcon("tangent.gif"));

        setOrientation(JToolBar.VERTICAL);
        if (skApplication.getUpdateFlag() && (!((skApplication.getModeFlag() == 4) || (skApplication.getModeFlag() == 7)))) {
            setEnabled(false);
        }

        btnPerpConstraint.setIcon(ResourceLoading.loadImageIcon("perp.gif"));
        btnPerpConstraint.setToolTipText("Perpendicular Constraint");
        btnPerpConstraint.setPreferredSize(new Dimension(30, 27));
        btnPerpConstraint.setMaximumSize(new Dimension(30, 27));
        btnPerpConstraint.setEnabled(false);
        btnPerpConstraint.setMinimumSize(new Dimension(30, 27));
        btnPerpConstraint.addActionListener(skApplication::mniPerpConstraint_actionPerformed);

        btnParallelConstraint.setIcon(ResourceLoading.loadImageIcon("parallel.gif"));
        btnParallelConstraint.setToolTipText("Parallel Constraint");
        btnParallelConstraint.setPreferredSize(new Dimension(30, 27));
        btnParallelConstraint.setMaximumSize(new Dimension(30, 27));
        btnParallelConstraint.setEnabled(false);
        btnParallelConstraint.setMinimumSize(new Dimension(30, 27));
        btnParallelConstraint.addActionListener(skApplication::mniParallelConstraint_actionPerformed);

        btnIncidenceConstraint.setIcon(ResourceLoading.loadImageIcon("incident.gif"));
        btnIncidenceConstraint.setToolTipText("Incidence Constraint");
        btnIncidenceConstraint.setPreferredSize(new Dimension(30, 27));
        btnIncidenceConstraint.setMaximumSize(new Dimension(30, 27));
        btnIncidenceConstraint.setEnabled(false);
        btnIncidenceConstraint.setMinimumSize(new Dimension(30, 27));
        btnIncidenceConstraint.addActionListener(skApplication::mniIncidenceConstr_actionPerformed);

        this.add(btnAngleConstraint, null);
        this.add(btnDistanceConstraint, null);
        this.add(btnIncidenceConstraint, null);
        this.add(btnParallelConstraint, null);
        this.add(btnPerpConstraint, null);
        this.add(btnTangentConstraint, null);

        //Group Shape buttons together
        this.buttonGroup.add(btnAngleConstraint);
        this.buttonGroup.add(btnDistanceConstraint);
        this.buttonGroup.add(btnIncidenceConstraint);
        this.buttonGroup.add(btnParallelConstraint);
        this.buttonGroup.add(btnPerpConstraint);
        this.buttonGroup.add(btnTangentConstraint);
    }

    public void updateConstraintUI() {
        final var constraintModel = skApplication.getConstraintModel();
        btnAngleConstraint.setEnabled(constraintModel.canApplyConstraintType(ConstraintType.ANGLE));
        btnDistanceConstraint.setEnabled(constraintModel.canApplyConstraintType(ConstraintType.DISTANCE));
        btnIncidenceConstraint.setEnabled(constraintModel.canApplyConstraintType(ConstraintType.INCIDENCE));
        btnParallelConstraint.setEnabled(constraintModel.canApplyConstraintType(ConstraintType.PARALLEL));
        btnPerpConstraint.setEnabled(constraintModel.canApplyConstraintType(ConstraintType.PERPENDICULAR));
        btnTangentConstraint.setEnabled(constraintModel.canApplyConstraintType(ConstraintType.TANGENT));
    }
}
