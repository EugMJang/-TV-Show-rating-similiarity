#include <stdlib.h>     
#include <time.h>  
#include <fstream>
#include <cmath>     
#include <iostream>
#include "ForceGraph.h"

using namespace std;

/**
 *   Function to upload the data from the strength matrix into a force graph
 *   @param strength_matrix 2d vector of strength between each node  
*/
void ForceGraph::uploadGraph(vector<vector<double>>* strength_matrix) {
    int its;
    srand (time(NULL));
    int ct = 0;
    vector<vector<bool>> list;
    list.resize(100);
    for(int i = 0; i < 100; i++) {
        list[i].resize(100,false);
    }
    int x;
    int y;
    int length = strength_matrix->size();
    for(int i = 0; i < length; i++) {
        //initialize each node to a random location
        do{
        x = rand() % 75;
        y = rand() % 75;
        }while(list[x][y]);
        list[x][y] = true;
        ForceGraphNode* node = new ForceGraphNode((x-50)*10, (y-50)*10, i);
        nodes.push_back(node);
    } 
    cout<< "How many iterations should the Force Directed Graph run?"<<endl;
    cout<< "Best looking graphs require >100, but <10 is good for tests"<<endl;
    cin >> its;
        char filename [18];
        double xf;
        double yf;
        for(int i = 0; i < its; i++) {
        for(auto it3 = nodes.begin(); it3 != nodes.end(); ++it3){
            //Progress Report
            if(ct % 100 == 0)
                std::cout<<"Iterating Force Graph: ("<<ct<<"/"<<its*length<<")"<<std::endl;
            ct++;
            //move each node based on the force with a timestep of .01
            xf = 0;
            yf = 0;
            for(int j = 0; j < 10; j++) {
            for(auto it2 = nodes.begin(); it2 != nodes.end(); ++it2){
                if(it2 != it3)
                    (*it3)->getForce(*it2, strength_matrix, &xf,&yf);
            }
            (*it3)->move(.01*xf,.01*yf);
            }
        
        }        
        }

}
/**
* Color version of the node drawing, tailored specifically for the data from the large set
* @param png the png that is being drawn over
* @param x x coordinate of the center of the node
* @param y y coordinate of the center of the node
* @param st the relative strength of the node compared to every other node
*/
void ForceGraph::drawNodeC(cs225::PNG* png, int x, int y, double st){
    if(st < 0) {
        return;
    }
    double str = 2-(st)/1200.;
    cs225::HSLAPixel col(str*360.,1,0.5,str);
    for(int j = -str*20+10; j < str*20-11; j++){
    for(int i = -str*20+10; i < str*20-11; i++) {
        if(x+i < 1005 && y-j >= 0 &&  y-j < 1005 && y-j >= 0) {
        cs225::HSLAPixel & p1 = png->getPixel(x+i, y-j);
        addPixel(&p1,&col);
        }
    }
    }
}
/**
* Standard version of the node drawing
* @param png the png that is being drawn over
* @param x x coordinate of the center of the node
* @param y y coordinate of the center of the node
*/
void ForceGraph::drawNode(cs225::PNG* png, int x, int y){
    cs225::HSLAPixel col(0,1,0.5,1);
    for(int i = -1; i < 2; i++) {
        if(x+i < 1005 && x+i >= 0 &&  y+2 < 1005 && y+2 >= 0) {
        cs225::HSLAPixel & p1 = png->getPixel(x+i, y+2);
        addPixel(&p1,&col);
        }
    }
    for(int j = -1; j < 2; j++){
    for(int i = -2; i < 3; i++) {
        if(x+i < 1005 && x+i >= 0 &&  y-j < 1005 && y-j >= 0) {
        cs225::HSLAPixel & p1 = png->getPixel(x+i, y-j);
        addPixel(&p1,&col);
        }
    }
    }
    for(int i = -1; i < 2; i++) {
        if(x+i < 1005 && x+i >= 0 &&  y-2 < 1005 && y-2 >= 0) {
        cs225::HSLAPixel & p1 = png->getPixel(x+i, y-2);
        addPixel(&p1,&col);
        }
    }    
}
/**
* Function to draw an edge between two nodes
* @param png the png that is being drawn over
* @param x1 x coordinate of the center of the first node
* @param y1 y coordinate of the center of the first node
* @param x2 x coordinate of the center of the second node
* @param y2 y coordinate of the center of the second node
* @param str strength of the edge
*/
void ForceGraph::drawEdge(cs225::PNG* png, int x1, int y1,int x2, int y2, double str) {
    cs225::HSLAPixel col(0,0,str/2.,(1-str));
    double xd = x2-x1;
    double yd = y2-y1;
    double slope;
    double xc, yc;
    if(abs(xd) > abs(yd)){
    if (xd < 0) {
        yd = y1-y2;
        xd *= -1;
        xc = x2;
        yc = y2;
        slope = yd/xd;
        while(xc < x1) {
            xc++;
            yc+=slope; 
            cs225::HSLAPixel & p1 = png->getPixel(int(xc), int(yc));
            addPixel(&p1,&col);
        }
        return;
    }
    if (xd > 0) {
        yd = y2-y1;
        xc = x1;
        yc = y1;
        slope = yd/xd;
        while(xc < x2) {
            xc++;
            yc+=slope; 
            cs225::HSLAPixel & p1 = png->getPixel(int(xc), int(yc));
            addPixel(&p1,&col);
        }
        return;
    }
    }
    if (yd < 0) {
        xd = x1-x2;
        yd *= -1;
        yc = y2;
        xc = x2;
        slope = xd/yd;
        while(yc < y1) {
            yc++;
            xc+=slope; 
            cs225::HSLAPixel & p1 = png->getPixel(int(xc), int(yc));
            addPixel(&p1,&col);
        }
        return;
    }
    if (yd > 0) {
        xd = x2-x1;
        yc = y1;
        xc = x1;
        slope = xd/yd;
        while(yc < y2) {
            yc++;
            xc+=slope; 
            cs225::HSLAPixel & p1 = png->getPixel(int(xc), int(yc));
            addPixel(&p1,&col);
        }
        return;
    }
}
/** 
*   Function for adding two pixels
*   @param p1 pointer to the bottom pixel 
*   @param p2 pointer to the top pixel
*/
void ForceGraph::addPixel(cs225::HSLAPixel* p1, cs225::HSLAPixel* p2) {
    double alpha = p1->a+p2->a;
    double c = ((p1->h*(p1->a)+p2->h*(p2->a)))/alpha;
    if(c > 360) {
        c -= 360;
    }
    if(p1->h == 0) {
        p1->h = p2->h;
    } else if(p2->h != 0) {
        p1->h = c;
    }
    p1->l = (p1->l*(p1->a)+p2->l*(p2->a))/alpha;
    p1->s = (p1->s*(p1->a)+p2->s*(p2->a))/alpha;
    p1->a = 1;
}
/**
* Function for printing the graph
* @param strength_matrix matrix of strengths of connections between each node
* @returns pointer to the png created
*/
cs225::PNG* ForceGraph::print(vector<vector<double>>* strength_matrix){
    cs225::PNG* png = new cs225::PNG(1005, 1005);
    vector<ForceGraphNode*> graphnodes;
    vector<double> stotal;
    int leng = strength_matrix->size();
    stotal.resize(leng,0);
    for(int i = 0; i < leng-1; i++) {
        for(int j = i+1; j < leng; j++) {
            stotal[j] += (*strength_matrix)[i][j];
            stotal[i] += (*strength_matrix)[i][j];
        }
    }
    int x,y;
    double xsum = 0;
    double ysum = 0;
    int count = 0;
    for(auto it2 = nodes.begin(); it2 != nodes.end(); ++it2) {
        if (abs((*it2)->getX()) < 1000 && abs((*it2)->getY()) < 1000){
           count++;
           xsum += (*it2)->getX();
           ysum += (*it2)->getY();
        }
    }
    x = int(xsum/count);
    y = int(ysum/count);
    for(auto it2 = nodes.begin(); it2 != nodes.end(); ++it2) {
        if(abs((*it2)->getX()-x) < (500) && abs((*it2)->getY()-y) < (500)){
            drawNode(png, int(((*it2)->getX()-x)+505), int(((*it2)->getY()-y)+505));
            for(auto it = graphnodes.begin(); it != graphnodes.end(); ++it) {
                if((*strength_matrix)[(*it2)->getId()][(*it)->getId()] < .85 && (*strength_matrix)[(*it2)->getId()][(*it)->getId()] > -.1){
                    drawEdge(png, int(((*it2)->getX()-x))+505,int(((*it2)->getY()-y))+505,int(((*it)->getX()-x))+505,int(((*it)->getY()-y))+505, (*strength_matrix)[(*it2)->getId()][(*it)->getId()]);
                }
            }
            graphnodes.push_back((*it2));
        }
    }
    return png;
}
/**
* Function for printing the graph in color
* @param strength_matrix matrix of strengths of connections between each node
* @returns pointer to the png created
*/
cs225::PNG* ForceGraph::printc(vector<vector<double>>* strength_matrix){
    png = new cs225::PNG(1005, 1005);
    vector<ForceGraphNode*> graphnodes;
    vector<double> stotal;
    int leng = strength_matrix->size();
    stotal.resize(leng,0);
    for(int i = 0; i < leng-1; i++) {
        for(int j = i+1; j < leng; j++) {
            stotal[j] += (*strength_matrix)[i][j];
            stotal[i] += (*strength_matrix)[i][j];
        }
    }
    int x,y;
    double xsum = 0;
    double ysum = 0;
    int count = 0;
    for(auto it2 = nodes.begin(); it2 != nodes.end(); ++it2) {
        if (abs((*it2)->getX()) < 1000 && abs((*it2)->getY()) < 1000){
           count++;
           xsum += (*it2)->getX();
           ysum += (*it2)->getY();
        }
    }
    x = int(xsum/count);
    y = int(ysum/count);
    cs225::HSLAPixel nodecol(0,1,0.5,.8);
    cs225::HSLAPixel edgecol(0,1,0.5,1);
    for(auto it2 = nodes.begin(); it2 != nodes.end(); ++it2) {
        if(abs((*it2)->getX()-x) < (500) && abs((*it2)->getY()-y) < (500)){
            drawNodeC(png, int(((*it2)->getX()-x)+505), int(((*it2)->getY()-y)+505),stotal[(*it2)->getId()]);
            for(auto it = graphnodes.begin(); it != graphnodes.end(); ++it) {
                if((*strength_matrix)[(*it2)->getId()][(*it)->getId()] < .85 && (*strength_matrix)[(*it2)->getId()][(*it)->getId()] > -.1){
                    drawEdge(png, int(((*it2)->getX()-x))+505,int(((*it2)->getY()-y))+505,int(((*it)->getX()-x))+505,int(((*it)->getY()-y))+505, (*strength_matrix)[(*it2)->getId()][(*it)->getId()]);
                }
            }
            graphnodes.push_back((*it2));
        }
    }
    return png;
}
/**
* Destructor
*/
ForceGraph::~ForceGraph() {
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
        delete *it;
    delete png;
}