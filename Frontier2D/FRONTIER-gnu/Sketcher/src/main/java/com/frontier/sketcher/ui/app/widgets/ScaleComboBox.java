package com.frontier.sketcher.ui.app.widgets;

import com.google.common.collect.ImmutableList;

import javax.swing.*;

public class ScaleComboBox extends JComboBox<String> {
    private static final ImmutableList<String> SCALE_VALUES = ImmutableList.of("200", "100", "50");

    public ScaleComboBox() {
        for (var s : SCALE_VALUES) {
            addItem(s);
        }

        setSelectedIndex(1);
    }
}
