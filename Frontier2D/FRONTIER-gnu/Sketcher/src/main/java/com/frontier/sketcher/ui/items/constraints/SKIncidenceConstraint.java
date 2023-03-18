/**
 * Title:        SKBaseIncidenceConstraint
 * Description:  Incidence Constraint
 */
package com.frontier.sketcher.ui.items.constraints;

import com.frontier.sketcher.ui.app.SKMainFrame;

public class SKIncidenceConstraint extends SKNormalConstraint
   {
      public SKIncidenceConstraint(SKMainFrame frameMain, int idnum)
      {
         super(frameMain,idnum,"Incidence"+idnum,"Incidence",1);
      }
   
   }