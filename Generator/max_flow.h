// C++ implementation of Dinic's Algorithm
#pragma once

//#include<bits/stdc++.h>
#include <vector>
#include <list>
#include <climits>
#include <iostream>
 
// A structure to represent a edge between
// two vertex
struct Edge
{
    int v ;  // Vertex v (or "to" vertex)
             // of a directed edge u-v. "From"
             // vertex u can be obtained using
             // index in adjacent array.
 
    int flow ; // flow of data in edge
 
    int C;    // capacity
 
    int rev ; // To store index of reverse
              // edge in adjacency list so that
              // we can quickly find it.
};
 
// Residual Network
class Network
{
    int V; // number of vertex
    std::vector<int> level ; // stores level of a node
    std::vector<std::vector<Edge> > adj;
public :
    Network(int V)
    {
        adj = std::vector<std::vector<Edge> >(V,std::vector<Edge>());
        this->V = V;
        level = std::vector<int>(V);
    }
 
    // add edge to the Network
    void addEdge(int u, int v, int C)
    {
        // Forward edge : 0 flow and C capacity
        Edge a{v, 0, C, adj[v].size()};
 
        // Back edge : 0 flow and 0 capacity
        Edge b{u, 0, 0, adj[u].size()};
 
        adj[u].push_back(a);
        adj[v].push_back(b); // reverse edge
    }

    void removeEdge(int u, int v)
    {
        for (size_t i = 0; i < adj[u].size(); i++)
            if (adj[u][i].v == v)
                adj[u].erase(adj[u].begin() + i);

        for (size_t i = 0; i < adj[v].size(); i++)
            if (adj[v][i].v == u)
                adj[v].erase(adj[v].begin() + i);
    }
 
    bool BFS(int s, int t);
    int sendFlow(int s, int flow, int t, int ptr[]);
    int DinicMaxflow(int s, int t);
};

