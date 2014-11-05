#include <stdbool.h>
#include "graph.h"


struct _ant* initiate_ant(struct _graph*,int);
void make_move(struct _graph*,struct _ant*);
void freeing_ant(struct _ant*);

struct _ant{
	int current_vertex;
	int *vertex_visited; // I prefer simple array to stack
	bool dead_end;
};

// vertex_visited index zero is started vertex
// we can have a pointer to graph in every ant
// but I dont like that way


