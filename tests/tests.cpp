#include "catch/catch.hpp"
#include "../Graph.h"
#include "../GraphNode.h"
#include "../ForceGraph.h"
#include "../ForceGraphNode.h"
#include "../reader.h"

#include <list>

using namespace std;

// Checks that graphNode is added correctly
TEST_CASE("Test Graph addNode") {
    Graph g;
    GraphNode* one = new GraphNode(20);
    g.addNode(one);

    auto list = g.getAdj().at(one);
    REQUIRE(list.size() == 0);
}

// Checks that edge is properly added between two nodes with proper strength
TEST_CASE("Test Graph addEdge") {
    Graph g;
    GraphNode* one = new GraphNode(4);
    g.addNode(one);
    GraphNode* two = new GraphNode(2);
    g.addNode(two);
    GraphNode* three = new GraphNode(1);
    g.addNode(three);

    g.addEdge(one, two, 2);
    SECTION("addEdge works, one node") {
        auto list = g.getAdj().at(one);
        REQUIRE(list.size() == 1);
        REQUIRE(list.front().dest == two);
        REQUIRE(list.front().strength == 2);
    }

    g.addEdge(one, three, 5);
    SECTION("addEdge works, two nodes") {
        auto list = g.getAdj().at(one);
        REQUIRE(list.size() == 2);
        REQUIRE(list.front().dest == two);
        REQUIRE(list.front().strength == 2);

        list.pop_front();
        REQUIRE(list.front().dest == three);
        REQUIRE(list.front().strength == 5);
    }
}

// Checks that removeNode works and doesn't give errors when missing
TEST_CASE("Test Graph removeNode") {
    Graph g;
    GraphNode* one = new GraphNode(4);
    g.addNode(one);
    GraphNode* two = new GraphNode(2);
    g.addNode(two);
    GraphNode* three = new GraphNode(1);
    g.addNode(three);

    SECTION("removeNode works on node") {
        g.removeNode(one);
        REQUIRE_THROWS(g.getAdj().at(one));
    }
    SECTION("removeNode doesn't give exception if node not found") {
        REQUIRE_NOTHROW(g.removeNode(one));
    }
}

// Checks that removeEdge works and doesn't give errors when missing
TEST_CASE("Test Graph removeEdge") {
    Graph g;
    GraphNode* one = new GraphNode(5);
    GraphNode* two = new GraphNode(2);
    GraphNode* three = new GraphNode(444);
    g.addNode(one);
    g.addNode(two);
    g.addNode(three);
    g.addEdge(one, two, 44);
    g.removeEdge(one, two);
    SECTION("removeEdge works with two nodes") {
        REQUIRE(g.getAdj().at(one).size() == 0);
    }
    SECTION("removeEdge doesn't give error with no edge") {
        REQUIRE_NOTHROW(g.removeEdge(one, two));
    }
    g.addEdge(one, two, 22);
    g.addEdge(one, three, 33);
    g.removeEdge(one, two);
    SECTION("removeEdge works with three nodes") {
        REQUIRE(g.getAdj().at(one).size() == 1);
        REQUIRE(g.getAdj().at(one).front().dest == three);
        REQUIRE(g.getAdj().at(one).front().strength == 33);
    }
}

// Test that BFS works and is in the right order
TEST_CASE("Test Graph BFS") {
    Graph g;
    GraphNode* one = new GraphNode(5);
    g.addNode(one);
    GraphNode* two = new GraphNode(2);
    g.addNode(two);
    GraphNode* three = new GraphNode(444);
    g.addNode(three);
    GraphNode* four = new GraphNode(34);
    g.addNode(four);
    GraphNode* five = new GraphNode(4);
    g.addNode(five);

    SECTION("Test BFS straightforward") {
        g.addEdge(one, two, 2);
        g.addEdge(two, three, 1);
        g.addEdge(three, four, 4);
        g.addEdge(four, five, 5);
        vector<GraphNode*> answer {one, two, three, four, five};
        list<GraphNode*> BFS = g.BFS(one);
        int index = 0;
        REQUIRE(BFS.size() == 5);
        for (GraphNode* n : BFS) {
            REQUIRE(n == answer[index]);
            index++;
        }
    }

    SECTION("Test BFS with repeating nodes") {
        g.addEdge(one, two, 2);
        g.addEdge(two, three, 1);
        g.addEdge(three, two, 2);
        g.addEdge(three, four, 4);
        g.addEdge(four, three, 1);
        g.addEdge(four, five, 5);
        g.addEdge(five, one, 11);
        vector<GraphNode*> answer {one, two, three, four, five};
        list<GraphNode*> BFS = g.BFS(one);
        int index = 0;
        REQUIRE(BFS.size() == 5);
        for (GraphNode* n : BFS) {
            REQUIRE(n == answer[index]);
            index++;
        }
    }
    SECTION("Test BFS with multiple neighbors") {
        g.addEdge(one, two, 2);
        g.addEdge(two, five, 2);
        g.addEdge(one, four, 1);
        g.addEdge(four, three, 1);
        vector<GraphNode*> answer {one, two, four, five, three};
        list<GraphNode*> BFS = g.BFS(one);
        int index = 0;
        REQUIRE(BFS.size() == 5);
        for (GraphNode* n : BFS) {
            REQUIRE(n == answer[index]);
            index++;
        }
    }
}

TEST_CASE("Test Dijkstra") {
    Graph g;
    GraphNode* one = new GraphNode(5);
    g.addNode(one);
    GraphNode* two = new GraphNode(2);
    g.addNode(two);
    GraphNode* three = new GraphNode(444);
    g.addNode(three);
    GraphNode* four = new GraphNode(34);
    g.addNode(four);
    GraphNode* five = new GraphNode(6);
    g.addNode(five);

    g.addEdge(one, two, 2);
    g.addEdge(two, three, 1);
    g.addEdge(three, four, 4);
    g.addEdge(four, five, 5);
    g.addEdge(one, four, 3);
    SECTION("Test Dijkstra with destination same as the source") {
        vector<int> expected;
        expected.push_back(2);
        vector<int> observed = g.dijkstra(2, 2);
        REQUIRE(observed[0] == expected[0]);
    }
    SECTION("Test Dijkstra with destination right next to source") {
        vector<int> expected;
        expected.push_back(444);
        expected.push_back(34);
        vector<int> observed = g.dijkstra(444, 34);
        for (int i = 0; i < expected.size(); i++) {
            REQUIRE(observed[i] == expected[i]);
        }
    }
    SECTION("Test Dijkstra with two pathways to get to destination") {
        vector<int> expected;
        expected.push_back(5);
        expected.push_back(34);
        expected.push_back(6);
        vector<int> observed = g.dijkstra(5, 6);
        for (int i = 0; i < expected.size(); i++) {
            REQUIRE(observed[i] == expected[i]);
        }
    }
}

// Test that csv reader reads in names correctly
TEST_CASE("Test fill_name_map") {
    Reader reader;
    reader.fill_name_map("Data/test_id.csv");
    REQUIRE(reader.getName(1) == "a");
    REQUIRE(reader.getName(2) == "b");
    REQUIRE(reader.getName(3) == "c");
    REQUIRE(reader.getName(4) == "d");
}

// Test that csv reader reads in ids and computes strength correctly
TEST_CASE("Test fill_user_ratings") {
    SECTION("Testing test_set.csv") {
        Reader reader;
        reader.fill_user_ratings("Data/test_set.csv");
        REQUIRE(reader.strength(1, 3) == 0);
        REQUIRE(reader.strength(1, 2) == 8.0 / 3);
    }
    SECTION("Testing test_set2.csv") {
        Reader reader;
        reader.fill_user_ratings("Data/test_set2.csv");
        REQUIRE(reader.strength(1, 2) == 0);
        REQUIRE(reader.strength(1, 3) == 9.0 / 2);
        REQUIRE(reader.strength(4, 5) == 9.0 / 3);
        REQUIRE(reader.strength(2, 3) == 0);
    }
}

// Test that csv reader reads csvs to graph correctly
TEST_CASE("Test upload_data") {
    SECTION("Testing test_set.csv") {
        Reader reader;
        Graph graph;
        reader.fill_name_map("Data/test_id.csv");
        reader.fill_user_ratings("Data/test_set.csv");
        reader.upload_data(&graph, false);
        REQUIRE(graph.getNode(1) != NULL);
        REQUIRE(graph.getEdge(graph.getNode(1), graph.getNode(3)) == 0);
        REQUIRE(graph.getEdge(graph.getNode(1), graph.getNode(2)) == -1);
    }
    SECTION("Testing test_set2.csv") {
        Reader reader;
        Graph graph;
        reader.fill_name_map("Data/test_id2.csv");
        reader.fill_user_ratings("Data/test_set2.csv");
        reader.upload_data(&graph, false);
        REQUIRE(graph.getNode(10) != NULL);
        REQUIRE(graph.getEdge(graph.getNode(1), graph.getNode(3)) == -1);
        REQUIRE(graph.getEdge(graph.getNode(4), graph.getNode(5)) == -1);
        REQUIRE(graph.getEdge(graph.getNode(10), graph.getNode(11)) == 0.5);
    }
}