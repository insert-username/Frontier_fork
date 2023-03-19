package com.frontier.sketcher.ui.app.toolbar;

import com.frontier.sketcher.ui.app.SKApplication;
import com.frontier.sketcher.ui.app.ShapeType;
import com.frontier.sketcher.utils.ResourceLoading;
import com.google.common.base.Preconditions;

import javax.swing.*;
import javax.swing.text.html.Option;
import java.awt.*;
import java.io.IOException;
import java.util.Optional;

public class SKToolBarShapes extends JToolBar {

    private final JToggleButton btnCursor = new JToggleButton();
    private final JToggleButton btnPoint = new JToggleButton();
    private final JToggleButton btnLine = new JToggleButton();
    private final JToggleButton btnCircle = new JToggleButton();
    private final JToggleButton btnArc = new JToggleButton();
    private final JToggleButton btnImage = new JToggleButton();

    private final SKApplication skApplication;
    private final ButtonGroup buttonGroup;

    public SKToolBarShapes(final SKApplication skApplication,
                           final ButtonGroup buttonGroup) throws IOException {
        this.skApplication = Preconditions.checkNotNull(skApplication);
        this.buttonGroup = buttonGroup;
        init(buttonGroup);
    }

    public boolean isCursorSelected() {
        return btnCursor.isSelected();
    }

    public Optional<ShapeType> getSelectedShapeType() {
        if (this.buttonGroup.isSelected(btnPoint.getModel())) {
            return Optional.of(ShapeType.POINT);
        } else if (this.buttonGroup.isSelected(btnLine.getModel())) {
            return Optional.of(ShapeType.LINE);
        } else if (this.buttonGroup.isSelected(btnArc.getModel())) {
            return Optional.of(ShapeType.ARC);
        } else if (this.buttonGroup.isSelected(btnCircle.getModel())) {
            return Optional.of(ShapeType.CIRCLE);
        } else if (this.buttonGroup.isSelected(btnImage.getModel())) {
            return Optional.of(ShapeType.IMAGE);
        }

        return Optional.empty();
    }

    private void init(ButtonGroup buttonGroup) throws IOException {
        btnCircle.setMaximumSize(new Dimension(30, 27));
        btnCircle.addActionListener(skApplication::ShapeButton_actionPerformed);
        btnCircle.setPreferredSize(new Dimension(30, 27));
        btnCircle.setToolTipText("Circle");
        btnCircle.setIcon(ResourceLoading.loadImageIcon("circle.gif"));

        btnArc.setMaximumSize(new Dimension(30, 27));
        btnArc.addActionListener(skApplication::ShapeButton_actionPerformed);
        btnArc.setPreferredSize(new Dimension(30, 27));
        btnArc.setToolTipText("Arc");
        btnArc.setIcon(ResourceLoading.loadImageIcon("arc.gif"));

        btnPoint.setIcon(ResourceLoading.loadImageIcon("point.gif"));
        btnPoint.setToolTipText("Point");
        btnPoint.setPreferredSize(new Dimension(30, 27));
        btnPoint.setMaximumSize(new Dimension(30, 27));
        btnPoint.setMinimumSize(new Dimension(30, 27));
        btnPoint.addActionListener(skApplication::ShapeButton_actionPerformed);

        btnCursor.setMaximumSize(new Dimension(30, 27));
        btnCursor.addActionListener(skApplication::btnCursor_actionPerformed);
        btnCursor.setPreferredSize(new Dimension(30, 27));
        btnCursor.setToolTipText("Deselects shapes");
        btnCursor.setIcon(ResourceLoading.loadImageIcon("cursor.gif"));
        btnCursor.setSelected(true);

        btnLine.setIcon(ResourceLoading.loadImageIcon("lineseg.gif"));
        btnLine.setToolTipText("Line Segment");
        btnLine.setPreferredSize(new Dimension(30, 27));
        btnLine.setMaximumSize(new Dimension(30, 27));
        btnLine.setMinimumSize(new Dimension(30, 27));
        btnLine.addActionListener(skApplication::ShapeButton_actionPerformed);

        btnImage.setIcon(ResourceLoading.loadImageIcon("imageicon.gif"));
        btnImage.setToolTipText("Image");
        btnImage.setPreferredSize(new Dimension(30, 27));
        btnImage.setMaximumSize(new Dimension(30, 27));
        btnImage.setMinimumSize(new Dimension(30, 27));
        btnImage.addActionListener(skApplication::ShapeButton_actionPerformed);

        this.setOrientation(JToolBar.VERTICAL);

        this.add(btnCursor, null);
        this.add(btnPoint, null);
        this.add(btnLine, null);
        this.add(btnCircle, null);
        this.add(btnArc, null);
        this.add(btnImage,null);

        buttonGroup.add(btnCursor);
        buttonGroup.add(btnPoint);
        buttonGroup.add(btnLine);
        buttonGroup.add(btnCircle);
        buttonGroup.add(btnCursor);
        buttonGroup.add(btnImage);
    }

    public void setCursorSelected(boolean b) {
        btnCursor.setSelected(b);
    }
}
