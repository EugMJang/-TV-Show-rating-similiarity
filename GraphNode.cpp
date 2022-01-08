#include "GraphNode.h"

using std::string;

/**
 * Constructor for the GraphNode class
 * 
 * @param id: the id of the anime to store
 */
GraphNode::GraphNode(unsigned id) {
    id_ = id;
}

/**
 * Gets the id of the graph node
 * 
 * @returns the id of the anime stored in the graph node
 */
unsigned GraphNode::getId() {
    return id_;
}