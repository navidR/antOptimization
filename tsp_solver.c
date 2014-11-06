
#include "tsp_solver.h"

struct _tsp_solver* init_tsp_solver(struct _graph *graph,int numofants){
	// not complete implementation
#ifdef DEBUG
	g_debug("init_tsp_solver:numofants:%d and graph->numofvertices:%d,graph->lenofalledges:%d",numofants,graph->numofvertices,graph->lenofalledges);
#endif
	struct _tsp_solver *tsp_solver = malloc(sizeof(struct _tsp_solver));
	tsp_solver->ants = calloc(numofants,sizeof(struct _ants*));
	tsp_solver->numofants = numofants;
	tsp_solver->length_param = graph->lenofalledges;
	for(int i = 0 ; i < tsp_solver->numofants;i++)
		tsp_solver->ants[i] = initiate_ant(graph,rand() % graph->numofvertices);
	return tsp_solver;
}


void freeing_tsp_solver(struct _tsp_solver *tsp_solver){
	// not complete implementation
#ifdef DEBUG
	g_debug("freeing_tsp_solver");
#endif
	for(int i = 0 ; i < tsp_solver->numofants;i++)
		freeing_ant(tsp_solver->ants[i]);
	free(tsp_solver->ants);
	free(tsp_solver);
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
void solve_tsp(struct _graph *graph,struct _tsp_solver *tsp_solver, int eva_value){
#ifdef DEBUG
	g_debug("solve_tsp with tsp_solver->numofants:%d,tsp_solver->length_param:%d",tsp_solver->numofants,tsp_solver->length_param);
#endif

	// not implemented yet
	// return answer 
	// return ant[i]->vertex_visited;
	for(int try = 0 ; try < 1000;try++){
		while(true){
			bool _continue = false;
			for(int i = 0 ; i < tsp_solver->numofants;i++){
				if(tsp_solver->ants[i]->_end == false){
					make_move(graph,tsp_solver->ants[i]);
					_continue = true;
				}
			}
			// evaporate 
			evaporate(graph,eva_value);
			if(_continue == false)
				break;
		}
	}
	int min_len = INT_MAX;
	int answer_index = 0;
	for(int i = 0 ; i < tsp_solver->numofants;i++){
		if(tsp_solver->ants[i] == true && tsp_solver->ants[i]->current_index == ZERO){
			if(min_len > tsp_solver->ants[i]->len){
				min_len = tsp_solver->ants[i]->len;
				answer_index = i;
			}
		}
	}
	int *answers = tsp_solver->ants[answer_index]->vertex_visited;
#ifdef DEBUG
		for(int i = 0; i < graph->numofvertices ; i++)
			g_debug("solve_tsp:answer[%d]:%d",i,answers[i]);
#endif
	for(int i = 0; i < (graph->numofvertices - 1);i++){
		select_edge(graph,answers[i],answers[i+1],true);
	}
	return ;
}
