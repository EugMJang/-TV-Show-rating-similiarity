CXX=clang++
CXX_FLAGS=-std=c++14 -stdlib=libc++ -lc++abi -g

main: main.cpp GraphNode.cpp Graph.cpp reader.cpp ForceGraph.cpp ForceGraphNode.cpp
	${CXX} ${CXX_FLAGS} main.cpp cs225/PNG.cpp cs225/HSLAPixel.cpp cs225/lodepng/lodepng.cpp GraphNode.cpp Graph.cpp reader.cpp ForceGraph.cpp ForceGraphNode.cpp -o main

test: tests/tests.cpp GraphNode.cpp Graph.cpp reader.cpp ForceGraph.cpp ForceGraphNode.cpp
	${CXX} ${CXX_FLAGS} tests/catch/catchmain.cpp tests/tests.cpp cs225/PNG.cpp cs225/HSLAPixel.cpp cs225/lodepng/lodepng.cpp GraphNode.cpp Graph.cpp reader.cpp ForceGraph.cpp ForceGraphNode.cpp -o test