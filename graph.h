#ifndef GRAPH_H
#define GRAPH_H

#include <glib.h>
#include <stdlib.h>
#include <stdbool.h>



// defines for graph
#define MIN_PHEROMONE 0
#define MAX_PHEROMONE 10
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
	double pheromone_value;
	int len;
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
void evaporate(struct _graph*,double);
void clear(struct _graph *graph);
void inc_pheromone(struct _graph *, int , int , double);
#endif
