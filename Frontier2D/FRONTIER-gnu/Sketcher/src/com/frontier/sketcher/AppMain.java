package com.frontier.sketcher;

import com.google.common.base.Preconditions;

import java.nio.file.Path;

class AppMain {
    public static void main(String[] args) {
        Preconditions.checkArgument(args.length == 1);
        var appRootPath = Path.of(args[0]);

        SKMainFrame mainFrame = new SKMainFrame(appRootPath);
    }
}