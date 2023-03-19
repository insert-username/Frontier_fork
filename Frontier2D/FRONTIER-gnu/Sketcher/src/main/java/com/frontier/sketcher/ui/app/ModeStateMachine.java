package com.frontier.sketcher.ui.app;

public interface ModeStateMachine {


    enum ModeFlag {
        VALUE_0(0),
        VALUE_1(1),
        DEFAULT_2(2),

        /*
        change constraint value, update
         */
        VALUE_3(3),

        /*
        Add constraint, update.
         */
        VALUE_4(4),

        /*
        Remove constraint, update..
         */
        VALUE_5(5),

        /*
        Add tree/update
         */
        VALUE_6(6),

        /*
         add shape and constraint, update?
         */
        VALUE_7(7),


        VALUE_8(8),
        VALUE_9(9),
        VALUE_10(10),
        VALUE_12(12),
        VALUE_13(13),
        VALUE_14(14),
        VALUE_15(15),
        VALUE_16(16),
        VALUE_17(17),
        VALUE_18(18);

        private final int code;

        ModeFlag(int code) {
            this.code = code;
        }

        public int getCode() {
            return code;
        }
    }


    /**
     * todo: need to figure out what mode is meant to control.
     */
    ModeFlag getModeFlag();

    /**
     * todo: can probably enforce some rules about the mode state machine here.
     */
    void setModeFlag(final ModeFlag value);

}
