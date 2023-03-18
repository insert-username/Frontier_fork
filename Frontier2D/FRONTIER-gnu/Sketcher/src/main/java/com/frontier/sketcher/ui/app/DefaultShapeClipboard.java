package com.frontier.sketcher.ui.app;

import com.frontier.sketcher.ui.items.shapes.SKBaseShape;
import com.google.common.collect.ImmutableList;
import com.google.common.collect.ImmutableSet;
import com.google.common.collect.Lists;

import java.util.LinkedHashSet;
import java.util.List;
import java.util.SortedSet;
import java.util.TreeSet;
import java.util.stream.Collectors;

public class DefaultShapeClipboard implements ShapeClipboard {

    /**
     * Use linked has set to preserve insertion order (perhaps not needed though..?) and uniqueness.
     */
    private final LinkedHashSet<SKBaseShape> shapes = new LinkedHashSet<>();

    @Override
    public int size() {
        return shapes.size();
    }

    @Override
    public ImmutableList<SKBaseShape> getStoredShapes() {
        return shapes.stream()
                .collect(ImmutableList.toImmutableList());
    }

    @Override
    public void clear() {
        shapes.clear();
    }

    @Override
    public void storeShapes(List<SKBaseShape> values) {
        for (final var s : values) {
            storeShape(s);
        }
    }

    @Override
    public void storeShape(SKBaseShape value) {
        if (shapes.contains(value)) {
            throw new IllegalArgumentException("Duplicate shape added to set.");
        }

        shapes.add(value);
    }
}
