#include "reader.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <utility>
#include <string>
#include <stdexcept>
#include <algorithm>

using namespace std;

/**
 * Initializes map that maps anime id to anime name and notes valid anime ids.
 * 
 * @param file_name: the file name to initialize the ids to name from
 */
void Reader::fill_name_map(string file_name) {
    fstream fin(file_name, ios::in);

    vector<string> row;
    string line, word, temp;
    while (getline(fin, line)) {
  
        row.clear();
  
  
        stringstream s(line);

        char c = ',';
        while (getline(s, word, c)) {
  
            row.push_back(word);
        }
        int anime_index = stoi(row[0]);
        id_search_map.push_back(anime_index);
        name_search_map.insert(pair<int, string>(anime_index, row[1]));
    }
    fin.close();
    strength_matrix.resize(id_search_map.size());
    for(int i = 0; i < id_search_map.size(); i++) {
        strength_matrix[i].resize(id_search_map.size());
    }
}
/**
* Function to upload the ratings for each node, grouped by the user
*
* @param file_name file name of a csv containing every user's rating 
*/
void Reader::fill_user_ratings(string file_name) {
    fstream fin(file_name, ios::in);
    
    vector<string> row;
    string line, word, temp;

    int user;
    bool initial = true;
    map<int, int> anime_ratings_map;
    
    while (getline(fin, line)) {
  
        row.clear();
  
        stringstream s(line);

        char c = ',';
        
        while (getline(s, word, c)) {
            row.push_back(word);
        }
        int user_csv = stoi(row[0]);
        if (initial) {
            user = user_csv;
            initial = false;
        }
        if (!initial && user != user_csv) {
            user_ratings_vec.push_back(anime_ratings_map);
            anime_ratings_map.clear();
            user = user_csv;
        }
        int anime_index = stoi(row[1]);
        int rating = stoi(row[2]);
  
        anime_ratings_map.insert(pair<int, int>(anime_index, rating));
    }
    user_ratings_vec.push_back(anime_ratings_map);
    fin.close();
}

/**
 * Creates graph from uploaded csv files, or uploads graph directly from binary file if true.
 * 
 * @param graph: graph to upload data into
 * @param upl: Creates graph from uploaded csv files if false, uploads graph directly from binary file if true
 */
void Reader::upload_data(Graph* graph, bool upl) {
    ofstream outfile ("test.txt");
    GraphNode* currNode;
    GraphNode* currNode2;
    double stren;
    int length = id_search_map.size();
    for(int i = 0; i < length; i++) {
        currNode = new GraphNode(id_search_map[i]);
        node_map.push_back(currNode);
        graph->addNode(currNode);
    }
    if(upl) 
        strength_upload();
    else
        strength_create();
    double str; 
    int count = 0;
    for(int i = 0; i < length-1; i++) {
        currNode = node_map[i];
        for(int j = i+1; j < length; j++) {
            str = strength_matrix[i][j];
            if(str < .85) {
                currNode2 = node_map[j];
                graph->addEdge(currNode, currNode2, str);
                graph->addEdge(currNode2, currNode, str);
            }
            }
            count++;
            cout<<"Graph Edge Implementation: ("<< count<<"/"<<length-1<<")"<<endl;
        }
}
/**
* Function to calculate the strength matrix by calculating the average difference over each user
*/
void Reader::strength_create() {
    int length = id_search_map.size();
    double stren;
    int count = 0;
    int max = (length*length)/2 - length;
    cout<<"Strength Matrix Seeding: ("<< count<<"/"<<max<<")"<<endl;
    for (int i = 0; i < length-1; i++){ 
    for(int j = i+1; j < length; j++) {
                stren = strength(id_search_map[i],id_search_map[j]);
                strength_matrix[i][j] = stren;
                strength_matrix[j][i] = stren;
        }
        count++;
        }
    cout<<"Strength Matrix Seeding: ("<< count<<"/"<<max<<")"<<endl;
}

/**
 * Destructor for reader class.
 */
Reader::~Reader() {
    for(auto it = node_map.begin(); it != node_map.end(); ++it)
        delete *it;
}
/**
* Function to read the strength matrix off of the .bin file
*/
void Reader::strength_upload() {
    FILE *fp;
    fp = fopen("Data/strmatrix.bin","rb");    
    strength_matrix.resize(1001);
    for(int i = 0; i < 1001; i++) {
        strength_matrix[i].resize(1001);
        strength_matrix[i][i] = 0;
    }
    for(int i = 0; i < 1000; i++) {
        for(int j = i+1; j < 1001; j++) {
            fread(&strength_matrix[i][j], 8, 1, fp);
            strength_matrix[j][i] = strength_matrix[i][j];
        }
    }
}

/**
 * Gets average rating difference for two different anime spanning across all users.
 * 
 * @param first_anime: id of first anime to compare
 * @param second_anime: id of second anime to compare
 * 
 * @returns the average rating difference for the two different anime. -1 if no similar ratings
 */
double Reader::strength(int first_anime, int second_anime) {
    double total_valid_users = 0;
    double total_differences = 0;
    size_t vec_size = user_ratings_vec.size();
    for (size_t i = 0; i < vec_size; i++) {
        map<int, int> anime_ratings_map = user_ratings_vec.at(i);
        auto it1 = anime_ratings_map.find(first_anime);
        auto it2 = anime_ratings_map.find(second_anime);
        if (it1 != anime_ratings_map.end() 
        && it2 != anime_ratings_map.end()) {
            total_valid_users += 1;
            total_differences += abs(it1->second-it2->second);
        }
    }
    if(total_valid_users == 0) {
        return 999;
    }
    double result = total_differences / total_valid_users;
    return result;
}

/**
 * Gets the name of the anime given the id
 * 
 * @param anime: the id of the requested anime
 * 
 * @returns the name of the requested anime
 */
string Reader::getName(int anime) {
    return (string) name_search_map[anime];
}