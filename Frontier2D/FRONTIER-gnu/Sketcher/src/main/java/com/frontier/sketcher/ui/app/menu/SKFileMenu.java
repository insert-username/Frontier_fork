package com.frontier.sketcher.ui.app.menu;

import com.frontier.sketcher.ui.app.SKApplication;
import com.frontier.sketcher.utils.ResourceLoading;
import com.google.common.base.Preconditions;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;

public class SKFileMenu extends JMenu {

    private final SKApplication skApplication;

    private final JMenuItem mniNew = new JMenuItem();
    private final JMenuItem mniOpen = new JMenuItem();
    private final JMenuItem mniReopen = new JMenuItem();
    private final JMenuItem mniSave = new JMenuItem();
    private final JMenuItem mniSaveAs = new JMenuItem();
    private final JMenuItem mniPref = new JMenuItem();
    private final JMenuItem mniExit = new JMenuItem();

    public SKFileMenu(final SKApplication skApplication) throws IOException {
        super("File");
        this.skApplication = Preconditions.checkNotNull(skApplication);

        init();
    }

    private void init() throws IOException {

        mniNew.setPreferredSize(new Dimension(70, 21));
        mniNew.setIcon(ResourceLoading.loadImageIcon("new.gif"));
        mniNew.setText("New");
        mniNew.addActionListener(skApplication::mniNew_actionPerformed);

        mniOpen.setIcon(ResourceLoading.loadImageIcon("open.gif"));
        mniOpen.setText("Open");
        mniOpen.addActionListener(skApplication::mniOpen_actionPerformed);

        mniExit.setIcon(ResourceLoading.loadImageIcon("exit.gif"));
        mniExit.setText("Exit");
        mniExit.addActionListener(skApplication::mniExit_actionPerformed);

        mniSave.setIcon(ResourceLoading.loadImageIcon("save.gif"));
        mniSave.setText("Save");
        mniSave.addActionListener(skApplication::mniSave_actionPerformed);

        mniSaveAs.setIcon(ResourceLoading.loadImageIcon("saveas.gif"));
        mniSaveAs.setText("Save as...");
        mniSaveAs.addActionListener(skApplication::mniSaveAs_actionPerformed);

        mniPref.setText("Preferences");
        mniPref.setIcon(ResourceLoading.loadImageIcon("/props.gif"));
        mniPref.addActionListener(skApplication::mniPref_actionPerformed);

        this.add(mniNew);
        this.add(mniOpen);
        this.add(mniReopen);
        this.add(mniSave);
        this.add(mniSaveAs);
        this.addSeparator();
        this.add(mniPref);
        this.addSeparator();
        this.add(mniExit);
    }

}
