#pragma once

#include <string>

class GraphNode {
    public:
        GraphNode(unsigned id);
        unsigned getId();
    private:
        unsigned id_;
};