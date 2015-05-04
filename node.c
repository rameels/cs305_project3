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
    print_node_init(n);
    a_node->node_number = n;
    for(i = 0; i < NUMOFNODES; i++) {
    	for(j = 0; j < NUMOFNODES; j++) {
    		if(i == n) {
    			a_node->distance_table[i][j] = matrix_row[j];
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
    for(i = 0; i < NUMOFNODES; i++) {
        a_node->spath[i] = matrix_row[i];
    }
    print_node_state(a_node);
    send_dv(a_node);
}

void recv_update(struct node *a_node, struct rtpkt *rcvdpkt) {
    int i;
    print_recv_update(rcvdpkt);
    for(i = 0; i < NUMOFNODES; i++) {
        a_node->distance_table[rcvdpkt->sourceid][i] =
        rcvdpkt->mincost[i];
    }
    dv_algorithm(a_node);
}

void link_change_handler(struct node *a_node, int linkid, int newcost) {
    print_link_change_handler(a_node->node_number, linkid, newcost);
    a_node->cost_to_neighbors[linkid] = newcost;
    dv_algorithm(a_node);
}

void dv_algorithm(struct node *a_node) {
    int i, j;
    int change = 0;
    int min = INFINITY;
    int new_next_hop[NUMOFNODES];
    for(i = 0; i < NUMOFNODES; i++) {
        new_next_hop[i] = a_node->next_hop[i];
    }
    for(i = 0; i < NUMOFNODES; i++) {
        min = INFINITY;
        for(j = 0; j < NUMOFNODES; j++) {
            printf("%d %d %d\n",
                   a_node->cost_to_neighbors[j],
                   a_node->distance_table[j][i],
                   min);
            if(a_node->cost_to_neighbors[j] +
               a_node->distance_table[j][i] < min) {
                min = a_node->cost_to_neighbors[j] + 
                a_node->distance_table[j][i];
                new_next_hop[i] = j;
            }
            else if(a_node->cost_to_neighbors[j] +
                    a_node->distance_table[j][i] == min &&
                    a_node->node_number != j) {
                new_next_hop[i] = j;
            }
        }
        if(min != a_node->distance_table[a_node->node_number][i]) {
            a_node->distance_table[a_node->node_number][i] = min;
            change = 1;
        }
    }
    for(i = 0; i < NUMOFNODES; i++) {
        a_node->spath[i] = 
        a_node->distance_table[a_node->node_number][i];
        a_node->next_hop[i] = new_next_hop[i];
    }
    if(change) {
        send_dv(a_node);
    }
    print_node_state(a_node);
}

void send_dv(struct node *a_node) {
    int i;
    struct rtpkt pkt;
    pkt.sourceid = a_node->node_number;
    for(i = 0; i < NUMOFNODES; i++) {
        pkt.mincost[i] = a_node->spath[i];
    }
    for(i = 0; i < NUMOFNODES; i++) {    
        pkt.destid = i;
        if(i != a_node->node_number && 
           a_node->cost_to_neighbors[i] != INFINITY) {
            print_tolayer2(pkt);
            tolayer2(pkt);
        }
    }
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
    printf("\nAt time t=%.3f, node_init() called on node %d.\n", 
           clocktime, n);
}

void print_recv_update(struct rtpkt *rcvdpkt) {
    printf("\nAt time t=%.3f, recv_update() called. ", clocktime); 
    printf("node %d receives a packet from node %d\n",
           rcvdpkt->destid, rcvdpkt->sourceid);
}

void print_link_change_handler(int nodenumber, int linkid, int newcost) {
    printf("\nAt time t=%.3f, link_change_handler() called. ", clocktime);
    printf("Link cost between node %d and node %d changes to %d.\n",
           nodenumber, linkid, newcost);
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
