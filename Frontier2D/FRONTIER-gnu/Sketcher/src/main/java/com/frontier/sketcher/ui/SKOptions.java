/**
 * Title:        SKOptions
 * Description:  This is where all options are handled
 */
package com.frontier.sketcher.ui;

import java.awt.Color;
import java.io.*;

public class SKOptions
{
//Default option filename
   public static final String optionFileName = new String("/sketcher.prf");

//Byte Options (options that can be represented by boolean or int -128 <= x <= 127)
   public static byte[]     byteOptions = new byte[5];
   public static final int  groupDisplayMode      = 0;  // 0=grp ids only, 1=grp ids and boxes
   public static final int unSelGroupsWhen       = 1;  // 0=Manual unselect only, 1=When leaving Group tab
   public static final int simpleSolverMode      = 2;  // 0=Don't use SimpleSolver, 1=Solve at end of drag, 2=Solve while dragging
   public static final int simpleSolverVersion   = 3;
   public static final int onlineMode            = 4;

//Color Options
   public static Color[]    colorOptions = new Color[10];
   public static final int shapeColor = 0;
   public static final int selShapeColor = 1;
   public static final int moShapeColor = 2;
   public static final int grpShapeColor = 3;
   public static final int shapeOutlineColor = 4;
   public static final int selShapeOutlineColor = 5;
   public static final int moShapeOutlineColor = 6;
   public static final int grpShapeOutlineColor = 7;
   public static final int fixShapeColor = 8;
   public static final int fixShapeOutlineColor = 9;
   public static final String[] colorOptionNames = new String[] { "Shape Body","Selected Shape Body","MouseOver Shape Body","Grouped Shape Body"
      ,"Shape Outline","Selected Shape Outline","MouseOver Shape Outline","Grouped Shape Outline","Color of Fixed Shape","Color of outline of fixed shape" };

   public static void setDefaults()
   {
   //Byte Options
      byteOptions[ groupDisplayMode ] = 0;
      byteOptions[ unSelGroupsWhen ]  = 1;
      byteOptions[ simpleSolverMode ] = 1;
      byteOptions[ simpleSolverVersion ] = 0;
     byteOptions[ onlineMode ] = 0;

   //Color options
      colorOptions[ shapeColor ]    = Color.blue;
      colorOptions[ selShapeColor ] = Color.blue;
      colorOptions[ moShapeColor ]  = Color.blue;
      colorOptions[ grpShapeColor ] = Color.cyan;
      colorOptions[ shapeOutlineColor ] = Color.blue;
      colorOptions[ selShapeOutlineColor ] = Color.red;
      colorOptions[ moShapeOutlineColor ]  = Color.white;
      colorOptions[ grpShapeOutlineColor ] = Color.blue;
      colorOptions[ fixShapeColor ] = Color.black;
      colorOptions[ fixShapeOutlineColor ] = Color.white;
   }

   public static void loadOptions(String fileName)
   {
      File data = new File(fileName);
      if (!data.exists())
      {
         setDefaults();
         saveOptions(fileName);
         return;
      }

      try
      {
         FileInputStream fstream = new FileInputStream(data);
         DataInputStream p = new DataInputStream(fstream);

         int i;
         for (i=0; i<byteOptions.length; i++)
            byteOptions[i] = p.readByte();

         for (i=0; i<colorOptions.length; i++)
            colorOptions[i] = new Color( p.readInt() );

         p.close();
         fstream.close();
      }
         catch(Exception e)
         {}
   }

   public static void saveOptions(String fileName)
   {
      try
      {
         FileOutputStream fstream = new FileOutputStream(fileName);
         DataOutputStream p = new DataOutputStream(fstream);

         int i;
         for (i=0; i<byteOptions.length; i++)
            p.writeByte( byteOptions[i] );

         for (i=0; i<colorOptions.length; i++)
            p.writeInt( colorOptions[i].getRGB() );

         p.flush();
         p.close();
         fstream.flush();
         fstream.close();
      }
         catch(Exception e)
         {}
   }
}