package com.frontier.sketcher;

import com.frontier.sketcher.ui.SKMainFrame;
import com.frontier.sketcher.ui.SKOptions;
import com.google.common.base.Preconditions;

import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

class AppMain {
    public static void main(String[] args) throws IOException {
        var logger = LoggerFactory.getLogger(AppMain.class);
        logger.trace("Application Started");

        Preconditions.checkArgument(args.length == 1, "Expected arg: sketcher dir.");
        var appRootPath = Path.of(args[0]);

        SKMainFrame mainFrame = new SKMainFrame(appRootPath.toString());

        SKOptions.loadOptions(Paths.get(appRootPath.toString(), SKOptions.optionFileName).toString());
        mainFrame.setVisible(true);
    }
}