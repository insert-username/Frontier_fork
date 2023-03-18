package com.frontier.sketcher.ui.app;

import java.awt.event.ActionEvent;

public interface SKApplication {


    void mniNew_actionPerformed(final ActionEvent e);
    void mniOpen_actionPerformed(final ActionEvent e);
    void mniExit_actionPerformed(final ActionEvent e);
    void mniSave_actionPerformed(final ActionEvent e);
    void mniSaveAs_actionPerformed(final ActionEvent e);
    void mniPref_actionPerformed(final ActionEvent e);

}
