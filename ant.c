#ifndef ANT_H
#include "ant.h"
#endif

// initiating a struct _ant 
struct _ant* initiate_ant(struct _graph *graph,int current_vertex,bool *condition){
#ifdef DEBUG
	g_debug("initiate_ant with current_vertex %d",current_vertex);
#endif
	struct _ant* ant = malloc(sizeof(struct _ant));
	ant->lenofarray = graph->numofvertices + 1;
	ant->vertex_visited = malloc( ant->lenofarray * sizeof(int));
	// put a trash value to each item in visited_vertex
	for(int i = 0 ; i < ant->lenofarray;i++)
		ant->vertex_visited[i] = INT_MAX;
	ant->current_index = ZERO;
	ant->vertex_visited[ant->current_index] = current_vertex;
	ant->current_index++;
	ant->condition = condition;
	ant->len = ZERO;
	ant->going_status = SEARCHING;
	ant->_end = false;
	return ant;
}

bool already_taking_this_vertex(struct _ant *ant,int point_vertex){
	for(int i = 0; i < ant->current_index && (*ant->condition) != false;i++)
		if(ant->vertex_visited[i] == point_vertex)
			return true;
	return false;
}

/*
 * check if ant complete its travel or not
 * if it does just return one by one and increase pheromone level
 * if its not then 
 * should check all available vertices
 * get all edges connected to current_index 
 * via is_connected
 * and choose one of them 
 * by calculating : sum of all pheromone value
 * picking random number [0,sumofpheromone)
 * picking which edge own it
 * if no vertices are available 
 * then should go in _end state
 */
void make_move(struct _graph* graph,struct _ant* ant){
#ifdef DEBUG
	g_debug("make_move with ant->current_index:%d,ant->len:%d",ant->current_index,ant->len);
	for(int i = 0; i < ant->lenofarray;i++)
		g_debug("ant->visited_vertex[%d]=%d",i,ant->vertex_visited[i]);
#endif
	if(ant->_end == true)
		g_error("make_move,never should call with ant->_end true value");
	// if current_index equal to graph->numofvertices
	// then our ant's travel is completed
	// it is time to reverse to home
	// and evaluate a point to each vertex
	if(ant->going_status == RETURNING)
	{
		//ant->len += is_connected(graph,ant->vertex_visited[ant->current_index],ant->vertex_visited[ZERO])->len;
		ant->current_index--;
		int current_vertex = ant->vertex_visited[ant->current_index];
		int next_vertex = ant->vertex_visited[ant->current_index - 1];
#ifdef DEBUG
		for(int i = 0 ; i < ant->lenofarray ; i++)
			g_debug("make_move:ant is done answer[%d]:%d",i,ant->vertex_visited[i]);
#endif
		g_debug("inc_pheromone via (graph->lenofalledges/ant->len):%f and graph->lenofalledges:%d ant->len:%d",(((double)graph->lenofalledges/ant->len)* (double) graph->numofvertices),graph->lenofalledges,ant->len);
		inc_pheromone(graph,current_vertex,next_vertex,((double)graph->lenofalledges/ant->len) * (double)graph->numofvertices);
		if((ant->current_index-1) == ZERO){
			// we are done with this ant
			ant->current_index = ZERO;
			ant->_end = true;
		}
		return;
	}
	else if(ant->current_index == (ant->lenofarray - 1)){
		int current_vertex = ant->vertex_visited[ant->current_index - 1];
		// if we are in last vertex
		if(is_connected(graph,current_vertex,ant->vertex_visited[ZERO])){
			ant->current_index++;
			ant->vertex_visited[ant->current_index - 1] = ant->vertex_visited[ZERO];
			ant->len += is_connected(graph,current_vertex,ant->vertex_visited[ZERO])->len;
			ant->going_status = RETURNING;
			return;
		}
	}else{
		int current_vertex = ant->vertex_visited[ant->current_index - 1];
		double sumofpheromone = 0;
		for(int i = 0 ; i < graph->numofvertices && (*ant->condition) != false ;i++){
			if(i == current_vertex || already_taking_this_vertex(ant,i) == true)
				continue;
			else if(is_connected(graph,current_vertex,i) != NULL)
				sumofpheromone += ((double)is_connected(graph,current_vertex,i)->pheromone_value + 1);
		}
		double rand_num = rand();
		double chosen_pheromone = (double) fmod(fabs(rand_num),fabs(sumofpheromone));
		double temp_pheromone = ZERO;
		g_debug("\n\n\n\n\n\nmake_move:rand_num:%f,chosen_pheromone:%f,len:%d",rand_num,chosen_pheromone,ant->len);
		for(int i = 0 ; i < graph->numofvertices && (*ant->condition) != false;i++){
			while (gtk_events_pending())
				gtk_main_iteration();
			if(i == current_vertex || already_taking_this_vertex(ant,i) == true)
				continue;
			else if(is_connected(graph,current_vertex,i) != NULL){
				temp_pheromone += ((double)is_connected(graph,current_vertex,i)->pheromone_value + 1);
				if(temp_pheromone >= chosen_pheromone){
					// now we have chosen a vertex
#ifdef DEBUG
					g_debug("make_move:ant:i:%d,len:%d,vertex->len:%d,vertex->pheromone_value:%f,temp_pheromone:%f",i,ant->len,is_connected(graph,current_vertex,i)->len,(double)is_connected(graph,current_vertex,i)->pheromone_value,(double)temp_pheromone);
#endif				
					ant->vertex_visited[ant->current_index] = i;
					ant->current_index++;
					ant->len += is_connected(graph,current_vertex,i)->len;
					break;
				}
			}
		}
	}
}

// freeing struct _ant data structure 
// associated resources
void freeing_ant(struct _ant* ant){
#ifdef DEBUG
	g_debug("freeing_ant");
#endif
	// non-complete implementation
	free(ant->vertex_visited);
	g_debug("free(ant->vertex_visited)");
	free(ant);
	g_debug("free(ant)");
}
