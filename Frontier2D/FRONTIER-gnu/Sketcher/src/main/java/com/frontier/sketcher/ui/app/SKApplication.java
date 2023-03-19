package com.frontier.sketcher.ui.app;

import java.awt.event.ActionEvent;
import java.awt.event.ItemEvent;

public interface SKApplication {

    ShapeClipboard getShapeClipboard();

    /**
     * @return the model representing the current application selection
     * state.
     */
    ItemSelectionModel getItemSelectionModel();

    /**
     * todo: need to figure out what mode is meant to control.
     */
    int getModeFlag();

    /**
     * todo: can probably enforce some rules about the mode state machine here.
     */
    void setModeFlag(final int value);


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
}
