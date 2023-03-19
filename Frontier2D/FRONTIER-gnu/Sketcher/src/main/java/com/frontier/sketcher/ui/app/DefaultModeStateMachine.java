package com.frontier.sketcher.ui.app;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class DefaultModeStateMachine implements ModeStateMachine {

    private Logger logger = LoggerFactory.getLogger(getClass().getName());

    private ModeFlag modeFlag = ModeFlag.DEFAULT_2;

    @Override
    public ModeFlag getModeFlag() {
        return modeFlag;
    }

    @Override
    public void setModeFlag(ModeFlag value) {
        logger.trace("Mode state transition from " + this.modeFlag + " to " + value);
        this.modeFlag = value;
    }
}
