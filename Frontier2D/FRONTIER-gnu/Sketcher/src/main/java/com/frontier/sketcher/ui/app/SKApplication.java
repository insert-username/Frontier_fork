package com.frontier.sketcher.ui.app;

import java.awt.event.ActionEvent;
import java.awt.event.ItemEvent;

public interface SKApplication {

    enum ModeFlag {
        VALUE_0(0),
        VALUE_1(1),
        DEFAULT_2(2),

        /*
        change constraint value, update
         */
        VALUE_3(3),

        /*
        Add constraint, update.
         */
        VALUE_4(4),

        /*
        Remove constraint, update..
         */
        VALUE_5(5),

        /*
        Add tree/update
         */
        VALUE_6(6),

        /*
         add shape and constraint, update?
         */
        VALUE_7(7),


        VALUE_8(8),
        VALUE_9(9),
        VALUE_10(10),
        VALUE_12(12),
        VALUE_13(13),
        VALUE_14(14),
        VALUE_15(15),
        VALUE_16(16),
        VALUE_17(17),
        VALUE_18(18);

        private final int code;

        ModeFlag(int code) {
            this.code = code;
        }

        public int getCode() {
            return code;
        }
    }

    ShapeClipboard getShapeClipboard();

    /**
     * @return the model representing the current application selection
     * state.
     */
    ItemSelectionModel getItemSelectionModel();

    /**
     * todo: need to figure out what mode is meant to control.
     */
    ModeFlag getModeFlag();

    /**
     * todo: can probably enforce some rules about the mode state machine here.
     */
    void setModeFlag(final ModeFlag value);


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
