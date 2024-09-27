#!/bin/bash

# This script is used to simulate a run of the client.
# Connect to 127.0.0.1 on port 9034
{
    # creating the graph
    echo "Newgraph 10,21"

    # creating the MST first
    echo "1 2"
    echo "3"

    echo "2 3"
    echo "2"

    echo "3 4"
    echo "2"

    echo "3 6"
    echo "8"

    echo "6 5"
    echo "8"

    echo "6 7"
    echo "7"

    echo "7 8"
    echo "4"

    echo "8 9"
    echo "1"

    echo "9 10"
    echo "3"

    # creating the rest of the edges
    echo "1 4"
    echo "6"

    echo "2 4"
    echo "4"

    echo "1 5"
    echo "9"

    echo "2 5"
    echo "9"

    echo "2 6"
    echo "9"

    echo "3 7"
    echo "9"

    echo "4 7"
    echo "9"

    echo "5 10"
    echo "18"

    echo "6 10"
    echo "10"

    echo "6 9"
    echo "9"

    echo "7 9"
    echo "5"

    echo "8 10"
    echo "4"

    # creating another edge
    echo "Newedge 1,3"
    echo "10"

    # deleting that edge
    echo "Removeedge 1,3"

    # doing all the MST algorithms

    # Prim's
    echo "p"

    # Tarjan's
    echo "t"

    # Kruskal's
    echo "k"

    # Boruvka's
    echo "b"

    # IntegerMST's
    echo "i"

    # Create some data about the MST

    # print the graph
    echo "printGraph"

    # print the weights
    echo "printWeight"

    # Print the maximum distance between any two vertices
    echo "maxDistance"

    # Print the average distance between any two vertices
    echo "avgDistance"

    # Print the shortest path between vertex s and vertex t
    echo "shortestPath 1 5"


} | nc 127.0.0.1 9034