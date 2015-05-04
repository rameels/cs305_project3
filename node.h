#define INFINITY            9999 /* a number to represent infinity */
#define NUMOFNODES          3    /* number of nodes in the graph. 
								   Note that if you change this you should 
								   also change connectcosts initialization 
								   in dv_routing.c */
#define USEPOISONEDREVERSE  1    /* should we use poisoned reverse? */

/* a structure to represent the pkt being sent */ 
struct rtpkt {	
	/* id of sending router sending this pkt */	
	int sourceid;

	/* id of router to which pkt being sent 
	   (must be an immediate neighbor) */
	int destid;

	/* min cost to node i */
	int mincost[NUMOFNODES];
};

/* gives you access to current time. */
extern float clocktime;

/* This structure represents all the data needed by a router in the network */
struct node {
	
	/* the id of the current node */
	int node_number;

	/* the distance table (as descirbed in figure 4.30 in the book. 
	   Since this always has the same number of rows (instead of it 
	   being dependent on the number of neighbors), simply fill 
	   unknown rows with infinity. */
	int distance_table[NUMOFNODES][NUMOFNODES]; 

	/* the cost from this node to its neighbors. Does not change 
	   unless done during link change */
	int cost_to_neighbors[NUMOFNODES];

	/* the forwarding table. next_hop[i] should represent the next 
	   node to send a packet destined to i. */
	int next_hop[NUMOFNODES];

	/* the current shortest path to each destination. */
	int spath[NUMOFNODES];
  
};

/* This function is called once in the beginning of the program and should 
   initialize all the necessary fields. See project description for more 
   information. */ 
void node_init(int matrix_row[], int n, struct node *a_node);

/* This function is called when a node receives an update from a neighbor. 
   See project description for more information. */
void recv_update(struct node *a_node, struct rtpkt *rcvdpkt);

/* this function is called when you have scheduled a link change. See 
   project description for more information. */
void link_change_handler(struct node *a_node, int linkid, int newcost);

/* this function is provided for you and you do not need to alter it. See 
   project description for more information. */
void tolayer2(struct rtpkt packet);

/* this function executes the distance vector algorithm for the 
   given node and advertises any updates to its neighbors */
void dv_algorithm(struct node *a_node);

/* this function sends a node's distance vector to its neighbors */
void send_dv(struct node *a_node);

/* prints out the node's distance table and next hop tables */
void print_node_state(struct node *a_node);

/* prints out time and node of node_init() call */
void print_node_init(int n);

/* prints out time, destination and source of recv_update() call */
void print_recv_update(struct rtpkt *rcvdpkt);

/* prints out time, nodes and new cost of link_change_handler() call */
void print_link_change_handler(int nodenumber, int linkid, int newcost);

/* prints out source, destination and distance vector of tolayer2() call */
void print_tolayer2(struct rtpkt pkt);
