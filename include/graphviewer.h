#ifndef _GRAPH_VIEWER_H_
#define _GRAPH_VIEWER_H_

#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>

#include "fpsmonitor.h"

#include <SFML/Graphics.hpp>

#include "edgetype.h"

using namespace std;

/**
 * @brief Class to save and represent a graph.
 */
class GraphViewer {
private:
    class LineShape;
    class FullLineShape;
    class DashedLineShape;

public:
    /**
     * @brief Class to represent a node.
     */
    class Node {
    private:
        int id;
        sf::Vector2f position;
        int size = 10;
        string label = "";
        sf::Color color = sf::Color::Red;
        sf::Texture icon; bool isIcon = false;
        float outlineThickness = 1.0;
        sf::Color outlineColor = sf::Color::Black;
        sf::Shape *shape = nullptr;
        sf::Text text;
        void update();
    public:
        Node();
        Node(int id, const sf::Vector2f &position);

        int getId() const;
        
        void setPosition(const sf::Vector2f &position);
        const sf::Vector2f& getPosition() const;
        
        void setSize(int size);
        int getSize() const;
        
        void setLabel(const string &label);
        string getLabel() const;
        
        void setColor(const sf::Color &color);
        const sf::Color& getColor() const;
        
        void setIcon(const string &path);
        const sf::Texture& getIcon() const;
        bool getIsIcon() const;

        void setOutlineThickness(int outlineThickness);
        int getOutlineThickness() const;
        
        void setOutlineColor(const sf::Color &outlineColor);
        const sf::Color& getOutlineColor() const;
        
        const sf::Shape* getShape() const;
        
        sf::Text getText() const;
    };

    class Edge {
    private:
        int id;
        const Node *u = nullptr;
        const Node *v = nullptr;
        int edgeType;
        string label = "";
        sf::Color color = sf::Color::Black;
        bool dashed = false;
        int thickness = 5;
        int *weight = nullptr;
        int *flow = nullptr;
        LineShape *shape = nullptr;
        sf::Text text;
        void update();
    public:
        Edge();
        Edge(int id, const Node *u, const Node *v, int edgeType);
        Edge& operator=(const Edge &u);
        int getId() const;
        void setFrom(const Node *u);
        const Node* getFrom() const;
        void setTo(const Node *v);
        const Node* getTo() const;
        void setEdgeType(int edgeType);
        int getEdgeType() const;
        void setLabel(const string &label);
        string getLabel() const;
        void setColor(const sf::Color &color);
        const sf::Color& getColor() const;
        void setDashed(bool dashed);
        bool getDashed() const;
        void setThickness(int thickness);
        int getThickness() const;
        void setWeight(int weight);
        const int* getWeight() const;
        void setFlow(int flow);
        const int* getFlow() const;
        const sf::VertexArray* getShape() const;
        sf::Text getText() const;
    };
    
public:
    /**
     * @brief Construct a new graph.
     */
    GraphViewer();

    /**
     * @brief Create the visualization window.
     *
     * @param width Window width (in pixels)
     * @param height Window height (in pixels)
     */
    void createWindow(int width, int height);

    /**
     * @brief Close visualization window.
     */
    void closeWindow();

    /**
     * @brief Add node.
     * 
     * @param node Node to be added
     */
    Node& addNode(const Node &node);

    Node& getNode(int id);

    /**
     * @brief Add edge.
     *
     * @param id Unique edge ID
     * @param v1 Unique ID of origin node
     * @param v2 Unique ID of destination node
     * @param edgeType EdgeType.DIRECTED if the edge is directed,
     *                 EdgeType.UNDIRECTED if the edge is undirected
     */
    Edge& addEdge(const Edge &edge);

    /**
     * @brief Remove node and all edges connected to it.
     *
     * @param id Unique ID of node to be removed.
     */
    void removeNode(int id);

    /**
     * @brief Remove edge.
     *
     * @param id Unique ID of edge to be removed
     */
    void removeEdge(int id);

    /**
     * @brief Set background image.
     *
     * @param path Filepath of new background
     */
    void setBackground(string path);

    /**
     * @brief Clear background image.
     */
    void clearBackground();

    /**
     * @brief Join the window main thread.
     * 
     * The window main thread exits when the user presses the close button in
     * the window.
     */
    void join();

    /**
     * @brief Enable node drawing.
     * 
     * If set to false, does not draw nodes but may improve performance
     * significantly.
     * 
     * @param b True to draw nodes, false to not draw nodes
     */
    void setEnabledNodes(bool b);
    /**
     * @brief Enable edge drawing.
     * 
     * @param b Draw nodes if true, false otherwise
     */
    void setEnabledEdges(bool b);
    /**
     * @brief Enable node label drawing.
     * 
     * @param b Draw node labels if true, false otherwise
     */
    void setEnabledNodesText(bool b);
    /**
     * @brief Enable edge label drawing.
     * 
     * @param b Draw edge labels if true, false otherwise
     */
    void setEnabledEdgesText(bool b);

    /**
     * @brief Allow edges to be zipped.
     * 
     * Zipping is the act of collecting all vertices that make up a set of
     * objects, and group all those vertices into a single, large vertex array.
     * This has the advantage of being a lot faster to draw than drawing each
     * object separately; performance improves by about 20 times in large
     * graphs with many edges.
     * 
     * @param b True to zip edges, false if not.
     */
    void setZipEdges(bool b);

    void lock();
    void unlock();

private:
    bool debug_mode = false;
    FPSMonitor fps_monitor = FPSMonitor(1000);
    static const sf::Font DEBUG_FONT;
    static const int DEBUG_FONT_SIZE = 14;
    sf::Text debug_text;

    static const sf::Font FONT;
    static const int FONT_SIZE = 16;

    float scale = 1.0;
    static constexpr float SCALE_DELTA = 1.5;
    float x0 = 0.0;
    float y0 = 0.0;

    sf::Texture background_texture;
    sf::Sprite background_sprite;
    sf::RenderWindow *window = nullptr;
    sf::View *view = nullptr;
    sf::View *debug_view = nullptr;
    thread *main_thread = nullptr;

    bool enabledNodes = true;
    bool enabledNodesText = true;
    bool enabledEdges = true;
    bool enabledEdgesText = true;

    class ZipEdges {
    private:
        vector<sf::Vertex> vertices;
    public:
        void append(const sf::VertexArray &a);
        const vector<sf::Vertex>& getVertices() const;
    };
    bool zipEdges = false;
    ZipEdges zip;
    void updateZip();

    mutex graphMutex;
    unordered_map<int, Node> nodes;
    unordered_map<int, Edge> edges;

    void run();
    void draw();
    void drawDebug();

    void onResize();
    void onScroll(float delta);

    void recalculateView();
};

#include "lines.h"

#endif
