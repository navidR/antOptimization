#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include "ant.h"

struct _tsp_solver{
	struct _ant** ants;
	int numofants;
};

struct _tsp_solver* init_tsp_solver(struct _graph *graph,int numofants);
void freeing_tsp_solver(struct _tsp_solver*);
int* solve_tsp(struct _graph*,struct _tsp_solver*);

#endif
