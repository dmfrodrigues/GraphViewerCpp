#include "graphviewer.h"

#include <cmath>
#include <iostream>

class LineShape {
public:
    explicit LineShape(const sf::Vector2f& u, const sf::Vector2f& v, float w):u(u),v(v),w(w){}

    void setFrom(const sf::Vector2f& u){
        this->u = u;
    }
    const sf::Vector2f& getFrom() const{ return u; }

    void setTo(const sf::Vector2f& v){
        this->v = v;
    }
    const sf::Vector2f& getTo() const{ return v; }

    void setWidth(const float& w){
        this->w = w;
    }
    const float& getWidth() const{ return w; }

private :
    sf::Vector2f u, v;
    float w;
};

class FullLineShape: public LineShape, public sf::Shape {
public:

    explicit FullLineShape(const sf::Vector2f& u, const sf::Vector2f& v, float w):LineShape(u,v,w){
        update();
    }

    virtual std::size_t getPointCount() const {
        return 4; // fixed, but could be an attribute of the class if needed
    }

    virtual sf::Vector2f getPoint(std::size_t index) const {
        sf::Vector2f edgeV = getTo()-getFrom();
        sf::Vector2f edgeNorm(-edgeV.y, edgeV.x);
        float magnitude = sqrt(edgeNorm.x*edgeNorm.x + edgeNorm.y*edgeNorm.y);
        edgeNorm /= magnitude;
        edgeNorm *= (getWidth()/2);

        switch(index){
            case 0: return getFrom()-edgeNorm;
            case 1: return getFrom()+edgeNorm;
            case 2: return getTo  ()+edgeNorm;
            case 3: return getTo  ()-edgeNorm;
            default: throw domain_error("LineShape: invalid index");
        }
    }

};

class DashedLineShape: public LineShape, public sf::VertexArray {
private:
    float dashFill = 0.5;
    size_t size = 0;
public:
    explicit DashedLineShape(const sf::Vector2f& u, const sf::Vector2f& v, float w):
        LineShape(u,v,w),
        sf::VertexArray(sf::Quads)    
    {
        process();
    }

    void setFrom (const sf::Vector2f& u){ LineShape::setFrom (u); process(); }
    void setTo   (const sf::Vector2f& v){ LineShape::setTo   (v); process(); }
    void setWidth(             float  w){ LineShape::setWidth(w); process(); }

    void process(){
        float interDashesSpace = 4.0*getWidth();
        const sf::Vector2f &u = getFrom();
        const sf::Vector2f &v = getTo  ();
        sf::Vector2f v_u = v-u;
        float magnitude = sqrt(v_u.x*v_u.x + v_u.y*v_u.y);
        v_u /= magnitude;

        sf::Vector2f edgeV = v-u;
        sf::Vector2f edgeNorm(-edgeV.y, edgeV.x);
        float magnitudeNorm = sqrt(edgeNorm.x*edgeNorm.x + edgeNorm.y*edgeNorm.y);
        edgeNorm /= magnitudeNorm;
        edgeNorm *= (getWidth()/2);

        resize(0); size = 0;

        // Most dashes
        int nDashes = magnitude/interDashesSpace;
        for(int i = 0; i < nDashes; ++i){
            sf::Vector2f u1 = u + v_u*interDashesSpace*float(i         );
            sf::Vector2f v1 = u + v_u*interDashesSpace*float(i+dashFill);
            
            append(sf::Vertex(u1-edgeNorm)); ++size;
            append(sf::Vertex(u1+edgeNorm)); ++size;
            append(sf::Vertex(v1+edgeNorm)); ++size;
            append(sf::Vertex(v1-edgeNorm)); ++size;
        }

        // Last dash
        sf::Vector2f u1 = u + v_u*    interDashesSpace*float(nDashes         )            ;
        sf::Vector2f v1 = u + v_u*min(interDashesSpace*float(nDashes+dashFill), magnitude);
            
        append(sf::Vertex(u1-edgeNorm)); ++size;
        append(sf::Vertex(u1+edgeNorm)); ++size;
        append(sf::Vertex(v1+edgeNorm)); ++size;
        append(sf::Vertex(v1-edgeNorm)); ++size;
    }

    void setFillColor(sf::Color color){
        for(size_t i = 0; i < size; ++i){
            (*this)[i].color = color;
        }
    }
};

GraphViewer::Node::Node(){}
GraphViewer::Node::Node(int id, const sf::Vector2f &position):id(id),position(position){}

GraphViewer::Node& GraphViewer::Node::operator=(const GraphViewer::Node &u){
    id       = u.id;
    position = u.position;
    return *this;
}

GraphViewer::Edge::Edge(){}
GraphViewer::Edge::Edge(int id, int v1, int v2, int edgeType):id(id),v1(v1),v2(v2),edgeType(edgeType){}

GraphViewer::Edge& GraphViewer::Edge::operator=(const GraphViewer::Edge &e){
    id       = e.id;
    v1       = e.v1;
    v2       = e.v2;
    edgeType = e.edgeType;
    return *this;
}

const int DEFAULT_WIDTH  = 800;
const int DEFAULT_HEIGHT = 600;

string getPath(const string &filename){
    const size_t last_slash_idx = min(filename.rfind('\\'), filename.rfind('/'));
    if(last_slash_idx == string::npos){
        throw invalid_argument("filename is not valid");
    }
    string directory = filename.substr(0, last_slash_idx);
    return directory;
}

GraphViewer::GraphViewer(){
    string fontPath = getPath(__FILE__)+"/../resources/fonts/arial.ttf";
    if(!font.loadFromFile(fontPath))
        throw runtime_error("Failed to load font from file; check if arial.ttf exists under resources/fonts/");
}

bool GraphViewer::createWindow(int width, int height){
    if(window != nullptr) return false;
    if(width  == 0) width  = DEFAULT_WIDTH ;
    if(height == 0) height = DEFAULT_HEIGHT;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window = new sf::RenderWindow(sf::VideoMode(width, height), "GraphViewer", sf::Style::Default, settings);
    x0 = width/2.0;
    y0 = height/2.0;
    window->setActive(false);
    mainThread = new thread(&GraphViewer::run, this);
    mainThread->detach();
    return true;
}

bool GraphViewer::closeWindow(){
    window->close();
    delete window;
    window = nullptr;
    return true;
}

bool GraphViewer::addNode(int id, int x, int y){
    lock_guard<mutex> lock(graphMutex);
    if(nodes.count(id)) return false;
    nodes[id] = Node(id, sf::Vector2f(x,y));
    nodes[id].color = nodeColor;
    nodes[id].size = nodeSize;
    nodes[id].icon = nodeIcon; if(nodes[id].icon != "") nodes[id].iconTex.loadFromFile(nodes[id].icon);
    nodes[id].outlineThickness = nodeOutlineThickness;
    nodes[id].outlineColor = nodeOutlineColor;
    return true;
}

bool GraphViewer::addEdge(int id, int v1, int v2, int edgeType){
    lock_guard<mutex> lock(graphMutex);
    if(edges.count(id)) return false;
    edges[id] = Edge(id, v1, v2, edgeType);
    edges[id].color = edgeColor;
    edges[id].dashed = edgeDashed;
    return true;
}

bool GraphViewer::removeNode(int id){
    lock_guard<mutex> lock(graphMutex);
    return (nodes.erase(id) != 0);
}

bool GraphViewer::removeEdge(int id){
    lock_guard<mutex> lock(graphMutex);
    return (edges.erase(id) != 0);
}

bool GraphViewer::setVertexLabel(int id, string label){
    lock_guard<mutex> lock(graphMutex);
    auto nodeIt = nodes.find(id);
    if(nodeIt == nodes.end()) return false;
    nodeIt->second.label = label;
    return true;
}

bool GraphViewer::clearVertexLabel(int id){
    lock_guard<mutex> lock(graphMutex);
    return setVertexLabel(id, "");
}

bool GraphViewer::setEdgeLabel(int id, string label){
    lock_guard<mutex> lock(graphMutex);
    auto edgeIt = edges.find(id);
    if(edgeIt == edges.end()) return false;
    edgeIt->second.label = label;
    return true;
}

bool GraphViewer::clearEdgeLabel(int id){
    lock_guard<mutex> lock(graphMutex);
    return setEdgeLabel(id, "");
}

bool GraphViewer::setEdgeColor(int id, string color){
    lock_guard<mutex> lock(graphMutex);
    auto edgeIt = edges.find(id);
    if(edgeIt == edges.end()) return false;
    edgeIt->second.color = color;
    return true;
}

bool GraphViewer::clearEdgeColor(int id){
    lock_guard<mutex> lock(graphMutex);
    return setEdgeColor(id, BLACK);
}

bool GraphViewer::setEdgeDashed(int id, bool dashed){
    lock_guard<mutex> lock(graphMutex);
    auto edgeIt = edges.find(id);
    if(edgeIt == edges.end()) return false;
    edgeIt->second.dashed = dashed;
    return true;
}

bool GraphViewer::setVertexColor(int id, string color){
    lock_guard<mutex> lock(graphMutex);
    auto nodeIt = nodes.find(id);
    if(nodeIt == nodes.end()) return false;
    nodeIt->second.color = color;
    return true;
}

bool GraphViewer::clearVertexColor(int id){
    lock_guard<mutex> lock(graphMutex);
    return setVertexColor(id, BLACK);
}

bool GraphViewer::setVertexSize(int id, int size){
    lock_guard<mutex> lock(graphMutex);
    auto nodeIt = nodes.find(id);
    if(nodeIt == nodes.end()) return false;
    nodeIt->second.size = size;
    return true;
}

bool GraphViewer::setVertexIcon(int id, string filepath){
    lock_guard<mutex> lock(graphMutex);
    auto nodeIt = nodes.find(id);
    if(nodeIt == nodes.end()) return false;
    nodeIt->second.icon = filepath; if(nodeIt->second.icon != "") nodeIt->second.iconTex.loadFromFile(nodeIt->second.icon);
    return true;
}

bool GraphViewer::clearVertexIcon(int id){
    lock_guard<mutex> lock(graphMutex);
    return setVertexIcon(id, "");
}

bool GraphViewer::setEdgeThickness(int id, int thickness){
    lock_guard<mutex> lock(graphMutex);
    auto edgeIt = edges.find(id);
    if(edgeIt == edges.end()) return false;
    edgeIt->second.thickness = thickness;
    return true;
}

bool GraphViewer::setEdgeWeight(int id, int weight){
    lock_guard<mutex> lock(graphMutex);
    auto edgeIt = edges.find(id);
    if(edgeIt == edges.end()) return false;
    edgeIt->second.weight = new int(weight);
    return true;
}

bool GraphViewer::setEdgeFlow(int id, int flow){
    lock_guard<mutex> lock(graphMutex);
    auto edgeIt = edges.find(id);
    if(edgeIt == edges.end()) return false;
    edgeIt->second.flow = new int(flow);
    return true;
}

bool GraphViewer::setVertexOutlineThickness(int id, float outlineThickness){
    lock_guard<mutex> lock(graphMutex);
    auto nodeIt = nodes.find(id);
    if(nodeIt == nodes.end()) return false;
    nodeIt->second.outlineThickness = outlineThickness;
    return true;
}

bool GraphViewer::setVertexOutlineColor(int id, string outlineColor){
    lock_guard<mutex> lock(graphMutex);
    auto nodeIt = nodes.find(id);
    if(nodeIt == nodes.end()) return false;
    nodeIt->second.outlineColor = outlineColor;
    return true;
}

/*
bool GraphViewer::defineEdgeCurved(bool curved){
    return true;
}
*/

bool GraphViewer::defineEdgeColor(string color){
    edgeColor = color;
    return true;
}

bool GraphViewer::resetEdgeColor(){
    edgeColor = BLACK;
    return true;
}

bool GraphViewer::defineEdgeDashed(bool dashed){
    edgeDashed = dashed;
    return true;
}

bool GraphViewer::defineVertexColor(string color){
    nodeColor = color;
    return true;
}

bool GraphViewer::resetVertexColor(){
    nodeColor = BLACK;
    return true;
}

bool GraphViewer::defineVertexSize(int size){
    nodeSize = size;
    return true;
}

bool GraphViewer::defineVertexIcon(string filepath){
    nodeIcon = filepath;
    return true;
}

bool GraphViewer::defineVertexOutlineThickness(float outlineThickness){
    nodeOutlineThickness = outlineThickness;
    return true;
}

bool GraphViewer::resetVertexOutlineThickness(){
    return defineVertexOutlineThickness(1.0);
}

bool GraphViewer::defineVertexOutlineColor(string outlineColor){
    nodeOutlineColor = outlineColor;
    return true;
}

bool GraphViewer::resetVertexOutlineColor(){
    return defineVertexOutlineColor(BLACK);
}

bool GraphViewer::resetVertexIcon(){
    nodeIcon = "";
    return true;
}

bool GraphViewer::setBackground(string path){
    lock_guard<mutex> lock(graphMutex);
    backgroundPath = path;
    backgroundTex.loadFromFile(path);
    backgroundSprite.setTexture(backgroundTex);
    auto bounds = backgroundSprite.getLocalBounds();
    backgroundSprite.setOrigin(bounds.width/2.0, bounds.height/2.0);
    return true;
}

bool GraphViewer::clearBackground(){
    lock_guard<mutex> lock(graphMutex);
    backgroundPath = "";
    backgroundTex = sf::Texture();
    backgroundSprite.setTexture(backgroundTex);
    return true;
}

sf::Color colorStringToSFColor(string colorStr){
    std::transform(colorStr.begin(), colorStr.end(),colorStr.begin(), ::toupper);
    if(colorStr == "BLUE"       ) return sf::Color::Blue;
    if(colorStr == "RED"        ) return sf::Color::Red;
    if(colorStr == "PINK"       ) return sf::Color(255, 192, 203);
    if(colorStr == "PURPLE"     ) return sf::Color(128, 0, 128);
    if(colorStr == "BLACK"      ) return sf::Color::Black;
    if(colorStr == "WHITE"      ) return sf::Color::White;
    if(colorStr == "ORANGE"     ) return sf::Color(255, 129,   0);
    if(colorStr == "YELLOW"     ) return sf::Color::Yellow;
    if(colorStr == "GREEN"      ) return sf::Color::Green;
    if(colorStr == "CYAN"       ) return sf::Color::Cyan;
    if(colorStr == "GRAY"       ) return sf::Color(128, 128, 128);
    if(colorStr == "DARK_GRAY"  ) return sf::Color(192, 192, 192);
    if(colorStr == "LIGHT_GRAY" ) return sf::Color( 64,  64,  64);
    if(colorStr == "MAGENTA"    ) return sf::Color::Magenta;
    throw domain_error("No such color '"+colorStr+"'");
}

void GraphViewer::run(){
    bool isLeftClickPressed = false;
    float x0Initial, y0Initial;
    int xMouseInitial, yMouseInitial;
    while (window->isOpen()){
        sf::Event event;
        while (window->pollEvent(event)){
            switch(event.type){
                case sf::Event::Closed            : window->close(); break;
                case sf::Event::Resized           : onResize(); break;
                case sf::Event::MouseWheelScrolled: onScroll(event.mouseWheelScroll.delta); break;
                case sf::Event::MouseButtonPressed:
                    switch(event.mouseButton.button){
                        case sf::Mouse::Button::Left:
                            isLeftClickPressed = true;
                            x0Initial = x0;
                            y0Initial = y0;
                            xMouseInitial = event.mouseButton.x;
                            yMouseInitial = event.mouseButton.y;
                            break;
                        default: break;
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    switch(event.mouseButton.button){
                        case sf::Mouse::Button::Left:
                            isLeftClickPressed = false;
                            break;
                        default: break;
                    }
                    break;
                case sf::Event::MouseMoved:
                    if(isLeftClickPressed){
                        x0 = x0Initial - scale*(event.mouseMove.x - xMouseInitial);
                        y0 = y0Initial - scale*(event.mouseMove.y - yMouseInitial);
                        recalculateView();
                    }
                    break;
                default: break;
            }
        }

        window->clear(sf::Color::Black);
        draw();
        window->display();
    }
}

void GraphViewer::draw() {
    lock_guard<mutex> lock(graphMutex);
    window->clear(sf::Color::White);
    window->draw(backgroundSprite);
    for(const auto &edgeIt: edges){
        const Edge &edge = edgeIt.second;
        const Node &u = nodes.at(edge.v1);
        const Node &v = nodes.at(edge.v2);
        const sf::Vector2f uPos = u.position;
        const sf::Vector2f vPos = v.position;

        if(!edge.dashed){
            FullLineShape line(uPos, vPos, edge.thickness);
            line.setFillColor(colorStringToSFColor(edge.color));
            window->draw(line);
        } else {
            DashedLineShape line(uPos, vPos, edge.thickness);
            line.setFillColor(colorStringToSFColor(edge.color));
            window->draw(line);
        }
    }
    for(const auto &nodeIt: nodes){
        const Node &node = nodeIt.second;
        if(node.icon == ""){
            sf::CircleShape nodeShape(node.size/2.0);
            nodeShape.setOrigin(node.size/2.0, node.size/2.0);
            nodeShape.setPosition(node.position);
            nodeShape.setFillColor(colorStringToSFColor(node.color));
            nodeShape.setOutlineThickness(node.outlineThickness);
            nodeShape.setOutlineColor(colorStringToSFColor(node.outlineColor));
            window->draw(nodeShape);
        } else {
            sf::RectangleShape nodeShape(sf::Vector2f(node.size,node.size));
            nodeShape.setOrigin(node.size/2.0, node.size/2.0);
            nodeShape.setPosition(node.position);
            // nodeShape.setFillColor(colorStringToSFColor(node.color));
            // nodeShape.setOutlineThickness(node.outlineThickness);
            // nodeShape.setOutlineColor(colorStringToSFColor(node.outlineColor));
            nodeShape.setTexture(&node.iconTex);
            window->draw(nodeShape);
        }
    }
    for(const auto &edgeIt: edges){
        const Edge &edge = edgeIt.second;
        const Node &u = nodes.at(edge.v1);
        const Node &v = nodes.at(edge.v2);
        const sf::Vector2f uPos = u.position;
        const sf::Vector2f vPos = v.position;
        string label = edge.label;
        if(edge.weight != nullptr) label += (label == "" ? "" : " ")+string("w: ")+to_string(*edge.weight);
        if(edge.flow   != nullptr) label += (label == "" ? "" : " ")+string("f: ")+to_string(*edge.flow  );
        if(label != ""){
            sf::Text text(label, font, FONT_SIZE);
            sf::FloatRect bounds = text.getLocalBounds();
            text.setPosition((uPos+vPos)/2.0f - sf::Vector2f(bounds.width/2.0, 0.8*bounds.height));
            text.setFillColor(sf::Color::Black);
            window->draw(text);
        }
    }
    for(const auto &nodeIt: nodes){
        const Node &node = nodeIt.second;
        if(node.label != ""){
            sf::Text text(node.label, font, FONT_SIZE);
            sf::FloatRect bounds = text.getLocalBounds();
            text.setPosition(node.position - sf::Vector2f(bounds.width/2.0, 0.8*bounds.height));
            text.setFillColor(sf::Color::Black);
            window->draw(text);
        }
    }
}

void GraphViewer::onResize(){
    sf::Vector2f size = static_cast<sf::Vector2f>(window->getSize());
    recalculateView();
}

void GraphViewer::onScroll(float delta){
    scale *= pow(scaleDelta, -delta);
    recalculateView();
}

void GraphViewer::recalculateView(){
    sf::Vector2f size = static_cast<sf::Vector2f>(window->getSize());
    sf::View windowView(sf::Vector2f(x0, y0), sf::Vector2f(size.x*scale, size.y*scale));
	window->setView(windowView);

    backgroundSprite.setPosition(x0, y0);
    auto bounds = backgroundSprite.getLocalBounds();
    sf::Vector2f scaleVec(scale*size.x/bounds.width, scale*size.y/bounds.height);
    backgroundSprite.setScale(scaleVec);
}
