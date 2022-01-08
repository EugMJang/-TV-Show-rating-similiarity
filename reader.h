#pragma once

#include <iostream>
#include <iterator>
#include <map>
#include <vector>
#include <string>
#include "Graph.h"
#include "GraphNode.h"

using namespace std;

class Reader {
public:
	void fill_name_map(string file_name);
    void fill_user_ratings(string file_name);
    void upload_data(Graph*,bool);
    double strength(int first_anime, int second_anime);
    string getName(int);
    vector<vector<double>> strength_matrix;
    ~Reader();
private:
    map<int, string> name_search_map;
    vector<int> id_search_map;
    vector<map<int, int>> user_ratings_vec;
    vector<GraphNode*> node_map;
    
    void strength_upload();
    void strength_create();
};
