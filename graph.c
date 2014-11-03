/*
 *   we only use (half-n) of matrix 
 *  n 0 1 2 3 4
 * m |x|x|x|x|x|
 * 1 |0|x|x|x|x|
 * 2 |1|2|x|x|x|
 * 3 |3|4|5|x|x|
 * 4 |6|7|8|9|x|
 *
 *  ((m)^2) - (m)
 *(--------------- + n) = index of (m,n) vertices in main_graph array
 *        2
 * m is | 
 * n is -
 */
#include "graph.h"

// handful function for turning (m,n) scheme to index scheme
// we can optimize this func via dynamic programming technique
// update : there is no for optimization,performance of this program dominate by cairo's draw function
static int index(int m, int n){
	if(m == n)
		g_error("index:fatal error: m:%d, n:%d graph cant have loop",m,n);
	else if(n > m){
		m ^= n;
		n ^= m;
		m ^= n;
	}
#ifdef DEBUG
	// only for debugging
	int temp = ((((m*m)-m)/2)+n);
	g_debug("index:for(%d,%d)->%d",m,n,temp);
	return temp; 
#else
	return ((((m*m)-m)/2)+n);
#endif
}

struct _edge* create_edge(int len, int pheromone_value){
#ifdef DEBUG
	g_debug("create_edge:len:%d",len);
#endif	
	if(pheromone_value > MAX_PHEROMONE)
		g_error("create_edge:phermone value is not acceptable %d",pheromone_value);
	struct _edge* edge = malloc(sizeof(struct _edge));
	edge->len = len;
	edge->pheromone_value = pheromone_value;
	edge->selected = false;
	return edge;
} 

void select_edge(struct _graph *graph, int m, int n, bool p_selected){
	(graph->edges[index(m,n)])->selected = p_selected;
}

void inc_pheromone(struct _graph *graph, int m, int n){
	(graph->edges[index(m,n)])->pheromone_value++;
}

struct _graph* initialize(int numofvertices){
	g_debug("initialize:numofvertices %d",numofvertices);
	struct _graph* graph = malloc(sizeof(struct _graph));
	graph->numofvertices = numofvertices;
	graph->numofedges = ZERO;
	// we need array size of 
	//    n^2 + n
	//   ---------- = len of array we need
	//       2
	// serious bug
	graph->len = ((numofvertices * numofvertices) + numofvertices)/2;
#ifdef DEBUG
	g_debug("initialize:graph->len is %d",graph->len);
#endif
	graph->edges = calloc(graph->len, sizeof(struct _edge*));
	return graph;
}

// freeing every associated resource to specific graph
// first surfing into every element , and freeing it
// then freeing array of struct _edges*
// and at the end freeing struct _graph itself
void free_graph(struct _graph *graph){
	g_debug("free_graph:graph->len:%d",graph->len);
	for(int i = 0 ; i < graph->len;i++)
		if(graph->edges[i] != NULL)
			free(graph->edges[i]);
#ifdef DEBUG
	g_debug("free_graph:after freeing elements one by one");
#endif
	free(graph->edges);
	graph->edges = NULL;
	free(graph);
	graph = NULL;
	return;
}
// just send two index for this function
// carefull if edge is not seted , this func just return's null
struct _edge* is_connected(struct _graph* graph , int m, int n){
	g_debug("is_connected");
	if(m > graph->numofvertices || n > graph->numofvertices)
		g_error("is_connected:fatal error: m:%d, n:%d is not acceptable in graph->numofvertices:%d", m, n, graph->numofvertices);
#ifdef DEBUG
	g_debug("is_connected:graph->len:%d",graph->len);
	struct _edge* edge = graph->edges[index(m,n)];
	if(edge == NULL)
		g_debug("is_connected:returning edge,edge is null");
	else
		g_debug("is_connected:returning edge,edge->len:%d",edge->len);
	return edge;
#else
	return graph->edges[index(m,n)];
#endif
}

bool is_selected(struct _graph* graph, int m, int n){
	if(m > graph->numofvertices || n > graph->numofvertices)
		g_error("is_connected:fatal error: m:%d, n:%d is not acceptable in graph->numofvertices:%d", m, n, graph->numofvertices);
	return (graph->edges[index(m,n)])->selected;
}

// if you want disconnect two element
// just send edge as null
void connect_edge(struct _graph* graph, int m, int n, struct _edge* edge)
{
#ifdef DEBUG
	g_debug("connect_edge:graph->numofvertices:%d,m:%d,n:%d,edge->len:%d", graph->numofvertices, m, n, edge->len);
#endif
	if(m > graph->numofvertices || n > graph->numofvertices)
		g_error("connect_edge:fatal error: m:%d, n:%d is not acceptable in graph->numofvertices:%d", m, n, graph->numofvertices);
	int indx = index(m,n);
	if(!graph->edges[indx])
		free(graph->edges[indx]);
	else
		graph->numofedges++;
	graph->edges[indx] = edge;
}

