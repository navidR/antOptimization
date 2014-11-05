
#include "tsp_solver.h"

struct _tsp_solver* tsp_solver(struct _graph *graph,int numofants){
	struct _tsp_solver *tsp_solver = malloc(sizeof(struct _tsp_solver));
	tsp_solver->ants = calloc(numofants,sizeof(struct _ants));
	tsp_solver->numofants = numofants;
	for(int i = 0 ; i < tsp_solver->numofants;i++)
		tsp_solver->ants[i] = initiate_ant(graph,rand() % graph->numofvertices);
	return tsp_solver;
}


void freeing_tsp_solver(struct _tsp_solver *tsp_solver){
	for(int i = 0 ; i < tsp_solver->numofants;i++)
		freeing_ant(tsp_solver->ants[i]);
	free(tsp_solver->ants);
	free(tsp_solver);
	tsp_solver = NULL;
}


void solve_problem(struct _graph *graph,struct _tsp_solver *tsp_solver){
	// not implemented yet
	// should call move ant , until a ant came with solution
}