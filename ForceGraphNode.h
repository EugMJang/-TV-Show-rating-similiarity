#pragma once
#include "GraphNode.h"
#include "Graph.h"
#include <vector>
class ForceGraphNode {
    private:
        double _x;
        double _y;
        int _id;
    public:
        ForceGraphNode(double, double, int);
        double getX();
        double getY();
        void move(double, double);
        int getId();
        double getDist(ForceGraphNode*);
        double getDir(ForceGraphNode*);
        void getForce(ForceGraphNode*,std::vector<std::vector<double>>* strength_matrix,double*,double*);
};