#include "edgemodel.h"

EdgeModel::EdgeModel(Graph* graph) : QAbstractTableModel(graph), graph(graph)
{
    connect(graph, SIGNAL(newEdge(int)), this, SLOT(addEdge(int)));
    connect(graph, SIGNAL(edgeRemoved(int)), this, SLOT(removeEdge(int)));
}

int EdgeModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
          return 0;
    return graph->edges().count();
}

int EdgeModel::columnCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
          return 0;
    return 2;
}

QVariant EdgeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= graph->edges().size())
        return QVariant();
    if (index.column() >= 2)
        return QVariant();

    if (role == Qt::DisplayRole)
        switch(index.column())
        {
            case 0:
                return graph->edges()[index.row()]->node1()->getNumber();
            case 1:
                return graph->edges()[index.row()]->node2()->getNumber();
            default:
                return QVariant();
        }
    else
        return QVariant();
}

QVariant EdgeModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const
 {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal)
         switch(section)
         {
            case 0:
             return QString("Node1");
            case 1:
             return QString("Node2");
            default:
             return QVariant();
         }
     else
         return QVariant();
}

void EdgeModel::addEdge(int index)
{
    beginInsertRows(QModelIndex(), index, index);
    endInsertRows();
}

void EdgeModel::removeEdge(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    endRemoveRows();
}
