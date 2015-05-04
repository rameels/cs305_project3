Rameel Sethi
CS 305
Project 3
Implementing the Distance Vector Algorithm

NOTE: See three_nodes_link_changes_poisoned_reverse.txt. 
	  The time saved by poisoned reverse is 44.558 units.

HOW TO RUN:

1.  Unzip the folder:
	$ unzip cs305_project3_rameel_sethi

2.  Enter the newly created directory:
	$ cd cs305_project3_rameel_sethi

3.  Make sure the files dv_routing.c, node.c, node.h and input.txt are present.

4.  If you want to change the simulation input, enter exactly one quantity 
on the first line of input.txt, the tracing level. Enter 0 to turn off tracing 
(recommended since I have printf statements to show what is going on during 
program execution), 1 for basic info, 2 for extended info and 3 or higher for 
emulator debugging info.

5.  In node.h:
    (1) Set NUMOFNODES to the desired number of nodes.
    (2) Set USEPOISONEDREVERSE to 1 if using poisoned reverse and 0 if not.

6.  In dv_routing.c:
	(1) Set LINKCHANGES to the desired number of link changes to occur.
	(2) Two network topologies are given: four nodes without link changes,
	    and three nodes with link changes. Comment out whichever one is not
	    being used (or come up with your own!).

7.  Compile the C program: 
	$ gcc -o dv_routing dv_routing.c node.c

8.  Delete any previous output files if using same name for output file 
	(e.g. four_nodes_no_link_changes.txt, 
	three_nodes_link_changes_no_poisoned_reverse.txt,
	three_nodes_link_changes_poisoned_reverse.txt)

9.  Run the program, getting input from input.txt and dumping output to a file (say output.txt):
	$ ./dv_routing < input.txt > output.txt

10. Inspect the newly created output.txt for the program output.
