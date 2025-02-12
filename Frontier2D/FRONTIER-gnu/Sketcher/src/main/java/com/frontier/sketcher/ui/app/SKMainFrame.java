/**
 * Title:        SKMainFrame
 * Description:  MainFrame for Sketcher
 * @version      1.00.04b
 */
package com.frontier.sketcher.ui.app;

import javax.swing.*;
import javax.swing.table.*;
import javax.swing.tree.*;
import javax.swing.event.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.zip.ZipFile;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;
import java.util.Enumeration;

import com.frontier.sketcher.ui.DefaultConstraintModel;
import com.frontier.sketcher.ui.SKOptions;
import com.frontier.sketcher.ui.SKSimpleSolver;
import com.frontier.sketcher.ui.app.menu.SKEditMenu;
import com.frontier.sketcher.ui.app.menu.SKFileMenu;
import com.frontier.sketcher.ui.app.menu.SKHelpMenu;
import com.frontier.sketcher.ui.app.toolbar.SKToolBarConstraints;
import com.frontier.sketcher.ui.app.toolbar.SKToolBarMain;
import com.frontier.sketcher.ui.app.toolbar.SKToolBarShapes;
import com.frontier.sketcher.ui.groups.SKGroupTreeNode;
import com.frontier.sketcher.ui.groups.SKGroups;
import com.frontier.sketcher.ui.items.SKItemArray;
import com.frontier.sketcher.ui.items.SKItemInterface;
import com.frontier.sketcher.ui.items.constraints.*;
import com.frontier.sketcher.ui.dialogs.SKOptionDialog;
import com.frontier.sketcher.ui.dialogs.TreeFrame;
import com.frontier.sketcher.ui.mru.MRUItem;
import com.frontier.sketcher.ui.mru.MRUManager;
import com.frontier.sketcher.ui.items.shapes.*;
import com.frontier.sketcher.ui.properties.SKPropArray;
import com.frontier.sketcher.ui.properties.SKPropFactory;
import com.frontier.sketcher.ui.utils.SKGTNArray;
import com.frontier.sketcher.ui.utu.SKMapleUTU;
import com.frontier.sketcher.ui.utu.SKUTU;
import com.frontier.sketcher.ui.utu.SKUTUFile;
import com.frontier.sketcher.ui.widgets.*;
import com.frontier.sketcher.utils.ResourceLoading;
import com.frontier.sketcher.ui.utu.utuJava;

import static com.frontier.sketcher.ui.app.ModeStateMachine.ModeFlag.*;

public class SKMainFrame extends JFrame implements SKApplication {

      public  String      HomeDir;
      public  boolean     editingShape; //When true, the property editor is currently showing SKShape data
      public  boolean     drawConstrDetails = true;
      public  boolean     drawGroupRects = false;
      public  boolean     allSelNeedsResolved = false;
      public  boolean     dragged = false;
      public  boolean     b2=false;
      public  boolean     b1=false;
      public  boolean     treeCont = false;
      public  boolean     update=false, optionsUsed=false, repFirst = true;
      public  boolean     solved = false;
      public  int         fixed = 0, treeID = 1, lastHorizScroll = 0, lastVertScroll = 0, lastHorizScroll1 = 0, lastVertScroll1 = 0, paracount=0;

      public  int         shI =0, conI =0, ex=100, ey= 100, repx=ex, repy=ey, repx1=0, repy1=0, repx2=0, repy2=0;
      public  int         IntArray[];
      public  double	     DblArray[];
      public  double[]    consArray = new double[50];
      public  float       lastScale = 1;
      public  int         repShIDs[];
      public  int         repConIDs[];
   
   
      public  SKBaseConstraint  deleteConstraint;
   
      public  SKConstraintArray vCurrentConstraints = new SKConstraintArray(2);  //Lists all constraints of selected shape
      public  SKConstraintArray allConstraints = new SKConstraintArray(100);
      public  SKConstraintArray oldConstraints = new SKConstraintArray(100);
      public  SKConstraintArray selectedConstraints = new SKConstraintArray(10);
      public  SKConstraintArray backupCons = new SKConstraintArray(2);
   
      public SKPropArray vCurrentProps = new SKPropArray(5); //Contains current shape(s) properties
   
      public SKPropFactory propFactory = new SKPropFactory(); //Used to get/free props
   
      public SKItemInterface mouseOverItem;
   
      public SKItemArray DrawnItems = new SKItemArray(5); //Keeps references to all special-drawn selectable shapes & constraints
   
      public MRUManager mruManager;
   
      public SKGroupTreeNode groupTree = new SKGroupTreeNode(null,-1,"Groups");
      public  SKGroupTreeNode   newGroupTree = null;
   
      public SKGTNArray selectedGroups = new SKGTNArray(3);
      public  SKGTNArray        allGroups = new SKGTNArray(100);
      public  SKGTNArray	oldGroups = new SKGTNArray(100);
      public  SKGTNArray        groupArray = new SKGTNArray(100);
      public  SKGTNArray	dagMain = new SKGTNArray(100);

      public  SKShapeArray      selectedGroupShapes = new SKShapeArray(6);
      public  SKShapeArray      solvingShapes = new SKShapeArray(4);
      public  SKShapeArray      allshapes = new SKShapeArray(100);
      public  SKShapeArray      solvedShapes = new SKShapeArray(100);
      public  SKShapeArray	     oldShapes = new SKShapeArray(100);
      public  SKShapeArray      backupShapes = new SKShapeArray(2);
      public  SKShapeArray      frontier = new SKShapeArray(2);

      public  JMenuBar jMenuBar1 = new JMenuBar();

      public  JMenu mniNewConstraint = new JMenu();
      public  JMenu mniView = new JMenu();
      public  JMenu mniReopen = new JMenu();
      public  JMenu mniRepository = new JMenu();
      public  JMenu mniDesign = new JMenu();
      public  JMenu mniSolveSketch = new JMenu();
      public  JMenu mniSolveOptions = new JMenu();
      public  JMenu mniOptions = new JMenu();
   
      public  JMenuItem mniTangentConstraint = new JMenuItem();
      public  JMenuItem mniAngleConstr = new JMenuItem();

      public  JMenuItem mniDelete = new JMenuItem();
      public  JMenuItem mniMakeGroup = new JMenuItem();
      public  JMenuItem mniFixGroup = new JMenuItem();
      public  JMenuItem mniUnFixGroup = new JMenuItem();
      public  JMenuItem mniMakeGrouptree = new JMenuItem();
      public  JMenuItem mniPopSelectAll = new JMenuItem();
      public  JMenuItem mniPopPaste = new JMenuItem();
      public  JMenuItem mniPopCopy = new JMenuItem();
      public  JMenuItem mniPopCut = new JMenuItem();
      public  JMenuItem mniPopMakeGroup = new JMenuItem();
      public  JMenuItem mniPopFixArcRadius = new JMenuItem();
      public  JMenuItem mniPopFixArcAngle = new JMenuItem();
      public  JMenuItem mniPopFixGroup = new JMenuItem();
      public  JMenuItem mniPopUnFixGroup = new JMenuItem();
      public  JMenuItem mniPopMakeGrouptree = new JMenuItem();

      public  JMenuItem mniDistanceConstr = new JMenuItem();
      public  JMenuItem mniIncidenceConstr = new JMenuItem();
      public  JMenuItem mniPerpConstraint = new JMenuItem();
      public  JMenuItem mniParallelConstraint = new JMenuItem();
      public  JMenuItem mniSolve = new JMenuItem();
      public  JMenuItem mniReStart = new JMenuItem();
      public  JMenuItem mniNewTree = new JMenuItem();
      public  JMenuItem mniNewLibrary = new JMenuItem();
      public  JMenuItem mniTreeDelete = new JMenuItem();
      public  JMenuItem mniTest = new JMenuItem();
      public  JMenuItem mniAutoSolve = new JMenuItem();
      public  JMenuItem mniChangeCons = new JMenuItem();
      public  JMenuItem mniAddCons = new JMenuItem();
      public  JMenuItem mniRemoveCons = new JMenuItem();
      public  JMenuItem mniAddTree = new JMenuItem();
      public  JMenuItem mniAddShapeCon = new JMenuItem();

      public  JSmallButton btnSolve = new JSmallButton();
      public  JSmallButton btnNewTree = new JSmallButton();

      public  JToolBar toolbarEditor = new JToolBar();
      public  JToolBar toolbarStatus = new JToolBar();
      private final SKToolBarConstraints toolbarConstraints;
      private final SKToolBarShapes toolbarShapes;

      public  JTabbedPane tabpaneObjectProp = new JTabbedPane();
      public  JTabbedPane tabpaneEditor = new JTabbedPane();
      public  JTabbedPane tabPartialSketch = new JTabbedPane();
   
      public  JPanel panelEditor = new JPanel();
      public  JPanel mainFrame = new JPanel();
      public  JPanel panelLeft = new JPanel();
      public  JPanel panelObjects = new JPanel();
      public  JPanel partialSketch = new JPanel();
      public  JPanel panelGroups = new JPanel();
   
      public  BorderLayout borderLayout1 = new BorderLayout();
      public  BorderLayout borderLayout2 = new BorderLayout();
      public  BorderLayout borderLayout3 = new BorderLayout();
      public  BorderLayout borderLayout4 = new BorderLayout();
      public  BorderLayout borderLayout5 = new BorderLayout();
   
      public JSelectionPanel panelShapeArea = new JSelectionPanel(this);
      public  JSelectionPanel panelPartial = new JSelectionPanel(this);
   
      public  JPopupMenu popupShape = new JPopupMenu();
      public  JPopupMenu popupGroup = new JPopupMenu();
      public  JPopupMenu popTree = new JPopupMenu();
   
      public JStatusBar sbStatus = new JStatusBar();
   
      public  Border border1;
   
      public  JComboBox cmbShapes = new JComboBox();

      public  JTable tableObjectProp = new JTable();
   
      public  JTree treeConstraints = new JTree();
      public  JTree treeGroups = new JTree();
   
      public  JList lstGroupShapes = new JList();
   
      public  JCheckBoxMenuItem mniDrawConstraints = new JCheckBoxMenuItem();
      public  JCheckBoxMenuItem mniRepositoryViewer = new JCheckBoxMenuItem();
   
      public static IndexedButtonGroup bgShapes = new IndexedButtonGroup();
   
      public  SKRepository repository;
   
      public  JSplitPane jSplitPane1 = new JSplitPane();
   
      public  JScrollBar scrHoriz = new JScrollBar();
      public  JScrollBar scrVert = new JScrollBar();
      public  JScrollBar scrHoriz1 = new JScrollBar();
      public  JScrollBar scrVert1 = new JScrollBar();
   
      private String      		sFileName;  //File and path to currently edited data -- Blank when new (never saved)
      //private SKShapeArray    ClipShapes = new SKShapeArray(0);  //This is our "clipboard"

    private final ShapeClipboard shapeClipboard = new DefaultShapeClipboard();
    private final ItemSelectionModel itemSelectionModel = new DefaultItemSelectionModel();
    private final ConstraintModel constraintModel = new DefaultConstraintModel(this);
    private final ModeStateMachine modeStateMachine = new DefaultModeStateMachine();

    private int         		Updating;  //Used to prevent redundant calls to updateShapeData() -- mainly from setting cmbShapes.setItemIndex()
      public int         		IDCnt=1,oldShapeID;  //This is incremented everytime a shape is created to give unique IDs
      public int         		ConstrIDCnt, oldConstraintID;  //This is incremented everytime a constraint is created to give unique IDs
      public int         		GroupIDCnt,oldGroupID;  //This is incremented everytime a group is created to give unique IDs
      private int         		SelectedPropLevel;  //indicates the PropTypes that are currently visible
      private boolean     		ClipWasCut;  //true if items were cut, false if copied
      private boolean     		bDataModified = false;  //true if user has modified data since last save
      private boolean         simpleSolve=false;
      private static boolean 			saveArrays = false; //save additional data?
      private static boolean			dataExists = false; //additional data present?
   
      public SKMainFrame(String Home) throws IOException {
         HomeDir = Home;

         toolbarShapes = new SKToolBarShapes(this, bgShapes);
         toolbarConstraints = new SKToolBarConstraints(this, bgShapes);
         jbInit();
         setLocation(0,0);
         setSize(970,720);

         //Center on desktop
         Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
         Dimension d = getSize();
         int x = (screen.width - d.width) >> 1;
         int y = (screen.height - d.height) >> 1;
         setLocation(x, y);
      }

      /**
      * Contructs the main window of sketcher.
      */
      private void jbInit() throws IOException {
         border1 = BorderFactory.createBevelBorder(BevelBorder.LOWERED,Color.white,Color.white,new Color(142, 142, 142),new Color(99, 99, 99));
         this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
         this.setJMenuBar(jMenuBar1);
         this.setTitle("Sketcher");
         this.addWindowListener(
                                 new java.awt.event.WindowAdapter()
                                 {
                                 
                                    public void windowClosing(WindowEvent e)
                                    {
                                       this_windowClosing(e);
                                    }
                                 });

         mniSolveSketch.setText("Solve");

         btnSolve.setMaximumSize(new Dimension(24, 24));
         btnSolve.setPreferredSize(new Dimension(24, 24));
         btnSolve.setToolTipText("Solve with Maple");
         btnSolve.setIcon(ResourceLoading.loadImageIcon("exit.gif"));
         btnSolve.addActionListener(this::mniSolve_actionPerformed);
         btnSolve.setMaximumSize(new Dimension(24, 24));

         btnNewTree.setPreferredSize(new Dimension(24, 24));
         btnNewTree.setToolTipText("Make New Tree");
         //btnNewTree.setIcon(ResourceLoading.loadImageIcon("exit.gif"));
         btnNewTree.addActionListener(this::mniNewTree_actionPerformed);

         toolbarEditor.setOrientation(JToolBar.VERTICAL);

         panelShapeArea.addMouseListener(
                                 new java.awt.event.MouseAdapter()
                                 {
                                 
                                    public void mouseClicked(MouseEvent e)
                                    {
                                       panelShapeArea_mouseClicked(e);
                                    }
                                 
                                    public void mouseReleased(MouseEvent e)
                                    {
                                       panelShapeArea_mouseReleased(e);
                                    }
                                 });

         panelGroups.addMouseListener(
                                 new java.awt.event.MouseAdapter()
                                 {
                                    public void mouseClicked(MouseEvent e)
                                    {
                                       panelGroups_mouseClicked(e);
                                    }
                                 
                                 
                                    public void mouseReleased(MouseEvent e)
                                    {
                                       panelGroups_mouseReleased(e);
                                    }
                                 });

         mniDelete.setText("Delete");
         mniDelete.setIcon(ResourceLoading.loadImageIcon("delete.gif"));
         mniDelete.addActionListener(this::mniDelete_actionPerformed);

         toolbarStatus.setBorder(border1);
         toolbarStatus.setFloatable(false);

         panelShapeArea.addMouseMotionListener(
                                 new java.awt.event.MouseMotionAdapter()
                                 {
                                 
                                    public void mouseDragged(MouseEvent e)
                                    {
                                       panelShapeArea_mouseDragged(e);
                                    }
                                 });

         sbStatus.setPreferredSize(new Dimension(20, 15));

         mniPopCut.setText("Cut");
         mniPopCut.setIcon(ResourceLoading.loadImageIcon("cut.gif"));
         mniPopCut.addActionListener(this::mniCut_actionPerformed);

         mniPopCopy.setText("Copy");
         mniPopCopy.setIcon(ResourceLoading.loadImageIcon("copy.gif"));
         mniPopCopy.addActionListener(this::mniCopy_actionPerformed);

         mniPopPaste.setText("Paste");
         mniPopPaste.setIcon(ResourceLoading.loadImageIcon("paste.gif"));
         mniPopPaste.addActionListener(this::mniPaste_actionPerformed);

         mniPopSelectAll.setText("Select All");
         mniPopSelectAll.addActionListener(this::mniSelectAll_actionPerformed);

         popupShape.addPopupMenuListener(
                                 new javax.swing.event.PopupMenuListener()
                                 {
                                 
                                    public void popupMenuCanceled(PopupMenuEvent e)
                                    {
                                    }
                                 
                                    public void popupMenuWillBecomeInvisible(PopupMenuEvent e)
                                    {
                                    }
                                 
                                    public void popupMenuWillBecomeVisible(PopupMenuEvent e)
                                    {
                                       popupShape_popupMenuWillBecomeVisible(e);
                                    }
                                 });

         popupGroup.addPopupMenuListener(
                              
                                 new javax.swing.event.PopupMenuListener()
                                 
                                 {
                                 
                                 
                                    public void popupMenuCanceled(PopupMenuEvent e)
                                    
                                    {
                                    }
                                 
                                 
                                    public void popupMenuWillBecomeInvisible(PopupMenuEvent e)
                                    
                                    {
                                    }
                                 
                                 
                                    public void popupMenuWillBecomeVisible(PopupMenuEvent e)
                                    
                                    {
                                       popupGroup_popupMenuWillBecomeVisible(e);
                                    }
                                 });

         mniNewConstraint.setText("New Constraint");

         mniDistanceConstr.setText("Distance");
         mniDistanceConstr.addActionListener(this::mniDistanceConstr_actionPerformed);

         mniView.setText("View");

         mniDrawConstraints.setText("Draw Constraints");
         mniDrawConstraints.setState(true);
         mniDrawConstraints.addActionListener(this::chkDrawConstraints_actionPerformed);

         panelShapeArea.addMouseMotionListener(
                                 new java.awt.event.MouseMotionAdapter()
                                 {
                                 
                                    public void mouseMoved(MouseEvent e)
                                    {
                                       panelShapeArea_mouseMoved(e);
                                    }
                                 });

         treeConstraints.addMouseListener(
                                 new java.awt.event.MouseAdapter()
                                 {
                                 
                                    public void mouseClicked(MouseEvent e)
                                    {
                                       treeConstraints_mouseClicked(e);
                                    }
                                 
                                    public void mouseReleased(MouseEvent e)
                                    {
                                       treeConstraints_mouseReleased(e);
                                    }
                                 });

         mniIncidenceConstr.setText("Incidence");
         mniIncidenceConstr.addActionListener(this::mniIncidenceConstr_actionPerformed);

         mniPerpConstraint.setText("Perpendicular");
         mniPerpConstraint.addActionListener(this::mniPerpConstraint_actionPerformed);

         mniParallelConstraint.setText("Parallel");
         mniParallelConstraint.addActionListener(this::mniParallelConstraint_actionPerformed);

         mniRepositoryViewer.setText("Repository Viewer");
         mniRepositoryViewer.setState(false);
         mniRepositoryViewer.addActionListener(this::mniRepositoryViewer_actionPerformed);

         mniRepository.setText("Repository");
         mniRepository.addMenuListener(
                                 new javax.swing.event.MenuListener()
                                 {
                                 
                                    public void menuCanceled(MenuEvent e)
                                    {
                                    }
                                 
                                    public void menuDeselected(MenuEvent e)
                                    {
                                    }
                                 
                                    public void menuSelected(MenuEvent e)
                                    {
                                       mniRepository_menuSelected(e);
                                    }
                                 });

         mniNewLibrary.setText("Create New Object Library");
         mniNewLibrary.addActionListener(e -> {
                if(!repository.isVisible())
                   repository.setVisible( true );
                repository.mniCreateLibrary_actionPerformed(e);
             });
      
         mniDesign.setText("Design");
         mniDesign.addMenuListener(
                              
                                 new javax.swing.event.MenuListener()
                                 
                                 {
                                 
                                 
                                    public void menuCanceled(MenuEvent e)
                                    
                                    {
                                    }
                                 
                                 
                                    public void menuDeselected(MenuEvent e)
                                    
                                    {
                                    }
                                 
                                 
                                    public void menuSelected(MenuEvent e)
                                    
                                    {
                                       mniDesign_menuSelected(e);
                                    }
                                 });

         mniSolve.setText("Solve using maple");
         mniSolve.addActionListener(this::mniSolve_actionPerformed);

         mniSolveOptions.setText("Solve");

         mniNewTree.setText("Make new tree");
         mniNewTree.addActionListener(this::mniNewTree_actionPerformed);

         treeConstraints.addKeyListener(
                                 new java.awt.event.KeyAdapter()
                                 {
                                 
                                    public void keyTyped(KeyEvent e)
                                    {
                                       treeConstraints_keyTyped(e);
                                    }
                                 });
         mniTreeDelete.setText("Delete");
         mniTreeDelete.addActionListener(this::mniTreeDelete_actionPerformed);

         mniAngleConstr.setText("Angle");
         mniAngleConstr.addActionListener(this::mniAngleConstr_actionPerformed);

         mniReopen.setText("Reopen");
         mniReopen.setIcon(ResourceLoading.loadImageIcon("open.gif"));

         mniTangentConstraint.setText("Tangent");
         mniTangentConstraint.addActionListener(this::mniTangentConstraint_actionPerformed);

         panelLeft.setMinimumSize(new Dimension(40, 104));
         panelLeft.setPreferredSize(new Dimension(40, 136));

         panelObjects.setLayout(borderLayout2);

         tabpaneEditor.setTabPlacement(JTabbedPane.BOTTOM);
         tabpaneEditor.addChangeListener(this::tabpaneEditor_stateChanged);

         partialSketch.setLayout(borderLayout4);

         tabPartialSketch.setTabPlacement(JTabbedPane.BOTTOM);
         tabPartialSketch.addChangeListener(this::tabPartialSketch_stateChanged);

         treeGroups.getSelectionModel().setSelectionMode(javax.swing.tree.TreeSelectionModel.DISCONTIGUOUS_TREE_SELECTION);
         treeGroups.setModel(
                               new TreeModel()
                               {
                                  public Object getChild(Object parent, int index)
                                  {
                                     return ((SKGroupTreeNode)parent).children.get(index);
                                  }
                                  public int getChildCount(Object parent)
                                  {
                                     return ((SKGroupTreeNode)parent).children.size();
                                  }
                                  public int getIndexOfChild(Object parent, Object child)
                                  {
                                     return ((SKGroupTreeNode)parent).children.indexOf(child);
                                  }
                                  public Object getRoot()
                                  {
                                     return groupTree;
                                  }
                                  public boolean isLeaf(Object node)
                                  {
                                     return (((SKGroupTreeNode)node).children.size() == 0);
                                  }
                                  public void valueForPathChanged(javax.swing.tree.TreePath path, Object newValue)
                                  {}
                                  public void addTreeModelListener(javax.swing.event.TreeModelListener l)
                                  {}
                                  public void removeTreeModelListener(javax.swing.event.TreeModelListener l)
                                  {}
                               });
         treeGroups.addMouseListener(
                                 new java.awt.event.MouseAdapter()
                                 {
                                 
                                    public void mouseClicked(MouseEvent e)
                                    {
                                       treeGroups_mouseClicked(e);
                                    }
                                 });
      
         lstGroupShapes.setModel(
                                 new ListModel()
                                 {
                                    public Object getElementAt(int index)
                                    {
                                       return selectedGroupShapes.get(index);
                                    }
                                    public int getSize()
                                    {
                                       return selectedGroupShapes.size();
                                    }
                                    public void addListDataListener(ListDataListener l)
                                    {}
                                    public void removeListDataListener(ListDataListener l)
                                    {}
                                 } );
      
         panelGroups.setLayout(borderLayout3);
         mniMakeGroup.setText("Make New Group");
         mniMakeGroup.addActionListener(this::mniMakeGroup_actionPerformed);
         mniPopMakeGroup.setText("Make New Group");
         mniPopMakeGroup.addActionListener(this::mniMakeGroup_actionPerformed);
         mniPopFixArcRadius.setText("Fix Radius of Arc");
         mniPopFixArcRadius.addActionListener(this::mniFixArcRadius_actionPerformed);
         mniPopFixArcAngle.setText("Fix Angle of Arc");
         mniPopFixArcAngle.addActionListener(this::mniFixArcAngle_actionPerformed);
      
         mniFixGroup.setText("Fix Group");
         mniFixGroup.addActionListener(e -> {
                for(int i = 0; i<selectedGroups.size(); i++)
                   selectedGroups.get(i).fixed = 1;
             });
         mniPopFixGroup.setText("Fix Group");
         mniPopFixGroup.addActionListener(e -> {
                for(int i = 0; i<selectedGroups.size(); i++)
                   selectedGroups.get(i).fixed = 1;
             //mniMakeGroup_actionPerformed(e);
             });
         mniUnFixGroup.setText("Unfix Group");
         mniUnFixGroup.addActionListener(
                 e -> {
                    for(int i = 0; i<selectedGroups.size(); i++)
                       selectedGroups.get(i).fixed = 0;
                 });
         mniPopFixGroup.setText("New Fixed Group");
         mniPopFixGroup.addActionListener(
                 e -> {
                    for(int i = 0; i<selectedGroups.size(); i++)
                       selectedGroups.get(i).fixed = 0;
                 });
      
         mniMakeGrouptree.setText("Group Tree");
         mniMakeGrouptree.addActionListener(this::mniMakeGrouptree_actionPerformed);

         mniPopMakeGrouptree.setText("Group Tree");
         mniPopMakeGrouptree.addActionListener(this::mniMakeGrouptree_actionPerformed);

         jSplitPane1.setOrientation(JSplitPane.VERTICAL_SPLIT);
         mniTest.setText("Get Bifurcations");
         mniTest.addActionListener(this::mniTest_actionPerformed);
         mniOptions.setText("Options");
         mniOptions.setEnabled(false);
         mniReStart.setText("Restart");
         mniReStart.setEnabled(false);
         mniReStart.addActionListener(
                 e -> {
                    PromptSaveSketch();
                    NewData();
                 });
         mniAutoSolve.setText("Auto-Solve");
         mniAutoSolve.addActionListener(this::mniAutoSolve_actionPerformed);
         mniChangeCons.setText("Change constraint value");
         mniChangeCons.addActionListener(this::mniChangeCons_actionPerformed);
         mniAddCons.setText("Add Constraint");
         mniAddCons.addActionListener(this::mniAddCons_actionPerformed);
         mniRemoveCons.setText("Remove Constraint");
         mniRemoveCons.addActionListener(this::mniRemoveCons_actionPerformed);
         mniAddTree.setText("Add Tree");
         mniAddTree.addActionListener(this::mniAddTree_actionPerformed);
         mniAddShapeCon.setText("Add Shape and Constraints");
         mniAddShapeCon.addActionListener(this::mniAddShapeCon_actionPerformed);
         mainFrame.setLayout(borderLayout5);
         mainFrame.setBorder(BorderFactory.createLoweredBevelBorder());
         scrHoriz.setOrientation(JScrollBar.HORIZONTAL);
         scrHoriz.addAdjustmentListener(this::scrHoriz_adjustmentValueChanged);
         scrVert.addAdjustmentListener(this::scrVert_adjustmentValueChanged);
         scrHoriz1.setOrientation(JScrollBar.HORIZONTAL);
         scrHoriz1.addAdjustmentListener(this::scrHoriz1_adjustmentValueChanged);
         scrVert1.addAdjustmentListener(this::scrVert1_adjustmentValueChanged);

         toolbarStatus.add(sbStatus);
      
         cmbShapes.addItemListener(this::cmbShapes_itemStateChanged);
         cmbShapes.setPreferredSize(new Dimension(126, 26));
         tableObjectProp.setBackground(Color.lightGray);
         tableObjectProp.setMinimumSize(new Dimension(20, 20));
         tableObjectProp.setPreferredSize(new Dimension(170, 30));
         tableObjectProp.setModel(
                                 new AbstractTableModel()
                                 {
                                    public int getColumnCount() { 
                                       return 2; }
                                    public int getRowCount() { 
                                       return vCurrentProps.size(); }
                                    public Object getValueAt(int row, int col)
                                    {
                                       if (col==0)
                                       {
                                          return vCurrentProps.getShapePropName(row);
                                       }
                                       else
                                       {
                                          return vCurrentProps.get(row);
                                       }
                                    }
                                    public boolean isCellEditable(int rowIndex, int columnIndex)
                                    {
                                       if(update && (!(getModeStateMachine().getModeFlag() == VALUE_3)))
                                          return false;
                                       else
                                          return (columnIndex==1 && vCurrentProps.get(rowIndex).isEditable);
                                    }
                                    public void setValueAt(Object aValue, int rowIndex, int columnIndex)
                                    {
                                       //Only update if new value and old value differ
                                       if (!vCurrentProps.getShapePropData(rowIndex).toString().equals(aValue.toString()))
                                       {
                                          vCurrentProps.setShapePropData(rowIndex,aValue.toString());
                                       
                                          String PropName = vCurrentProps.getShapePropName(rowIndex);
                                          Object PropData = vCurrentProps.getShapePropData(rowIndex);
                                          if (editingShape)
                                          {
                                            //Have selected shapes update themselves
                                            itemSelectionModel.selectedShapes()
                                                    .forEach(s -> s.setShapeData(PropName, PropData));
                                          
                                           //Update name in cmbShapes?
                                             if (vCurrentProps.getShapePropName(rowIndex)=="Name")  cmbShapes.repaint();
                                          }
                                          else
                                          { //editing a constraint
                                             itemSelectionModel.selectedConstraint().get().setMainData(PropName,PropData);
                                          }
                                       }
                                    }
                                 });
         tableObjectProp.getColumnModel().getColumn(1).setCellRenderer(new SKTableCellRenderer());
         tableObjectProp.getColumnModel().getColumn(1).setCellEditor(new MultiCellEditor());
      
         treeConstraints.setModel(
                                 new TreeModel()
                                 {
                                    public Object getChild(Object parent, int index)
                                    {
                                       if (parent.toString()=="Constraints")
                                       { //Constraint Names
                                          switch (index)
                                          {
                                             case 0  : 
                                                return "Distance";
                                             case 1  : 
                                                return "Incidence";
                                             case 2  : 
                                                return "Angle";
                                             case 3  : 
                                                return "Parallel";
                                             case 4  : 
                                                return "Perpendicular";
                                             case 5  :
                                                return "Tangent";
                                             default : 
                                                return "";
                                          }
                                       }
                                       else if (parent.toString()=="Shapes")
                                       {
                                          return itemSelectionModel.selectedConstraint().get().ShapeList.get(index);
                                       }
                                       else
                                       { //Constraints
                                          int i, cnt=0;
                                          for (i=0; i<vCurrentConstraints.size() && cnt!=index+1; i++)
                                          {
                                             if ( parent.toString().equals(vCurrentConstraints.get(i).cType) )
                                                cnt++;
                                          }
                                       
                                          return vCurrentConstraints.get(i-1);
                                       }
                                    }
                                    public int getChildCount(Object parent)
                                    {
                                       if (parent.toString()=="Constraints")
                                       {
                                          return 6;
                                       }
                                       else if (parent.toString()=="Shapes")
                                       {
                                          if (itemSelectionModel.selectedConstraint().isEmpty()) {
                                              return 0;
                                          } else {
                                              return itemSelectionModel.selectedConstraint().get().ShapeList.size();
                                          }
                                      }
                                       else
                                       {
                                          int cnt=0;
                                          for (int i=0; i<vCurrentConstraints.size(); i++)
                                          {
                                             if ( parent.toString().equals(vCurrentConstraints.get(i).cType) )
                                                cnt++;
                                          }
                                       
                                          return cnt;
                                       }
                                    }
                                    public int getIndexOfChild(Object parent, Object child)
                                    {
                                       if (parent.toString()=="Constraints")
                                       { //Don't care about Constraint Names
                                          return 0;
                                       }
                                       else
                                       {
                                          return vCurrentConstraints.indexOf(child);
                                       }
                                    }
                                    public Object getRoot()
                                    {
                                       if (editingShape)
                                          return "Constraints";
                                       else
                                          return "Shapes";
                                    }
                                    public boolean isLeaf(Object node)
                                    {
                                       if (node instanceof String) 
                                          return false;
                                    
                                       return true;
                                    }
                                    public void valueForPathChanged(javax.swing.tree.TreePath path, Object newValue)
                                    {
                                    
                                    }
                                    public void addTreeModelListener(javax.swing.event.TreeModelListener l)
                                    {
                                    
                                    }
                                    public void removeTreeModelListener(javax.swing.event.TreeModelListener l)
                                    {
                                    
                                    }
                                 });
         tabpaneObjectProp.setMinimumSize(new Dimension(45, 49));
         panelEditor.setLayout(borderLayout1);
         panelShapeArea.setLayout(null);
         panelPartial.setLayout(null);

         jMenuBar1.add(new SKFileMenu(this));
         jMenuBar1.add(new SKEditMenu(this));
         jMenuBar1.add(mniDesign);
         jMenuBar1.add(mniSolveSketch);
         jMenuBar1.add(mniOptions);
         jMenuBar1.add(mniRepository);
         jMenuBar1.add(mniView);
         jMenuBar1.add(new SKHelpMenu());

         mniOptions.add(mniChangeCons);
         mniOptions.add(mniAddCons);
         mniOptions.add(mniRemoveCons);
         mniOptions.add(mniAddTree);
         mniOptions.add(mniAddShapeCon);
         mniSolveSketch.add(mniSolve);
         mniSolveSketch.add(mniSolveOptions);
         mniSolveOptions.add(mniTest);
         mniSolveOptions.add(mniAutoSolve);
         mniSolveSketch.add(mniReStart);
         //mniSolveSketch.add(mniAutoSolve);
         //mniSolveSketch.add(mniTest);
      
         this.getContentPane().add(new SKToolBarMain(this), BorderLayout.NORTH);

         this.getContentPane().add(tabPartialSketch, BorderLayout.CENTER);
         mainFrame.add(panelShapeArea, BorderLayout.CENTER);
         partialSketch.add(panelPartial, BorderLayout.CENTER);
         tabPartialSketch.addTab("Original Sketch",mainFrame);
         tabPartialSketch.addTab("Partially solved sketch",partialSketch);
         this.getContentPane().add(toolbarEditor, BorderLayout.EAST);
         toolbarEditor.add(panelEditor, null);
         panelEditor.add(tabpaneEditor, BorderLayout.CENTER);
         tabpaneEditor.add(panelObjects, "Objects");
         panelObjects.add(cmbShapes, BorderLayout.NORTH);
         panelObjects.add(tabpaneObjectProp, BorderLayout.CENTER);
         tabpaneObjectProp.add(tableObjectProp, "Properties");
         tabpaneObjectProp.add(treeConstraints, "Constraints");
         tabpaneEditor.add(panelGroups, "Groups");
         panelGroups.add(jSplitPane1, BorderLayout.CENTER);
         jSplitPane1.add(treeGroups, JSplitPane.TOP);
         jSplitPane1.add(lstGroupShapes, JSplitPane.BOTTOM);
         this.getContentPane().add(toolbarStatus, BorderLayout.SOUTH);
         this.getContentPane().add(panelLeft, BorderLayout.WEST);
         panelLeft.add(toolbarShapes, null);
         panelLeft.add(toolbarConstraints, null);

         popupShape.add(mniPopCut);
         popupShape.add(mniPopCopy);
         popupShape.add(mniPopPaste);
         popupShape.add(mniPopSelectAll);
         popupShape.addSeparator();
         popupShape.add(mniPopFixArcRadius);
         popupShape.add(mniPopFixArcAngle);
         popupShape.addSeparator();
         popupShape.add(mniDelete);
         popupShape.addSeparator();
         popupShape.add(mniNewConstraint);
         popupShape.addSeparator();
         popupShape.add(mniPopMakeGroup);        
         popupShape.add(mniPopMakeGrouptree);
      
         popupGroup.add(mniPopFixGroup);
         popupGroup.add(mniPopUnFixGroup);

         mniNewConstraint.add(mniAngleConstr);
         mniNewConstraint.add(mniDistanceConstr);
         mniNewConstraint.add(mniIncidenceConstr);
         mniNewConstraint.add(mniPerpConstraint);
         mniNewConstraint.add(mniParallelConstraint);
         mniNewConstraint.add(mniTangentConstraint);
         mniView.add(mniDrawConstraints);
         mniRepository.add(mniRepositoryViewer);
         mniRepository.addSeparator();
         mniRepository.add(mniNewLibrary);
         mniDesign.add(mniNewTree);
         mniDesign.add(mniMakeGroup);
         mniDesign.add(mniMakeGrouptree);
         mniDesign.add(mniFixGroup);
         mniDesign.add(mniUnFixGroup);
         popTree.add(mniTreeDelete);
         mainFrame.add(scrHoriz, BorderLayout.SOUTH);
         mainFrame.add(scrVert, BorderLayout.EAST);
         partialSketch.add(scrHoriz1, BorderLayout.SOUTH);
         partialSketch.add(scrVert1, BorderLayout.EAST);
      
      //Setup statusbar
         sbStatus.addPanel("Ready",55, JStatusBar.ALIGN_CENTER); //Status
         sbStatus.addPanel("Modified",50, JStatusBar.ALIGN_CENTER); //Modified
         sbStatus.addPanel("",70, JStatusBar.ALIGN_CENTER); //Distance
         sbStatus.addPanel("",80, JStatusBar.ALIGN_CENTER); //Shape name that mouse it over
         sbStatus.addPanel("Untitled",50, JStatusBar.ALIGN_LEFT); //Current filename
      
         tabpaneObjectProp.setEnabled(false);
      
         cmbShapes.setRenderer( new ImageComboBoxRenderer() );
      
         scrHoriz.setValues(0,panelShapeArea.getWidth(),-1000,1000);
         scrHoriz.setUnitIncrement(10);
         scrVert.setValues(0,panelShapeArea.getHeight(),-1000,1000);
         scrVert.setUnitIncrement(10);
      
         scrHoriz1.setValues(0,panelPartial.getWidth(),-1000,1000);
         scrHoriz1.setUnitIncrement(10);
         scrVert1.setValues(0,panelPartial.getHeight(),-1000,1000);
         scrVert1.setUnitIncrement(10);
      
         repository = new SKRepository(this);
         repository.setVisible(false);
      
         mruManager = new MRUManager(8,this);
         mruManager.readMRUfromFile(HomeDir+"/mruData.dat");
         mruManager.syncMenu(mniReopen);
         jSplitPane1.setDividerLocation(260);
      }
   
      public void MRUclicked(MRUItem item)
      {
         LoadFile( item.data );
      }

    @Override
    public ConstraintModel getConstraintModel() {
        return constraintModel;
    }

    @Override
    public ItemSelectionModel getItemSelectionModel() {
        return itemSelectionModel;
    }

    @Override
    public ModeStateMachine getModeStateMachine() {
          return modeStateMachine;
    }

    @Override
    public boolean getUpdateFlag() {
        return update;
    }

    @Override
    public void setUpdateFlag(boolean value) {
        this.update = value;
    }

    @Override
    public ShapeClipboard getShapeClipboard() {
        return shapeClipboard;
    }

    /**
     * Action routine for exit button
     */
    public void mniExit_actionPerformed(ActionEvent e) {
        if (dataExists) {
            if (PromptSaveSketch()) {
                System.exit(0);
            }
        } else if (PromptSaveChanges()) {
            System.exit(0);
        }
    }
   /**
    * Action routine for New button. Prompts messages to save sketch and or solver data.
    */
   public void mniNew_actionPerformed(ActionEvent e) {
       if (dataExists) {
           if (!PromptSaveSketch())
               return;
       } else if (bDataModified) {
           if (!PromptSaveChanges())
               return;
       }

       NewData();
   }

    /**
     * Action routine for open button
     */
    public void mniOpen_actionPerformed(ActionEvent e) {
        OpenFile();
    }
   /**
   * Action routine for window closing event
   */
      void this_windowClosing(WindowEvent e)
      {
         if(dataExists)
         {
            if (PromptSaveSketch())
            {
               System.exit(0);
            }
         }
         else if (PromptSaveChanges())
         {
            System.exit(0);
         }
      }
   /**
   * Action routine for delete button. Calls deleteItem(Object o) method.
    */
   public void mniDelete_actionPerformed(ActionEvent e) {
       cmbShapes.setSelectedIndex(-1);
       for (final var s: itemSelectionModel.selectedShapes()) {
           deleteItem(s);
       }

       clearSelectedShapes(true, false);
       if (deleteConstraint != null)
           deleteItem(deleteConstraint);

       panelShapeArea.repaint();
   }
   /**
   * Method for selecting more than one objects. Adds the selected shapes to the 'selectedShapes' array using addOnlySelectedShape(SKBaseShape sh) method.
   */
      void cmbShapes_itemStateChanged(ItemEvent e)
      { //Make this is selected item
         if (e.getStateChange()==e.SELECTED && Updating==0 && cmbShapes.getSelectedIndex() != -1)
         {
            Updating++;
            addOnlySelectedShape( (SKBaseShape)cmbShapes.getSelectedItem() );
            Updating--;
         }
      }
   	/**
   * In update mode calls PromptSaveAsSketch() method if the solver data needs to decide whether or not to save the data. If no data exists then InternalSaveAs()
   * method is called.
     */
    public void mniSaveAs_actionPerformed(ActionEvent e) {
        if (dataExists)
            PromptSaveAsSketch();
        else InternalSaveAs();
    }

    /**
     * In calls PromptSaveSketch() method if the unsaved and modified sketch exits to decide whether or not to save the sketch. If no unsaved and modified sketch
     * exists then InternalSaveChanges() method is called.
     */
    public void mniSave_actionPerformed(ActionEvent e) {
        if (dataExists)
            PromptSaveSketch();
        else InternalSaveChanges();
    }

   /**
   * Handles mouse events on design menu.
   */
      void mniDesign_menuSelected(MenuEvent e)
      
      {
         if(update &&(!(getModeStateMachine().getModeFlag()==VALUE_6)))
            mniMakeGrouptree.setEnabled(false);
         else
            mniMakeGrouptree.setEnabled(b1);
         if(update &&(!(getModeStateMachine().getModeFlag()==VALUE_6)))
            mniMakeGroup.setEnabled(false);
         else
            mniMakeGroup.setEnabled(b1);
         mniFixGroup.setEnabled(selectedGroups.size()>0); 
         mniUnFixGroup.setEnabled(selectedGroups.size()>0);        
      }
   
   /**
   * Handles mouse drag event on main screen.
   */
      void panelShapeArea_mouseDragged(MouseEvent e)
      {
      
         if (!toolbarShapes.isCursorSelected())
         {
            panelShapeArea_mouseClicked(e);
            return;
         }
         dragged = true;
         /*if (mouseOverItem != null && mouseOverItem instanceof SKBaseShape)
         {SKBaseShape sh = (SKBaseShape)mouseOverItem;
            if (sh.isSelected())
               for(int i=0; i<sh.ConstraintList.size(); i++)
                  sh.ConstraintList.get(i).solved =false;
         }
       For dragging DrawnItems (dragging lineShape by line)
      if (mouseOverItem != null && mouseOverItem instanceof SKBaseShape)
      { //Drag shape
      SKBaseShape sh = (SKBaseShape)mouseOverItem;
      if (sh.isSelected())
      {
        if (sh.DragState==-1)
        {
          sh.DragState = 0;
          sh.DragX = e.getX();
          sh.DragY = e.getY();
          sh.setCursor( Cursor.getPredefinedCursor(Cursor.MOVE_CURSOR) );
        }
        else
        { //Draw as user is dragging
          sh.doMove(e.getX()-sh.DragX,e.getY()-sh.DragY,(SKOptions.byteOptions[ SKOptions.simpleSolverMode ]==2));
        }
      
        RefreshShapeArea();
      }
      }
      else
      { //Do selection rect
      */
         if (!panelShapeArea.DrawSelRect)
         { panelShapeArea.DrawSelRect = true;
         
            panelShapeArea.StartX = e.getX();
            panelShapeArea.StartY = e.getY();
         }
         else
         { //Draw selection rect
            panelShapeArea.EndX = e.getX();
            panelShapeArea.EndY = e.getY();
            panelShapeArea.repaint();
         }
      //    }
      }
   /**
   * Calculates the distance between two points p1(x1,y1) and p2(x2,y2).
   */
      public float distance(int x1, int y1, int x2, int y2)
      {
         return (float)java.lang.Math.sqrt(java.lang.Math.pow((double)(x1-x2),2)+java.lang.Math.pow((double)(y1-y2),2));
      }
   /**
   * Handles mouse release event on main screen. Changes the DragState of the objects appropriately. Handles the event of dragging the circle to increase its
   *radius by using the increaseRadius method in SKCircleShape.
   */
      void panelShapeArea_mouseReleased(MouseEvent e)
      {
         if (mouseOverItem != null && mouseOverItem instanceof SKBaseShape)
            ((SKBaseShape)mouseOverItem).DragState = -1;
      
         if(itemSelectionModel.selectedShapes().size()==1)
            if((itemSelectionModel.selectedShapes().get(0) instanceof SKCircleShape) )
            {
            
               SKCircleShape circle = (SKCircleShape) itemSelectionModel.selectedShapes().get(0);
               int distance = (int)distance(panelShapeArea.StartX,panelShapeArea.StartY,circle.center.getShapeX(),circle.center.getShapeY());
               int rad=20;
               if(circle.radius!=-1) rad=(int)circle.radius;
               //System.out.println("reached here 1"+distance+" "+rad+" "+e.getX()+" "+circle.center.getX());
            
               if(dragged )
               
               {
                  if((distance<(rad+5)) && (distance>(rad-5)))
                     circle.increaseRadius(e.getX(),e.getY());
               }
                                        //((SKCircleShape)sh).radius=java.lang.Math.sqrt(java.lang.Math.pow((double)(((SKCircleShape)sh).center.getX()-e.getX()),2)+java.lang.Math.pow((double)(((SKCircleShape)sh).center.getY()-e.getY()),2));
            }
         dragged = false;
         if (e.isPopupTrigger())
         {
            popupShape.show(panelShapeArea,e.getX(),e.getY());
            return;
         }
         if (panelShapeArea.DrawSelRect)
         { //Selected shapes in rect and repaint
            if (!e.isControlDown()) clearSelectedShapes(true,true);
         
            int sx,sy,ex,ey;
            if (panelShapeArea.StartX<panelShapeArea.EndX)
            {
               sx = panelShapeArea.StartX;
               ex = panelShapeArea.EndX;
            }
            else
            {
               sx = panelShapeArea.EndX;
               ex = panelShapeArea.StartX;
            }
            if (panelShapeArea.StartY<panelShapeArea.EndY)
            {
               sy = panelShapeArea.StartY;
               ey = panelShapeArea.EndY;
            }
            else
            {
               sy = panelShapeArea.EndY;
               ey = panelShapeArea.StartY;
            }
         
            SKBaseShape sh;
            for (int i=0; i<panelShapeArea.getComponentCount(); i++)
            {
               sh = (SKBaseShape)panelShapeArea.getComponent(i);
               if (sh.getX()>sx && (sh.getX()+sh.getWidth())<ex &&
                  sh.getY()>sy && (sh.getY()+sh.getHeight()<ey))
               {
                  addSelectedShape(sh);
               }
            }
         
            panelShapeArea.DrawSelRect=false;
            panelShapeArea.repaint();
         }
      }
   /**
   * Handles mouse events on panelGroups which displays the user defined groups.
   */
      void panelGroups_mouseReleased(MouseEvent e)
      
      {
         if (e.isPopupTrigger())
         {
            popupGroup.show(panelGroups,e.getX(),e.getY());
            return;
         }
         panelGroups.repaint();
      }
   	/**
   * Uses selectAllShapes to select all the shapes on the screen.
   */
    public void mniSelectAll_actionPerformed(ActionEvent e) {
        selectAllShapes();
    }

    /**
   * Uses cutSelectedShapes method.
   */
    public void mniCut_actionPerformed(ActionEvent e) {
        cutSelectedShapes();
    }

    /**
   * Uses copySelectedShapes method.
   */
    public void mniCopy_actionPerformed(ActionEvent e) {
        copySelectedShapes();
    }

    /**
     * Uses pasteClipShapes method.
     */
    public void mniPaste_actionPerformed(ActionEvent e) {
        pasteClipShapes();
    }

    /**
     * Handles pop-up mouse trigger and displays the pop up menu.
   */
      void popupShape_popupMenuWillBecomeVisible(PopupMenuEvent e)
      {
         boolean b = (itemSelectionModel.selectedShapes().size() > 0);
         boolean c = (itemSelectionModel.selectedConstraint().isPresent()) ;
         mniPopCut.setEnabled(b);
         mniPopCopy.setEnabled(b);
         mniPopFixArcRadius.setEnabled(itemSelectionModel.selectedShapes().size()==1 && itemSelectionModel.selectedShapes().get(0) instanceof SKArcShape);
         mniPopFixArcAngle.setEnabled(itemSelectionModel.selectedShapes().size()==1 && itemSelectionModel.selectedShapes().get(0) instanceof SKArcShape);
         mniDelete.setEnabled(b || c);
         mniPopMakeGroup.setEnabled(b1);
         mniPopMakeGrouptree.setEnabled(b1); 
         mniNewConstraint.setEnabled(itemSelectionModel.selectedShapes().size()>1);
         mniPopSelectAll.setEnabled(panelShapeArea.getComponentCount()>0);
         mniPopPaste.setEnabled(shapeClipboard.size()>0);
      }
   /**
   * Handles pop-up mouse trigger and displays the pop up sub menu for groups.
   */
      void popupGroup_popupMenuWillBecomeVisible(PopupMenuEvent e)
      
      {
         mniPopFixGroup.setEnabled(selectedGroups.size()>0);
         mniPopUnFixGroup.setEnabled(selectedGroups.size()>0);
      
      }

    /**
     * Handles mouse click on preferences button. Invokes simpleSolver to solve the sketch on the screen as soon the simpleSolver is enabled by the user
     */
    public void mniPref_actionPerformed(ActionEvent e) { //Create and show SKOptionFrame
        SKOptionDialog fr = new SKOptionDialog(this, "Preferences", true, HomeDir);
        fr.show();

        //Used when enabling SimpleSolving
        if (fr.needRedoAllSolving) {
            SKConstraintArray cons = new SKConstraintArray(5);
            SKBaseShape sh;
            for (int i = 0; i < panelShapeArea.getComponentCount(); i++) {
                sh = (SKBaseShape) panelShapeArea.getComponent(i);
                if (sh.getSelectable().isPrimaryShape(sh))
                    SKSimpleSolver.ResolveSimpleConstraints(sh.getSelectable(), cons);
            }
        }

        fr.dispose();

        RefreshShapeArea();
    }

    /**
     * Handles mouse click on distance constraint button using InitConstraint method.
     */
    @Override
    public void mniDistanceConstr_actionPerformed(ActionEvent e) {
        InitConstraint(createConstraint(0, ++ConstrIDCnt), 2, 2);
    }

    /**
     * Handles mouse click on incidence constraint button using InitConstraint method.
     */
    @Override
    public void mniIncidenceConstr_actionPerformed(ActionEvent e) {
        InitConstraint(createConstraint(1, ++ConstrIDCnt), 2, 2);
    }

    /**
     * Handles mouse click on perpendicular constraint button using InitConstraint method.
     */
    @Override
    public void mniPerpConstraint_actionPerformed(ActionEvent e) {
        InitConstraint(createConstraint(2, ++ConstrIDCnt), 2, 2);
    }

    /**
     * Handles mouse click on parallel constraint button using InitConstraint method.
     */
    @Override
    public void mniParallelConstraint_actionPerformed(ActionEvent e) {
        InitConstraint(createConstraint(3, ++ConstrIDCnt), 2, 2);
    }

    /**
     * Handles mouse click on angle constraint button using InitConstraint method.
     */
    @Override
    public void mniAngleConstr_actionPerformed(ActionEvent e) {
        InitConstraint(createConstraint(4, ++ConstrIDCnt), 2, 2);
    }

    /**
     * Handles mouse click on tangent constraint button using InitConstraint method.
     */
    @Override
    public void mniTangentConstraint_actionPerformed(ActionEvent e) {
        InitConstraint(createConstraint(5, ++ConstrIDCnt), 2, 2);
    }

   /**
   * Handles mouse click on shape button. Changes the cursor to cross hair cursor to indicate that a shape has been picked to draw.
   */
   public void ShapeButton_actionPerformed(ActionEvent e) {
       panelShapeArea.setCursor(Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR));
   }

    /**
     * Handles mouse click on cursor button.
     */
    public void btnCursor_actionPerformed(ActionEvent e) {
        panelShapeArea.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
    }
   /**
   * switches DrawConstraints flag.
   */
      void chkDrawConstraints_actionPerformed(ActionEvent e)
      {
         panelShapeArea.DrawConstraints=!panelShapeArea.DrawConstraints;
      
         panelShapeArea.repaint();
      }
   /**
   * Handles mouse click on repository button.
   */
      void mniRepositoryViewer_actionPerformed(ActionEvent e)
      {
         repository.setVisible( !repository.isVisible() );}
   
      void mniView_menuSelected(MenuEvent e)
      {
         mniRepositoryViewer.setState( repository.isVisible() );
      }
   /**
   * Handles mouse movement events on main screen. To change the color of the shapes and the constraints that are under the mouse.
   */
      void panelShapeArea_mouseMoved(MouseEvent e)
      {
      //Update mouse coordinates on statusbar
      //sbStatus.updatePanelText(e.getX()+" , "+e.getY(),2);
      
         boolean wasNull = (mouseOverItem==null);
         if (mouseOverItem==null || !mouseOverItem.doHitTest(e.getX(),e.getY()) )
         {
            for (int i=0; i<DrawnItems.size(); i++)
            {
               mouseOverItem = DrawnItems.get(i);
               if ( mouseOverItem.doHitTest(e.getX(),e.getY()) )
               {
                  sbStatus.updatePanelText( mouseOverItem.toString(),3);
                  panelShapeArea.repaint();
                  return;
               }
            }
         
            mouseOverItem=null;
            sbStatus.updatePanelText("",3);
            if (!wasNull)  panelShapeArea.repaint();
         }
      }
   /**
   * Handles mouse click on constraints on the right hand side panel. Uses EditConstraint and addSelectedShape methods to let the user edit the selected constraints
   * and to highlight the shapes, that are involved in the selected constraint.
   */
      void treeConstraints_mouseClicked(MouseEvent e)
      {
         if (e.getClickCount()==2)
         { //Start editing this constraint, if one is selected
            TreePath Path = treeConstraints.getPathForLocation(e.getX(),e.getY());
            if (Path==null)  
               return;
            Object ob = Path.getLastPathComponent();
            if (ob instanceof SKBaseConstraint)
            {
               EditConstraint( (SKBaseConstraint)ob );
            }
            else if (ob instanceof SKBaseShape)
            {
               addSelectedShape( (SKBaseShape)ob );
            }
         }
      }
   
      void mniRepository_menuSelected(MenuEvent e)
      {
         mniRepositoryViewer.setState( repository.isVisible() );
      }
   
      void treeConstraints_keyTyped(KeyEvent e)
      {
         if (e.getKeyChar()=='\b' || e.getKeyCode()==e.VK_DELETE)
            deleteItem(treeConstraints.getSelectionPath().getLastPathComponent());
      }
   /**
   * Uses deleteItem method to delete trees.
   */
      void mniTreeDelete_actionPerformed(ActionEvent e)
      {
         deleteItem(treeConstraints.getSelectionPath().getLastPathComponent());
      }
   /**
   * Action routine for make new group button.
   */
      void mniMakeGroup_actionPerformed(ActionEvent e)
      {
         b2=true;
         ++GroupIDCnt;
         SKGroupTreeNode newNode1 = (SKGroupTreeNode) SKGroups.makeGrouptree(
                 groupTree,
                 itemSelectionModel.selectedShapesShapeArray(),
                 GroupIDCnt,
                 groupArray,
                 treeID-2,
                 fixed);
         groupArray.add(newNode1);
      
         for(int a=0; a<groupArray.size(); a++)
         {
            for(int b=0; b<newNode1.parents.size(); b++)
            {
               if(groupArray.get(a)==newNode1.parents.get(b))
               {SKGTNArray temp=new SKGTNArray(100);
                  groupArray.get(a).children.copyArrayTo(temp);
                  groupArray.copyArrayTo(groupArray.get(a).children);
                  int d=0;
                  while (d<groupArray.get(a).children.size())
                  {
                     if(temp.size()==0)
                     { 
                        if(groupArray.get(a).children.get(d)!=newNode1)
                        {groupArray.get(a).children.removeNode(groupArray.get(a).children.get(d));
                           d--;}}
                     else
                     {boolean belongs=false;
                        for(int c=0; c<temp.size(); c++)
                           belongs=belongs||(groupArray.get(a).children.get(d)==temp.get(c));
                        if(!belongs&&groupArray.get(a).children.get(d)!=newNode1 )
                        { groupArray.get(a).children.removeNode(groupArray.get(a).children.get(d));
                           d--;}
                     }
                     d++;
                  }
               }  
            }
            for(int b=0; b<newNode1.children.size(); b++)
            {
               if(groupArray.get(a)==newNode1.children.get(b))
               {
                  SKGTNArray temp=new SKGTNArray(100);
                  groupArray.get(a).parents.copyArrayTo(temp);
                  groupArray.copyArrayTo(groupArray.get(a).parents);
                  int d=0;
                  while (d<groupArray.get(a).parents.size())
                  {
                     if(temp.size()==0)
                     { 
                        if( groupArray.get(a).parents.get(d)!=newNode1)
                        {groupArray.get(a).parents.removeNode(groupArray.get(a).parents.get(d));
                           d--;}}
                     else
                     {boolean belongs=false;
                        for(int c=0; c<temp.size(); c++)
                           belongs=belongs||(groupArray.get(a).parents.get(d)==temp.get(c));
                        if(!belongs && groupArray.get(a).parents.get(d)!=newNode1)
                        {groupArray.get(a).parents.removeNode(groupArray.get(a).parents.get(d));
                           d--;}
                     }
                     d++;
                  }
               
               }
            }
         }
      
      
         treeGroups.updateUI();
      }
   /**
   * Action routine for fix arc radius button. Allows the user to drag the arc to change its angle.
   */
      void mniFixArcRadius_actionPerformed(ActionEvent e)
      {
          final SKBaseShape firstShape = itemSelectionModel.selectedShapes().get(0);

         if(((SKArcShape)firstShape).fixradius ==false)((SKArcShape)firstShape).fixradius = true;
         else ((SKArcShape)firstShape).fixradius = false;
      }
   	/**
   * Action routine for fix arc angle button. Allows the user to drag the arc to change its radius.
   */
      void mniFixArcAngle_actionPerformed(ActionEvent e)
      
      {
          final SKBaseShape firstShape = itemSelectionModel.selectedShapes().get(0);


         if(((SKArcShape)firstShape).fixangle == false)((SKArcShape)firstShape).fixangle =true;
         else ((SKArcShape)firstShape).fixangle =false;
      }
   /**
   * Action routine for make group tree button. Displays the groups in the tree form.
   */
      void mniMakeGrouptree_actionPerformed(ActionEvent e)
      
      {
      
         File f = new File("");
         String st = f.getAbsolutePath();
         for(int i=0; i<groupArray.size(); i++)
         {
            boolean present=false;
            for(int a=0; a<allGroups.size(); a++)
               present = present || (allGroups.get(a).name.equals(groupArray.get(i).name));
            if (!present)allGroups.add(groupArray.get(i));   
         }
         int[] fin = new int[allGroups.size()];
         TreeFrame frame = new TreeFrame(this,allGroups,0,fin);
      
         frame.show();
      
      }
   
   /**
   * Action routine for solve with maple button.
   */
   
      void mniSolve_actionPerformed(ActionEvent e)
      {
         SKUTU solver = new SKMapleUTU();
      
      //Put all shapes into a SKShapeArray
         selectAllShapes();
      
      //Put all constraints into a SKConstraintArray
         SKConstraintArray cons = new SKConstraintArray( DrawnItems.size() );
         for (int i=0; i<DrawnItems.size(); i++)
            if (DrawnItems.get(i) instanceof SKBaseConstraint)
               cons.add( (SKBaseConstraint)DrawnItems.get(i) );
      
         solver.solveSystem(
                 this,
                 this.itemSelectionModel.selectedShapesShapeArray(),
                 cons);
      }
   /**
   * Action routine for make new tree button.
   */
   
      void mniNewTree_actionPerformed(ActionEvent e)
      
      {         
         for(int i=0; i<groupArray.size(); i++)
            allGroups.add(groupArray.get(i));  
         groupArray.clear();
         SKGroupTreeNode   newGroupTree = new SKGroupTreeNode(groupTree,treeID,"User"+treeID);
         //SKGTNArray groupArray = new SKGTNArray(100);
         groupTree.children.add(newGroupTree);
         groupArray.add(newGroupTree);
         treeID++;
         b1=true;
         GroupIDCnt=0;
      }
   
   /**
   * Handles mouse clicks on the groups in the trees displayed on the right hand side panel. Uses selectGroupShapes method to select all the shapes in that 
   * group.
   */
      void treeGroups_mouseClicked(MouseEvent e)
      {
      //Update group selection
         int i;
         for (i=0; i<selectedGroupShapes.size(); i++)
            selectedGroupShapes.get(i).highestGroup = null;
      
         for (i=0; i<selectedGroups.size(); i++)
            selectedGroups.get(i).groupRect.w = 0;
      
         selectedGroupShapes.clear();
         selectedGroups.clear();
      
         TreePath path;
         for (i=0; i<treeGroups.getSelectionCount(); i++)
         {
            path = treeGroups.getSelectionPaths()[i];
            if (path.getPathCount()>1)
            {
               selectedGroups.add( (SKGroupTreeNode)path.getLastPathComponent() );
               SKGroups.getAllGroupShapes( (SKGroupTreeNode)path.getLastPathComponent(), selectedGroupShapes, true );
            
            }
         }
      
         for (i=0; i<selectedGroupShapes.size(); i++)
            selectedGroupShapes.get(i).highestGroup = SKGroups.getHighestGroupWithShape(selectedGroupShapes.get(i),selectedGroups);
      
         drawGroupRects = (SKOptions.byteOptions[ SKOptions.groupDisplayMode ] == 1 && selectedGroups.size() > 0);
      
         lstGroupShapes.updateUI();
         RefreshShapeArea();
      }
   /**
   * Action routine for autosolve button. Calls mniAutoSolve method
   */
      void mniAutoSolve_actionPerformed(ActionEvent e)
      
      {mniAutoSolve();
      }
   	/**
   * Sets appropriate flags in the array sent to the UTU according to the mode selected by the user to update. The first integer in the array passed
   * to the UTU is the flag. for e.g. sets the flag to 13 for change constraint value and autosolve.
   */
      void mniAutoSolve()
      {
         if(!update)
         {
            getModeStateMachine().setModeFlag(VALUE_1);
            IntArray = new int[5000];
            DblArray = new double[2000];
            allConstraints.copyArrayTo(oldConstraints);
            allshapes.copyArrayTo(oldShapes);
            SKUTUFile.writeUTUFile(IntArray,DblArray,allshapes,groupTree);
            if(SKOptions.byteOptions[SKOptions.onlineMode]==0)
               update = true;
            mniReStart.setEnabled(true);
            mniOptions.setEnabled(true);
            dataExists=true;
         }
         else
         {
            switch(getModeStateMachine().getModeFlag())
            {
               case VALUE_3:
                  IntArray[0]=13;
                  getModeStateMachine().setModeFlag(VALUE_13);
                  break;
               case VALUE_4:
                  IntArray[0]=14;
                  getModeStateMachine().setModeFlag(VALUE_14);
                  break;
               case VALUE_5:
                  IntArray[0]=15;
                   getModeStateMachine().setModeFlag(VALUE_15);
                  break;
               case VALUE_6:
                  IntArray[0]=16;

                   getModeStateMachine().setModeFlag(VALUE_16);
                  break;
               case VALUE_7:
                  IntArray[0]=17;
                   getModeStateMachine().setModeFlag(VALUE_17);
                  break;
               default:
                  IntArray[0]=1;
                   getModeStateMachine().setModeFlag(VALUE_1);
                  break;
            }
            System.out.println("main frame1"+ IntArray[0]);
            utuJava utuDriver = new utuJava();
            utuDriver.utuC(IntArray, DblArray);
         
         }
         solve();
      }
   	/**
   * Action routine for get bifurcation button.
   */
      void mniTest_actionPerformed(ActionEvent e)
      {
         mniTest();
      }
   	/**
   * Sets appropriate flags in the array sent to the UTU according to the mode selected by the user to update. The first integer in the array passed
   * to the UTU is the flag. for e.g. sets the flag to 3 for change constraint value and solve using get bifurcation mode. This method calls 
   * writeUTUFile method in SKUTUFile.java which writes all the data into the array and sends the arrays to the UTU.
   */
      void mniTest()
      {
         if(!update)
         {
            getModeStateMachine().setModeFlag(DEFAULT_2);
            IntArray = new int[5000];
            DblArray = new double[2000];
            allConstraints.copyArrayTo(oldConstraints);
            allshapes.copyArrayTo(oldShapes);
            SKUTUFile.writeUTUFile(IntArray,DblArray,allshapes,groupTree);
            if(SKOptions.byteOptions[SKOptions.onlineMode]==0)
               update = true;
            mniReStart.setEnabled(true);
            mniOptions.setEnabled(true);
            dataExists=true;
         }
         else
         {
            switch(getModeStateMachine().getModeFlag())
            {
               case VALUE_3, VALUE_7, VALUE_6, VALUE_5, VALUE_4:
                  break;
                default :
                  IntArray[0]=2;
                  utuJava utuDriver = new utuJava();
                  utuDriver.utuC(IntArray, DblArray);
                  break;
            }
         }
         solve();
      }

    /**
     * sets mode to 3 for change constraint value, update
     */
    void mniChangeCons_actionPerformed(ActionEvent e) {
        getModeStateMachine().setModeFlag(VALUE_3);
        update = true;
    }

    /**
     * sets mode to 4 for add constraint, update
     */

    void mniAddCons_actionPerformed(ActionEvent e) {
        getModeStateMachine().setModeFlag(VALUE_4);
        update = true;

    }

    /**
     * sets mode to 5 for remove constraint, update
     */
    void mniRemoveCons_actionPerformed(ActionEvent e) {
        getModeStateMachine().setModeFlag(VALUE_5);
        update = true;
    }

    /**
     *sets mode to 6 for add tree, update
     */
    void mniAddTree_actionPerformed(ActionEvent e) {
        getModeStateMachine().setModeFlag(VALUE_6);
        update = true;

    }

    /**
     * sets mode to 7 for add shape and constraint, update
     */
    void mniAddShapeCon_actionPerformed(ActionEvent e) {
        getModeStateMachine().setModeFlag(VALUE_7);
        update = true;
    }

   /**
   *checks whether the proper update has been done to the sketch and then sends the arrays to UTU for solving. Also in the online mode invokes UTU each time 
   * a constraint is drawn
   */
      void solve()
      {
            //SKUTUFile.writeUTUFile(HomeDir+"/utu.dat",panelShapeArea,groupTree);
         boolean solve = true;
         System.out.println("main frame2"+ getModeStateMachine().getModeFlag().getCode());
         utuJava utuDriver = new utuJava();
         int lengthi, lengthd;
      
         sbStatus.updatePanelText("Solving...",0);
         selectAllShapes();
         switch(getModeStateMachine().getModeFlag())
         {
            case VALUE_0:
               break;
            case VALUE_1:
               break;
             case DEFAULT_2:
               break;
            case VALUE_3:
               lengthi = IntArray[0];
               lengthd = (int)DblArray[0];
               IntArray[0]=getModeStateMachine().getModeFlag().getCode();
               boolean changed = false, flag = false;
               for(int i = 0; i<allConstraints.size(); i++)
               {
                  if(allConstraints.get(i) instanceof SKDistanceConstraint)
                  {
                     SKDistanceConstraint con =(SKDistanceConstraint)allConstraints.get(i);
                  //SKDistanceConstraint con = (SKDistanceConstraint)oldConstraints.findByID(allConstraints.get(i).ID);
                     System.out.println("change cons "+consArray[con.ID]);
                     if(con.distance != consArray[con.ID]) 
                     {
                        System.out.println("change cons "+lengthi+" "+con.ID+" "+con.distance);
                        IntArray[lengthi++] = con.ID;
                        DblArray[lengthd++] = con.distance;
                        changed = false;
                        flag = true;
                     }
                  }
                  if(oldConstraints.get(i) instanceof SKAngleConstraint)
                  {
                     SKAngleConstraint con = (SKAngleConstraint)allConstraints.findByID(oldConstraints.get(i).ID);
                     changed = changed || ((int)con.angle != (int)((SKAngleConstraint)oldConstraints.get(i)).angle);
                     if(changed) 
                     {
                        IntArray[lengthi++] = con.ID;
                        DblArray[lengthd++] = con.angle;
                        changed = false;
                        flag = true;
                     }
                  }
               }
               if(flag)
               {
                  for (int x= IntArray[0]; x<IntArray[0]+30 ; x++)
                     System.out.println(IntArray[x]);
                  for (int x= (int)DblArray[0]; x<DblArray[0]+30 ; x++)
                     System.out.println(DblArray[x]);
               
                  IntArray[0]=3;
                  utuDriver.utuC(IntArray, DblArray);
               }
               else 
               {
                  solve =false;
                  JOptionPane dialog = null;
                  dialog.showMessageDialog(panelShapeArea,"Not done the correct update");
                   getModeStateMachine().setModeFlag(VALUE_0);
               }
               break;
            case VALUE_4:
               lengthi = IntArray[0];
               IntArray[0]=getModeStateMachine().getModeFlag().getCode();
               System.out.println("main frame flag"+lengthi);
               lengthd = (int) DblArray[0];
               if(allConstraints.size()>oldConstraints.size())
               {
                  for(int i=0; i<allConstraints.size(); i++)
                     if(oldConstraints.indexOf(allConstraints.get(i)) == -1)
                     {
                        SKBaseConstraint con = allConstraints.get(i);
                        SKConstraintArray Cons =new SKConstraintArray(10);
                        if (Cons.indexOf(con)==-1)
                        {
                           Cons.add(con);
                           IntArray[lengthi++]=con.typeID;
                           System.out.println("add constraint "+con.typeID);
                        //p.writeInt(con.typeID);
                           IntArray[lengthi++]=con.ID;
                           System.out.println("add constraint "+con.ID);
                        //p.writeInt(con.ID);
                           IntArray[lengthi++]=con.ShapeList.size();
                           System.out.println("add constraint "+con.ShapeList.size());
                        //p.writeInt(con.ShapeList.size());
                           if (con instanceof SKNormalConstraint)
                           { //CAN consist of specific subshapes
                              for (int g=0; g<con.ShapeList.size(); g++)
                              {
                                 SKBaseShape tmp = con.ShapeList.get(g);
                                 IntArray[lengthi++]=tmp.ID;
                                 System.out.println("add constraint "+tmp.ID);                                 
                              //p.writeInt(tmp.ID);
                                 IntArray[lengthi++]=con.getConInfo(tmp,0);
                                 System.out.println("add constraint "+con.getConInfo(tmp,0));
                              //p.writeInt(con.getConInfo(tmp,0));
                              }
                           
                              switch (con.typeID)
                              {
                                 case 0: DblArray[lengthd++]=((SKDistanceConstraint)con).distance;
                                 //p.writeDouble( ((SKDistanceConstraint)con).distance );
                                    break;
                                 case 4: DblArray[lengthd++]=((SKAngleConstraint)con).angle;
                              //p.writeDouble( ((SKAngleConstraint)con).angle );
                              }
                           }
                           else
                           { //Cannot consist of specific subshapes
                              for (int g=0; g<con.ShapeList.size(); g++)
                                 IntArray[lengthi++]=con.ShapeList.get(g).ID;
                           //p.writeInt(con.ShapeList.get(g).ID);
                           }
                        }
                     }
               
                  IntArray[lengthi]=-1;
                  for (int x= IntArray[0]; x<IntArray[0]+30 ; x++)
                     System.out.println(IntArray[x]);
                  for (int x= (int)DblArray[0]; x<DblArray[0]+30 ; x++)
                     System.out.println(DblArray[x]);
               
                  IntArray[0]=4;
                  utuDriver.utuC(IntArray, DblArray);
               }
               else 
               {
                  solve =false;
                  JOptionPane dialog = null;
                  dialog.showMessageDialog(panelShapeArea,"Not done the correct update");
                   getModeStateMachine().setModeFlag(VALUE_0);
               }
               break;
            case VALUE_5:
               lengthi = IntArray[0];
               IntArray[0]=getModeStateMachine().getModeFlag().getCode();
               if(allConstraints.size()<oldConstraints.size())
               {
                  for(int i = 0 ; i<oldConstraints.size(); i++)
                     if(allConstraints.indexOf(oldConstraints.get(i)) == -1)
                        IntArray[lengthi++] = oldConstraints.get(i).ID;
                  for (int x= IntArray[0]; x<IntArray[0]+30 ; x++)
                     System.out.println(IntArray[x]);
                  for (int x= (int)DblArray[0]; x<DblArray[0]+30 ; x++)
                     System.out.println(DblArray[x]);
               
                  IntArray[0]=5;
                  utuDriver.utuC(IntArray, DblArray);
                  allConstraints.copyArrayTo(oldConstraints);
               }
               else 
               {
                  solve =false;
                  JOptionPane dialog = null;
                  dialog.showMessageDialog(panelShapeArea,"Not done the correct update");
                   getModeStateMachine().setModeFlag(VALUE_0);
               }
               break;
            case VALUE_6:
               lengthi = IntArray[0];
               lengthd = (int)DblArray[0];
               IntArray[0]=getModeStateMachine().getModeFlag().getCode();
               if(allGroups.size()>oldGroups.size())
               {
                  for(int i=0; i<allGroups.size(); i++)
                     if(oldGroups.indexOf(allGroups.get(i)) == -1)
                        allGroups.get(i).saveToUTUFile(lengthi,IntArray);
                  for (int x= IntArray[0]; x<IntArray[0]+30 ; x++)
                     System.out.println(IntArray[x]);
                  for (int x= (int)DblArray[0]; x<DblArray[0]+30 ; x++)
                     System.out.println(DblArray[x]);
               
                  IntArray[0]=6;
                  utuDriver.utuC(IntArray, DblArray);
                  allConstraints.copyArrayTo(oldConstraints);
               }
               else 
               {
                  solve =false;
                  JOptionPane dialog = null;
                  dialog.showMessageDialog(panelShapeArea,"Not done the correct update");
                  getModeStateMachine().setModeFlag(VALUE_0);
               }
               break;
            case VALUE_7:
               lengthi = IntArray[0];
               lengthd = (int) DblArray[0];
               IntArray[0] = getModeStateMachine().getModeFlag().getCode();
               if((allshapes.size()>oldShapes.size()) || (allConstraints.size()>oldConstraints.size()))
               {
                  if(allshapes.size()>oldShapes.size())
                  {
                     for(int i=0; i<allshapes.size(); i++)
                        if(oldShapes.indexOf(allshapes.get(i)) == -1)
                        {
                           SKBaseShape sh = allshapes.get(i);
                           switch(sh.ShapeTypeID)
                           {
                              case 3:
                                 SKLineShape line = (SKLineShape)sh;
                              //Write TypeID based if Segment,Ray,Line
                                 switch (line.pt1.pointType+line.pt2.pointType)
                                 {
                                    case 0:IntArray[lengthi++]=3; //Segment
                                    //System.out.println("typeID"+3);
                                       break;
                                    case 1: IntArray[lengthi++]=2; //Ray
                                       break;
                                    case 2: IntArray[lengthi++]=1; //Line (infinite)
                                 }
                                 break;
                              default:
                                 IntArray[lengthi++]=sh.ShapeTypeID;
                                 break;
                           }
                           IntArray[lengthi++]=sh.ID;	
                           switch (sh.ShapeTypeID)
                           {
                           
                              case 0:
                                 {
                                    DblArray[lengthd++]=(double)sh.getShapeX();
                                    DblArray[lengthd++]=(double)sh.getShapeY();
                                 }
                                 break;
                              case 3 : 
                                 { //SKLineShape
                                 //p.writeInt(888);
                                    SKLineShape line = (SKLineShape)sh;
                                    DblArray[lengthd++]=(double)line.pt1.getShapeX();
                                    DblArray[lengthd++]=(double)line.pt1.getShapeY();
                                    DblArray[lengthd++]=(double)line.pt2.getShapeX();
                                    DblArray[lengthd++]=(double)line.pt2.getShapeY();
                                    DblArray[lengthd++]=line.length;
                                 //p.writeInt(999);
                                 }
                                 break;
                              case 4 : 
                                 { //SKCircleShape

                                    DblArray[lengthd++]=(double)((SKCircleShape)sh).center.getShapeX();
                                    DblArray[lengthd++]=(double)((SKCircleShape)sh).center.getShapeY() ;
                                    DblArray[lengthd++]=((SKCircleShape)sh).radius ;
                                 }
                                 break;
                              case 5:
                                 {
                                    DblArray[lengthd++]=(double)((SKArcShape)sh).center.getShapeX();
                                    DblArray[lengthd++]=(double)((SKArcShape)sh).center.getShapeY();
                                    DblArray[lengthd++]=((SKArcShape)sh).radius;
                                    DblArray[lengthd++]=((SKArcShape)sh).angle;
                                 }
                              
                                 break;
                              default: 
                                 break;
                           }
                        }
                  
                     IntArray[lengthi++]=-1;
                     DblArray[lengthd++]=-1;
                  }
                  if(allConstraints.size()>oldConstraints.size())
                  {
                     for(int i=0; i<allConstraints.size(); i++)
                        if(oldConstraints.indexOf(allConstraints.get(i)) == -1)
                        {
                           SKBaseConstraint con = allConstraints.get(i);
                           SKConstraintArray Cons =new SKConstraintArray(10);
                           if (Cons.indexOf(con)==-1)
                           {
                              Cons.add(con);
                              IntArray[lengthi++]=con.typeID;
                           //p.writeInt(con.typeID);
                              IntArray[lengthi++]=con.ID;
                           //p.writeInt(con.ID);
                              IntArray[lengthi++]=con.ShapeList.size();
                           //p.writeInt(con.ShapeList.size());
                              if (con instanceof SKNormalConstraint)
                              { //CAN consist of specific subshapes
                                 for (int g=0; g<con.ShapeList.size(); g++)
                                 {
                                    SKBaseShape tmp = con.ShapeList.get(g);
                                    IntArray[lengthi++]=tmp.ID;
                                 //p.writeInt(tmp.ID);
                                    IntArray[lengthi++]=con.getConInfo(tmp,0);
                                 //p.writeInt(con.getConInfo(tmp,0));
                                 }
                              
                                 switch (con.typeID)
                                 {
                                    case 0: DblArray[lengthd++]=((SKDistanceConstraint)con).distance;
                                    //p.writeDouble( ((SKDistanceConstraint)con).distance );
                                       break;
                                    case 4: DblArray[lengthd++]=((SKAngleConstraint)con).angle;
                                 //p.writeDouble( ((SKAngleConstraint)con).angle );
                                 }
                              }
                              else
                              { //Cannot consist of specific subshapes
                                 for (int g=0; g<con.ShapeList.size(); g++)
                                    IntArray[lengthi++]=con.ShapeList.get(g).ID;
                              //p.writeInt(con.ShapeList.get(g).ID);
                              }
                           }
                        }
                     IntArray[lengthi++]=-1;
                     DblArray[lengthd++]=-1;
                  }
                  for (int x= IntArray[0]; x<IntArray[0]+30 ; x++)
                     System.out.println(IntArray[x]);
                  for (int x= (int)DblArray[0]; x<DblArray[0]+30 ; x++)
                     System.out.println(DblArray[x]);
               
                  IntArray[0]=7;
                  utuDriver.utuC(IntArray, DblArray);
                  allConstraints.copyArrayTo(oldConstraints);
                  allshapes.copyArrayTo(oldShapes);
               
               }
               else 
               {
                  solve =false;
                  JOptionPane dialog = null;
                  dialog.showMessageDialog(panelShapeArea,"Not done the correct update");
                  getModeStateMachine().setModeFlag(VALUE_0);
               }
               break;
            default:
               break;
         }
      
         if(solve)
            while(SKUTUFile.waitForUpdate(IntArray, DblArray, itemSelectionModel.selectedShapesShapeArray(), this, panelShapeArea.getWidth(), panelShapeArea.getHeight(),dagMain ))
            {
               System.out.println("main treeCont"+treeCont);
               boolean singleton=true;
               SKGTNArray roots = new SKGTNArray(2);
               for(int i=0; i<dagMain.size(); i++)
                  if(dagMain.get(i).parents.size() == 0) roots.add(dagMain.get(i));
               for(int i=0; i<roots.size() ; i++)
                  for(int j=0; j<roots.get(i).children.size(); j++)
                  {singleton = singleton && (roots.get(i).children.get(j).shapes.size() == 1);
                     System.out.println( "singleton "+singleton+" "+roots.get(i).children.get(j)+" "+roots.get(i).children.get(j).shapes.size());
                  }
               if((SKOptions.byteOptions[ SKOptions.onlineMode ] >0) && singleton)
               {simpleSolve = true;
                  break;}
               selectAllShapes();
               if(treeCont)
               {
                  System.out.println("main treeCont");
                  IntArray[0]=2;
                  treeCont = false;
               }
               if(solved)
               {
                  if(isFrontier())
                     checkForFrontier();
                  /*for(int i=0; i<frontier.size(); i++)
                  {
                     SKBaseShape sh = frontier.get(i);
                     for(int j=0; j<sh.ConstraintList.size(); j++)
                     {SKBaseConstraint con = sh.ConstraintList.get(j);
                        if(SKOptions.byteOptions[ SKOptions.simpleSolverMode ] != 0)
                        {
                           if (SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] == 1)
                              con.isDrawn = (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0 && !SKSimpleSolver.DoSimpleSolver(con,null,new SKConstraintArray(3)));
                           else 
                              con.isDrawn = (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0 && !SKSimpleSolver.DoSimpleSolver(con));
                        }
                     }
                  }*/
                  resetState();
               }
               System.out.println("main frame flag"+ IntArray[0]);
               utuDriver.utuC(IntArray, DblArray);
               dataExists = true;
            }
         //System.out.println(SelectedShapes.size());
         clearSelectedShapes(true,true);
         //System.out.println(SelectedShapes.size());
         sbStatus.updatePanelText("Ready",0);
         RefreshShapeArea();
         //System.out.println(SelectedShapes.size());
      }
   
   /**
   	*adjusts sketch when horizontal scroll bar is moved
   	*/
      void scrHoriz_adjustmentValueChanged(AdjustmentEvent e)
      {
         doTranslate(lastHorizScroll-e.getValue(),0);
      }
   	/**
   	*adjusts sketch when vertical scroll bar is moved
   	*/
      void scrVert_adjustmentValueChanged(AdjustmentEvent e)
      {
         doTranslate(0,lastVertScroll-e.getValue());
      }
      void scrHoriz1_adjustmentValueChanged(AdjustmentEvent e)
      {
         doTranslate1(lastHorizScroll1-e.getValue(),0);
      }
   
      void scrVert1_adjustmentValueChanged(AdjustmentEvent e)
      
      {
         doTranslate1(0,lastVertScroll1-e.getValue());
      }
      void tabpaneEditor_stateChanged(ChangeEvent e)
      {
         if (tabpaneEditor.getSelectedIndex() != 1 && SKOptions.byteOptions[ SKOptions.unSelGroupsWhen ] == 1)
         {
            clearSelectedGroups();
            RefreshShapeArea();
         }
      }
      void tabPartialSketch_stateChanged(ChangeEvent e)
      
      {
         if (tabPartialSketch.getSelectedIndex() != 1 )
         {
            clearSelectedGroups();
            RefreshShapeArea();
         }
      }

      @Override
      public void cmbScale_itemStateChanged(ItemEvent e)
      {
         if (e.getStateChange()==e.SELECTED)
            doScale( (Float.parseFloat(e.getItem().toString())) / 100 );
      }
   
      void treeConstraints_mouseReleased(MouseEvent e)
      {
         TreePath path = treeConstraints.getPathForLocation(e.getX(),e.getY());
         treeConstraints.setSelectionPath(path);
      
         if (e.isPopupTrigger() && path != null && path.getLastPathComponent() instanceof SKItemInterface)
            popTree.show(treeConstraints,e.getX(),e.getY());
      }
   
   ////////////////////////////////////////////////////////////////////////////
   //
   //      Methods related to major functionality
   //
   ////////////////////////////////////////////////////////////////////////////
   
   /**
   *Clears data
   */
      public void NewData()
      {
         paracount=0;
      //Clear data & update displa;
         mniTest.setEnabled(true);
         mniAutoSolve.setEnabled(true);
         IntArray = new int[1];
         DblArray = new double[1];
         dataExists = false;
         solved = false;
         lastScale=1;
         if(dataExists)
            mniOptions.setEnabled(true);
         else 
            mniOptions.setEnabled(false);
         clearSelectedShapes(true,true);
         DrawnItems.clear();
         cmbShapes.removeAllItems();
         allGroups.clear();
         allshapes.clear();
         groupArray.clear();
         treeID=1;
         b1=false;
         update = false;
         for (int i=0; i<panelShapeArea.getComponentCount(); i++)
         {
            System.out.println("main "+(SKBaseShape)panelShapeArea.getComponent(i));
            ((SKBaseShape)panelShapeArea.getComponent(i)).severTies();
            ((SKBaseShape)panelShapeArea.getComponent(i)).fixed=false;
         }
         panelShapeArea.removeAll();
         panelShapeArea.repaint();
      
      //Setup vars
         IDCnt = 1;
         ConstrIDCnt = 0;
         GroupIDCnt = 0;
         groupTree.severTies();
         sFileName = "";
         sbStatus.updatePanelText("Untitled",4);
         bDataModified = false;
         saveArrays = false;
         sbStatus.updatePanelText("",1);
         mouseOverItem = null;
      }
   
   /**
   *Lets user choose a file, and then calls LoadData(FileName)
   */
      public void OpenFile()
      {
      //Let user find file
         JFileChooser dlgOpenFile = new JFileChooser(HomeDir);
      
         ExtensionFileFilter filter = new ExtensionFileFilter("Sketcher Data (*.skr)");
         filter.addExtension("skr");
      
         dlgOpenFile.setFileFilter(filter);
      
         int res = dlgOpenFile.showOpenDialog(this);
         if (res == JFileChooser.APPROVE_OPTION)
            LoadFile( dlgOpenFile.getSelectedFile().toString() ); //load the chosen file
      }
   
   /**
   *Actually loads data from the specified file
   */
      public void LoadFile(String FileName)
      {
      //Make sure the file exists
         File fDataFile = new File(FileName);
         if (!fDataFile.isFile() || !fDataFile.exists()) 
            return;
      
         NewData();
      
      //Load the data
         try
         {
            readFromFile(FileName);
         }
            catch(Exception e)
            {
               JOptionPane.showMessageDialog(this,"File Error: An error occured while loading "+FileName,"File Error",JOptionPane.ERROR_MESSAGE);
               e.printStackTrace();
            }
      
         mruManager.addMRUItem(fDataFile.getName(), fDataFile.toString());
         mruManager.saveMRUtoFile(HomeDir+"/mruData.dat");
         mruManager.syncMenu(mniReopen);
      
      //Update vars
         sFileName = FileName;
         sbStatus.updatePanelText(sFileName,4);
         bDataModified = false;
         saveArrays = false;
         sbStatus.updatePanelText("",1);
      
      }
   /**
   *saves the new sketch
   */
      public boolean InternalSaveAsChanges()
      {
      //First saveas, prompt for filename
         JFileChooser dlgSaveFile = new JFileChooser(HomeDir);
      
         int res = dlgSaveFile.showSaveDialog(this);
         if (res == JFileChooser.APPROVE_OPTION)
         {
            sFileName = dlgSaveFile.getSelectedFile().toString();
            sbStatus.updatePanelText(sFileName,4);
         
            //Create file
            try
            {
               saveToFile(sFileName);
            
            }
            
               catch(Exception e)
               
               {
                  JOptionPane.showMessageDialog(this,"File Error: An error occured while saving to "+sFileName,"File Error",JOptionPane.ERROR_MESSAGE);
                  e.printStackTrace();
               }
         }
         else
         {
            return false;
         }
         bDataModified = false;
         saveArrays = false;
         sbStatus.updatePanelText("",1);
         return true;
      }
   /**
   *Saves changes to data (returns false if canceled)
   */
      public boolean InternalSaveChanges()
      {
      //Actually save data
         if (sFileName=="" || sFileName==null)
         {
         //First save, prompt for filename
            JFileChooser dlgSaveFile = new JFileChooser(HomeDir);
         
            int res = dlgSaveFile.showSaveDialog(this);
            if (res == JFileChooser.APPROVE_OPTION)
            {
               sFileName = dlgSaveFile.getSelectedFile().toString();
               sbStatus.updatePanelText(sFileName,4);
            
            //Create file
               try
               {
                  saveToFile(sFileName);
               
               }
                  catch(Exception e)
                  {
                     JOptionPane.showMessageDialog(this,"File Error: An error occured while saving to "+sFileName,"File Error",JOptionPane.ERROR_MESSAGE);
                     e.printStackTrace();
                  }
            }
            else
            {
               return false;
            }
         }
         else
         {
         //File exists, overwrite
            try
            {
               saveToFile(sFileName);
            }
               catch(Exception e)
               {
                  JOptionPane.showMessageDialog(this,"File Error: An error occured while saving to "+sFileName,"File Error",JOptionPane.ERROR_MESSAGE);
                  e.printStackTrace();
               }
         }
      
         bDataModified = false;
         saveArrays = false;
         sbStatus.updatePanelText("",1);
         return true;
      }
   
   /**
   * If changes have been made, prompts user to save (and saves if user wants) Returns false if canceled, else returns true
   */
      public boolean PromptSaveChanges()
      {
      //Has there been any changes?
         if (bDataModified)
         {
         //Prompt user to save
            int ret = JOptionPane.showConfirmDialog( this, "Data modified. Save changes?", "Save changes?" , JOptionPane.YES_NO_CANCEL_OPTION );
            if (ret == JOptionPane.CANCEL_OPTION) 
            { 
               return false;
            }
            if (ret == JOptionPane.YES_OPTION) InternalSaveChanges();
         }
      
         return true;
      }
      public boolean PromptSaveSketch()
      
      {
      //Is there additional data?
         if (dataExists)
         {
         //Prompt user to save sketch or sketch and data
            int ret = JOptionPane.showConfirmDialog( this, "Do you want to save solver data ?" , "Save Sketch Data?", JOptionPane.YES_NO_CANCEL_OPTION );
            if (ret == JOptionPane.CANCEL_OPTION) 
            {
               dataExists = false;
               saveArrays = false;
               int IntArray[];
               double DblArray[];
               if(bDataModified)
                  return PromptSaveChanges();
               else 
                  return false;
            }
            if (ret == JOptionPane.YES_OPTION)
            { saveArrays = true;
               InternalSaveChanges();}
         }
      
         return true;
      }
   
      public boolean PromptSaveAsSketch()
      
      
      {
      //Is there additional data?
         if (dataExists)
         {
         //Prompt user to save sketch or sketch and data
            int ret = JOptionPane.showConfirmDialog( this, "Do you want to save solver data ?" , "Save Sketch Data?", JOptionPane.YES_NO_CANCEL_OPTION );
            if (ret == JOptionPane.CANCEL_OPTION) 
            {
               dataExists = false;
               saveArrays = false;
               InternalSaveAs();
               return false;
            }
            if (ret == JOptionPane.YES_OPTION)
            { saveArrays = true;
               InternalSaveAs();}
         }
      
         return true;
      }
      public boolean InternalSaveAs()
      {
         String OldName = new String(sFileName);
      
         sFileName="";
         if ( !InternalSaveChanges() )
         { //Revert to old filename
            sFileName=OldName;
            return false;
         }
         else 
            return true;
      }
   
   /**
   *Copys a shape clone to the clipboard
   */
      public void copySelectedShapes()
      {
         shapeClipboard.clear();
         ClipWasCut = false;

         shapeClipboard.storeShapes(itemSelectionModel.selectedShapes());
      }
   /**
   *Cuts a shape from screen and copies a shape clone to the clipboard
   */
      public void cutSelectedShapes()
      {
         shapeClipboard.clear();
         ClipWasCut = true;
         SKBaseShape sh;
         for (int i=0; i<itemSelectionModel.selectedShapesCount(); i++)
         {
            sh = itemSelectionModel.selectedShapeAt(i);
            shapeClipboard.storeShape(sh);
            for (int g=1; g<sh.getNumSubShapes()+1; g++)
            {panelShapeArea.remove(sh.getSubShape(g));
            
            }
         
         }
      
         clearSelectedShapes(true,true);
      }
   /**
   *pastes the shape in clipboard to the screen
   */
      public void pasteClipShapes()
      {
         if (ClipWasCut)
         {
            SKBaseShape sh;
            for (int i=0; i<shapeClipboard.getStoredShapes().size(); i++)
            {
               sh = shapeClipboard.getStoredShapes().get(i);
               for (int g=1; g<sh.getNumSubShapes()+1; g++)
               {
                  panelShapeArea.add(sh.getSubShape(g));
                  sh.repaint();
               }
            
            }
         
            ClipWasCut = false; //can't just add them back anymore
         }
         else
         {
            for (int i=0; i<shapeClipboard.size(); i++)
            {
            //ClipShapes.get(i).
            
               SKBaseShape l=shapeClipboard.getStoredShapes().get(i);
            
               SKBaseShape r= createShape(l.getShapeTypeID(),l.getShapeX()+50,l.getShapeY()+50,IDCnt,true);
               if(r.ShapeTypeID==0) IDCnt++;
               else
                  IDCnt=IDCnt+r.getNumSubShapes()+1;
            }}
      }
   
   /**
   *Adds a shape to the selectedShapes array
   */
      public void addSelectedShape(SKBaseShape newShape)
      {
         if (newShape.getSelectable().isSelected()) 
            return;
      
         newShape.getSelectable().selShape = newShape;
         newShape = newShape.getSelectable();
         itemSelectionModel.selectShape(newShape);
         newShape.setSelected(true);
      
         if (itemSelectionModel.selectedShapesCount()==1)
         {
            if (!editingShape)
            {
               editingShape = true;
               itemSelectionModel.deselectConstraint(true);
               tabpaneObjectProp.setTitleAt(1,"Constraints");
               tabpaneObjectProp.setSelectedIndex(0);
            }
         
            SelectedPropLevel=0;
         
            newShape.updateMainShapeData(vCurrentProps,vCurrentConstraints);
         
            Updating++;
            cmbShapes.setSelectedItem(newShape);
            Updating--;
         
            tableObjectProp.updateUI();
            treeConstraints.updateUI();
            treeConstraints.expandRow(1);
         
            tabpaneObjectProp.setEnabled(true);
         }
         else
         {
            if (SelectedPropLevel==0)
            {
               Updating++;
               cmbShapes.setSelectedIndex(-1);
               Updating--;
            }
         
            if (SelectedPropLevel<2)  mergeProps(newShape,1);
         
         }
         if (itemSelectionModel.selectedShapesCount() == 2)
         {
            Point pt1 = itemSelectionModel.selectedShapeAt(0).getPointForDistance(itemSelectionModel.selectedShapeAt(1)),
            pt2 = itemSelectionModel.selectedShapeAt(1).getPointForDistance(itemSelectionModel.selectedShapeAt(0));
            sbStatus.updatePanelText(Float.toString((float)pt1.distance(pt2)),2);
         }
         else sbStatus.updatePanelText("",2);
      
         updateConstraintUI();
      
      }
   
   /**
   *Clears all shapes, and adds newShape to the selectedShapes array (so its the only selected shape)
   */
      public void addOnlySelectedShape(SKBaseShape newShape)
      {
      //Already only selected shape?
         if (itemSelectionModel.selectedShapesCount()==1 && newShape.getSelectable().isSelected())
            return;
      
         clearSelectedShapes(false,true);
         addSelectedShape(newShape);
      }
   
   /**
   *Removes oldShape from the selectedShapes array
   */
      public void removeSelectedShape(SKBaseShape oldShape)
      {
         oldShape = oldShape.getSelectable();
         itemSelectionModel.deselectShape(oldShape);
         oldShape.setSelected(false);
      
         if (itemSelectionModel.selectedShapesCount()==0)
         {
            cmbShapes.setSelectedIndex(-1);
            propFactory.freeProps(vCurrentProps);
            vCurrentConstraints.clear();
         
            tableObjectProp.removeEditor();
            tableObjectProp.updateUI();
            treeConstraints.updateUI();
            treeConstraints.collapseRow(1);
         
            tabpaneObjectProp.setEnabled(false);
         }
      
         if (itemSelectionModel.selectedShapesCount() == 2)
         {
            Point pt1 = itemSelectionModel.selectedShapeAt(0).getPointForDistance(itemSelectionModel.selectedShapeAt(1)),
            pt2 = itemSelectionModel.selectedShapeAt(1).getPointForDistance(itemSelectionModel.selectedShapeAt(0));
            sbStatus.updatePanelText(Float.toString((float)pt1.distance(pt2)),2);
         }
         else sbStatus.updatePanelText("",2);
      
         mergeProps(null,2);
         updateConstraintUI();
      }
   
      public void toggleSelectedShape(SKBaseShape sh)

      {
         if (sh.getSelectable().isSelected())
            removeSelectedShape(sh);
         else
            addSelectedShape(sh);
      }
   
   /**
   *Removes all shapes from the selectedShapes array(updateView should be true!)
   */
      public void clearSelectedShapes(boolean updateView, boolean unselectShapes)
      {
         if (unselectShapes)
         {
            SKBaseShape sh;
            for (int i=0; i<itemSelectionModel.selectedShapesCount(); i++)
            {
               sh=itemSelectionModel.selectedShapeAt(i);
               sh.setSelected(false);
            }
         }

         itemSelectionModel.clearSelectedShapes();
         tableObjectProp.removeEditor();
         tabpaneObjectProp.setSelectedIndex(0);
      
         if (updateView)
         {
            propFactory.freeProps(vCurrentProps);
            vCurrentConstraints.clear();
         
            cmbShapes.setSelectedIndex(-1);
            tableObjectProp.updateUI();
            treeConstraints.updateUI();
            treeConstraints.collapseRow(1);
         
            tabpaneObjectProp.setEnabled(false);
         
            updateConstraintUI();
         }

         final var selectedConstraint = itemSelectionModel.selectedConstraint();
         if (!editingShape && selectedConstraint.isPresent())
         {
             selectedConstraint.get().repaint();
             itemSelectionModel.deselectConstraint();
         }
      
         sbStatus.updatePanelText("",2);
      }
   
   /**
   *Selects all shapes
   */
      public void selectAllShapes()
      {
         for (int i=0; i<panelShapeArea.getComponentCount(); i++)
            addSelectedShape( ((SKBaseShape)panelShapeArea.getComponent(i)) );
      }
   
   /**
   *Merges properties of currently selected shapes with new selected shape(s)
   */
      public void mergeProps(SKBaseShape newShape, int newPropLevel)
      {
      //Adjust SelectedPropLevel
         if (itemSelectionModel.selectedShapesCount()==1)
         { //Only 1 selected shape
            newPropLevel=0;
            cmbShapes.setSelectedItem(itemSelectionModel.selectedShapeAt(0));
         }
         else
         {
            if (newShape==null)
            { //Removed shape
               newPropLevel = 1;
               if (SelectedPropLevel==2)
               { //Are they now all the same class?
                  Class base = itemSelectionModel.selectedShapeAt(0).getClass();
                  int i=0;
                  while (newPropLevel==1 && i<itemSelectionModel.selectedShapesCount())
                  {
                     if (itemSelectionModel.selectedShapeAt(i).getClass() != base)  newPropLevel=2;
                     i++;
                  }
               }
            }
            else
            { //Added shape
               if (newPropLevel==1 && itemSelectionModel.selectedShapeAt(0).getClass() != newShape.getClass())  newPropLevel=2;
            }
         }
      
      //Don't do anything if there is no change
         if (newPropLevel==SelectedPropLevel) 
            return;
      
      //Refresh props
         if (newPropLevel<SelectedPropLevel)  itemSelectionModel.selectedShapeAt(0).updateMainShapeData(vCurrentProps,null);
      
      //Remove props < SelectedPropLevel
         int i=0;
         while ( i<vCurrentProps.size() )
            if (vCurrentProps.getShapePropType(i)<newPropLevel)
               vCurrentProps.remove(i);
            else
               i++;
      
         SelectedPropLevel=newPropLevel;
         tableObjectProp.repaint();
      }
   
      void updateConstraintUI()
      {
          toolbarConstraints.updateConstraintUI();

          mniAngleConstr.setEnabled(constraintModel.canApplyConstraintType(ConstraintType.ANGLE));
          mniDistanceConstr.setEnabled(constraintModel.canApplyConstraintType(ConstraintType.DISTANCE));
          mniIncidenceConstr.setEnabled(constraintModel.canApplyConstraintType(ConstraintType.INCIDENCE));
          mniParallelConstraint.setEnabled(constraintModel.canApplyConstraintType(ConstraintType.PARALLEL));
          mniPerpConstraint.setEnabled(constraintModel.canApplyConstraintType(ConstraintType.PERPENDICULAR));
          mniTangentConstraint.setEnabled(constraintModel.canApplyConstraintType(ConstraintType.TANGENT));
      }

      void panelShapeArea_mouseClicked(MouseEvent e) {
         ex = e.getX();
         ey = e.getY();
         if (e.isPopupTrigger()) {
            return;
         }

         if (mouseOverItem != null)
         {
            if (mouseOverItem instanceof SKBaseShape)
            {
               if (e.isControlDown())
                  toggleSelectedShape((SKBaseShape)mouseOverItem);
               else
               {addOnlySelectedShape((SKBaseShape)mouseOverItem);
                  //System.out.println("shape"+itemSelectionModel.selectedShapeAt()(0));
               }
            }
            else
               EditConstraint( (SKBaseConstraint)mouseOverItem );

            return;
         }

         if (toolbarShapes.isCursorSelected())
         {
            clearSelectedShapes(true,true);
            return;
         }

      //A shape is selected, add to data and create
         if (!bDataModified)
         {
            bDataModified=true;
            sbStatus.updatePanelText("Modified",1);
         }

          final var selectedShapeType = toolbarShapes.getSelectedShapeType();

          if (selectedShapeType.isEmpty()) {
              createShape(ShapeType.POINT.getIdCode(), e.getX(), e.getY(), IDCnt, true);
              IDCnt++;
          } else {
              IDCnt = IDCnt + (createShape(selectedShapeType.get().getIdCode(), e.getX(), e.getY(), IDCnt, true)).getNumSubShapes() + 1;
          }
      }
   
      void panelGroups_mouseClicked(MouseEvent e)
      
      {
         if (e.isPopupTrigger())
         {
            return;
         }
      
      }
   	/**
   	*Creates new shapes using the constructor of the specified shape, and then adds the shape to the array of all the shapes on the screen; allShapes
   	*/
      public SKBaseShape createShape(int shapeTypeID,int x, int y, int ID, boolean autoSelect)
      {  SKBaseShape newShape = null;
         boolean ResetCursorButton = true;
         boolean RedrawAll = false;
         boolean AddToShapeArea = true;
      
         switch (shapeTypeID)
         {
            case 0:
               {
                  newShape = new SKPointShape(this,ID,x,y);
                  panelShapeArea.add((SKPointShape)newShape);
                  allshapes.add(newShape);
               }
               break;
            case 1:
               break;
            case 2:
               break;
            case 3:  
               { //Line segment
                  newShape = new SKLineShape(this,ID,x,y);
                  allshapes.add(newShape);
               //Add points to ShapeArea
                  panelShapeArea.add( ((SKLineShape)newShape).pt1 );
                  panelShapeArea.add( ((SKLineShape)newShape).pt2 );
                  AddToShapeArea = false;
                  ResetCursorButton = true;
                  RedrawAll = true;
               }
               break;
            case 4:  
               { //Circle Shape
                  newShape = new SKCircleShape(this,ID,x,y);
                  allshapes.add(newShape);
                 //Add point to ShapeArea
                  panelShapeArea.add( ((SKCircleShape)newShape).center );
               
                  AddToShapeArea = false;
                  ResetCursorButton = true;
                  RedrawAll = true;
               }
               break;
            case 5:  
               { //Arc Shape
                  newShape = new SKArcShape(this,ID,x,y);
                  allshapes.add(newShape);
                  panelShapeArea.add( ((SKArcShape)newShape).center );
                  panelShapeArea.add( ((SKArcShape)newShape).pt1 );
                  panelShapeArea.add( ((SKArcShape)newShape).pt2 );
               
                  AddToShapeArea = false;
                  ResetCursorButton = true;
                  RedrawAll = true;
               }
            /* 4-Ray, 5-Line (infinite) */
               break;
            case 6:  newShape = new SKImageShape(this,ID,x,y);
               allshapes.add(newShape);
               panelShapeArea.add( ((SKImageShape)newShape).pt1);
               panelShapeArea.add( ((SKImageShape)newShape).pt2);
               /*panelShapeArea.add( ((SKImageShape)newShape).circle );
               panelShapeArea.add( ((SKImageShape)newShape).circle.center );*/
               break;
            default: 
               return null; //Unknown shape
         }
         DrawnItems.add( newShape.getSelectable() );
      
         if (AddToShapeArea) panelShapeArea.add(newShape);
         Updating++;
         cmbShapes.addItem( newShape );
         cmbShapes.setSelectedItem( newShape );
         Updating--;
      
         if (ResetCursorButton)
         {
            toolbarShapes.setCursorSelected(true);
            panelShapeArea.setCursor( Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR) );
         }
      
         if (autoSelect)  addOnlySelectedShape(newShape);
      
         if (RedrawAll)
            panelShapeArea.repaint();
         else
            newShape.repaint();
         //System.out.println(panelShapeArea.getComponentCount());
      // allshapes.add(newShape);
         return newShape;
      }
   
      SKBaseConstraint canCreate(int conTypeID, SKShapeArray selectedShapes)
      {
         if (selectedShapes.size() == 0)  
            return null;
      
         SKBaseShape sh = selectedShapes.get(0);
         SKBaseConstraint con;
         for (int i=0; i<sh.ConstraintList.size(); i++)
         {
            con = sh.ConstraintList.get(i);
            if (con.typeID==conTypeID)
            {
               if (con.constrEquals(itemSelectionModel.selectedShapesShapeArray(),con))
                  return con;
            }
            else
            {
               switch (conTypeID)
               {
                  case 3 : 
                     { //Perp -- same as Tangent (5)
                        if (con instanceof SKParallelConstraint)
                           return con;
                     }
                     break;
                  case 4 : 
                     {//Parallel -- same as Tangent (5)
                        if (con instanceof SKPerpConstraint)
                           return con;
                     }
               }
            }
         }
      
         return null;
      }
   /**
   *Creates a new constraint using the constructor of the specified constraint and adds it to the array of all constraints, allConstraints.
   */
      public SKBaseConstraint createConstraint(int typeID, int ID)
      {
         SKBaseConstraint con    = null,
         oldCon = canCreate(typeID,itemSelectionModel.selectedShapesShapeArray());
      
         if (oldCon==null)
            switch (typeID)
            {
               case 1 : con = new SKIncidenceConstraint(this,ID);
                  break;
               case 2 : con = new SKPerpConstraint(this,ID);
                  break;
               case 3 : con = new SKParallelConstraint(this,ID);
                  break;
               case 4 : con = new SKAngleConstraint(this,ID);
                  break;
               case 5 : con = new SKTangentConstraint(this,ID);
                  break;
               default: con = new SKDistanceConstraint(this,ID); //
            }
         allConstraints.add(con);
         toolbarShapes.setCursorSelected(true);
         panelShapeArea.setCursor( Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR) );
      
         if (con == null)
         { //Constraint already existed, tell user and edit it
            if (oldCon.typeID==typeID)
               JOptionPane.showMessageDialog(this,"Constraint Error: This constraint already exists.","Constraint Error",JOptionPane.ERROR_MESSAGE);
            else
               JOptionPane.showMessageDialog(this,"Constraint Error: There is a conflicting constraint.","Constraint Error",JOptionPane.ERROR_MESSAGE);
         
            EditConstraint(oldCon);
         }
         else
         { //Added new constraint
            DrawnItems.add(con);
         }
      
         return con;
      }
   
   //Allows an easy way for shapes to cause panelShapeArea to repaint
      public void RefreshShapeArea()
      {
         panelShapeArea.repaint();
      }
   
      public void RefreshShapeArea(int x,int y,int w,int h)
      {
         panelShapeArea.repaint(x,y,w,h);
      }
   
      public void RefreshShapeArea(Rectangle rt)
      {
         panelShapeArea.repaint(rt);
      }
   
      public void EditConstraint(SKBaseConstraint con)
      {
         editingShape = false;
         clearSelectedShapes(false,true);
         vCurrentConstraints.clear();
         tabpaneObjectProp.setTitleAt(1,"Shapes");
         tabpaneObjectProp.setEnabled(true);
         tabpaneObjectProp.setSelectedIndex(0);

         con.updateMainProperties(vCurrentProps);
         con.repaint();
         itemSelectionModel.selectConstraint(con);

         cmbShapes.setSelectedIndex(-1);
         tableObjectProp.removeEditor();
         tableObjectProp.updateUI();
         treeConstraints.updateUI();
         deleteConstraint = con;
      }
   /**
   *This method is called each time a constrait is edited. It decides whether or not to invoke the simpleSolver when a constraint is added, depending
   * on the user's preferences
   */
      public void InitConstraint(SKBaseConstraint con, int minShapes, int maxShapes)
      {
         if (con == null) 
            return;
         //System.out.println("main: "+con);
         if (minShapes == -1 || itemSelectionModel.selectedShapesCount()>=minShapes && itemSelectionModel.selectedShapesCount()<=maxShapes)
         {
            for (int i=0; i<itemSelectionModel.selectedShapesCount(); i++)
            {
               con.addShape( itemSelectionModel.selectedShapeAt(i) );
            }
            if(con instanceof SKParallelConstraint)
            {
               for(int i=0; i<con.ShapeList.size(); i++)
                  for(int j=0; j<con.ShapeList.get(i).ConstraintList.size(); j++)
                     if(con.ShapeList.get(i).ConstraintList.get(j) instanceof SKParallelConstraint)
                        ((SKParallelConstraint)con).count=((SKParallelConstraint)con.ShapeList.get(i).ConstraintList.get(j)).count ;
               if(((SKParallelConstraint)con).count == 0)
               {paracount++;
                  ((SKParallelConstraint)con).count = paracount;}
            }
            if(SKOptions.byteOptions[ SKOptions.onlineMode ] ==1)
            {
               if(update && oldConstraints.size()<allConstraints.size())
                   getModeStateMachine().setModeFlag(VALUE_4);
               else if(update && oldConstraints.size()>allConstraints.size())
                   getModeStateMachine().setModeFlag(VALUE_5);
               else if(update && oldShapes.size()<allshapes.size() && oldConstraints.size()<allConstraints.size())
                   getModeStateMachine().setModeFlag(VALUE_7);
               mniTest();
               if(simpleSolve)
                  if(SKOptions.byteOptions[ SKOptions.simpleSolverMode ] != 0)
                  {
                     if (SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] == 0)
                        con.isDrawn = (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0 && !SKSimpleSolver.DoSimpleSolver(con,null,new SKConstraintArray(3)));
                     else 
                        con.isDrawn = (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0 && !SKSimpleSolver.DoSimpleSolver(con));
                  }
            }
            else if(SKOptions.byteOptions[ SKOptions.onlineMode ] ==2)
            {
               if(update && oldConstraints.size()<allConstraints.size())
                   getModeStateMachine().setModeFlag(VALUE_4);
               else if(update && oldConstraints.size()>allConstraints.size())
                   getModeStateMachine().setModeFlag(VALUE_5);
               else if(update && oldShapes.size()<allshapes.size() && oldConstraints.size()<allConstraints.size())
                   getModeStateMachine().setModeFlag(VALUE_7);
            
               mniAutoSolve();
               if(simpleSolve)
                  if(SKOptions.byteOptions[ SKOptions.simpleSolverMode ] != 0)
                  {
                     if (SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] == 0)
                        con.isDrawn = (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0 && !SKSimpleSolver.DoSimpleSolver(con,null,new SKConstraintArray(3)));
                     else 
                        con.isDrawn = (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0 && !SKSimpleSolver.DoSimpleSolver(con));
                  }
            }
            else if (SKOptions.byteOptions[ SKOptions.onlineMode ] ==0)
            {
               if(SKOptions.byteOptions[ SKOptions.simpleSolverMode ] != 0)
               {
                  if (SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] == 0)
                     con.isDrawn = (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0 && !SKSimpleSolver.DoSimpleSolver(con,null,new SKConstraintArray(3)));
                  else 
                     con.isDrawn = (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0 && !SKSimpleSolver.DoSimpleSolver(con));
               }
            
            }
            repaint();
         
            EditConstraint(con);
         }
         else
         {
            if (minShapes==maxShapes)
               JOptionPane.showMessageDialog(this,"Incorrect number of items selected. Allowed: "+minShapes,"Constraint Error",JOptionPane.ERROR_MESSAGE);
            else
               JOptionPane.showMessageDialog(this,"Incorrect number of items selected. Allowed: ["+minShapes+","+maxShapes+"]","Constraint Error",JOptionPane.ERROR_MESSAGE);
         }
         //checkForFrontier();
      }
      public boolean isFrontier()
      {
         if(solved)
         {
            SKGTNArray roots = new SKGTNArray(2);
         
            for(int i=0; i<dagMain.size() ; i++)
               if(dagMain.get(i).parents.size() == 0)
                  roots.add(dagMain.get(i));
            if(roots.size()>1)
               return true;
            else 
               return false;
         }
         return false;
      }
      public void checkForFrontier()
      {
         if(solved)
         {
            SKGTNArray roots = new SKGTNArray(2);
            SKShapeArray shapes = new SKShapeArray(2);
            for(int i=0; i<dagMain.size() ; i++)
               if(dagMain.get(i).parents.size() == 0)
                  roots.add(dagMain.get(i));
            if(roots.size()>1)
            {
               for(int i=0; i<dagMain.size() ; i++)
                  for(int j=0; j<dagMain.get(i).shapes.size(); j++)
                  {
                     SKBaseShape sh = dagMain.get(i).shapes.get(j);
                     if(shapes.indexOf(sh) == -1)
                        shapes.add(sh);
                     else 
                     {
                        frontier.add(sh);
                        dagMain.get(i).shapes.copyArrayTo(sh.clusterShapes);
                        sh.clusterShapes.removeShape(sh);
                     }
                  }
               for(int i=0; i<frontier.size(); i++)
               {
                  SKBaseShape sh = frontier.get(i);
                  for( int j=0; j<sh.ConstraintList.size() ; j++)
                  {
                     SKBaseConstraint con = sh.ConstraintList.get(j);
                     for(int k =0; k< con.ShapeList.size() ; k++)
                        if(frontier.indexOf(con.ShapeList.get(k)) == -1)
                        {
                           sh.ConstraintList.remove(con);
                           j--;
                           break;               
                        }
                  }
               }
            }                      
         }
      }
   
      public void resetState()
      {
         for(int i=0; i<allConstraints.size() ; i++)
         {
            SKBaseConstraint con = allConstraints.get(i);
            for(int j=0; j<con.ShapeList.size() ; j++)
            {
               SKBaseShape sh = con.ShapeList.get(j);
               if(sh.ConstraintList.indexOf(con) == -1)
                  sh.ConstraintList.add(con);
            }
         }
      }
   	/**
   *Saves the sketch to file.
   */
      public void saveToFile(String fileName) throws FileNotFoundException,IOException
      {
         sbStatus.updatePanelText("Writing...",0);
         setCursor( Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR) );
      
         FileOutputStream fStream = new FileOutputStream(fileName);
         DataOutputStream p = new DataOutputStream(fStream);
      
      //Write header
         p.writeInt(IDCnt);
         p.writeInt(ConstrIDCnt);
      
      //Loop through shapes and write to file
         SKBaseShape sh;
         for (int i=0; i<panelShapeArea.getComponentCount(); i++)
         {
            sh = (SKBaseShape)panelShapeArea.getComponent(i);
            if ( sh.getSelectable().isPrimaryShape(sh) )
               sh.getSelectable().writeToStream(p,true);
         }
         System.out.println("saved shapes");
      //Signify end of shapes
         p.writeInt(-2);
      
      //Loop through and write constraints to file
         for (int i=0; i<panelShapeArea.getComponentCount(); i++)
         {
            sh = (SKBaseShape)panelShapeArea.getComponent(i);
            if ( sh.getSelectable().isPrimaryShape(sh) )
               sh.getSelectable().writeConstraintsToStream(p,true,null);
         }
         System.out.println("saved constraints");
      //Signify end of constraints
         p.writeInt(-2);
      
      //Write group to tree
         groupTree.saveToStream(p);
         System.out.println("saved trees");
      //Signify end of tree
         p.writeInt(-2);
      
      //Write arrays
         if(saveArrays)
         {
            p.writeInt(1);
            p.writeInt(IntArray.length);
            for(int i=0; i<IntArray.length; i++)
               p.writeInt(IntArray[i]);
            p.writeInt(DblArray.length);	
            for(int i=0; i<DblArray.length; i++)
               p.writeDouble(DblArray[i]);
            System.out.println("saved arrays");
         }
      //Signify end of file
         p.writeInt(-3);
      //Flush to file
         p.flush();
         p.close();
         fStream.flush();
         fStream.close();
      
         sbStatus.updatePanelText("Ready",0);
         setCursor( Cursor.getDefaultCursor() );
      }
   /**
   *Reads from the specified file. Calls readFromStream method
   */
      public void readFromFile(String fileName) throws FileNotFoundException,IOException,StreamCorruptedException
      {
         FileInputStream fStream = new FileInputStream(fileName);
      
         readFromStream(fStream,true);
      
         fStream.close();
      }
   
      public void readFromStream(InputStream strm, boolean readID) throws IOException,StreamCorruptedException
      {
         sbStatus.updatePanelText("Reading...",0);
         setCursor( Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR) );
      
      //FileInputStream fStream = new FileInputStream(fileName);
         DataInputStream p = new DataInputStream(strm);
      
      //Read header
         if (readID)
         {
            IDCnt = p.readInt();
            ConstrIDCnt = p.readInt();
         }
         else
         {
            shI = p.readInt();
            conI = p.readInt();
         }
         repShIDs = new int[shI+1];
         repConIDs = new int[conI];
         shI=0;
         conI=0;
         //Read shapes
         SKBaseShape sh;
         int typeID = p.readInt(), newID;
         while (typeID != -2)
         {
            newID = p.readInt(); 
            if(!readID)
            {
               repShIDs[newID] = IDCnt;
               newID = IDCnt;
            }
            sh = createShape(typeID,1,1,newID,false);
         
            if (!readID)
            {
               if(typeID==0) 
                  IDCnt++;
               else
                  IDCnt += sh.getNumSubShapes()+1;
            }
            if(readID)sh.readFromStream(p);
            
            else if (!readID && repFirst)
            {
               repx = ex;
               repy = ey; 
               sh.readFromStream(p,ex,ey);
               repFirst = false;
            }
            else sh.readFromStream(p,ex,ey);
         
            typeID = p.readInt();
         
         }
         repFirst = true;
      //Read constraints
         SKBaseConstraint con;
         int g,h,i,j;
         typeID = p.readInt();
         while (typeID != -2)
         {
            newID = p.readInt() ;
            if(!readID)
            {
               newID = ++ConstrIDCnt;
               repConIDs[conI++] = newID;
            }
            con = createConstraint(typeID,newID);
            con.readFromStream(p);
            //associate shapes
            g = p.readInt();
            int[] IDlist = new int[g];
            for (h=0; h<g; h++)
            {
               if (readID)
                  IDlist[h] = p.readInt();
               else 
               {
                  int x = p.readInt();
                  IDlist[h] = repShIDs[x];
               }
            }
            j=0;
            while (j<g)
            {
               h=0;
               while (/* h<panelShapeArea.getComponentCount() && */ ((SKBaseShape)panelShapeArea.getComponent(h)).getSelectable().ID != IDlist[j])
                  h++;
               con.addShape( ((SKBaseShape)panelShapeArea.getComponent(h)).getSelectable() );
               j++;
            }
         
            typeID = p.readInt();
         }
         System.out.println("read constraints");
      //Read groups from stream
         if(readID)
         {groupTree.readFromStream(p,panelShapeArea);
            GroupIDCnt = groupTree.findMaxID(0);
            treeGroups.updateUI();}
      //check if additional data exists
         p.readInt();
         System.out.println("read trees");
         if(p.readInt()!=-3)
         {
            int print = p.readInt();
            if(print == 1)
            {
               IntArray = new int[p.readInt()];
               dataExists = true;
               mniOptions.setEnabled(true);
               for( i=0; i<IntArray.length; i++)
                  IntArray[i] = p.readInt();
               DblArray = new double[p.readInt()];
               for( i=0; i<DblArray.length; i++)
                  DblArray[i]=p.readDouble();
               System.out.println("read arrays");
            }
         }
         else
         {
            dataExists = false;
            mniOptions.setEnabled(false);
            mniReStart.setEnabled(false);
            int IntArray[];
            double DblArray[];
         }
      //Reset other vars
         mouseOverItem = null;
         System.out.println("read "+IDCnt);
         sbStatus.updatePanelText("Ready",0);
         setCursor( Cursor.getDefaultCursor() );
      }
   /**
   *Method to add a new library of shapes to the repository
   */
      public void addUserObject(String fileName, String objectName) throws FileNotFoundException,IOException
      {
         int repShID=1, repConID=1;
         shI=0;
         conI=0;
         int maxID=0;
         for(int i=0; i<itemSelectionModel.selectedShapesCount(); i++)
            if(maxID<itemSelectionModel.selectedShapeAt(i).ID)
               maxID = itemSelectionModel.selectedShapeAt(i).ID;
      
         repShIDs = new int[itemSelectionModel.selectedShapesCount()];
         repConIDs = new int [allConstraints.size()];
         sbStatus.updatePanelText("Writing...",0);
         setCursor( Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR) );
      
         File tmpFile = new File(repository.repositoryDir+"tmpfile.$$$");
         File zipFile = new File(fileName);
      
         FileOutputStream fStream = new FileOutputStream(tmpFile);
         ZipOutputStream zip = new ZipOutputStream(fStream);
      
         InputStream ins;
         ZipFile zin = new ZipFile(zipFile);
         Enumeration entries = zin.entries();
         byte[] buf = new byte[1024];
         int len;
         ZipEntry zEntry;
         while (entries.hasMoreElements())
         { //write current entries into new file
            zEntry = (ZipEntry)entries.nextElement();
            if (zEntry.getName() != "~Default")
            {
               zip.putNextEntry( new ZipEntry( zEntry.getName() ) );
               ins = zin.getInputStream( zEntry );
            
               do
               {
                  len = ins.read(buf);
                  if (len>-1) zip.write(buf,0,len);
               }
               while (len!=-1);
            
               ins.close();
               zip.closeEntry();
            }
         }
      
         zEntry = null;
         ins = null;
         zin.close();
         zin = null;
      
         zip.putNextEntry( new ZipEntry(objectName) );
      
         DataOutputStream p = new DataOutputStream(zip);
         p.writeInt(maxID);
         SKConstraintArray temp = new SKConstraintArray(2);
         for(int i=0; i<itemSelectionModel.selectedShapesCount(); i++)
         {
            for(int j=0; j<itemSelectionModel.selectedShapeAt(i).ConstraintList.size(); j++)
               if(temp.indexOf(itemSelectionModel.selectedShapeAt(i).ConstraintList.get(j))==-1)
                  temp.add(itemSelectionModel.selectedShapeAt(i).ConstraintList.get(j));
         }
         p.writeInt(temp.size());
      //Loop through shapes and write to file
         SKBaseShape sh ;
         for (int i=0; i<itemSelectionModel.selectedShapesCount(); i++)
         {
            sh = itemSelectionModel.selectedShapeAt(i);
              // if ( sh.getSelectable().isPrimaryShape(sh) )
            if(sh.getSelectable() instanceof SKNormalShape)
               sh.getSelectable().repID = repShID++;
            else 
            { sh.getSelectable().repID = repShID;
               repShID = repShID+sh.getSelectable().getNumSubShapes()+1;}
            repShIDs[shI++] = sh.getSelectable().ID;
            sh.getSelectable().writeToStream(p,false);
         
         }
        //Signify end of shapes
         p.writeInt(-2);
      
      //Loop through and write constraints to file
         for (int i=0; i<itemSelectionModel.selectedShapesCount(); i++)
         {
            sh = itemSelectionModel.selectedShapeAt(i);
            //if ( sh.getSelectable().isPrimaryShape(sh) )
            sh.getSelectable().writeConstraintsToStream(p,false,itemSelectionModel.selectedShapesShapeArray());
         }
      //Signify end of file
         p.writeInt(-2);
         p.writeInt(-2);
         p.writeInt(-3);
         repShIDs = new int[maxID];
         repConIDs = new int[allConstraints.size()];
      //Flush to file
         p.flush();
         zip.flush();
         zip.close();
         fStream.flush();
         fStream.close();
      
      //Delete old file, replace it with new file
         boolean res;
         res = zipFile.delete();
         res = tmpFile.renameTo(zipFile);
      
         sbStatus.updatePanelText("Ready",0);
         setCursor( Cursor.getDefaultCursor() );
      }
   
      public void clearSelectedGroups()
      {
         drawGroupRects = false;
      
         selectedGroups.clear();
      
         for(int i=0; i<selectedGroupShapes.size(); i++)
            selectedGroupShapes.get(i).highestGroup = null;
      
         selectedGroupShapes.clear();
      
         treeGroups.setSelectionPath(null);
      }
   /**
   *Changes the scale of the whole sketch depending on the specified scale
   */
      public void doScale(float newScale)
      {
         float delta = newScale/lastScale;
         Component cmpFix=null,cmp;
         int x=0,y=0;
         for(int i=0; i<panelShapeArea.getComponentCount(); i++)
         { cmpFix = panelShapeArea.getComponent(i);
            SKBaseShape sh = (SKBaseShape)cmpFix;
            if(sh instanceof SKPointSubShape)
            { SKPointSubShape sh1 = (SKPointSubShape)sh;
               if(!(sh1.parentShape instanceof SKImageShape))
               {x= cmpFix.getX();
                  y= cmpFix.getY();
                  break;}
            }
            else
            {x= cmpFix.getX();
               y= cmpFix.getY();
               break;}
         
         }
      
         for(int i=0; i<panelShapeArea.getComponentCount(); i++)
         {
            cmp = panelShapeArea.getComponent(i);
            int x1,y1;
            x1=java.lang.Math.round(((cmp.getX()-x)*delta)+x);
            y1=java.lang.Math.round(((cmp.getY()-y)*delta)+y);
            SKBaseShape sh = (SKBaseShape)cmpFix;
            SKBaseShape sh1 = (SKBaseShape)cmp;
            if((sh.ID != sh1.ID) || ((x==0) && (y==0)))
               cmp.setLocation( x1,y1 );
         }
         lastScale = newScale;
         int[] outline = new int[4];
         testLoc(outline);
         if (outline[0] < 0)
            doTranslate(-outline[0],0);
         else if (outline[2] > 1120)
            doTranslate(1120-outline[2],0);
      
         if (outline[1] < 0)
            doTranslate(-outline[1],0);
         else if (outline[3] > 820)
            doTranslate(820-outline[3],0);
      
      }
      public void doTranslate(int deltaX, int deltaY)
      {
         Component cmp;
         for (int i=0; i<panelShapeArea.getComponentCount(); i++)
         {
            cmp = panelShapeArea.getComponent(i);
            cmp.setLocation(cmp.getX()+deltaX,cmp.getY()+deltaY);
         }
         panelShapeArea.repaint();
      
         if (deltaX != 0)  lastHorizScroll = lastHorizScroll - deltaX;
         if (deltaY != 0)  lastVertScroll = lastVertScroll - deltaY;
      }
      public void doTranslate1(int deltaX, int deltaY)
      
      {   
         Component cmp;
         for (int i=0; i<panelPartial.getComponentCount(); i++)
         {
            cmp = panelPartial.getComponent(i);
            cmp.setLocation(cmp.getX()+deltaX,cmp.getY()+deltaY);
         }
         panelPartial.repaint();
         if (deltaX != 0)  lastHorizScroll1 = lastHorizScroll1 - deltaX;
         if (deltaY != 0)  lastVertScroll1 = lastVertScroll1 - deltaY;
      }
   /**
   *Deletes item
   */
      public void deleteItem(Object item)
      {//System.out.println("entered deleted item");
         if (item instanceof SKBaseConstraint)
         {
            SKBaseConstraint con = (SKBaseConstraint)item;
            if (itemSelectionModel.isConstraintSelected(con)) {
                itemSelectionModel.deselectConstraint();
            }
            if(vCurrentConstraints.size()>0)
               vCurrentConstraints.remove(con);
            allConstraints.remove(con);
            con.severTies();
            con = null;
         
            treeConstraints.updateUI();
            tableObjectProp.updateUI();
            RefreshShapeArea();
         }
         else if (item instanceof SKBaseShape)
         {
            SKBaseShape sh = (SKBaseShape)item;
            cmbShapes.removeItem(sh);
         
            SKGroups.removeShapeFromGroup(sh,groupTree,selectedGroups);
            selectedGroupShapes.removeShape(sh);
            treeGroups.updateUI();
            lstGroupShapes.updateUI();
            for (int g=1; g<sh.getNumSubShapes()+1; g++)
            {
               panelShapeArea.remove(sh.getSubShape(g));
               sh.getSubShape(g).severTies();
            }
         
            if(sh.getNumSubShapes()!=1)sh.severTies();
         
            DrawnItems.remove(sh);
            sh = null;
            RefreshShapeArea();
         
         }
      
         mouseOverItem = null;
      
      }
      public void paint(Graphics g)
      {
         super.paint(g);
         if(tabPartialSketch.getSelectedIndex()==0)
            panelShapeArea.drawSt(g,solvingShapes);
         else
            panelPartial.drawNames(g);
         return;
      }
      void drawSt( Graphics g)
      {Color red = Color.red;
      
         for (int j=0; j<panelShapeArea.getComponentCount(); j++)
         {  //if(solvingShapes.get(i).ID==((SKBaseShape)panelShapeArea.getComponent(j)).ID);
            SKBaseShape solveShape = (SKBaseShape) panelShapeArea.getComponent(j);
            if(solvingShapes.findByID(solveShape.ID) !=null)
            {
               g.setColor(red);
               g.drawString(solveShape.Name.charAt(0)+""+solveShape.ID, solveShape.getShapeX()+20, solveShape.getShapeY()+80);
            }
         }
      }
      public void testLoc (int[] outline)
      {
         if(panelShapeArea.getComponentCount()>0) 
         {
            outline[0] = panelShapeArea.getComponent(0).getX();
            outline[1] = panelShapeArea.getComponent(0).getY();
            outline[2] = panelShapeArea.getComponent(0).getX();
            outline[3] = panelShapeArea.getComponent(0).getY();
         }
         for(int i=0; i<panelShapeArea.getComponentCount(); i++)
         {
            if(outline[0]>panelShapeArea.getComponent(i).getX()) outline[0] = panelShapeArea.getComponent(i).getX();
            if(outline[1]>panelShapeArea.getComponent(i).getY()) outline[1] = panelShapeArea.getComponent(i).getY();
            if(outline[2]<panelShapeArea.getComponent(i).getX()) outline[2] = panelShapeArea.getComponent(i).getX();
            if(outline[3]<panelShapeArea.getComponent(i).getY()) outline[3] = panelShapeArea.getComponent(i).getY();
         }
      }
   }
