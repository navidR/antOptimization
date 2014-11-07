#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include "ant.h"
#include <gtk/gtk.h>

struct _tsp_solver{
	struct _ant** ants;
	int numofants;
	int length_param;
	int numofcycle;
	bool *condition;
};

struct _tsp_solver* init_tsp_solver(struct _graph *,int ,int, bool*);
void freeing_tsp_solver(struct _tsp_solver*);
int solve_tsp(struct _graph*,struct _tsp_solver*,double,GtkProgressBar*);

#endif
