#include "graphviewer.h"

#include <cmath>
#include <iostream>

GraphViewer::Node::Node(){
    text.setFont(GraphViewer::FONT);
    text.setCharacterSize(GraphViewer::FONT_SIZE);
    text.setFillColor(sf::Color::Black);
    update();
}
GraphViewer::Node::Node(int id, const sf::Vector2f &position):
    id(id),
    position(position)
{
    text.setFont(GraphViewer::FONT);
    text.setCharacterSize(GraphViewer::FONT_SIZE);
    text.setFillColor(sf::Color::Black);
    update();
}

int GraphViewer::Node::getId() const{ return id; }
void GraphViewer::Node::setPosition(const sf::Vector2f &position){ this->position = position; update(); }
const sf::Vector2f& GraphViewer::Node::getPosition() const{ return position; }
void GraphViewer::Node::setSize(int size){ this->size = size; update(); }
int GraphViewer::Node::getSize() const{ return size; }
void GraphViewer::Node::setLabel(const string &label){ text.setString(label); update(); }
string GraphViewer::Node::getLabel() const{ return text.getString(); }
void GraphViewer::Node::setColor(const sf::Color &color){ this->color = color; update(); }
const sf::Color& GraphViewer::Node::getColor() const{ return color; }
void GraphViewer::Node::setIcon(const string &path){
    if(path == ""){ icon = sf::Texture()   ; isIcon = false; }
    else          { icon.loadFromFile(path); isIcon = true; }
    update();
}
const sf::Texture& GraphViewer::Node::getIcon() const{ return icon; }
bool GraphViewer::Node::getIsIcon() const{ return isIcon; }
void GraphViewer::Node::setOutlineThickness(int outlineThickness){ this->outlineThickness = outlineThickness; update(); }
int GraphViewer::Node::getOutlineThickness() const{ return outlineThickness; }
void GraphViewer::Node::setOutlineColor(const sf::Color &outlineColor){ this->outlineColor = outlineColor; update(); }
const sf::Color& GraphViewer::Node::getOutlineColor() const{ return outlineColor; }
const sf::Shape* GraphViewer::Node::getShape() const { return shape; }
sf::Text GraphViewer::Node::getText() const { return text; }

void GraphViewer::Node::update(){
    delete shape;
    shape = nullptr;
    if(!getIsIcon()){
        sf::CircleShape *newShape = new sf::CircleShape(getSize()/2.0);
        newShape->setFillColor(getColor());
        newShape->setOutlineThickness(getOutlineThickness());
        newShape->setOutlineColor(getOutlineColor());
        shape = newShape;
    } else {
        sf::RectangleShape *newShape = new sf::RectangleShape(sf::Vector2f(getSize(),getSize()));
        newShape->setTexture(&getIcon());
        shape = newShape;
    }
    shape->setOrigin(getSize()/2.0, getSize()/2.0);
    shape->setPosition(getPosition());    

    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition(getPosition() - sf::Vector2f(bounds.width/2.0, 0.8*bounds.height));
}

GraphViewer::Edge::Edge(){
    text.setFont(GraphViewer::FONT);
    text.setCharacterSize(GraphViewer::FONT_SIZE);
    text.setFillColor(sf::Color::Black);
}
GraphViewer::Edge::Edge(int id, const GraphViewer::Node *u, const GraphViewer::Node *v, int edgeType):
    id(id),
    u(u),
    v(v),
    edgeType(edgeType)
{
    text.setFont(GraphViewer::FONT);
    text.setCharacterSize(GraphViewer::FONT_SIZE);
    text.setFillColor(sf::Color::Black);
    
    update();
}

GraphViewer::Edge& GraphViewer::Edge::operator=(const GraphViewer::Edge &e){
    id       = e.id;
    u        = e.u;
    v        = e.v;
    edgeType = e.edgeType;
    label    = e.label;
    color    = e.color;
    dashed   = e.dashed;
    thickness= e.thickness;
    if(e.weight != nullptr) weight = new int(*e.weight);
    if(e.flow   != nullptr) flow   = new int(*e.flow  );
    return *this;
}

int GraphViewer::Edge::getId() const{ return id; }
void GraphViewer::Edge::setFrom(const Node *u){ this->u = u; update(); }
const GraphViewer::Node* GraphViewer::Edge::getFrom() const{ return u; }
void GraphViewer::Edge::setTo(const Node *v){ this->v = v; update(); }
const GraphViewer::Node* GraphViewer::Edge::getTo() const{ return v; }
void GraphViewer::Edge::setEdgeType(int edgeType){ this->edgeType = edgeType; update(); }
int GraphViewer::Edge::getEdgeType() const{ return edgeType; }
void GraphViewer::Edge::setLabel(const string &label){ this->label = label; update(); }
string GraphViewer::Edge::getLabel() const{ return label; }
void GraphViewer::Edge::setColor(const sf::Color &color){ this->color = color; update(); }
const sf::Color& GraphViewer::Edge::getColor() const{ return color; }
void GraphViewer::Edge::setDashed(bool dashed){ this->dashed = dashed; update(); }
bool GraphViewer::Edge::getDashed() const{ return dashed; }
void GraphViewer::Edge::setThickness(int thickness){ this->thickness = thickness; update(); }
int GraphViewer::Edge::getThickness() const{ return thickness; }
void GraphViewer::Edge::setWeight(int weight){
    delete this->weight;
    this->weight = new int(weight);
    update();
}
const int* GraphViewer::Edge::getWeight() const{ return weight; }
void GraphViewer::Edge::setFlow(int flow){
    delete this->flow;
    this->flow = new int(flow);
    update();
}
const int* GraphViewer::Edge::getFlow() const{ return flow; }
const sf::VertexArray* GraphViewer::Edge::getShape() const { return shape; }
sf::Text GraphViewer::Edge::getText() const { return text; }
void GraphViewer::Edge::update(){
    delete shape;
    shape = nullptr;
    if(!getDashed()){
        shape = new FullLineShape(u->getPosition(), v->getPosition(), getThickness());
    } else {
        shape = new DashedLineShape(u->getPosition(), v->getPosition(), getThickness());
    }
    shape->setFillColor(getColor());

    string label = getLabel();
    if(getWeight() != nullptr) label += (label == "" ? "" : " ")+string("w: ")+to_string(*getWeight());
    if(getFlow  () != nullptr) label += (label == "" ? "" : " ")+string("f: ")+to_string(*getFlow  ());
    text.setString(label);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition((u->getPosition() + v->getPosition())/2.0f - sf::Vector2f(bounds.width/2.0, 0.8*bounds.height));
}

void GraphViewer::ZipEdges::append(const sf::VertexArray &a){
    for(size_t i = 0; i < a.getVertexCount(); ++i){
        vertices.push_back(a[i]);
    }
}
const vector<sf::Vertex>& GraphViewer::ZipEdges::getVertices() const{ return vertices; }

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

sf::Font getFont(const string &path){
    sf::Font font;
    string fontPath = getPath(__FILE__)+path;
    if(!font.loadFromFile(fontPath))
        throw runtime_error("Failed to load font from file; check if font exists");
    return font;
}
const sf::Font GraphViewer::DEBUG_FONT = getFont("/../resources/fonts/inconsolata.ttf");
const sf::Font GraphViewer::FONT       = getFont("/../resources/fonts/arial.ttf");

GraphViewer::GraphViewer():
    debug_text("", DEBUG_FONT, DEBUG_FONT_SIZE)
{
    debug_text.setFillColor(sf::Color::Black);
    debug_text.setStyle(sf::Text::Bold);
}

void GraphViewer::createWindow(int width, int height){
    if(window != nullptr) throw runtime_error("Window was already created");
    if(width  == 0) width  = DEFAULT_WIDTH ;
    if(height == 0) height = DEFAULT_HEIGHT;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window = new sf::RenderWindow(sf::VideoMode(width, height), "GraphViewer", sf::Style::Default, settings);
    view = new sf::View(window->getDefaultView());
    debug_view = new sf::View(window->getDefaultView());

    x0 = width/2.0;
    y0 = height/2.0;
    window->setActive(false);
    main_thread = new thread(&GraphViewer::run, this);
}

void GraphViewer::closeWindow(){
    window->close();
    delete window    ; window     = nullptr;
    delete view      ; view       = nullptr;
    delete debug_view; debug_view = nullptr;
}

GraphViewer::Node& GraphViewer::addNode(const GraphViewer::Node &node){
    lock_guard<mutex> lock(graphMutex);
    return (nodes[node.getId()] = node);
}

GraphViewer::Node& GraphViewer::getNode(int id){
    return nodes.at(id);
}

GraphViewer::Edge& GraphViewer::addEdge(const Edge &edge){
    lock_guard<mutex> lock(graphMutex);
    Edge &ret = (edges[edge.getId()] = edge);
    if(zipEdges) updateZip();
    return ret;
}

void GraphViewer::removeNode(int id){
    lock_guard<mutex> lock(graphMutex);
    if(nodes.erase(id) == 0)
        throw out_of_range("No such node ID "+to_string(id));
}

void GraphViewer::removeEdge(int id){
    lock_guard<mutex> lock(graphMutex);
    if(edges.erase(id) == 0)
        throw out_of_range("No such edge ID "+to_string(id));
    if(zipEdges) updateZip();
}

void GraphViewer::setBackground(string path){
    lock_guard<mutex> lock(graphMutex);
    background_texture.loadFromFile(path);
    background_sprite.setTexture(background_texture);
    auto bounds = background_sprite.getLocalBounds();
    background_sprite.setOrigin(bounds.width/2.0, bounds.height/2.0);
}

void GraphViewer::clearBackground(){
    lock_guard<mutex> lock(graphMutex);
    background_texture = sf::Texture();
    background_sprite.setTexture(background_texture);
}

void GraphViewer::join(){
    main_thread->join();
}

void GraphViewer::setEnabledNodes(bool b){ enabledNodes = b; }
void GraphViewer::setEnabledEdges(bool b){ enabledEdges = b; }
void GraphViewer::setEnabledNodesText(bool b){ enabledNodesText = b; }
void GraphViewer::setEnabledEdgesText(bool b){ enabledEdgesText = b; }

void GraphViewer::setZipEdges(bool b){
    zipEdges = b;
    if(zipEdges) updateZip();
}

void GraphViewer::lock  (){ graphMutex.lock  (); }
void GraphViewer::unlock(){ graphMutex.unlock(); }

void GraphViewer::updateZip(){
    lock_guard<mutex> lock(graphMutex);
    zip = ZipEdges();
    for(const auto &p: edges)
        zip.append(*p.second.getShape());
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
                case sf::Event::TextEntered:
                    switch(toupper(event.text.unicode)){
                        case 'D': debug_mode = !debug_mode; break;
                        default: break;
                    }
                    break;
                default: break;
            }
        }
        draw();
        window->display();
    }
}

void GraphViewer::draw() {
    lock_guard<mutex> lock(graphMutex);
    window->clear(sf::Color::White);

    window->setView(*view);
    window->draw(background_sprite);
    if(enabledEdges){
        if(zipEdges){
            const vector<sf::Vertex> &v = zip.getVertices();
            window->draw(&v[0], v.size(), sf::Quads);
        } else {
            for(const auto &edgeIt: edges){
                const Edge &edge = edgeIt.second;
                window->draw(*edge.getShape());
            }
        }
    }
    if(enabledNodes){
        for(const auto &nodeIt: nodes){
            const Node &node = nodeIt.second;
            window->draw(*node.getShape());
        }
    }
    if(enabledEdges && enabledEdgesText){
        for(const auto &edgeIt: edges){
            const Edge &edge = edgeIt.second;
            if(edge.getText().getString() != "")
                window->draw(edge.getText());
        }
    }
    if(enabledNodes && enabledNodesText){
        for(const auto &nodeIt: nodes){
            const Node &node = nodeIt.second;
            if(node.getText().getString() != "")
                window->draw(node.getText());
        }
    }

    fps_monitor.count();

    if(debug_mode){
        drawDebug();
    }
}

void GraphViewer::drawDebug(){
    window->setView(*debug_view);
    
    string debugInfo;
    debugInfo += "FPS: " + to_string(int(fps_monitor.getFPS())) + "\n";
    
    if(debugInfo[debugInfo.size()-1] == '\n')
        debugInfo = debugInfo.substr(0, debugInfo.size()-1);
    debug_text.setString(debugInfo);
    sf::Vector2f size = sf::Vector2f(window->getSize());
    sf::FloatRect bounds = debug_text.getLocalBounds();
    debug_text.setOrigin(0, bounds.height);
    debug_text.setPosition(sf::Vector2f(0.2*DEBUG_FONT_SIZE, size.y-0.7*DEBUG_FONT_SIZE));

    window->draw(debug_text);
}

void GraphViewer::onResize(){
    sf::Vector2f size = static_cast<sf::Vector2f>(window->getSize());
    recalculateView();
}

void GraphViewer::onScroll(float delta){
    scale *= pow(SCALE_DELTA, -delta);
    recalculateView();
}

void GraphViewer::recalculateView(){
    sf::Vector2f size = static_cast<sf::Vector2f>(window->getSize());
    *view = sf::View(sf::Vector2f(x0, y0), sf::Vector2f(size.x*scale, size.y*scale));
    *debug_view = sf::View(sf::FloatRect(0.0, 0.0, size.x, size.y));

    background_sprite.setPosition(x0, y0);
    auto bounds = background_sprite.getLocalBounds();
    sf::Vector2f scaleVec(scale*size.x/bounds.width, scale*size.y/bounds.height);
    background_sprite.setScale(scaleVec);
}
