#pragma once

#include "GraphNode.h"

#include <map>
#include <list>

using namespace std;

class Graph {
    public:
        /**
         *
         * Contains a connection used in the adj map.
         * 
         * @param dest: The destination node of the connection.
         * @param strength: The strength value of the connection.
         **/
        struct Connection {
            GraphNode* dest;
            double strength;
        };
        
        void addNode(GraphNode* node);
        void addEdge(GraphNode* src, GraphNode* dest, double strength);
        void removeNode(GraphNode* node);
        void removeEdge(GraphNode* src, GraphNode* dest);
        double getEdge(GraphNode* src, GraphNode* dest);
        vector<int> shortest_path(vector<GraphNode*> g, int s, vector<int>& path);
        int minDistance(std::map<int, double> distances, std::map<int, bool> shortest_path_tree_set);
        vector<int> dijkstra(int source, int destination);
        void getPathFromPreviousMap(std::map<int, int> previous, int destination, vector<int> & path);
        std::map<GraphNode*, std::list<Connection>> getAdj();
        std::list<GraphNode*> BFS(GraphNode* start);
        GraphNode* getNode(unsigned int id);
    private:
        std::map<GraphNode*, std::list<Connection>> adj_;
};