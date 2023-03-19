package com.frontier.sketcher.ui.app.toolbar;

import com.frontier.sketcher.ui.app.SKApplication;
import com.frontier.sketcher.ui.app.widgets.ScaleComboBox;
import com.frontier.sketcher.ui.widgets.JSmallButton;
import com.frontier.sketcher.utils.ResourceLoading;
import com.google.common.base.Preconditions;
import com.google.common.collect.ImmutableList;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;

public class SKToolBarMain extends JToolBar {

    private final JSmallButton btnNew = new JSmallButton();
    private final JSmallButton btnOpen = new JSmallButton();
    private final JSmallButton btnSave = new JSmallButton();
    private final JSmallButton btnSaveAs = new JSmallButton();
    private final JSmallButton btnExit = new JSmallButton();

    public  JComboBox<String> cmbScale = new ScaleComboBox();

    private final SKApplication skApplication;

    public SKToolBarMain(SKApplication skApplication) throws IOException {
        this.skApplication = Preconditions.checkNotNull(skApplication);
    }

    void init() throws IOException {
        cmbScale.setMaximumSize(new Dimension(90, 26));
        cmbScale.setEditable(true);
        cmbScale.addItemListener(skApplication::cmbScale_itemStateChanged);

        btnNew.setMaximumSize(new Dimension(24, 24));
        btnNew.setPreferredSize(new Dimension(24, 24));
        btnNew.setToolTipText("Creates a new project");
        btnNew.setIcon(ResourceLoading.loadImageIcon("new.gif"));
        btnNew.addActionListener(skApplication::mniNew_actionPerformed);

        btnOpen.setMaximumSize(new Dimension(24, 24));
        btnOpen.setPreferredSize(new Dimension(24, 24));
        btnOpen.setToolTipText("Open a project");
        btnOpen.setIcon(ResourceLoading.loadImageIcon("open.gif"));
        btnOpen.addActionListener(skApplication::mniOpen_actionPerformed);

        btnExit.setMaximumSize(new Dimension(24, 24));
        btnExit.setPreferredSize(new Dimension(24, 24));
        btnExit.setToolTipText("Exit Sketcher");
        btnExit.setIcon(ResourceLoading.loadImageIcon("exit.gif"));
        btnExit.addActionListener(skApplication::mniExit_actionPerformed);

        btnSave.setMaximumSize(new Dimension(24, 24));
        btnSave.setPreferredSize(new Dimension(24, 24));
        btnSave.setToolTipText("Save current project");
        btnSave.setIcon(ResourceLoading.loadImageIcon("save.gif"));
        btnSave.addActionListener(skApplication::mniSave_actionPerformed);

        btnSaveAs.setMaximumSize(new Dimension(24, 24));
        btnSaveAs.setPreferredSize(new Dimension(24, 24));
        btnSaveAs.setToolTipText("Save current project as another file");
        btnSaveAs.setIcon(ResourceLoading.loadImageIcon("saveas.gif"));
        btnSaveAs.addActionListener(skApplication::mniSaveAs_actionPerformed);

        this.add(btnNew, null);
        this.add(btnOpen, null);
        this.add(btnSave, null);
        this.add(btnSaveAs, null);
        this.addSeparator();
        this.add(btnExit, null);
        this.addSeparator();
        this.addSeparator();
        this.addSeparator();
        this.add(new JLabel("Scale "), null);
        this.add(cmbScale, null);
    }
}
