package com.frontier.sketcher.ui.app.menu;

import com.frontier.sketcher.utils.ResourceLoading;

import javax.swing.*;
import java.io.IOException;

public class SKHelpMenu extends JMenu {

    private final JMenuItem mniAbout = new JMenuItem();

    public SKHelpMenu() throws IOException {
        super("Help");
        init();
    }


    private void init() throws IOException {

        mniAbout.setText("About");
        mniAbout.setIcon(ResourceLoading.loadImageIcon("about.gif"));

        this.add(mniAbout);
    }

}
