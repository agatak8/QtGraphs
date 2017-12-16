#ifndef GRAPH_H
#define GRAPH_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMap>
#include <QList>
#include <QObject>

class Node : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    class Edge : public QGraphicsLineItem
    {
    public:
        enum { Type = UserType + 2 };
        int type() const override { return Type;}

        Edge(Node* n1, Node* n2);
        void update();
        void remove();
        Node* node1();
        Node* node2();
    private:
        Node *n1, *n2;
    };

    enum { Type = UserType + 1 };
    int type() const override { return Type;}

    Node(int number);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void addEdge(Edge* edge);
    void removeEdge(Edge* edge);
    int getNumber() const;
    void decNumber();
    bool hasEdge(Node* other);
    const QSet<Edge*>& getEdges() const;
signals:
    void positionChanged(int nodeNumber);
private:
    int number;
    QSet<Edge*> edges;
};

class Graph : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode {Move, AddNode, AddEdge, Remove};
    Graph(QObject* parent = 0);
    void clear();
    void loadFromFile(QString fileName);
    void saveToFile(QString fileName);
    Node* addNode(QPointF point);
    Node::Edge* addEdge(Node* n1, Node* n2);
    void removeNode(Node* node);
    void removeEdge(Node::Edge* edge);
    const QList<Node*>& nodes() const;
    const QList<Node::Edge*>& edges() const;
    void setMode(Mode mode);


signals:
    void newNode(int nodeNumber);
    void newEdge(int index);
    void nodeRemoved(int nodeNumber);
    void edgeRemoved(int index);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    Mode mode;
    //QMap<int, Node*> nodesMap;
    QList<Node*> nodesList;
    QList<Node::Edge*> edgesList;
    QGraphicsLineItem *line;

    // QGraphicsScene interface
};





#endif // GRAPH_H
