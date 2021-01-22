#ifndef _GRAPH_VIEWER_H_
#define _GRAPH_VIEWER_H_

#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>

#include "fpsmonitor.h"

#include <SFML/Graphics.hpp>

#include "edgetype.h"

#define BLUE "BLUE"
#define RED "RED"
#define PINK "PINK"
#define PURPLE "PURPLE"
#define BLACK "BLACK"
#define WHITE "WHITE"
#define ORANGE "ORANGE"
#define YELLOW "YELLOW"
#define GREEN "GREEN"
#define CYAN "CYAN"
#define GRAY "GRAY"
#define DARK_GRAY "DARK_GRAY"
#define LIGHT_GRAY "LIGHT_GRAY"
#define MAGENTA "MAGENTA"

using namespace std;

/**
 * @brief Class to save and represent a graph.
 * 
 * All functions return a boolean indicating they executed successfully.
 */
class GraphViewer {
private:
    class LineShape;
    class FullLineShape;
    class DashedLineShape;

public:
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
        Node& operator=(const Node &u);
        int getId() const;
        void setPosition(const sf::Vector2f &position);
        const sf::Vector2f& getPosition() const;
        void setSize(int size);
        int getSize() const;
        void setLabel(const string &label);
        string getLabel() const;
        void setColor(const sf::Color &color);
        void setColor(const string &color);
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
    bool createWindow(int width, int height);

    /**
     * @brief Close visualization window.
     */
    bool closeWindow();

    /**
     * @brief Add node.
     * 
     * @param node Node to be added
     */
    Node& addNode(const Node &node);

    /**
     * @brief Add edge.
     *
     * @param id Unique edge ID
     * @param v1 Unique ID of origin node
     * @param v2 Unique ID of destination node
     * @param edgeType EdgeType.DIRECTED if the edge is directed,
     *                 EdgeType.UNDIRECTED if the edge is undirected
     */
    bool addEdge(int id, int v1, int v2, int edgeType);

    /**
     * @brief Remove node and all edges connected to it.
     *
     * @param id Unique ID of node to be removed.
     */
    bool removeNode(int id);

    /**
     * @brief Remove edge.
     *
     * @param id Unique ID of edge to be removed
     */
    bool removeEdge(int id);

    /**
     * @brief Set node text label.
     * 
     * @param id Node ID
     * @param label Node label
     */
    bool setVertexLabel(int id, string label);

    /**
     * @brief Clear node text label, if it was defined.
     *
     * @param id Node ID
     */
    bool clearVertexLabel(int id);

    /**
     * @brief Set edge text label.
     *
     * @param id Edge ID
     * @param label Edge label
     */
    bool setEdgeLabel(int id, string label);

    /**
     * @brief Clear edge text label, if it was defined.
     *
     * @param id Edge ID
     */
    bool clearEdgeLabel(int id);

    /**
     * @brief Set edge color.
     * 
     * @param id Edge ID
     * @param color New color
     */
    bool setEdgeColor(int id, const sf::Color &color);

    /**
     * @overload
     * 
     * @param id Edge ID
     * @param color New color (as a string, please use the defined color macros)
     */
    bool setEdgeColor(int id, string color);

    /**
     * @brief Clear edge color.
     *
     * @param id Edge ID
     */
    bool clearEdgeColor(int id);

    /**
     * @brief Set if an edge is drawn as a full or dashed line.
     *
     * @param id Edge ID
     * @param dashed True if edge is dashed, false if full
     */
    bool setEdgeDashed(int id, bool dashed);

    /**
     * @brief Set node color.
     *
     * @param id Node ID
     * @param color Node color
     */
    bool setVertexColor(int id, const sf::Color &color);

    /**
     * @overload
     * 
     * @param id Node ID
     * @param color New color (as a string, please use the defined color macros)
     */
    bool setVertexColor(int id, string color);

    /**
     * @brief Clear node color.
     *
     * @param id Node ID
     */
    bool clearVertexColor(int id);

    /**
     * @brief Set node size.
     *
     * @param id Node ID
     * @param size Node size
     */
    bool setVertexSize(int id, int size);

    /**
     * @brief Set node icon instead of a circle.
     * 
     * @param id Node ID
     * @param filepath Filepath of icon
     */
    bool setVertexIcon(int id, string filepath);

    /**
     * @brief Clear node icon.
     *
     * @param id Node ID
     */
    bool clearVertexIcon(int id);

    /**
     * @brief Set edge thickness.
     *
     * @param id Node ID
     * @param thickness Edge thickness (10 by default)
     */
    bool setEdgeThickness(int id, int thickness);

    /**
     * @brief Set edge weight.
     * 
     * Edge weight is shown in the label.
     *
     * @param id Edge ID
     * @param weight Edge weight
     */
    bool setEdgeWeight(int id, int weight);

    /**
     * @brief Set edge flow.
     *
     * @param id Edge ID
     * @param flow Edge flow
     */
    bool setEdgeFlow(int id, int flow);

    /**
     * @brief Set node outline thickness.
     * 
     * @param id                Node ID
     * @param outlineThickness  Outline thickness in pixels (1 by default)
     */
    bool setVertexOutlineThickness(int id, float outlineThickness);

    /**
     * @brief Set node outline color.
     * 
     * @param id            Node ID
     * @param outlineColor  Node outline color
     */
    bool setVertexOutlineColor(int id, const sf::Color &outlineColor);

    /**
     * @overload
     * 
     * @param id            Node ID
     * @param outlineColor  Node outline color (as a string, use the defined macros)
     */
    bool setVertexOutlineColor(int id, string outlineColor);
    
    /**
     * @brief Set default edge color.
     * 
     * @param color Default edge color
     */
    bool defineEdgeColor(const sf::Color &color);

    /**
     * @overload
     *
     * @param color Default edge color (as a string, use the defined macros)
     */
    bool defineEdgeColor(string color);
    
    /**
     * @brief Reset default edge color.
     */
    bool resetEdgeColor();

    /**
     * @brief Set default edge dashing.
     *
     * @param dashed True for dashed, false for full line
     */
    bool defineEdgeDashed(bool dashed);

    /**
     * @brief Set default node color.
     *
     * @param color Default node color
     */
    bool defineVertexColor(const sf::Color &color);
    
    /**
     * @overload
     * 
     * @param color Default node color (as a string, use the defined macros)
     */
    bool defineVertexColor(string color);

    /**
     * @brief Restore default vertex color.
     */
    bool resetVertexColor();

    /**
     * @brief Set default node size.
     *
     * @param size Default node size in pixels (10 by default)
     */
    bool defineVertexSize(int size);

    /**
     * @brief Set default vertex icon.
     *
     * @param filepath Filepath of new icon
     */
    bool defineVertexIcon(string filepath);

    /**
     * @brief Delete default vertex icon.
     */
    bool resetVertexIcon();

    /**
     * @brief Set default node outline thickness.
     * 
     * @param outlineThickness Node outline thickness in pixels (1 by default)
     */
    bool defineVertexOutlineThickness(float outlineThickness);

    /**
     * @brief Reset default node outline thickness.
     */
    bool resetVertexOutlineThickness();

    /**
     * @brief Define default node outline color.
     * 
     * @param outlineColor Default outline color
     */
    bool defineVertexOutlineColor(const sf::Color &outlineColor);

    /**
     * @overload
     * 
     * @param outlineColor Default outline color (as a string, use the defined macros)
     */
    bool defineVertexOutlineColor(string outlineColor);

    /**
     * @brief Reset default node outline color.
     */
    bool resetVertexOutlineColor();

    /**
     * @brief Set background image.
     *
     * @param path Filepath of new background
     */
    bool setBackground(string path);

    /**
     * @brief Clear background image.
     */
    bool clearBackground();

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
    bool debugMode = false;
    FPSMonitor fps_monitor = FPSMonitor(1000);
    static const sf::Font DEBUG_FONT;
    static const int DEBUG_FONT_SIZE = 14;
    sf::Text debugText;

    static const sf::Font FONT;
    static const int FONT_SIZE = 16;

    float scale = 1.0;
    static constexpr float SCALE_DELTA = 1.5;
    float x0 = 0.0;
    float y0 = 0.0;

    string backgroundPath = "";
    bool isBackgroundUpdated = false;
    sf::Texture backgroundTex;
    sf::Sprite backgroundSprite;
    sf::RenderWindow *window = nullptr;
    sf::View *view = nullptr;
    sf::View *debugView = nullptr;
    thread *mainThread = nullptr;

    bool enabledNodes = true;
    bool enabledNodesText = true;
    bool enabledEdges = true;
    bool enabledEdgesText = true;

    sf::Color nodeColor = sf::Color::Red;
    int nodeSize = 10;
    string nodeIcon = "";
    float nodeOutlineThickness = 1.0;
    sf::Color nodeOutlineColor = sf::Color::Black;

    sf::Color edgeColor = sf::Color::Black;
    bool edgeDashed = false;

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
