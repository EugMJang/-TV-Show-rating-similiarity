#pragma once
#include "ForceGraphNode.h"
#include "Graph.h"
#include "cs225/PNG.h"
#include <vector>
using namespace std;
class ForceGraph {
    public:
    void uploadGraph(vector<vector<double>>* strength_matrix);
    cs225::PNG* print(vector<vector<double>>* strength_matrix);
    cs225::PNG* printc(vector<vector<double>>* strength_matrix);
    ~ForceGraph();
    private:
    cs225::PNG* png;
    void addPixel(cs225::HSLAPixel* p1, cs225::HSLAPixel* p2);
    void drawNode(cs225::PNG*, int, int);
    void drawNodeC(cs225::PNG*, int, int,double);
    void drawEdge(cs225::PNG*, int, int,int,int,double);
    std::vector<ForceGraphNode*> nodes;
};