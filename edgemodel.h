#ifndef EDGEMODEL_H
#define EDGEMODEL_H


#include <QAbstractTableModel>
#include <QMap>
#include <QGraphicsItem>

#include "graph.h"

class EdgeModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    EdgeModel(Graph* graph);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
public slots:
    void addEdge(int index);
    void removeEdge(int index);
private:
    Graph* graph;
};

#endif // EDGEMODEL_H
