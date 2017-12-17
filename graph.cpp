#include "graph.h"

#include <QTextStream>
#include <fstream>
#include <iostream>

Graph::Graph(QObject *parent) : QGraphicsScene(parent), line(Q_NULLPTR), startNode(Q_NULLPTR)
{
    setBackgroundBrush(Qt::white);
}

Node* Graph::addNode(QPointF point)
{
    Node* node = new Node(nodesList.count());
    nodesList << node;
    node->setPos(point);
    addItem(node);
    emit newNode(node->getNumber());
    return node;
}

Node::Edge* Graph::addEdge(Node* n1, Node* n2)
{
    if(n1->hasEdge(n2) || n1 == n2) return Q_NULLPTR;
    Node::Edge* edge = (n1->getNumber() <= n2->getNumber()) ? new Node::Edge(n1, n2) : new Node::Edge(n2, n1);
    edgesList << edge;
    addItem(edge);
    emit newEdge(edgesList.count() - 1);
    return edge;
}

void Graph::removeNode(Node *node)
{
    for (int i = node->getNumber() + 1; i < nodesList.count(); ++i)
    {
        nodesList[i]->decNumber();
    }
    nodesList.removeAt(node->getNumber());
    foreach(Node::Edge* edge, node->getEdges())
    {
        removeEdge(edge);
    }
    removeItem(node);
    emit nodeRemoved(node->getNumber());
    delete node;
}

void Graph::removeEdge(Node::Edge *edge)
{
    int i = edgesList.indexOf(edge);
    edgesList.removeAt(i);
    edge->remove();
    removeItem(edge);
    emit edgeRemoved(i);
    delete edge;
}

const QList<Node*> &Graph::nodes() const
{
    return nodesList;
}

const QList<Node::Edge *> &Graph::edges() const
{
    return edgesList;
}

void Graph::setMode(Graph::Mode mode)
{
    this->mode = mode;
}

Graph::Mode Graph::getMode() const
{
    return mode;
}

void Graph::clear()
{
    for(int i = nodesList.count()-1; i >= 0; --i)
    {
        removeNode(nodesList[i]);
    }
}

void Graph::loadFromFile(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
    clear();

    QTextStream in(&file);
    int n_vertices, vertex;
    qreal x, y;

    in >> n_vertices;
    for(int i = 0; i < n_vertices; ++i)
    {
        addNode(QPointF(0, 0));
    }
    for(int i = 0; i < n_vertices; ++i)
    {
        in >> vertex;
        in >> x;
        in >> y;
        nodesList[vertex]->setPos(x,y);
    }
    int n1, n2;
    while (!in.atEnd()) {
        in >> n1;
        in >> n2;
        addEdge(nodesList[n1], nodesList[n2]);
    }

}

void Graph::saveToFile(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
    QTextStream out(&file);
    out << nodesList.count() << "\n";
    for(int i = 0; i < nodesList.count(); ++i)
    {
        out << i << " " << nodesList[i]->x() << " " << nodesList[i]->y() << "\n";
    }
    for (int i = 0; i < edgesList.count(); ++i)
    {
        out << edgesList[i]->node1()->getNumber() << " " << edgesList[i]->node2()->getNumber() << "\n";
    }
}

void Graph::removeSelected()
{
    // delete edges first to ensure integrity
    // if we first deleted nodes we might delete a node that has edges selected
    foreach (QGraphicsItem* item, selectedItems())
    {
        if(item->type() == Node::Edge::Type)
        {
            Node::Edge* edge = qgraphicsitem_cast<Node::Edge *>(item);
            removeEdge(edge);
        }
    }
    foreach (QGraphicsItem* item, selectedItems())
    {
        if(item->type() == Node::Type)
        {
            Node* node = qgraphicsitem_cast<Node *>(item);
            removeNode(node);
        }
    }
}


void Graph::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        switch (mode) {
            case AddNode:
                addNode(QPointF(mouseEvent->scenePos()));
                break;
            case AddEdge:
                foreach(QGraphicsItem* item, items(mouseEvent->scenePos()))
                {
                    if(item->type() == Node::Type)
                    {
                        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                                    mouseEvent->scenePos()));
                        line->setPen(QPen(Qt::black, 2));
                        addItem(line);
                        startNode = qgraphicsitem_cast<Node*>(item);
                        break;
                    }
                }
                return;
            default:
                break;
        }
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void Graph::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mode == AddEdge && line != Q_NULLPTR) {
        QLineF newLine(startNode->pos(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void Graph::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (startNode != Q_NULLPTR && line != Q_NULLPTR && mode == AddEdge) {
        foreach(QGraphicsItem* item, items(line->line().p2()))
        {
            if(item->type() == Node::Type)
            {
                addEdge(startNode, qgraphicsitem_cast<Node*>(item));
                break;
            }
        }
        removeItem(line);
        delete(line);
    }
    line = Q_NULLPTR;
    startNode = Q_NULLPTR;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

// NODE

Node::Node(int number) : QGraphicsEllipseItem(-16, -16, 32, 32), number(number)
{
    setBrush(QBrush(Qt::black));
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edges)
            edge->update();
        emit positionChanged(number);
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsEllipseItem::paint(painter, option, widget);
    painter->setPen(QPen(Qt::white));
    painter->drawText(QRectF(-16, -16, 32, 32), Qt::AlignVCenter | Qt::AlignHCenter, QString("%1").arg(number));
}

void Node::addEdge(Edge *edge)
{
    edges << edge;
}

void Node::removeEdge(Node::Edge *edge)
{
    edges.remove(edge);
}

int Node::getNumber() const
{
    return number;
}

void Node::decNumber()
{
    --number;
    this->update();
}

bool Node::hasEdge(Node *other)
{
    foreach(Edge* edge, edges)
    {
        if(edge->node1() == other || edge->node2() == other)
            return true;
    }
    return false;
}

const QSet<Node::Edge *> &Node::getEdges() const
{
    return edges;
}

// EDGE

Node::Edge::Edge(Node* n1, Node* n2) : n1(n1), n2(n2)
{
    setZValue(-1);
    setPen(QPen(QBrush(Qt::black), 2));
    n1->addEdge(this);
    n2->addEdge(this);
    update();
    setFlags(ItemIsSelectable);
}

void Node::Edge::update()
{
    prepareGeometryChange();
    setLine(0, 0, n2->x()-n1->x(), n2->y()-n1->y());
    setPos(n1->x(), n1->y());
}

void Node::Edge::remove()
{
    n1->removeEdge(this);
    n2->removeEdge(this);
}

Node *Node::Edge::node1()
{
    return n1;
}

Node *Node::Edge::node2()
{
    return n2;
}

