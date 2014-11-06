#ifndef UI_LOGIC_H
#define UI_LOGIC_H

// Defined Variable for program
#define ZERO 0
#define GRAPH_WIN_CONTAINER_BORDER_WIDTH 5
#define GRAPH_WIN_WIDTH 1000
#define GRAPH_WIN_HEIGHT 600
#define DRAWING_AREA_WIDTH 1000
#define DRAWING_AREA_HEIGHT 600
#define MAIN_WIN_WIDTH 400
#define MAIN_WIN_HEIGHT 600
#define RESIZABLE FALSE
#define RADIUS 5
#define FIRST_ANGLE 0
#define SECOND_ANGLE 8
#define RGB_SURFACE 1
#define RESERVED_SIZE_OF_ARRAY_POINTS 20
#define DISTANCE 25
#define DISTANCE_FROM_BORDER 50
#define DISTANCE_CLICK 10
#define RED 0.99
#define MODE_INDEX 0
#define INPUT_NUMOFVERTICES_INDEX 1
#define INPUT_NUMOFEDGES_INDEX 2
#define BUTTON_GENERATING_RANDOM_GRAPH_INDEX 3
#define GRAPHWIN_INDEX 4
#define DRAWING_AREA_INDEX 5
#define MAINWIN_INDEX 6
#define INPUT_EVAPORATION_RATE_INDEX 7
#define MIN_NUMOFANTS 1
#define MAX_NUMOFANTS 500
#define MIN_NUMOFVERTICES 3
#define MAX_NUMOFVERTICES 500
#define DEFAULT_NUMOFEDGES 3
#define MIN_EVAPORATION_RATE 1
#define MAX_EVAPORATION_RATE 100
#define INCREMENT_RATE 1
#define LINE_WIDTH_UNSELECTED 0.75
#define LINE_WIDTH_SELECTED 5
#define CLIMB_RATE 1
#define LENGTH_TOOLTIP_TEXT 1000
#define NUM_SHOW_LENGTH_VIA_TOOLTIP 100
#define MIN_PHEROMONE 0
#define MAX_PHEROMONE 100


// used EVENT
const char *EVENT_DESTROY = "destroy";
const char *EVENT_CONFIGURE_EVENT = "configure-event";
const char *EVENT_SHOW = "show";
const char *EVENT_CLICKED = "clicked";
const char *EVENT_DRAW = "draw";
const char *EVENT_DELETE_EVENT = "delete-event";
const char *EVENT_TOGGLED = "toggled";
const char *EVENT_BUTTON_PRESS_EVENT = "button-press-event";
const char *EVENT_VALUE_CHANGED = "value-changed";
const char *EVENT_RESPONSE = "response";
const char *EVENT_QUERY_TOOLTIP = "query-tooltip";

// global strings
const char *mainwin_ui_name = "mainwindow.glade";
const char *mainwin_title = "Main Window";
const char *graphwin_title = "Graph Window";

// global string , for loading ui file objects
const char *mainwin_ui_frame_name = "frame_ui";
const char *input_evaporation_rate_name = "input_evaporation_rate";
const char *input_numofants_name = "input_numofants";
const char *input_numofvertices_name = "input_numofvertices";
const char *input_numofedges_name = "input_numofedges";
const char *button_drawing_mode_name = "button_drawing_mode";
const char *button_solve_problem_name = "button_solve_problem";
const char *button_generating_random_graph_name =
	"button_generating_random_graph";
const char *dialog_title = "Number of Vertices";
const char *dialog_first_button_text = "Ok";

// global variable
static cairo_surface_t *surface = NULL;
static struct _graph *graph;

typedef enum { DRAWING_MODE, RANDOM_MODE } _mode;

// bag for keeping two item of _points
struct _bag {
	GdkPoint *first_item;
	GdkPoint *second_item;
	int first_item_index;
} bag;

// array of _points for tracking points
static GArray *ui_points;

// main value's for graph's
static int numofvertices = MIN_NUMOFVERTICES;
static int numofedges = MIN_NUMOFVERTICES ;
static int numofants = MIN_NUMOFANTS;
static int evaporation_rate = MIN_EVAPORATION_RATE;
static char *tooltip_text;
#endif
