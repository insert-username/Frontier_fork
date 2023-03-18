package com.frontier.sketcher.ui.app.menu;

import com.frontier.sketcher.ui.app.SKApplication;
import com.frontier.sketcher.utils.ResourceLoading;
import com.google.common.base.Preconditions;

import javax.swing.*;
import javax.swing.event.MenuEvent;
import java.awt.event.ActionEvent;
import java.io.IOException;

public class SKEditMenu extends JMenu {

    private final SKApplication skApplication;

    public  JMenuItem mniCut = new JMenuItem();
    public  JMenuItem mniCopy = new JMenuItem();
    public  JMenuItem mniPaste = new JMenuItem();
    public  JMenuItem mniEditDelete = new JMenuItem();
    public  JMenuItem mniSelectAll = new JMenuItem();


    public SKEditMenu(final SKApplication skApplication) throws IOException {
        super("Edit");

        this.skApplication = Preconditions.checkNotNull(skApplication);

        init();
    }

    private void init() throws IOException {

        mniSelectAll.setText("Select All");
        mniSelectAll.addActionListener(skApplication::mniSelectAll_actionPerformed);


        mniCut.addActionListener(skApplication::mniCut_actionPerformed);

        mniCopy.addActionListener(skApplication::mniCopy_actionPerformed);

        mniPaste.addActionListener(skApplication::mniPaste_actionPerformed);


        addMenuListener(
                new javax.swing.event.MenuListener()
                {

                    public void menuCanceled(MenuEvent e)
                    {
                    }

                    public void menuDeselected(MenuEvent e)
                    {
                    }

                    public void menuSelected(MenuEvent e)
                    {
                        mniEdit_menuSelected(e);
                    }
                });

        mniCut.setText("Cut");
        mniCut.setIcon(ResourceLoading.loadImageIcon("cut.gif"));
        mniCopy.setText("Copy");
        mniCopy.setIcon(ResourceLoading.loadImageIcon("copy.gif"));
        mniPaste.setText("Paste");
        mniPaste.setIcon(ResourceLoading.loadImageIcon("paste.gif"));
        mniEditDelete.setText("Delete");
        mniEditDelete.setIcon(ResourceLoading.loadImageIcon("delete.gif"));
        mniEditDelete.addActionListener(
                new java.awt.event.ActionListener()
                {

                    public void actionPerformed(ActionEvent e)
                    {skApplication.mniDelete_actionPerformed(e);
                    }
                });

        this.add(mniCut);
        this.add(mniCopy);
        this.add(mniPaste);
        this.add(mniSelectAll);
        this.addSeparator();
        this.add(mniEditDelete);
        this.addSeparator();
    }


    /**
     * Handles mouse events on edit menu.
     */
    private void mniEdit_menuSelected(MenuEvent e)
    {
        // for(int i=0; i<allshapes.size(); i++)
        //    System.out.println(allshapes.get(i)+" "+allshapes.get(i).ShapeTypeID);

        final var selectionModel = skApplication.getItemSelectionModel();

        final boolean hasSelectedShapes = selectionModel.selectedShapesCount() > 0;
        final boolean hasSelectedConstraint = selectionModel.selectedConstraint().isPresent();

        mniCut.setEnabled(hasSelectedShapes);
        mniCopy.setEnabled(hasSelectedShapes);

        if(skApplication.getUpdateFlag() && (!(skApplication.getModeFlag()==5))) {
            mniEditDelete.setEnabled(false);
        } else {
            mniEditDelete.setEnabled(hasSelectedShapes || hasSelectedConstraint);
        }

        mniPaste.setEnabled(skApplication.getShapeClipboard().size()>0);
    }
}
