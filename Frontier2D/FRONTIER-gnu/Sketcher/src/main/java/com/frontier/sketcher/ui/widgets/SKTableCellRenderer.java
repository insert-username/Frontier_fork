/**
 * Title:        SKTableCellRenderer
 * Description:  Table cell renderer for ShapeProp table
 */
package com.frontier.sketcher.ui.widgets;

import com.frontier.sketcher.ui.properties.SKProp;

import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.JTable;
import java.awt.Component;

public class SKTableCellRenderer extends DefaultTableCellRenderer
{

   public SKTableCellRenderer()
   {
   }

   public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column)
   {
      if (value instanceof SKProp)
      {
         setForeground( ((SKProp)value).foregroundColor );
      }

      return super.getTableCellRendererComponent(table,value,isSelected,hasFocus,row,column);
   }

}