#include <algorithm>
#include <map>
#include <string>
#include <iterator>
#include <limits>
#include <queue>
#include <set>
#include <iostream>
#include "Graph.h"

using std::list;
using std::map;
using std::queue;
using std::set;
using namespace std;

/**
 * Adds a node to the graph.
 * 
 * @param node: the node to add to the graph
 */
void Graph::addNode(GraphNode* node) {
    if (adj_.find(node) == adj_.end()) {
        adj_[node] = list<Connection>();
    }
}

/**
 * Adds a directed weighted edge to the graph
 * 
 * @param src: the node that the edge comes from
 * @param dest: the nde that the edge goes to
 * @param strength: the weight of the edge
 */
void Graph::addEdge(GraphNode* src, GraphNode* dest, double strength) {
    auto it = adj_.find(src);
    if (it != adj_.end()) {
        (*it).second.push_back(Connection {dest, strength});
    }
}

/**
 * Removes a node from the graph
 * 
 * @param node: the node to remove from the graph
 */
void Graph::removeNode(GraphNode* node) {
    auto it = adj_.find(node);
    if (it != adj_.end()) {
        adj_.erase(it);
    }
}

/**
 * Removes an edge from the graph
 * 
 * @param src: the node that the edge comes from
 * @param dest: the node that the edge goes to
 */
void Graph::removeEdge(GraphNode* src, GraphNode* dest) {
    if (adj_.find(src) != adj_.end()) {
        list<Connection>& edges = adj_[src]; 
        for (auto idx = edges.begin(); idx != edges.end(); idx++) {
            if ((*idx).dest == dest) {
                edges.erase(idx);
                return;
            }
        }
    }
}

/**
 * Gets the strength value between two nodes
 * 
 * @param src: the node that the edge comes from
 * @param dest: the node that the edge goes to
 * 
 * @returns the strength value between the two nodes
 */
double Graph::getEdge(GraphNode* src, GraphNode* dest) {
    if (adj_.find(src) != adj_.end()) {
        list<Connection>& edges = adj_[src]; 
        for (auto idx = edges.begin(); idx != edges.end(); idx++) {
            if ((*idx).dest == dest) {
                return (*idx).strength;
            }
        }
    }
    return -1.;
}

/**
 * Function used by dijkstra to find the id of the node that 
 * has the smallest distance value while not in the set
 * 
 * @param distances : used to check the distance of the nodes
 * @param shortest_path_tree_set :used to check if node has already been added to the set
 * 
 * @returns id of the node 
 */
int Graph::minDistance(std::map<int, double> distances, std::map<int, bool> shortest_path_tree_set)
{
    int imax = std::numeric_limits<int>::max();
    int min = imax;
    int min_index;
 
    std::map<GraphNode*, std::list<Connection>>::iterator it = adj_.begin();
    while (it != adj_.end())
    {
        int v = it->first->getId();
        if (shortest_path_tree_set[v] == false) {
            if (distances[v] <= min) {
                min_index = v;
                min = distances[v];
            }
        }
        it++;
    }
    return min_index;
}

/**
 * Gets the shortest path from source to destination using
 * Dijkstra's algorithm to account for weighted edges
 * 
 * @param source: id of the node where the path starts
 * @param destination: id of the node where the path ends
 * 
 * @returns the shortest path from source to destination
 */
vector<int> Graph::dijkstra(int source, int destination)
{
    std::map<int, double> distances;
    std::map<int, bool> shortest_path_tree_set;
    std::map<int, int> previous;
    int imax = std::numeric_limits<int>::max();

    std::map<GraphNode*, std::list<Connection>>::iterator it = adj_.begin();
    while (it != adj_.end())
    {
        int i = it->first->getId();
        distances[i] = imax; 
        shortest_path_tree_set[i] = false;
        previous[i] = -1;
        it++;
    }
 
    distances[source] = 0;
 
    for (int count = 0; count < adj_.size() - 1; count++) {
        int u = minDistance(distances, shortest_path_tree_set);
 
        shortest_path_tree_set[u] = true;

        std::map<GraphNode*, std::list<Connection>>::iterator it = adj_.begin();
        while (it != adj_.end())
        {
            int v = it->first->getId();
            bool connection_exists = false;
            double strength_result;
            GraphNode* u_graph_node = getNode(u);
            list<Connection>& edges = adj_[u_graph_node];
            //checks if there is a connection between the two nodes
            for (auto idx = edges.begin(); idx != edges.end(); idx++) {
                if ((*idx).dest->getId() == v) {
                    connection_exists = true;
                    strength_result = (*idx).strength;
                }
            }
            if (!shortest_path_tree_set[v] && connection_exists) {
                if (distances[u] != imax && distances[u] + strength_result < distances[v]) {
                    distances[v] = distances[u] + strength_result;
                    previous[v] = u;
                }
            }
            it++;
        }
        if (count % 10 == 0) {
            std::cout << count << std::endl;
        }
    }
    vector<int> path;
    getPathFromPreviousMap(previous, destination, path);
    return path;
}

/**
 * updates path to have the list of ids from the source of the shortest path
 * to the destination of the shortest path
 * 
 * @param previous: map of current nodes to their previous nodes
 * @param destination: id of node where the path ends
 * @param path: the initially empty vector that is filled by this function
 */
void Graph::getPathFromPreviousMap(std::map<int, int> previous, int destination, vector<int> & path)
{
       
    // base case - the previous of source is -1
    if (previous[destination] == -1) {
        path.push_back(destination);
        return;
    }
    
    getPathFromPreviousMap(previous, previous[destination], path);
   
    path.push_back(destination);
}

/**
 * Gets a list of the Breadth first traversal, starting from a node
 * 
 * @param start: the node that the BFS starts from
 * 
 * @returns a list containing the traversal
 */
list<GraphNode*> Graph::BFS(GraphNode* start) {
    if (adj_.find(start) == adj_.end()) {
        return list<GraphNode*>();
    }
    list<GraphNode*> bfs_order;
    queue<GraphNode*> nodes;
    nodes.push(start);

    set<GraphNode*> visited;
    visited.insert(start);
    while(!nodes.empty()) {
        bfs_order.push_back(nodes.front());
        for (Connection connection :  adj_[nodes.front()]) {
            GraphNode* neighbor = connection.dest;
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                nodes.push(neighbor);
            }
        }
        nodes.pop();
    }
    return bfs_order;
}

/**
 * Gets the adjacency list for testing purposes.
 * 
 * @returns the adjacency list
 */
map<GraphNode*, list<Graph::Connection>> Graph::getAdj() {
    return adj_;
}
/**
 * Gets the GraphNode pointer for the specified anime id.
 * 
 * @param id: the anime id to get the GraphNode pointer of
 * 
 * @returns a GraphNode pointer
 */
GraphNode* Graph::getNode(unsigned int id) {
    for (auto pair : adj_) {
        if (pair.first->getId() == id) {
            return pair.first;
        }
    }
    return NULL;
}
