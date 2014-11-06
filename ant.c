
#ifndef ANT_H
#include "ant.h"
#endif

// initiating a struct _ant 
struct _ant* initiate_ant(struct _graph *graph,int current_vertex){
#ifdef DEBUG
	g_debug("initiate_ant with current_vertex %d",current_vertex);
#endif
	struct _ant* ant = malloc(sizeof(struct _ant));
	ant->vertex_visited = calloc(graph->numofvertices,sizeof(int));
	// put a trash value to each item in visited_vertex
	for(int i = 0 ; i < graph->numofvertices;i++)
		ant->vertex_visited[i] = INT_MAX;
	ant->current_index = ZERO;
	ant->vertex_visited[ant->current_index] = current_vertex;
	ant->current_index++;
	ant->len = ZERO;
	ant->going_status = SEARCHING;
	ant->_end = false;
	return ant;
}

bool already_taking_this_vertex(struct _graph *graph,struct _ant *ant,int point_vertex){
	for(int i = 0; i < graph->numofvertices;i++)
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
	for(int i = 0; i < graph->numofvertices;i++)
		g_debug("ant->visited_vertex[%d]=%d",i,ant->vertex_visited[i]);
#endif
	if(ant->_end == true)
		g_error("make_move,never should call with ant->_end true value");
	// if current_index equal to graph->numofvertices
	// then our ant's travel is completed
	// it is time to reverse to home
	// and evaluate a point to each vertex
	if(ant->current_index == graph->numofvertices || ant->going_status == RETURNING)
	{
		ant->going_status = RETURNING;
		ant->current_index--;
		int current_vertex = ant->vertex_visited[ant->current_index];
		int next_vertex = ant->vertex_visited[ant->current_index - 1];
		inc_pheromone(graph,current_vertex,next_vertex,(graph->lenofalledges/ant->len));
		if((ant->current_index-1) == 0)
			ant->_end = true;
#ifdef DEBUG
		for(int i = 0 ; i < graph->numofvertices ; i++)
			g_debug("make_move:ant is done answer[%d]:%d",i,ant->vertex_visited[i]);
#endif

		return;
	}
	int current_vertex = ant->vertex_visited[ant->current_index - 1];
	int sumofpheromone = 0;
	for(int i = 0 ; i < graph->numofvertices;i++)
		if(i == current_vertex || already_taking_this_vertex(graph,ant,i) == true)
			continue;
		else if(is_connected(graph,current_vertex,i) != NULL)
			sumofpheromone += is_connected(graph,current_vertex,i)->pheromone_value;

#ifdef DEBUG
	g_debug("make_move:sumofpheromone:%d",sumofpheromone);
#endif
	if(sumofpheromone == ZERO)
	{
		// ant are in dead state
#ifdef DEBUG
		g_debug("make_move:and going to dead state via sumofpheromone:%d ,len:%d",sumofpheromone,ant->len);
#endif
		ant->_end = true;
		return;
	}
	int chosen_pheromone = rand() % sumofpheromone;
	int temp_pheromone = ZERO;
#ifdef DEBUG
	g_debug("make_move:ant have chosen a pheromone %d ,len:%d",chosen_pheromone,ant->len);
#endif
	for(int i = 0 ; i < graph->numofvertices;i++){
		if(i == current_vertex || already_taking_this_vertex(graph,ant,i) == true)
			continue;
		else if(is_connected(graph,current_vertex,i) != NULL){
			temp_pheromone += is_connected(graph,current_vertex,i)->pheromone_value;
			if(temp_pheromone >= chosen_pheromone){
				// now we have chosen a vertex
#ifdef DEBUG
				g_debug("make_move:ant chosen  move i:%d ,len:%d",i,ant->len);
#endif				
				ant->vertex_visited[ant->current_index] = i;
				ant->current_index++;
				ant->len += graph->edges[i]->len;
				break;
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
	free(ant);
}
