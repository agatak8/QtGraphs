#ifndef NODEMODEL_H
#define NODEMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QGraphicsItem>

#include "graph.h"

class NodeModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    NodeModel(Graph* graph);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
public slots:
    void addNode(int nodeNumber);
    void removeNode(int nodeNumber);
    void updateNode(int nodeNumber);
private:
    Graph* graph;

};

#endif // NODEMODEL_H
