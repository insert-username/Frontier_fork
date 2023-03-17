package com.frontier.sketcher;

import com.frontier.sketcher.ui.SKMainFrame;
import com.frontier.sketcher.ui.SKOptions;
import com.google.common.base.Preconditions;

import java.nio.file.Path;
import java.nio.file.Paths;

class AppMain {
    public static void main(String[] args) {
        Preconditions.checkArgument(args.length == 1);
        var appRootPath = Path.of(args[0]);

        SKMainFrame mainFrame = new SKMainFrame(appRootPath.toString());

        SKOptions.loadOptions(Paths.get(appRootPath.toString(), SKOptions.optionFileName).toString());
        mainFrame.setVisible(true);
    }
}