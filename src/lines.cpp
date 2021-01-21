#include "graphviewer.h"

#include <cmath>

GraphViewer::LineShape::LineShape(const sf::Vector2f& u, const sf::Vector2f& v, float w):
    sf::VertexArray(sf::Quads),
    u(u),v(v),w(w)
{}
void GraphViewer::LineShape::setFrom(const sf::Vector2f& u){ this->u = u; }
const sf::Vector2f& GraphViewer::LineShape::getFrom() const{ return u; }
void GraphViewer::LineShape::setTo(const sf::Vector2f& v){ this->v = v; }
const sf::Vector2f& GraphViewer::LineShape::getTo() const{ return v; }
void GraphViewer::LineShape::setWidth(const float& w){ this->w = w; }
const float& GraphViewer::LineShape::getWidth() const{ return w; }

void GraphViewer::LineShape::setFillColor(sf::Color color){
    for(size_t i = 0; i < getVertexCount(); ++i){
        (*this)[i].color = color;
    }
}

GraphViewer::FullLineShape::FullLineShape(const sf::Vector2f& u, const sf::Vector2f& v, float w):
    GraphViewer::LineShape(u,v,w)
{
    process();
}

void GraphViewer::FullLineShape::setFrom (const sf::Vector2f& u){ LineShape::setFrom (u); process(); }
void GraphViewer::FullLineShape::setTo   (const sf::Vector2f& v){ LineShape::setTo   (v); process(); }
void GraphViewer::FullLineShape::setWidth(             float  w){ LineShape::setWidth(w); process(); }

void GraphViewer::FullLineShape::process(){
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

    resize(0);

    append(sf::Vertex(u-edgeNorm));
    append(sf::Vertex(u+edgeNorm));
    append(sf::Vertex(v+edgeNorm));
    append(sf::Vertex(v-edgeNorm));
}

GraphViewer::DashedLineShape::DashedLineShape(const sf::Vector2f& u, const sf::Vector2f& v, float w):
    GraphViewer::LineShape(u,v,w)
{
    process();
}

void GraphViewer::DashedLineShape::setFrom (const sf::Vector2f& u){ LineShape::setFrom (u); process(); }
void GraphViewer::DashedLineShape::setTo   (const sf::Vector2f& v){ LineShape::setTo   (v); process(); }
void GraphViewer::DashedLineShape::setWidth(             float  w){ LineShape::setWidth(w); process(); }

void GraphViewer::DashedLineShape::process(){
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

    resize(0);

    // Most dashes
    int nDashes = magnitude/interDashesSpace;
    for(int i = 0; i < nDashes; ++i){
        sf::Vector2f u1 = u + v_u*interDashesSpace*float(i         );
        sf::Vector2f v1 = u + v_u*interDashesSpace*float(i+dashFill);
        
        append(sf::Vertex(u1-edgeNorm));
        append(sf::Vertex(u1+edgeNorm));
        append(sf::Vertex(v1+edgeNorm));
        append(sf::Vertex(v1-edgeNorm));
    }

    // Last dash
    sf::Vector2f u1 = u + v_u*    interDashesSpace*float(nDashes         )            ;
    sf::Vector2f v1 = u + v_u*min(interDashesSpace*float(nDashes+dashFill), magnitude);
        
    append(sf::Vertex(u1-edgeNorm));
    append(sf::Vertex(u1+edgeNorm));
    append(sf::Vertex(v1+edgeNorm));
    append(sf::Vertex(v1-edgeNorm));
}
