package com.frontier.sketcher.ui.app;

import java.awt.event.ActionEvent;
import java.awt.event.ItemEvent;

public interface SKApplication {

    ModeStateMachine getModeStateMachine();

    ShapeClipboard getShapeClipboard();

    /**
     * @return the model representing the current application selection
     * state.
     */
    ItemSelectionModel getItemSelectionModel();

    boolean getUpdateFlag();

    void setUpdateFlag(final boolean value);

    /*
     * File menu
     */
    void mniNew_actionPerformed(final ActionEvent e);
    void mniOpen_actionPerformed(final ActionEvent e);
    void mniExit_actionPerformed(final ActionEvent e);
    void mniSave_actionPerformed(final ActionEvent e);
    void mniSaveAs_actionPerformed(final ActionEvent e);
    void mniPref_actionPerformed(final ActionEvent e);

    /*
     * Edit menu
     */
    void mniSelectAll_actionPerformed(final ActionEvent e);
    void mniCut_actionPerformed(final ActionEvent e);
    void mniCopy_actionPerformed(final ActionEvent e);
    void mniPaste_actionPerformed(final ActionEvent e);
    void mniDelete_actionPerformed(final ActionEvent e);

    /*
    Main toolbar
     */
    void cmbScale_itemStateChanged(ItemEvent itemEvent);


    /*
    Shapes toolbar
     */
    void ShapeButton_actionPerformed(ActionEvent e);
    void btnCursor_actionPerformed(ActionEvent actionEvent);

    /*
    Constraints toolbar
     */
    void mniAngleConstr_actionPerformed(ActionEvent actionEvent);
    void mniDistanceConstr_actionPerformed(ActionEvent actionEvent);
    void mniTangentConstraint_actionPerformed(ActionEvent actionEvent);
    void mniPerpConstraint_actionPerformed(ActionEvent actionEvent);
    void mniParallelConstraint_actionPerformed(ActionEvent actionEvent);
    void mniIncidenceConstr_actionPerformed(ActionEvent actionEvent);

    ConstraintModel getConstraintModel();
}
