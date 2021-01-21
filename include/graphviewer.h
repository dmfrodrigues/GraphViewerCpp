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
 * Classe que guarda o grafo e o representa. Todas as suas funções retornam um booleano a indicar
 * se a sua execução decorreu ou não com sucesso.
 */
class GraphViewer {
public:
	/**
	 * Construtor que cria um novo grafo e atribui automaticamente a porta.
	 * Exemplo: GraphViewer *gv = new GraphViewer(600, 600, true); instancia um grafo
	 * 600x600, onde a posição dos nós é determinada automaticamente.
	 *
	 * @param width Inteiro que representa a lagura da área do grafo.
	 * @param height Inteiro que representa a altura da área do grafo.
	 * @param dynamic Booleano que determina se a localização dos nós é automaticamente.
	 * determinado pelo programa (true) ou se deve ser determinado pelo utilizador (false).
	 */
	GraphViewer();

	/**
	 * Função que cria a janela para visualização.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->createWindow(600, 600); abre uma janela 600x600 onde mostra o grafo.
	 *
	 *
	 * @param width Largura da janela a criar.
	 * @param height Altura da janela a criar.
	 */
	bool createWindow(int width, int height);

	/**
	 * Fecha a janela a ser utilizada para visualização.
	 */
	bool closeWindow();

	/**
	 * Acrescenta um nó à representação do grafo, numa posição específica, irrelevante se o grafo
	 * for dinâmico.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer com isDynamic = false:
	 * gv->addNode(0, 1, 2); adiciona um nó com ID 0 na posição (x, y) = (1, 2)
	 *
	 * @param id Identificador único do nó.
	 * @param x Posição horizontal do nó.
	 * @param y Posição vertical do nó.
	 */
	bool addNode(int id, int x, int y);

	/**
	 * Acrescenta uma aresta à representação do grafo.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->addEdge(0, 1, 2, EdgeType::UNDIRECTED); adiciona uma aresta não-dirigida com ID 0
	 * que liga os nós com os IDs 1 e 2
	 *
	 * @param id Identificador único da aresta.
	 * @param v1 Identificador único do nó de origem da aresta.
	 * @param v2 Identificador único do nó de destino da aresta.
	 * @param edgeType EdgeType.DIRECTED caso a aresta seja unidirecional
	 * ou EdgeType.UNDIRECTED caso a aresta seja bidirecional.
	 */
	bool addEdge(int id, int v1, int v2, int edgeType);

	/**
	 * Remove um nó da representação do grafo e todas as arestas ligadas a este.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->removeNode(0) remove o nó com ID 0
	 *
	 * @param id Identificador único do nó a a remover.
	 */
	bool removeNode(int id);

	/**
	 * Remove uma aresta da representação do grafo.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->removeEdge(0) remove a aresta com ID 0
	 *
	 * @param id Identificador único da aresta a remover.
	 */
	bool removeEdge(int id);

	/**
	 * Função que define o texto de um nó.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->setVertexLabel(0, "Isto é um nó"); adiciona o texto "Isto é um nó" ao nó com ID 0
	 *
	 * @param id Identificador único do nó com o texto a alterar.
	 * @param label Novo texto do nó.
	 */
	bool setVertexLabel(int id, string label);

	/**
	 * Função que apaga o texto de um nó, caso o mesmo tenha sido definido anteriormente.
	 *
	 * @param id Identificador único do nó com o texto a apagar.
	 */
	bool clearVertexLabel(int id);

	/**
	 * Função que define o texto de uma aresta.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->setEdgeLabel(0, "Isto é uma aresta"); adiciona o texto "Isto é uma aresta" à aresta com ID 0
	 *
	 * @param id Identificador único da aresta com o texto a alterar.
	 * @param label Novo texto da aresta.
	 */
	bool setEdgeLabel(int id, string label);

	/**
	 * Função que apaga o texto de uma aresta, caso o mesmo tenha sido definido anteriormente.
	 *
	 * @param id Identificador único da aresta com o texto a apagar.
	 */
	bool clearEdgeLabel(int id);

	/**
	 * Função que define a cor de uma aresta.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->setEdgeColor(0, BLUE); modifica a cor da aresta com ID 0 para azul
	 *
	 * @param id Identificador único da aresta com a cor a alterar.
	 * @param color Nova cor da aresta, utilizar as constantes definidas no graphviewer.h para conveniência.
	 */
	bool setEdgeColor(int id, string color);

	bool setEdgeColor(int id, const sf::Color &color);

	/**
	 * Função que apaga a cor de uma aresta, caso tenha sido definida.
	 *
	 * @param id Identificador único da aresta com a cor a apagar.
	 */
	bool clearEdgeColor(int id);

	/**
	 * Função que define se uma aresta é desenhada, ou não, a tracejado.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->setEdgeDashed(0, false); faz com que a aresta com ID 0 seja desenhada a traço contínuo
	 *
	 * @param id Identificador único da aresta com a cor a alterar.
	 * @param dashed Nova cor da aresta, utilizar as constantes definidas no graphviewer.h para conveniência.
	 */
	bool setEdgeDashed(int id, bool dashed);

	/**
	 * Função que define a cor de um nó.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->setVertexColor(0, GREEN); modifica a cor do nó com ID 0 para verde
	 *
	 * @param id Identificador único do nó com a cor a alterar.
	 * @param color Nova cor do nó, utilizar as constantes definidas no graphviewer.h para conveniência.
	 */
	bool setVertexColor(int id, string color);

	bool setVertexColor(int id, const sf::Color &color);

	/**
	 * Função que apaga a cor de um vértice, colocando-a com o valor por omissão.
	 *
	 * @param id Identificador único do nó com a cor a apagar.
	 */
	bool clearVertexColor(int id);

	/**
	 * Função que define o tamanho de um nó.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->setVertexSize(0, 10); modifica o tamanho do nó com ID 0 para 40
	 *
	 * @param id Identificador único do nó com o tamanho a alterar.
	 * @param size Novo tamanho do nó.
	 */
	bool setVertexSize(int id, int size);

	/**
	 * Função que define um ícone para um nó.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->setVertexIcon(0, "icon.png"); faz com que o nó, quando desenhado, não seja um círculo, mas sim a imagem icon.png
	 *
	 * @param id Identificador único do nó com o ícone a alterar.
	 * @param filepath Caminho do ficheiro a utilizar como novo ícone do nó.
	 */
	bool setVertexIcon(int id, string filepath);

	/**
	 * Função que apaga o ícone de um nó, caso tenha sido definido.
	 *
	 * @param id Identificador único do nó com o ícone a apagar.
	 */
	bool clearVertexIcon(int id);

	/**
	 * Função que define a espessura de uma aresta.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->setEdgeThickness(0, 20); modifica a espessura da aresta com ID 0 para 20
	 *
	 * @param id Identificador único da aresta com a espessura a alterar.
	 * @param thickness Nova espessura da aresta, sendo que por base, as
	 * arestas são criadas com a espessura de 1.
	 */
	bool setEdgeThickness(int id, int thickness);

	/**
	 * Função que define o peso de uma aresta na representação do grafo, a ser visualizado
	 * como w: valor_do_peso, seguido de qualquer outro texto associado à aresta.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->setEdgeWeight(0, 20); modifica o peso da aresta com ID 0 para 20
	 *
	 * @param id Identificador único da aresta a modificar.
	 * @param weight Peso associado à aresta.
	 */
	bool setEdgeWeight(int id, int weight);

	/**
	 * Função que define o fluxo de uma aresta na representação do grafo, a ser visualizado
	 * como f: valor_do_fluxo, precedido pelo peso e seguido por texto definido pelo utilizador.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->setEdgeFlow(0, 20); modifica o fluxo da aresta com ID 0 para 20
	 *
	 * @param id Identificador único da aresta a modificar.
	 * @param flow Fluxo associado à aresta.
	 */
	bool setEdgeFlow(int id, int flow);

	bool setVertexOutlineThickness(int id, float outlineThickness);
	
	bool setVertexOutlineColor(int id, string outlineColor);

	bool setVertexOutlineColor(int id, const sf::Color &outlineColor);

	/**
	 * Função que define se as arestas do grafo serão desenhadas como curvas ou retas.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->defineEdgeCurved(false); faz com que as arestas sejam desenhadas como retas
	 *
	 * @param curved Booleano que representa se as arestas serão curvas (true) ou retas (false), sendo o valor por defeito é true.
	 */
	bool defineEdgeCurved(bool curved);

	/**
	 * Função que define a cor global das arestas.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->defineEdgeColor(GRAY); modifica a cor por defeito das arestas para cinzento
	 *
	 * @param color Nova cor das arestas, utilizar as constantes definidas no graphviewer.h para conveniência.
	 */
	bool defineEdgeColor(string color);

	bool defineEdgeColor(const sf::Color &color);

	/**
	 * Função que restaura a cor global das arestas.
	 */
	bool resetEdgeColor();

	/**
	 * Função que define globalmente se as arestas são desenhadas, ou não, a tracejado.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->defineEdgeDashed(true); faz com que por defeito as arestas sejam desenhadas a tracejado
	 *
	 * @param dashed Booleano que representa se as arestas vão estar, ou não, todas a tracejado (o valor por defeito é false).
	 */
	bool defineEdgeDashed(bool dashed);

	/**
	 * Função que define a cor global dos nós.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->defineVertexColor(RED); modifica a cor por defeito dos nós para vermelho
	 *
	 * @param color Nova cor dos nós, utilizar as constantes definidas no graphviewer.h para conveniência.
	 */
	bool defineVertexColor(const sf::Color &color);
	
	bool defineVertexColor(string color);

	/**
	 * Função que restaura a cor global dos nós.
	 */
	bool resetVertexColor();

	/**
	 * Função que define o tamanho global dos nós.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->defineVertexSize(20); modifica o tamanho por defeito dos nós para 20
	 *
	 * @param size Nova cor dos nós, utilizar as constantes definidas no graphviewer.h para conveniência.
	 */
	bool defineVertexSize(int size);

	/**
	 * Função que define um ícone global para os nós.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->defineVertexIcon("icon.gif"); faz com que por defeito os nós, quando desenhados,
	 * não sejam um círculo, mas sim a imagem icon.gif
	 *
	 * @param filepath Caminho do ficheiro a utilizar como novo ícone do nó.
	 */
	bool defineVertexIcon(string filepath);

	/**
	 * Função que apaga o ícone global para os nós.
	 */
	bool resetVertexIcon();

	bool defineVertexOutlineThickness(float outlineThickness);

	bool resetVertexOutlineThickness();

	bool defineVertexOutlineColor(string outlineColor);

	bool defineVertexOutlineColor(const sf::Color &outlineColor);

	bool resetVertexOutlineColor();

	/**
	 * Função que altera a imagem de fundo do grafo.
	 * Exemplo, para um apontador gv onde foi instanciada a classe GraphViewer:
	 * gv->setBackGround("fundo.png"); faz com que o fundo da janela seja a imagem fundo.png,
	 * em vez de cinzento
	 *
	 * @param path Caminho para o ficheiro com a imagem.
	 */
	bool setBackground(string path);

	/**
	 * Apaga a imagem de fundo do grafo, se tiver sido previamente definida.
	 */
	bool clearBackground();

	void join();

	void setEnabledNodes(bool b);
	void setEnabledEdges(bool b);
	void setEnabledNodesText(bool b);
	void setEnabledEdgesText(bool b);

	void setZipEdges(bool b);

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

	sf::Color edgeColor = sf::Color::Black;
	bool edgeDashed = false;

	class LineShape;
	class FullLineShape;
	class DashedLineShape;

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
