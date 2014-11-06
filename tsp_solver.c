
#include "tsp_solver.h"

struct _tsp_solver* init_tsp_solver(struct _graph *graph,int numofants){
	// not complete implementation
	struct _tsp_solver *tsp_solver = malloc(sizeof(struct _tsp_solver));
	tsp_solver->ants = calloc(numofants,sizeof(struct _ants*));
	tsp_solver->numofants = numofants;
	for(int i = 0 ; i < tsp_solver->numofants;i++)
		tsp_solver->ants[i] = initiate_ant(graph,rand() % graph->numofvertices);
	return tsp_solver;
}


void freeing_tsp_solver(struct _tsp_solver *tsp_solver){
	// not complete implementation
	for(int i = 0 ; i < tsp_solver->numofants;i++)
		freeing_ant(tsp_solver->ants[i]);
	free(tsp_solver->ants);
	free(tsp_solver);
	tsp_solver = NULL;
}

/*
 * should call move ant , until all ant done via one of the 
 * two following situation :
 *   -> an ant have done an travel via all vertices 
 *   -> or an ant become dead_end
 * after all ant is done , we have our solution
 * this will return a array of ints with (numofvertices) element
 * which is answer for the problem
 */
int* solve_problem(struct _graph *graph,struct _tsp_solver *tsp_solver){
	// not implemented yet
	
	// return answer 
	// return ant[i]->vertex_visited;
}
