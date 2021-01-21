#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

#include "graphviewer.h"

#include <X11/Xlib.h>

GraphViewer* drawGraphFromFile(std::string name);

int main() {
    XInitThreads();

    GraphViewer *gv1 = drawGraphFromFile("random");
    GraphViewer *gv2 = drawGraphFromFile("star");
    GraphViewer *gv3 = drawGraphFromFile("rainbow");
    gv1->join();
    gv2->join();
    gv3->join();
    delete gv1;
    delete gv2;
    delete gv3;
    
    return 0;
}

GraphViewer* drawGraphFromFile(std::string name){
    std::ifstream nodes("resources/graphs/"+name+"/nodes.txt");
    std::ifstream edges("resources/graphs/"+name+"/edges.txt");
    std::ifstream window("resources/graphs/"+name+"/window.txt");
    std::string line, background_path;
    std::istringstream iss;
    unsigned int n_nodes, n_edges, height, width, v1, v2, type, scale, dynamic, thickness, size, dashed, curved;
    float x, y;
    char color[20], label[256], icon_path[256], flow[256], weight[256];

    window >> width >> height >> dynamic >> scale >> dashed >> curved >> background_path;
    GraphViewer *gv = new GraphViewer();
    if (background_path[0] != '-')
        gv->setBackground(background_path);
    gv->createWindow(width, height);
    gv->defineEdgeDashed(dashed);
    // gv->defineEdgeCurved(curved);

    // read num of nodes
    std::getline(nodes, line);
    iss.str(line);
    iss >> n_nodes;

    // draw nodes
    for(int i = 0; i < n_nodes;i++) {
        std::getline(nodes, line);
        sscanf( line.c_str(), "(%f, %f, %s , %s , %u, %s )", &x, &y, color, label, &size, icon_path);
        gv->addNode(i , x*scale, y*scale);
        gv->setVertexColor(i, color);
        if (label[0] != '-')
            gv->setVertexLabel(i, label);
        if (icon_path[0] != '-')
            gv->setVertexIcon(i, std::string(icon_path));
        gv->setVertexSize(i, size);
    }

    // read num of edges
    std::getline(edges, line);
    sscanf( line.c_str(), "%d", &n_edges);

    //draw edges
    for(int i = 0; i < n_edges ; i++) {
        std::getline(edges, line);
        sscanf( line.c_str(), "(%u, %u, %u, %s ,%u, %s , %s , %s )", &v1, &v2, &type, color, &thickness, label, flow, weight);
        (type)? gv->addEdge(i, v1, v2, EdgeType::DIRECTED): gv->addEdge(i, v1, v2, EdgeType::UNDIRECTED);
        gv->setEdgeColor(i, color);
        gv->setEdgeThickness(i, thickness);
        if (label[0] != '-')
            gv->setEdgeLabel(i, label);
        if (flow[0] != '%')
            gv->setEdgeFlow(i, atoi(flow));
        if (weight[0] != '%')
            gv->setEdgeWeight(i, atoi(weight));
    }

    return gv;
}
