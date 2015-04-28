/* Rameel Sethi
   CS 305
   Project 3
   Implementing the Distance Vector Algorithm */

#include <stdio.h>
#include <stdlib.h>
#include "node.h"

float clocktime;

void node_init(int matrix_row[], int n, struct node *a_node) {
    print_node_init(n);
    int i, j;
    a_node->node_number = n;
    for(i = 0; i < NUMOFNODES; i++) {
    	for(j = 0; j < NUMOFNODES; j++) {
    		if(i == n) {
    			a_node->distance_table[i][j] = matrix_row[j];
    			a_node->spath[j] = matrix_row[j];
    		}
    		else if(i == j) {
    			a_node->distance_table[i][j] = 0;
    		}
    		else {
    			a_node->distance_table[i][j] = INFINITY;
    		}
        }
        a_node->cost_to_neighbors[i] = matrix_row[i];
        if(i == n) {
        	a_node->next_hop[i] = n;
        }
        else {
        	a_node->next_hop[i] = 0;
        }
    }
    print_node_state(a_node);
    struct rtpkt pkt;
    pkt.sourceid = n;
    for(i = 0; i < NUMOFNODES; i++) {
        pkt.mincost[i] = matrix_row[i];
    }
    for(i = 0; i < NUMOFNODES; i++) {    
        pkt.destid = i;
        if(i != n && a_node->cost_to_neighbors[i] != INFINITY) {
            print_tolayer2(pkt);
            tolayer2(pkt);
        }
    }
}

void recv_update(struct node *a_node, struct rtpkt *rcvdpkt) {

}

void link_change_handler(struct node *a_node, int linkid, int newcost) {

}

void print_node_state(struct node *a_node) {
    int i, j;
    for(j = 0; j < 5 * ((NUMOFNODES / 2) + 1) - 1; j++) {
        printf(" ");
    }
    printf("To\n");
    printf(" D%d |", a_node->node_number);
    for(j = 0; j < NUMOFNODES; j++) {
        printf(" %4d", j);
    }
    printf("\n");
    for(j = 0; j < NUMOFNODES; j++) {
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
    for(j = 0; j < NUMOFNODES; j++) {
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
    printf("\nAt time t=%.3f, node_init() called on node %d.\n", 
           clocktime, n);
}

void print_recv_update(struct node *a_node) {

}

void print_link_change_handler(struct node *a_node) {

}

void print_tolayer2(struct rtpkt pkt) {
    int i;
    printf("At time t=%.3f, node %d sends packet to ",
           clocktime, pkt.sourceid);
    printf("node %d with: ", pkt.destid);
    for(i = 0; i < NUMOFNODES; i++) {
        printf("%d ", pkt.mincost[i]);
    }
    printf("\n");
}
