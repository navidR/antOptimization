

#ifndef ANT_H
#include "ant.h"
#endif

struct _tsp_solver{
	struct _ant** ants;
	int numofants;
};

struct _tsp_solver* tsp_solver(struct _graph *graph,int numofants);
void freeing_tsp_solver(struct _tsp_solver*);
void solve_problem(struct _graph*,struct _tsp_solver*);
