#include "ForceGraphNode.h"
#include <cmath>
#include <math.h>
#include <cmath>
#include <iostream>
#define cr (.1)
#define cs (.1)
using namespace std;
/**
* Standard constructor
* @param x _x initialization
* @param y _y initialization
* @param id _id initialization
*/
ForceGraphNode::ForceGraphNode(double x, double y, int id) {
    _x = x;
    _y = y;
    _id = id;
}
/**
*   Returns X
*   @returns x
*/
double ForceGraphNode::getX() {
    return _x;
}
/**
*   Returns Y
*   @returns y
*/
double ForceGraphNode::getY() {
    return _y;
}
/**
*   Returns Id
*   @returns id
*/
int ForceGraphNode::getId() {
    return _id;
}
/**
*   Changes the position of x and y  
*   @param x how much to change x
*   @param y how much to change y
*/
void ForceGraphNode::move(double x, double y) {
    _x += x;
    _y += y;
}
/**
*   Returns distance between the current node and the parameterized one
*   @param node2 pointer to the other node
*   @returns distance 
*/
double ForceGraphNode::getDist(ForceGraphNode* node2) {
    return sqrt(pow(node2->getX()-_x,2)+pow(node2->getY()-_y,2));
}
/**
*   Returns direction between the current node and the parameterized one
*   @param node2 pointer to the other node
*   @returns direction in radians
*/
double ForceGraphNode::getDir(ForceGraphNode* node2) {
    return atan2(node2->getY()-_y,node2->getX()-_x);
}
/**
*  Calculates the force felt on the current node by the parameterized node and adds said force to xf and yf
*  @param node2 pointer to the other node
*  @param strength_matrix matrix detailing strengths
*  @param xf cumulative force in the x direction
*  @param yf cumulative force in the y direction
*/
void ForceGraphNode::getForce(ForceGraphNode* node2,vector<vector<double>>* strength_matrix, double* xf, double* yf) {
    double f;
    f = 0;
    if(abs(_x) > 1000){
        *xf = 0;
        *yf = 0;
        return;
    }
    double l = (*strength_matrix)[_id][node2->getId()];
    if(l == -1) 
        l = 5;
    if(l > .85)
        f -= 10*(pow(getDist(node2),-1));
    if(l < .85)
        f += 2*(log(getDist(node2)/(l+.2)));
    double dir = getDir(node2);
    *xf += cos(dir)*f;
    *yf += sin(dir)*f;
}