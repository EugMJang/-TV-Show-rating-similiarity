#include "reader.h"
#include "ForceGraph.h"
#include "cs225/PNG.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <list>
using namespace std;

int main()
{    
    
    char ans;
    cout << "WARNING: This program calculates the Graph from the raw data, running a multitude of functions with runtime O(n^3) over thousands of nodes"<<endl;
    cout << "Typical runtime on most computers averages 20+ hours"<<endl;
    cout<< "Type 'Y' to use precalculated data that runs much faster (Y/N)"<<endl;
    cin >> ans;
    bool n = true;
    if(ans != 'Y')
        n = false;
    Reader reader;
    Graph graph;
    reader.fill_name_map("Data/id_map.csv");
    reader.fill_user_ratings("Data/data.csv");
    reader.upload_data(&graph, n);
    ForceGraph fg;
    fg.uploadGraph(&reader.strength_matrix);
    fg.printc(&reader.strength_matrix)->writeToFile("ForceGraph"+ string(".png"));
    
    cout<< "Type the id of the anime you wish to run BFS on: ";
    int id;
    cin >> id;
    list<GraphNode*> bfs = graph.BFS(graph.getNode(id));
    for (GraphNode* node : bfs) {
        std::cout << reader.getName(node->getId()) << " | ";
    }
    cout << endl;

    cout << "Type the id of the anime you wish to start Dijkstra's: ";
    int start;
    cin >> start;
    cout << "Type the id of the anime you wish to finish Dijkstra's: ";
    int finish;
    cin >> finish;
    vector<int> dijkstras = graph.dijkstra(start, finish);
    for (int id : dijkstras) {
        std::cout << reader.getName(id) << " -> ";
    }
}
