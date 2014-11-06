#ifndef GRAPH_H
#define GRAPH_H

#include <glib.h>
#include <stdlib.h>
#include <stdbool.h>



// defines for graph
#define MIN_PHEROMONE 1
#define MAX_PHEROMONE 10000
#define ZERO 0

// graph should not have loop in any circumstance
struct _graph{
	int numofvertices;
	int numofedges;
	int len; // length of array
	int lenofalledges;
	struct _edge** edges;
};

struct _edge{
	int len;
	int pheromone_value;
	bool selected;
};

struct _graph* initialize(int);
struct _edge* is_connected(struct _graph*, int, int);
void free_graph(struct _graph *);
void connect_edge(struct _graph*,int,int,struct _edge*);
struct _edge* create_edge(int);
static int index(int,int);
void select_edge(struct _graph*,int,int,bool);
bool is_selected(struct _graph *, int, int);
void evaporate(struct _graph*,int);
void unselect(struct _graph *graph);

#endif
