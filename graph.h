
// defines for graph
#define MIN_PHEROMONE 0
#define MAX_PHEROMONE 100
#define ZERO 0

struct _graph;
struct _edge;

struct _graph* initialize(int);
struct _edge* is_connected(struct _graph*, int, int);
void free_graph(struct _graph *);
void connect_edge(struct _graph*,int,int,struct _edge*);
struct _edge* create_edge(int, int );
static int index(int,int);

