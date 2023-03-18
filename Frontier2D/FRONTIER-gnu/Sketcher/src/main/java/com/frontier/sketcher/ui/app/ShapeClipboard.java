package com.frontier.sketcher.ui.app;

import com.frontier.sketcher.ui.items.shapes.SKBaseShape;
import com.google.common.collect.ImmutableList;
import com.google.common.collect.ImmutableSet;

import java.util.List;

public interface ShapeClipboard {

    int size();

    ImmutableList<SKBaseShape> getStoredShapes();

    void clear();

    void storeShapes(List<SKBaseShape> values);

    void storeShape(SKBaseShape values);

}
