#define INFINITY           9999 /* a number to represent infinity */
#define NUMOFNODES         4    /* number of nodes in the graph. 
								   Note that if you change this you should 
								   also change connectcosts initialization 
								   in dv_routing.c */
#define USEPOISONEDREVERSE 0    /* should we use poisoned reverse? */

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
void node_init(int matrix_row[], int n, struct node * a_node);

/* This function is called when a node receives an update from a neighbor. 
   See project description for more information. */
void recv_update(struct node * a_node, struct rtpkt *rcvdpkt);

/* this function is called when you have scheduled a link change. See project 
   description for more information. */
void link_change_handler(struct node * a_node, int linkid, int newcost);

/* this function is provided for you and you do not need to alter it. See 
   project description for more information. */
void tolayer2(struct rtpkt packet);