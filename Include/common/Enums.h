#ifndef ENUMS_H
#define ENUMS_H

/*	These defines are for the keyboard press event. These were obtained from the ASCII chart
    at http://www.bibase.com/images/ascii.gif */
#define LETTER_a 0x41
#define LETTER_b 0x42
#define LETTER_c 0x43
#define LETTER_d 0x44
#define LETTER_e 0x45
#define LETTER_f 0x46
#define LETTER_g 0x47
#define LETTER_h 0x48
#define LETTER_i 0x49
#define LETTER_j 0x4A
#define LETTER_k 0x4B
#define LETTER_l 0x4C
#define LETTER_m 0x4D
#define LETTER_n 0x4E
#define LETTER_o 0x4F
#define LETTER_p 0x50
#define LETTER_q 0x51
#define LETTER_r 0x52
#define LETTER_s 0x53
#define LETTER_t 0x54
#define LETTER_u 0x55
#define LETTER_v 0x56
#define LETTER_w 0x57
#define LETTER_x 0x58
#define LETTER_y 0x59
#define LETTER_z 0x5A

#define LETTER_A 0x61
#define LETTER_B 0x62
#define LETTER_C 0x63
#define LETTER_D 0x64
#define LETTER_E 0x65
#define LETTER_F 0x66
#define LETTER_G 0x67
#define LETTER_H 0x68
#define LETTER_I 0x69
#define LETTER_J 0x6A
#define LETTER_K 0x6B
#define LETTER_L 0x6C
#define LETTER_M 0x6D
#define LETTER_N 0x6E
#define LETTER_O 0x6F
#define LETTER_P 0x70
#define LETTER_Q 0x71
#define LETTER_R 0x72
#define LETTER_S 0x73
#define LETTER_T 0x74
#define LETTER_U 0x75
#define LETTER_V 0x76
#define LETTER_W 0x77
#define LETTER_X 0x78
#define LETTER_Y 0x79
#define LETTER_Z 0x7A

#define DEL_KEY 0x7F

//! The system state enum that describes the state that the UI is in
/*!
    This is used to keep track of what state that the UI is in. This will
    effect how frames are drawn and what variables are initilized or accessed.
    Some variables are accessed only when the user is drawing on the canvas,
    for example.
*/
enum class systemState
{
    ON_START_UP_STATE,/*!< The default value for the enum. This is the state that the program is in when the user first opens the progam in order to load any default settings */
    INITIAL_START_UP,/*!< This is the state that the program is in when the user can choose either new or open. The startup screen */
    PHYSICS_SELECTION,/*!< This is the state that the program is in when the user can choose the simulation they would like to run */
    MODEL_DEFINING,/*!< This is the state that the program is in when the user is drawing their geometry on the canvas */
    SIMULATING,/*!< This is the state that the program is in when the user is simulating their simulation */
    VIEWING_RESULTS/*!< This is the state that the program is in when the user is viewing the results of the simulation */
};


//! Enum used to designate the mesh size
/*!
    This enum is used mainly to indicate what mesh size
    the user would like to apply to a particular area
    The enum ranges from extremely fine to extremely coarse.
    There is also an option for a custom mesh size
*/
//class planarCoordinateEnum;
enum meshSize
{
    MESH_NONE_,/*!< Value to indicate that there is no mesh size for the designated area */
    MESH_EXTREMELY_FINE,/*!< Values to indicate that the mesh needs to contain the smallest element size possible */
    MESH_EXTRA_FINE,/*!< Values to indicate that the mesh needs to contain very small element sizes */
    MESH_FINER,/*!< Value to indicate that the mesh needes to contain small element sizes */
    MESH_FINE,/*!< Value to indicate that the mesh needs to contain mesh sizes that are a little bit smaller then the default */
    MESH_NORMAL,/*!< Value used to indicate that the mesh needs to contain the default element size */
    MESH_COARSE,/*!< Value used to indicate that the mesh needs to contain elements that are a little bit bigger then the default element size */
    MESH_COARSER,/*!< Value used to indicate that the mesh neeeds to contain large element sizes */
    MESH_EXTRA_COARSE,/*!< Value used to indicate that the mesh needs to contain very large element sizes */
    MESH_EXTREMELY_COARSE,/*!< Value used to indicate that the mesh needs to contain the largest possible element size */
    MESH_CUSTOM /*!< Value used to indicate that the user is manually inputting the element size */
};

//! Enum used in the event table to disgiunish the event as a file menu event
/*!
    This enum is used to indicate that the event came from the file
    menu. This will also allow propery routing for the event to
    call the correct function
*/
enum FileMenuID
{
    ID_NO_FILE_ID,/*!< Default value for the enum */
    ID_FILE_NEW,/*!< Value used to indicate that the event is a file new event */
    ID_SAVE,/*!< Value used to indicate that the event is a save event */
    ID_SAVE_AS,/*!< Value used to indicate that the event is a save as event */
    ID_OPEN/*!< Value used to indicate that the event is a open event */
};


//! Enum used in the event table to disguish that the event came from the edit menu
/*!
    This enum is used to indicate that the event came from the edit menu. This will
    allow the event table to properly route the event to the correct function
*/
enum EditMenuID
{
    NO_EDIT_ID = 100, /*!< Default value for the enum */
    ID_UNDO,/*!< Value used to indicate that the event is an undo event */
    ID_COPY,/*!< Value used to indicate that the event is a copy event*/
    ID_DELETE,/*!< Value used to indicate that the event is a delete event */
    ID_MOVE,/*!< Value used to indicate that the event is a move event */
    ID_SCALE,/*!< Value used to indicate that the event is a scale event */
    ID_MIRROR,/*!< Value used to indicate that the event is a mirror event */
    ID_CREATE_RADIUS,/*!< Value used to indicate that the event is a fillet event */
    ID_CREATE_OPEN_BOUNDARY,/*!< Value used to indicate that the event is to create an open boundary */
    ID_PREFERENCES, /*!< Valus used to indicate that the event is a global preferences event */
    ID_SELECT_GROUP,/*!< Value used to indicate that the event is a select group event */
    ID_EDIT_PROPERTY,/*!< Value used to indicate that the event is a edit property event */
    ID_LUA_RUN, /*!< Value used to indicate that the event is a LUA run event */
    ID_ADD_NODE, /*!< Value used to indicate that the event is a Add node event */
    ID_CLEAR
};


//! Enum used in the event table to disguish that the event came from the view menu
/*!
    This enum is used to indicate that the event came from the view menu. This will
    allow the event table to properly route the event to the correct function(s)
*/
enum ViewMenuID
{
    NO_VIEW_ID = 200,/*!< Default value for the enum */
    ID_ZOOM_IN,/*!< Value used to indicate that a zoom in event occured */
    ID_ZOOM_OUT,/*!< Value used to indicate that a zoom out event occured */
    ID_ZOOM_WINDOW,/*!< Value used to indicate that a zoom window event occured */
    ID_LUA_CONSOLE,/*!< Value used to indicate that the event for needing to bring up the Lua console occured */
    ID_SHOW_STATUSBAR,/*!< Value used to indicate the that user is toggleing the display of the statusbar */
    ID_SHOW_BLOCK_NAMES,/*!< Value used to indicate that the user is toggling the display of block label names */
    ID_SHOW_ORPHANS/*!< Value used to indicate that the user is toggling the selection of dangling nodes */
};

//! Enum used in the event table to disguish that the event came from the grid menu
/*!
    This enum is used to indicate that the event came from the grid menu. This will
    allow the event table to properly route the event to the correct function
*/
enum GridMenuID
{
    NO_GRID_ID = 300, /*!< Default value for the enum */
    ID_SHOW_GRID,/*!< Value used to indicate that the user is toggeling the display of the grid */
    ID_SNAP_GRID,/*!< Value used to indicate that the user is toggeling the snapping of the grid */
    ID_SET_GRID_PREFERENCES/*!< Value used to indicate that the user will be editing the grid preferences */
};

//! Enum used in the event table to disguish that the event came from the properties menu
/*!
    This enum is used to indicate that the event came from the properties menu. This will
    allow the event table to properly route the event to the correct function
*/
enum PropertiesMenuID
{
    NO_PROPERTY_ID = 400,/*!< Defualt value for the enum */
    ID_MATERIALS,/*!< Value used to indicate that the event for needing to bring up the Material Definition window occured */
    ID_BOUNDARY,/*!< Value used to indicate that the event for needing to bring up the Boundary Definition window occured */
    ID_POINT,/*!< Value used to indicate that the event for needing to bring up the Nodal Definition window occured */
    ID_CONDUCTORS,/*!< Value used to indicate that the event for needing to bring up the Conductors Definition window occured */
    ID_EXTERIOR_REGION,/*!< Value used to indicate that the event for needing to bring up the Exterior Region window occured */
    ID_MATERIAL_LIBRARY/*!< Value used to indicate that the event for needing to bring up the Material Library window occured */
};


//! Enum used in the event table to disguish that the event came from the Mesh menu
/*!
    This enum is used to indicate that the event came from the Mesh menu. This will
    allow the event table to properly route the event to the correct function
*/
enum MeshMenuID
{
    NO_MESH_MENU_ID = 500,/*!< Default value for the enum */
    ID_CREATE_MESH,/*!< Value used to indicate that the event was a create mesh event */
    ID_SHOW_MESH,/*!< Value used to indicate that the event is to toggle the display of the mesh */
    ID_DELETE_MESH/*!< Value used to indicate that the event is to delete the mesh */
};


//! Enum used in the event table to disguish that the event came from the analysis menu
/*!
    This enum is used to indicate that the event came from the analysis menu. This will
    allow the event table to properly route the event to the correct function
*/
enum AnalysisMenuID
{
    NO_ANALYSIS_MENU_ID = 600,/*!< Default value for the enum */
    ID_ANALYZE,/*!< Value used to indicate that the event was an Analyze event */
    ID_VIEW_RESULTS/*!< Value used to indicate that the event was a View Results event */
};



//! Enum used for the event proceudre on the tool bar
/*!
    This enum is specifically used on the toolbar. This allows
    a seperation between the menu bar and the tool and the program
    is able to properly route the event procedure to the correct
    function
*/
enum ToolBarID
{
    ID_TOOLBAR_NEW_FILE = 700, /*!< Value used to indicate that the user clicked on the new file icon */
    ID_TOOLBAR_SAVE_FILE,/*!< Value used to indicate that the user clicked on the save file icon */
    ID_TOOLBAR_SAVE_FILE_AS,/*!< Value used to indicate that the user clicked on the save as icon */
    ID_TOOLBAR_OPEN_FILE,/*!< Value used to indicate that the user clicked on the open file icon */
    ID_TOOLBAR_ZOOM_IN,/*!< Value used to indicate that the user clicked on the zoom in icon */
    ID_TOOLBAR_ZOOM_OUT,/*!< Value used to indicate that the user clicked on the zoom out icon */
    ID_TOOLBAR_EDIT_PROPERTY,/*!< Value used to indicate that the user clicked on the edit property icon */
    ID_TOOLBAR_MOVE,/*!< Value used to indicate that the user clicked on the move icon */
    ID_TOOLBAR_COPY,/*!< Value used to indicate that the user clicked on the copy icon */
    ID_TOOLBAR_SCALE,/*!< Value used to indicate that the user clicked on the scale icon */
    ID_TOOLBAR_DELETE,/*!< Value used to indicate that the user clicked on the delete icon */
    ID_TOGGLE_NODE,/*!< Value used to indicate that the user clicked on the draw node icon */
    ID_TOGGLE_LINE,/*!< Value used to indicate that the user clicked on the draw line icon */
    ID_TOOLBAR_CREATE_MESH,/*!< Value used to indicate that the user clicked on the create mesh icon */
    ID_TOOLBAR_SOLVE,/*!< Value used to indicate that the user clicked on the solve icon */
    ID_TOOLBAR_VIEW_RESULTS/*!< Value used to indicate that the user clicked on the view results icon */
};

//! Enum for the menus of the menu bar
/*!
    This is a general purpose enum
    used during the menu bar creation.
    This enum is primarly used
    for event proceuder routing. Normally,
    these would go in their own enum. But since
    there would be 2 additional enums with 2 items
    inside, it is more convient to place these in one
    larger enum.
*/
enum menubarID
{
    ID_NO_ID = 800, /*!< Defuault value for this eneum */
    ID_MANUAL, /*!< Value used to indicate that the user sent a manual event */
    ID_LICENSE, /*!< Value used to indicate that the user sent a license event */
    ID_PROBLEM_PREFERNCES,/*!< Value used to indicate that the user sent a problem preferences event */
    ID_SOLVE_PROBLEM/*!< Value used to indicate that the event is a solve problem event */
};



//! Enum containing the different physics problems
/*!
    This is the enum that is used to determine what physics problem
    will be simulated. This will effect the view of some menus.

    For now, the enum number must correspond to the index number in the wxArrayString object plus one.
*/
enum class physicProblems
{
    NO_PHYSICS_DEFINED,/*!< Defuault value for the enum */
    PROB_MAGNETICS,/*!< Value used to indicate that the problem is a magnetics problem */
    PROB_ELECTROSTATIC/*!< Value used to indicate that the problem is an electrostatic problem */
};


//! This enum is used to set the AC Solver that Omni-FEM will use
/*!
    This enum is only used when the user is simulating a magnetics
    problem. This enum will determine how the program will
    incorporate the circuit aspect into the simulation
*/
enum acSolverEnum
{
    SUCCAPPROX,/*!< Value used to indicate that the user would like to solve the circuit with a successive spproxamation */
    NEWTON/*!< Value used to indicate that the user would like to solve the circuit with a newton iterative process */
};


//! This enum is used to set the units that Omni-FEM will use for lengths or distances
enum unitLengthEnum
{
    INCHES,/*!< Value used to indicate that the user is working with inches as their units */
    MILLIMETERS,/*!< Value used to indicate that the user is working with millimeters as their units */
    CENTIMETERS,/*!< Value used to indicate that the user is working with centimeters as their units */
    METERS,/*!< Value used to indicate that the user is working with meters as their units */
    MILS,/*!< Value used to indicate that the user is working with mils as their units */
    MICROMETERS/*!< Value used to indicate that the user is working with micrometer as their units */
};

//! This enum is used to set what coordinate system Omni-FEM should use when solving
/*!
    In many aspects, this will not effect much of how the system will run. For
    example, this will not effect how the program draws points on the canvas.
    Although, to the user, the points will have different meanings. Canvas wise,
    the point (1,1) in cartesian is in the same location as the polar form.
    The difference will arise when the user simulates the problem
*/
enum class planarCoordinateEnum
{
    NO_COORDINATE_DEFINED = 0,/*!< Default value for the enum */
    CARTESIAN,/*!< Value used to indicate that the user is working in the cartesian plane */
    POLAR/*!< value used to indicate that the user is working with polar coordinates */
};


//! This enum is used to specify what problem type Omni-FEM needs to use when solving
enum problemTypeEnum
{
    PLANAR,/*!< Value used to indicate that the problem is defined in 2-D Euclidean geometry */
    AXISYMMETRIC/*!< Value used to indicate that the problem has cylindrical symmetry */
};



//! For the magnetics solver, this enum will inform Omni-FEM if laminations or wire should be used in solving. This sets a specific region to be laminated or wired
enum lamWireEnum
{
    NOT_LAMINATED_OR_STRANDED,/*!< Default value for the enum */
    LAMINATED_IN_PLANE,/*!< Value used to indiacte that the laminations are in the iagminary z-plane */
    LAMINATED_PARALLEL_X_OR_R_AXISYMMETRIC,/*!< Value used to indicate that the laminations are directed in the x-plane */
    LAMINATED_PARALLEL_Y_OR_Z_AXISYMMETRIC,/*!< Value used to indicate that the laminations are directed in the y-plane */
    MAGNET_WIRE,/*!< Value used to indicate that the block is magnet wire. This means that there is 1 strand */
    PLAIN_STRANDED_WIRE,/*!< Value used to indicate that the block label is stranded */
    LITZ_WIRE,/*!< Vcalue used to indicate that the block label is stranded */
    SQUARE_WIRE, /*!< Value used to indicate that the block label is squared wire */
    CCA_10,/*!< Value used to indicate that the block label is copper clad aluminium 10% wire */
    CCA_15/*!< Value used to indicate that the block label is copper clad aluminium 15% wire */
};



//! This enum will be used to designate the different Boundary conditions for the magnetic Boundary Conditions
enum bcEnumMagnetic
{
    PRESCRIBE_A, /*!< Value used that the vector potental A is prescribed along a given boundary. */
    SMALL_SKIN_DEPTH, /*!< Value used to indicate that the material is subjected to the skin effect */
    MIXED,/*!< Value used to indicate that the boundary is an open boundary or to solve for the field intensity H */
    //! An experimental BC
    STRATEGIC_DUAL_IMAGE,/*!< Value used to indicate that the boundary is an open boundary */
    PERIODIC,/*!< Value used to indicate that the boundary condition is periodic and there is some symmetry to the problem */
    ANTIPERIODIC/*!< Value used to indicate that the boundary condition is anti-periodic and there is some symmetry to the problem */
};

//! Enum that is used to differentiate the different boundary conditions for electrostatic simulation
/*!
    This enum is used to help identify which boundary condition needs to be applied
    to segments in an electrical simulation
*/
enum bcEnumElectroStatic
{
   /* These are specific for E-stat */
    FIXED_VOLTAGE, //!< Value used to indicate that the boundary condition is a fixed voltage
    SURFACE_CHARGE_DENSITY, //!< Value used to indicate that the boundary condition is a surface charge density
    E_STATIC_MIXED, //!< Value used to indicate that the boundary condition is a mixed BC
    E_STATIC_PERIODIC, //!< Value used to indicate that the boundary condition is periodic
    E_STATIC_ANTIPERIODIC//!< Value used to indicate that the boundary condition is anti-periodic
};


//! This enum is used for the properties dialog box
/*!
    This enum is sepcifically for the dialog that appears before the user
    goes in to edit the approiate list properties. The enum here
    assists in distiguishing the buttons in order to properly
    route the event procedure
*/
enum propertiesDialogEnum
{
    ID_ButtonOk,
    ID_ButtonAdd,
    ID_ButtonDelete,
    ID_ButtonModify
};

//! This enum is a general enum for frames that will include buttons or other widgets
/*!
    Some of these ID were not located in the wx_ID enum so this was created
    for general widgets to do general tasks
*/
enum generalFrameButton
{
    ID_ComboBox1,
    ID_ComboBox2,
    ID_ComboBox3,
    ID_ComboBox4,
    ID_ComboBox5,
    ID_ComboBox6,
    ID_ComboBox7,
    ID_RadioButton1,
    ID_RadioButton2,
    ID_CHECKBOX1,
    ID_BUTTON1,
    ID_BUTTON2,
    ID_BUTTON3,
    ID_TREECTRL1,
    ID_TREECTRL2,
    ID_LISTBOX
};

//! Enum for the buttons
/*!
    This enum is primarly used in the first two
    frames that the user sees. The function that
    this enum serves is to route the events to the proper function
*/
enum buttonID
{
    ID_NO_BUTTON,
    ID_buttonTwoDim,
    ID_buttonBack,
    ID_buttonNew,
    ID_buttonOpen,
    ID_buttonFinish
};

//! This enum is used during the process of creating an open boundary.
/*!
    The enum is used in order to create the approiate open boundary.
    Depending on the create open boundary, the program will create and setup the
    property list one way or another
*/
enum OpenBoundaryEdge
{
    NO_BOUNDARY_DEFINED,//!< Default value for the enum
    DIRICHLET,/*!< Value that this used to indicate that the value of the solution is given along the open boundary */
    NEUMANN/*!< Value used to indicate that the derivative of the solution is tangent to the open boundary */
};

//! Enum that is used to differentiate between which geometry has been edited
enum EditGeometry
{
    EDIT_NONE,//!< Default value for the enum
    EDIT_NODES,//!< Value used to indicate that the node list was edited
    EDIT_LINES,//!< Value used to indicate that the line list was edited
    EDIT_ARCS,//!< Value used to indicate that the arc list was edited
    EDIT_LABELS,//!< Value used to indicate that the block label list was edited
    EDIT_ALL//!< Value used to indicate that all of the geometry lists were edited
};

//! Enum that is used to determine which property to edit
/*!
    This enum will tell the program which set of geometry to scan through
    and edit which property
*/
enum EditProperty
{
    EDIT_PROPERTY_NONE,//!< Default value for the enum
    EDIT_CONDUCTOR,//!< Value used to indicate that the conductor property list was edited
    EDIT_NODAL,//!< Value used to indicate that the nodal property list was edited
    EDIT_BOUNDARY,//!< Value used to indicate that the boundary property list was edited
    EDIT_MATERIAL,//!< Value used to indicate that the material property list was edited
    EDIT_CIRCUIT//!< Value used to indicate that the circuit property list was edited
};


#endif // ENUMS_H
