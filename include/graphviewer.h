#ifndef _GRAPH_VIEWER_H_
#define _GRAPH_VIEWER_H_

#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>

#include "fpsmonitor.h"

#include <SFML/Graphics.hpp>

/**
 * @brief Class to save and represent a graph.
 */
class GraphViewer {
public:
    typedef long id_t;

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
        id_t id;
        sf::Vector2f position;
        float size = 10.0;
        std::string label = "";
        sf::Color color = sf::Color::Red;
        sf::Texture icon; bool isIcon = false;
        float outlineThickness = 1.0;
        sf::Color outlineColor = sf::Color::Black;
        sf::Shape *shape = nullptr;
        sf::Text text;
        void update();
    public:
        /**
         * @brief Construct a new Node object
         */
        Node();
        /**
         * @brief Construct a new Node object with ID and position
         * 
         * @param id        Unique node ID
         * @param position  Node position in the window, in pixels
         */
        Node(id_t id, const sf::Vector2f &position);

        /**
         * @brief Get node ID.
         * 
         * @return id_t     Node ID
         */
        id_t getId() const;
        
        /**
         * @brief Set node position.
         * 
         * @param position  Position, in pixels
         */
        void setPosition(const sf::Vector2f &position);
        /**
         * @brief Get node position;
         * 
         * @return const sf::Vector2f&  Position, in pixels
         */
        const sf::Vector2f& getPosition() const;
        
        /**
         * @brief Set node size.
         * 
         * In the case it is not an icon, it is the diameter of the node;
         * if it is an icon, it is the width of the node.
         * 
         * @param size  Node size, in pixels
         */
        void setSize(int size);
        /**
         * @brief Get node size.
         * 
         * @return int Node size, in pixels
         */
        float getSize() const;
        
        /**
         * @brief Set node label.
         * 
         * @param label     Node label
         */
        void setLabel(const std::string &label);
        /**
         * @brief Get node label.
         * 
         * @return std::string  Node label
         */
        std::string getLabel() const;
        
        /**
         * @brief Set node color.
         * 
         * @param color Node color
         */
        void setColor(const sf::Color &color);
        /**
         * @brief Get node color.
         * 
         * @return const sf::Color&     Node color
         */
        const sf::Color& getColor() const;
        
        /**
         * @brief Set node icon.
         * 
         * @param path  Path of file to be used as icon
         */
        void setIcon(const std::string &path);
        /**
         * @brief Get node icon texture.
         * 
         * @return const sf::Texture& Icon texture
         */
        const sf::Texture& getIcon() const;
        /**
         * @brief Check if node is an icon.
         * 
         * @return true     When it is an icon
         * @return false    Otherwise
         */
        bool getIsIcon() const;

        /**
         * @brief Set node outline thickness.
         * 
         * @param outlineThickness  Outline thickness, in pixels
         */
        void setOutlineThickness(float outlineThickness);
        /**
         * @brief Get node outline thickness.
         * 
         * @return float    Outline thickness, in pixels
         */
        float getOutlineThickness() const;
        
        /**
         * @brief Set node outline color.
         * 
         * @param outlineColor  Outline color
         */
        void setOutlineColor(const sf::Color &outlineColor);
        /**
         * @brief Get outline color.
         * 
         * @return const sf::Color&     Outline color
         */
        const sf::Color& getOutlineColor() const;
        
        /**
         * @brief Get shape that represents the node.
         * 
         * @return const sf::Shape*     Pointer to node shape
         */
        const sf::Shape* getShape() const;
        
        /**
         * @brief Get node text.
         * 
         * @return const sf::Text&  Node text 
         */
        const sf::Text& getText() const;
    };

    class Edge {
    public:
        enum EdgeType {
            DIRECTED,
            UNDIRECTED
        };
    private:
        id_t id;
        const Node *u = nullptr;
        const Node *v = nullptr;
        EdgeType edge_type;
        std::string label = "";
        sf::Color color = sf::Color::Black;
        bool dashed = false;
        float thickness = 5.0;
        float *weight = nullptr;
        float *flow = nullptr;
        LineShape *shape = nullptr;
        sf::Text text;
        void update();
    public:
        Edge();
        Edge(id_t id, const Node *u, const Node *v, EdgeType edge_type);
        
        id_t getId() const;

        void setFrom(const Node *u);
        const Node* getFrom() const;

        void setTo(const Node *v);
        const Node* getTo() const;

        void setEdgeType(EdgeType edge_type);
        EdgeType getEdgeType() const;

        void setLabel(const std::string &label);
        const std::string& getLabel() const;

        void setColor(const sf::Color &color);
        const sf::Color& getColor() const;

        void setDashed(bool dashed);
        bool getDashed() const;

        void setThickness(float thickness);
        float getThickness() const;

        void setWeight(float weight);
        const float* getWeight() const;

        void setFlow(float flow);
        const float* getFlow() const;

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
    void createWindow(unsigned int width, unsigned int height);

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

    Node& getNode(id_t id);

    /**
     * @brief Add edge.
     *
     * @param id Unique edge ID
     * @param v1 Unique ID of origin node
     * @param v2 Unique ID of destination node
     * @param edge_type EdgeType.DIRECTED if the edge is directed,
     *                 EdgeType.UNDIRECTED if the edge is undirected
     */
    Edge& addEdge(const Edge &edge);

    /**
     * @brief Remove node and all edges connected to it.
     *
     * @param id Unique ID of node to be removed.
     */
    void removeNode(id_t id);

    /**
     * @brief Remove edge.
     *
     * @param id Unique ID of edge to be removed
     */
    void removeEdge(id_t id);

    /**
     * @brief Set background image.
     *
     * @param path Filepath of new background
     */
    void setBackground(std::string path);

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
    std::thread *main_thread = nullptr;

    bool enabledNodes = true;
    bool enabledNodesText = true;
    bool enabledEdges = true;
    bool enabledEdgesText = true;

    class ZipEdges {
    private:
        std::vector<sf::Vertex> vertices;
    public:
        void append(const sf::VertexArray &a);
        const std::vector<sf::Vertex>& getVertices() const;
    };
    bool zipEdges = false;
    ZipEdges zip;
    void updateZip();

    std::mutex graphMutex;
    std::unordered_map<id_t, Node> nodes;
    std::unordered_map<id_t, Edge> edges;

    void run();
    void draw();
    void drawDebug();

    void onResize();
    void onScroll(float delta);

    void recalculateView();
};

#include "lines.h"

#endif
