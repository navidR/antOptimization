#ifndef ANT_H
#define ANT_H

#include <stdbool.h>
#include "graph.h"


struct _ant* initiate_ant(struct _graph*,int);
void make_move(struct _graph*,struct _ant*);
void freeing_ant(struct _ant*);
bool already_taking_this_vertex(struct _graph *,struct _ant *,int );

typedef enum { SEARCHING, RETURNING } _going;

struct _ant{
	int current_index;
	int *vertex_visited; // I prefer simple array to stack
	int len;
	_going going_status;
	bool _end;
};

// vertex_visited index zero is started vertex
// we can have a pointer to graph in every ant
// but I dont like that way

// if an ant is true == _end && current_index != ZERO
// it is in dead end
// if an ant is true == _end && current_index == ZERO
// then it is completed travel

#endif
