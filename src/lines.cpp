#include "graphviewer.h"

#include <cmath>

using namespace std;
using namespace sf;

GraphViewer::LineShape::LineShape(const Vector2f& u, const Vector2f& v, float w):
    VertexArray(Quads),
    u(u),v(v),w(w)
{}
void GraphViewer::LineShape::setFrom(const Vector2f& u){ this->u = u; }
const Vector2f& GraphViewer::LineShape::getFrom() const{ return u; }
void GraphViewer::LineShape::setTo(const Vector2f& v){ this->v = v; }
const Vector2f& GraphViewer::LineShape::getTo() const{ return v; }
void GraphViewer::LineShape::setWidth(float w){ this->w = w; }
float GraphViewer::LineShape::getWidth() const{ return w; }

void GraphViewer::LineShape::setFillColor(Color color){
    for(size_t i = 0; i < getVertexCount(); ++i){
        (*this)[i].color = color;
    }
}

GraphViewer::FullLineShape::FullLineShape(const Vector2f& u, const Vector2f& v, float w):
    GraphViewer::LineShape(u,v,w)
{
    process();
}

void GraphViewer::FullLineShape::setFrom (const Vector2f& u){ LineShape::setFrom (u); process(); }
void GraphViewer::FullLineShape::setTo   (const Vector2f& v){ LineShape::setTo   (v); process(); }
void GraphViewer::FullLineShape::setWidth(             float  w){ LineShape::setWidth(w); process(); }

void GraphViewer::FullLineShape::process(){
    const Vector2f &u = getFrom();
    const Vector2f &v = getTo  ();
    Vector2f v_u = v-u;
    float magnitude = sqrt(v_u.x*v_u.x + v_u.y*v_u.y);
    v_u /= magnitude;

    Vector2f edgeV = v-u;
    Vector2f edgeNorm(-edgeV.y, edgeV.x);
    float magnitudeNorm = sqrt(edgeNorm.x*edgeNorm.x + edgeNorm.y*edgeNorm.y);
    edgeNorm /= magnitudeNorm;
    edgeNorm *= (getWidth()/2);

    resize(0);

    append(Vertex(u-edgeNorm));
    append(Vertex(u+edgeNorm));
    append(Vertex(v+edgeNorm));
    append(Vertex(v-edgeNorm));
}

GraphViewer::DashedLineShape::DashedLineShape(const Vector2f& u, const Vector2f& v, float w):
    GraphViewer::LineShape(u,v,w)
{
    process();
}

void GraphViewer::DashedLineShape::setFrom (const Vector2f& u){ LineShape::setFrom (u); process(); }
void GraphViewer::DashedLineShape::setTo   (const Vector2f& v){ LineShape::setTo   (v); process(); }
void GraphViewer::DashedLineShape::setWidth(             float  w){ LineShape::setWidth(w); process(); }

void GraphViewer::DashedLineShape::process(){
    float interDashesSpace = 4.0*getWidth();
    const Vector2f &u = getFrom();
    const Vector2f &v = getTo  ();
    Vector2f v_u = v-u;
    float magnitude = sqrt(v_u.x*v_u.x + v_u.y*v_u.y);
    v_u /= magnitude;

    Vector2f edgeV = v-u;
    Vector2f edgeNorm(-edgeV.y, edgeV.x);
    float magnitudeNorm = sqrt(edgeNorm.x*edgeNorm.x + edgeNorm.y*edgeNorm.y);
    edgeNorm /= magnitudeNorm;
    edgeNorm *= (getWidth()/2);

    resize(0);

    // Most dashes
    int nDashes = magnitude/interDashesSpace;
    for(int i = 0; i < nDashes; ++i){
        Vector2f u1 = u + v_u*interDashesSpace*float(i         );
        Vector2f v1 = u + v_u*interDashesSpace*float(i+dashFill);
        
        append(Vertex(u1-edgeNorm));
        append(Vertex(u1+edgeNorm));
        append(Vertex(v1+edgeNorm));
        append(Vertex(v1-edgeNorm));
    }

    // Last dash
    Vector2f u1 = u + v_u*    interDashesSpace*float(nDashes         )            ;
    Vector2f v1 = u + v_u*min(interDashesSpace*float(nDashes+dashFill), magnitude);
        
    append(Vertex(u1-edgeNorm));
    append(Vertex(u1+edgeNorm));
    append(Vertex(v1+edgeNorm));
    append(Vertex(v1-edgeNorm));
}
