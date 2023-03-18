/**
 * Title:        SKUTU
 * Description:  Sketcher's Universal Translation Unit abstract class.  All UTUs should descend from this class
 */
package com.frontier.sketcher.ui.utu;

import com.frontier.sketcher.ui.app.SKMainFrame;
import com.frontier.sketcher.ui.items.constraints.SKConstraintArray;
import com.frontier.sketcher.ui.items.shapes.SKShapeArray;

public abstract class SKUTU
{
   public abstract void solveSystem(SKMainFrame frameMain, SKShapeArray shapes, SKConstraintArray cons);
}