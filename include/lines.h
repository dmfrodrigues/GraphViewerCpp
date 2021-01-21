#ifndef GV_LINES_H_INCLUDED
#define GV_LINES_H_INCLUDED

class GraphViewer::LineShape: public sf::VertexArray {
public:
    explicit LineShape(const sf::Vector2f& u, const sf::Vector2f& v, float w);

    void setFrom(const sf::Vector2f& u);
    const sf::Vector2f& getFrom() const;

    void setTo(const sf::Vector2f& v);
    const sf::Vector2f& getTo() const;

    void setWidth(const float& w);
    const float& getWidth() const;

    void setFillColor(sf::Color color);

private :
    sf::Vector2f u, v;
    float w;
};

class GraphViewer::FullLineShape: public LineShape {
public:
    explicit FullLineShape(const sf::Vector2f& u, const sf::Vector2f& v, float w);

    void setFrom (const sf::Vector2f& u);
    void setTo   (const sf::Vector2f& v);
    void setWidth(             float  w);

    void process();
};

class GraphViewer::DashedLineShape: public GraphViewer::LineShape {
private:
    float dashFill = 0.5;
public:
    explicit DashedLineShape(const sf::Vector2f& u, const sf::Vector2f& v, float w);

    void setFrom (const sf::Vector2f& u);
    void setTo   (const sf::Vector2f& v);
    void setWidth(             float  w);

    void process();
};

#endif // GV_LINES_H_INCLUDED
