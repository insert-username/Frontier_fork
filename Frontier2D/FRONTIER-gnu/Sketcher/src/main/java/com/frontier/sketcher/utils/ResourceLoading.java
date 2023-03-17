package com.frontier.sketcher.utils;

import com.google.common.base.Preconditions;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.io.IOException;
import java.nio.file.Paths;
import java.util.Objects;

public class ResourceLoading {


    public static ImageIcon loadImageIcon(final String imageName) throws IOException {

        final String fullpath = Paths.get("/com/frontier/sketcher/ui/images", imageName).toString();

        try (var resourceStream = ResourceLoading.class.getResourceAsStream(fullpath)) {
            var bufferedImage = ImageIO.read(Preconditions.checkNotNull(resourceStream));
            return new ImageIcon(bufferedImage);
        }
    }

}