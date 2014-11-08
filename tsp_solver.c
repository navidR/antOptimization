
#include "tsp_solver.h"

struct _tsp_solver* init_tsp_solver(struct _graph *graph,int numofants,int numofcycle,bool *condition){
	// not complete implementation
	struct _tsp_solver *tsp_solver = malloc(sizeof(struct _tsp_solver));
	tsp_solver->ants = calloc(numofants,sizeof(struct _ants*));
	tsp_solver->numofants = numofants;
	tsp_solver->numofcycle = numofcycle;
	tsp_solver->condition = condition;
	tsp_solver->length_param = graph->lenofalledges;
	for(int i = 0 ; i < tsp_solver->numofants;i++)
		tsp_solver->ants[i] = initiate_ant(graph,rand() % graph->numofvertices,tsp_solver->condition);
	return tsp_solver;
}


void freeing_tsp_solver(struct _tsp_solver *tsp_solver){
	// not complete implementation
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
int solve_tsp(struct _graph *graph,struct _tsp_solver *tsp_solver, double eva_value,GtkProgressBar *progressbar){

	// not implemented yet
	// return answer 
	// return ant[i]->vertex_visited;
	// condition is for ui responsivnes
	// and nothing to do with internal calculation
	double percent = 0;
	for(int try = 0;try < tsp_solver->numofcycle && (*(tsp_solver->condition) != false);try++){
		for(int i = 0 ; i < tsp_solver->numofants ; i++){
			if(tsp_solver->ants[i]->_end == true)
				travel_again(tsp_solver->ants[i]);
		}
		while(*(tsp_solver->condition)){
			bool _continue = false;
			for(int i = 0 ; i < tsp_solver->numofants && (*tsp_solver->condition != false);i++){
				while (gtk_events_pending())
					gtk_main_iteration();
				if(tsp_solver->ants[i]->_end == false){	
					make_move(graph,tsp_solver->ants[i]);
					_continue = true;
				}
			}
			percent += ((1/(double)graph->numofvertices) / (double) tsp_solver->numofcycle);
			gtk_progress_bar_set_fraction(progressbar,(double)percent);
			// evaporate
			evaporate(graph,eva_value);
			if(_continue == false){
				break;
			}
		}
		percent = try/(double)tsp_solver->numofcycle;
		gtk_progress_bar_set_fraction(progressbar,(double)percent);
		while (gtk_events_pending())
			gtk_main_iteration();
	}
	if(!(*(tsp_solver->condition)))
		return;
	gtk_progress_bar_set_fraction(progressbar,1);
	int min_len = INT_MAX;
	int answer_index = INT_MAX;
	for(int i = 0 ; i < tsp_solver->numofants;i++){
		if(tsp_solver->ants[i]->_end == true && tsp_solver->ants[i]->current_index == ZERO){
			if(min_len > tsp_solver->ants[i]->len){
				min_len = tsp_solver->ants[i]->len;
				answer_index = i;
			}
		}
	}
	int *answers = tsp_solver->ants[answer_index]->vertex_visited;
//#ifdef DEBUG
//		for(int i = 0; i < graph->numofvertices ; i++)
//			g_debug("solve_tsp:answer[%d]:%d",i,answers[i]);
//#endif
	for(int i = 0; i < graph->numofvertices;i++){
		select_edge(graph,answers[i],answers[i+1],true);
	}
	return tsp_solver->ants[answer_index]->len;
}
