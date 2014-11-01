
/*
 *   we only use (half-n) of matrix 
 *  |x|x|x|x|x|
 *  |0|x|x|x|x|
 *  |1|2|x|x|x|
 *  |3|4|5|x|x|
 *  |6|7|8|9|x|
 *
 * ((m-1)^2) - (m-1)
 *(------------------ + n + 1) = index of (m,n) vertices in main_graph array
 *        2
 */


struct _graph{
	int numofvertices;
	int numofedges;
	struct _edge edges[];
};

struct _edge{
	unsigned short len;
	unsigned short evaporation_rate;
};

static struct _graph* initialize(int numofvertices){
	struct _graph* graph = malloc(sizeof(struct _graph));
	graph->numofvertices = numofvertices;
	graph->numofedges = ZERO;
	graph->edges = calloc(numofvertices*numofvertices, sizeof(struct _edge));
	return graph;
}

static struct _edge* is_connected(struct _graph *graph , int m, int n ){
	if(m == n || m > graph->numofvertices || n > graph->numofvertices)
		g_error("is_connected:fatal error: m:%d, n:%d is not acceptable in graph->numofvertices:%d",m,n,graph->numofvertices);
	else if(n > m){
		m ^= n;
		n ^= m;
		m ^= n;
	}
	return graph->edges[(((m-1)*(m-1))/2) + n + 1];
}
