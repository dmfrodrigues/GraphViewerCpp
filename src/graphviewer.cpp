#include "graphviewer.h"

#include <cmath>
#include <iostream>

using namespace std;
using namespace sf;

std::mutex GraphViewer::createWindowMutex;

const GraphViewer::Color GraphViewer::BLACK   = sf::Color::Black;
const GraphViewer::Color GraphViewer::WHITE   = sf::Color::White;
const GraphViewer::Color GraphViewer::RED     = sf::Color::Red;
const GraphViewer::Color GraphViewer::GREEN   = sf::Color::Green;
const GraphViewer::Color GraphViewer::BLUE    = sf::Color::Blue;
const GraphViewer::Color GraphViewer::YELLOW  = sf::Color::Yellow;
const GraphViewer::Color GraphViewer::MAGENTA = sf::Color::Magenta;
const GraphViewer::Color GraphViewer::CYAN    = sf::Color::Cyan;
const GraphViewer::Color GraphViewer::PINK  (255, 192, 203);
const GraphViewer::Color GraphViewer::ORANGE(255, 128,   0);
const GraphViewer::Color GraphViewer::GRAY  (128, 128, 128);
const GraphViewer::Color GraphViewer::LIGHT_GRAY(192, 192, 192);
const GraphViewer::Color GraphViewer::DARK_GRAY(64, 64, 64);

void GraphViewer::ZipEdges::append(const VertexArray &a){
    for(size_t i = 0; i < a.getVertexCount(); ++i){
        vertices.push_back(a[i]);
    }
}
const vector<Vertex>& GraphViewer::ZipEdges::getVertices() const{ return vertices; }

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

Font getFont(const string &path){
    Font font;
    string fontPath = getPath(__FILE__)+path;
    if(!font.loadFromFile(fontPath))
        throw runtime_error("Failed to load font from file; check if font exists");
    return font;
}
const Font GraphViewer::DEBUG_FONT = getFont("/../resources/fonts/inconsolata.ttf");
const Font GraphViewer::FONT       = getFont("/../resources/fonts/arial.ttf");

GraphViewer::GraphViewer():
    debug_text("", DEBUG_FONT, DEBUG_FONT_SIZE)
{
    debug_text.setFillColor(Color::Black);
    debug_text.setStyle(Text::Bold);
}

void GraphViewer::createWindow(unsigned int width, unsigned int height){
    if(window != nullptr) throw runtime_error("Window was already created");
    if(width  == 0) width  = DEFAULT_WIDTH ;
    if(height == 0) height = DEFAULT_HEIGHT;
    this->width  = width;
    this->height = height;

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
    if(nodes.count(node.getId()))
        throw invalid_argument("A node with that ID already exists");
    return (nodes[node.getId()] = node);
}

GraphViewer::Node& GraphViewer::getNode(GraphViewer::id_t id){
    return nodes.at(id);
}

GraphViewer::Edge& GraphViewer::addEdge(const Edge &edge){
    lock_guard<mutex> lock(graphMutex);
    if(edges.count(edge.getId()))
        throw invalid_argument("An edge with that ID already exists");
    Edge &ret = (edges[edge.getId()] = edge);
    if(zipEdges) updateZip();
    return ret;
}

void GraphViewer::removeNode(GraphViewer::id_t id){
    lock_guard<mutex> lock(graphMutex);
    for(auto it = edges.cbegin(); it != edges.cend(); ){
        if(
            it->second.getFrom()->getId() == id ||
            it->second.getTo  ()->getId() == id
        ){
            edges.erase(it++);
        } else {
            ++it;
        }
    }
    if(nodes.erase(id) == 0)
        throw out_of_range("No such node ID "+to_string(id));
}

void GraphViewer::removeEdge(GraphViewer::id_t id){
    lock_guard<mutex> lock(graphMutex);
    if(edges.erase(id) == 0)
        throw out_of_range("No such edge ID "+to_string(id));
    if(zipEdges) updateZip();
}

void GraphViewer::setBackground(const string &path){
    lock_guard<mutex> lock(graphMutex);
    background_texture.loadFromFile(path);
    background_sprite.setTexture(background_texture);
    auto bounds = background_sprite.getLocalBounds();
    background_sprite.setOrigin(bounds.width/2.0, bounds.height/2.0);
}

void GraphViewer::clearBackground(){
    lock_guard<mutex> lock(graphMutex);
    background_texture = Texture();
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
    ContextSettings settings;
    settings.antialiasingLevel = 8;
    GraphViewer::createWindowMutex.lock();
    window = new RenderWindow(VideoMode(this->width, this->height), "GraphViewer", Style::Default, settings);
    GraphViewer::createWindowMutex.unlock();

    view = new View(window->getDefaultView());
    debug_view = new View(window->getDefaultView());

    center = Vector2f(this->width/2.0, this->height/2.0);

    bool isLeftClickPressed = false;
    Vector2f centerInitial;
    Vector2f posMouseInitial;

    recalculateView();

    while (window->isOpen()){
        Event event;
        while (window->pollEvent(event)){
            switch(event.type){
                case Event::Closed            : window->close(); break;
                case Event::Resized           : onResize(); break;
                case Event::MouseWheelScrolled: onScroll(event.mouseWheelScroll.delta); break;
                case Event::MouseButtonPressed:
                    switch(event.mouseButton.button){
                        case Mouse::Button::Left:
                            isLeftClickPressed = true;
                            centerInitial = center;
                            posMouseInitial = Vector2f(
                                event.mouseButton.x,
                                event.mouseButton.y
                            );
                            break;
                        default: break;
                    }
                    break;
                case Event::MouseButtonReleased:
                    switch(event.mouseButton.button){
                        case Mouse::Button::Left:
                            isLeftClickPressed = false;
                            break;
                        default: break;
                    }
                    break;
                case Event::MouseMoved:
                    if(isLeftClickPressed){
                        Vector2f mouse_pos(event.mouseMove.x, event.mouseMove.y);
                        center = centerInitial - (mouse_pos - posMouseInitial)*scale;
                        recalculateView();
                    }
                    break;
                case Event::TextEntered:
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
    window->clear(Color::White);

    window->setView(*view);
    window->draw(background_sprite);
    if(enabledEdges){
        if(zipEdges){
            const vector<Vertex> &v = zip.getVertices();
            window->draw(&v[0], v.size(), Quads);
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
    Vector2f size = Vector2f(window->getSize());
    FloatRect bounds = debug_text.getLocalBounds();
    debug_text.setOrigin(0, bounds.height);
    debug_text.setPosition(Vector2f(0.2*DEBUG_FONT_SIZE, size.y-0.7*DEBUG_FONT_SIZE));

    window->draw(debug_text);
}

void GraphViewer::onResize(){
    recalculateView();
}

void GraphViewer::onScroll(float delta){
    scale *= pow(SCALE_DELTA, -delta);
    recalculateView();
}

void GraphViewer::recalculateView(){
    Vector2f size = static_cast<Vector2f>(window->getSize());
    *view = View(center, size*scale);
    *debug_view = View(FloatRect(0.0, 0.0, size.x, size.y));

    background_sprite.setPosition(center);
    auto bounds = background_sprite.getLocalBounds();
    Vector2f scaleVec(scale*size.x/bounds.width, scale*size.y/bounds.height);
    background_sprite.setScale(scaleVec);
}
