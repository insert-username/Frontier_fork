/**
 * Title:        SKTangentConstraint
 * Description:  Represents tangent Line-Circle constraints
 */
package com.frontier.sketcher.ui.items.constraints;

import com.frontier.sketcher.ui.items.shapes.SKCircleShape;
import com.frontier.sketcher.ui.items.shapes.SKLineShape;
import com.frontier.sketcher.ui.SKMainFrame;
import com.frontier.sketcher.ui.items.shapes.SKShapeArray;

public class SKTangentConstraint extends SKBaseConstraint
{

   public SKTangentConstraint(SKMainFrame frameMain, int idnum)
   {
      super(frameMain,idnum,"Tangent"+idnum,"Tangent",5);

      autoAddShapeProps = false;
      drawMode = 1;
   }

   public boolean setMainData(String PropName, Object PropData)
   {
      return super.setMainData(PropName,PropData);
   }

   public static boolean isAvailable(SKShapeArray selectedShapes)
   {
      return ( (selectedShapes.get(0) instanceof SKLineShape) && (selectedShapes.get(1) instanceof SKCircleShape) ||
                (selectedShapes.get(0) instanceof SKCircleShape) && (selectedShapes.get(1) instanceof SKLineShape) ||
                (selectedShapes.get(0) instanceof SKCircleShape) && (selectedShapes.get(1) instanceof SKCircleShape)
             );
   }
}