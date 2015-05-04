/* Rameel Sethi
   CS 305
   Project 3
   Implementing the Distance Vector Algorithm */

#include <stdio.h>
#include <stdlib.h>
#include "node.h"

float clocktime;

void node_init(int matrix_row[], int n, struct node *a_node) {
    int i, j;

    /* let user know that node_init() called */
    print_node_init(n);

    /* initialize node number */
    a_node->node_number = n;

    /* for each row in distance table */
    for(i = 0; i < NUMOFNODES; i++) {
        /* and for each column */
    	for(j = 0; j < NUMOFNODES; j++) {
            /* initialize node's distance vector */
    		if(i == n) {
    			a_node->distance_table[i][j] = matrix_row[j];
    		}

            /* initialize distances of nodes to themselves to zero */ 
    		else if(i == j) {
    			a_node->distance_table[i][j] = 0;
    		}

            /* initialize all other distances to infinity */
    		else {
    			a_node->distance_table[i][j] = INFINITY;
    		} 
        }

        /* initialize neighbor costs from matrix row*/
        a_node->cost_to_neighbors[i] = matrix_row[i];
        
        /* node's next hop to itself is itself */
        if(i == n) {
        	a_node->next_hop[i] = n;
        }

        /*otherwise, start with next hop as node 0 by default */
        else {
        	a_node->next_hop[i] = 0;
        }
    }

    /* initialize shortest path costs from matrix row */
    for(i = 0; i < NUMOFNODES; i++) {
        a_node->spath[i] = matrix_row[i];
    }

    /* print current node state */
    print_node_state(a_node);

    /* send distance vector to neighbors */
    send_dv(a_node);
}

void recv_update(struct node *a_node, struct rtpkt *rcvdpkt) {
    int i;
    
    /* let user know that recv_update() called */
    print_recv_update(rcvdpkt);
    
    /* update distance table with received distance vector */
    for(i = 0; i < NUMOFNODES; i++) {
        a_node->distance_table[rcvdpkt->sourceid][i] =
        rcvdpkt->mincost[i];
    }

    /* update node's distance vector */
    dv_algorithm(a_node);
}

void link_change_handler(struct node *a_node, int linkid, int newcost) {
    /* let user know that link_change_handler() called */
    print_link_change_handler(a_node->node_number, linkid, newcost);
    
    /* update neighbor cost with new cost */
    a_node->cost_to_neighbors[linkid] = newcost;
    
    /* update node's distance vector */
    dv_algorithm(a_node);
}

void dv_algorithm(struct node *a_node) {
    int i, j;

    /* has the distance vector changed? */
    int change = 0;

    /* current minimum distance to a node over all neighbors */
    int min = INFINITY;

    /* newly calculated next hops for forwarding table  */
    int new_next_hop[NUMOFNODES];

    /* copy existing forwarding table */
    for(i = 0; i < NUMOFNODES; i++) {
        new_next_hop[i] = a_node->next_hop[i];
    }

    /* for each destination node */
    for(i = 0; i < NUMOFNODES; i++) {
        /* begin with infinite minimum */
        min = INFINITY;

        /* skip if destination same as node, it's already 0 */
        if(a_node->node_number == i) {
            continue;
        }

        /* for all neighbors of node */
        for(j = 0; j < NUMOFNODES; j++) {
            /* if neighbor is same as current node
               or not a neighbor at all, skip */
            if(a_node->cost_to_neighbors[j] == INFINITY ||
               a_node->cost_to_neighbors[j] == 0) {
                continue;
            }

            /* if cost to neighbor plus distance of node
               to destination less than distance of this
               node to destination */
            if(a_node->cost_to_neighbors[j] +
               a_node->distance_table[j][i] < min) {
                /* update minimum */
                min = a_node->cost_to_neighbors[j] + 
                a_node->distance_table[j][i];

                /* new next hop is neighbor which gives new
                   minimum */
                new_next_hop[i] = j;
            }

            /* special case: if some other neighbor achieves
               equal minimum and is not the same as current
               node */
            else if(a_node->cost_to_neighbors[j] +
                    a_node->distance_table[j][i] == min &&
                    a_node->node_number != j) {
                /* new next hop is this neighbor */
                new_next_hop[i] = j;
            }
        }

        /* if minimum different from distance table entry */
        if(min != a_node->distance_table[a_node->node_number][i]) {
            /* update distance vector */
            a_node->distance_table[a_node->node_number][i] = min;
            
            /* a change in distance vector has occured */
            change = 1;
        }
    }

    /* for each destination */
    for(i = 0; i < NUMOFNODES; i++) {
        /* update cost of shortest path */
        a_node->spath[i] = 
        a_node->distance_table[a_node->node_number][i];

        /* update next hop */
        a_node->next_hop[i] = new_next_hop[i];
    }

    /* if distance vector has changed */
    if(change) {
        /* send distance vector to neighbors */
        send_dv(a_node);
    }

    /* print current node state */
    print_node_state(a_node);
}

void send_dv(struct node *a_node) {
    int i, j;

    /* link-layer packet to be sent */
    struct rtpkt pkt;

    /* source is current node */
    pkt.sourceid = a_node->node_number;

    /* for all destinations */
    for(i = 0; i < NUMOFNODES; i++) {    
        /* destination is number of that node */
        pkt.destid = i;

        /* for all shortest paths */
        for(j = 0; j < NUMOFNODES; j++) {
            /* if using poisoned reverse and current node is
               routing through destination to get to some node */ 
            if(USEPOISONEDREVERSE && a_node->next_hop[j] == i) {
                /* advertise current node distance to that
                   node as infinity */
                pkt.mincost[j] = INFINITY;
            } else {
                /* otherwise simply copy shortest path cost */
                pkt.mincost[j] = a_node->spath[j];
            } 
        }

        /* if destination not same as current node and actually
           a neighbor */
        if(i != a_node->node_number && 
           a_node->cost_to_neighbors[i] != INFINITY) {
            /* let user know that packet is being sent */
            print_tolayer2(pkt);

            /* send packet */
            tolayer2(pkt);
        }
    }
}

void print_node_state(struct node *a_node) {
    int i, j;
    
    /* print distance and forwarding tables in 
       pretty and easily readable format */
    for(j = 0; j < 5 * ((NUMOFNODES / 2) + 1) - 1; j++) {
        printf(" ");
    }
    printf("To\n");
    printf(" D%d |", a_node->node_number);
    for(j = 0; j < NUMOFNODES; j++) {
        printf(" %4d", j);
    }
    printf("\n");
    for(j = 0; j < 4; j++) {
        printf("-");
    }
    printf("|");
    for(j = 0; j < 5 * NUMOFNODES; j++) {
        printf("-");
    }
    printf("\n");
    for(i = 0; i < NUMOFNODES; i++) {
        printf("%4d|", i);
        for(j = 0; j < NUMOFNODES; j++) {
            printf(" %4d", a_node->distance_table[i][j]);
        }
        printf("\n");
    }
    printf("next hop table:\n");
    printf(" D%d |", a_node->node_number);
    for(j = 0; j < NUMOFNODES; j++) {
        printf(" %4d", j);
    }
    printf("\n");
    for(j = 0; j < 4; j++) {
        printf("-");
    }
    printf("|");
    for(j = 0; j < 5 * NUMOFNODES; j++) {
        printf("-");
    }
    printf("\n");
    for(j = 0; j < 4; j++) {
        printf(" ");
    }
    printf("|");
    for(j = 0; j < NUMOFNODES; j++) {
        printf(" %4d", a_node->next_hop[j]);
        
    }
    printf("\n");
}

void print_node_init(int n) {
    /* print time and node */
    printf("\nAt time t=%.3f, node_init() called on node %d.\n", 
           clocktime, n);
}

void print_recv_update(struct rtpkt *rcvdpkt) {
    /* print time. destination and source */
    printf("\nAt time t=%.3f, recv_update() called. ", clocktime); 
    printf("node %d receives a packet from node %d\n",
           rcvdpkt->destid, rcvdpkt->sourceid);
}

void print_link_change_handler(int nodenumber, int linkid, int newcost) {
    /* print time, nodes sharing link and new cost */
    printf("\nAt time t=%.3f, link_change_handler() called. ", clocktime);
    printf("Link cost between node %d and node %d changes to %d.\n",
           nodenumber, linkid, newcost);
}

void print_tolayer2(struct rtpkt pkt) {
    int i;
    
    /* print time, source, destination and 
       distance vector */ 
    printf("At time t=%.3f, node %d sends packet to ",
           clocktime, pkt.sourceid);
    printf("node %d with: ", pkt.destid);
    for(i = 0; i < NUMOFNODES; i++) {
        printf("%d ", pkt.mincost[i]);
    }
    printf("\n");
}
