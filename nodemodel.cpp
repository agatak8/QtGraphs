#include "nodemodel.h"

NodeModel::NodeModel(Graph* graph) : QAbstractTableModel(graph), graph(graph)
{
    connect(graph, SIGNAL(newNode(int)), this, SLOT(addNode(int)));
    connect(graph, SIGNAL(nodeRemoved(int)), this, SLOT(removeNode(int)));
    foreach(Node* node, graph->nodes())
        connect(node, SIGNAL(positionChanged(int)), this, SLOT(updateNode(int)));
}

int NodeModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
          return 0;
    return graph->nodes().count();
}

int NodeModel::columnCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
          return 0;
    return 3;
}

QVariant NodeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= graph->nodes().size())
        return QVariant();
    if (index.column() >= 3)
        return QVariant();

    if (role == Qt::DisplayRole)
        switch(index.column())
        {
            case 0:
                return index.row();
            case 1:
                return graph->nodes()[index.row()]->x();
            case 2:
                return graph->nodes()[index.row()]->y();
            default:
                return QVariant();
        }
    else
        return QVariant();
}

QVariant NodeModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const
 {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal)
         switch(section)
         {
            case 0:
             return QString("Node #");
            case 1:
             return QString("X");
            case 2:
             return QString("Y");
            default:
             return QVariant();
         }
     else
         return QVariant();
}

void NodeModel::addNode(int nodeNumber)
{
    beginInsertRows(QModelIndex(), nodeNumber, nodeNumber);
    connect(graph->nodes()[nodeNumber], SIGNAL(positionChanged(int)), this, SLOT(updateNode(int)));
    endInsertRows();
}

void NodeModel::removeNode(int nodeNumber)
{
    beginRemoveRows(QModelIndex(), nodeNumber, nodeNumber);
    endRemoveRows();
}

void NodeModel::updateNode(int nodeNumber)
{
    QModelIndex a = createIndex(nodeNumber, 1);
    QModelIndex b = createIndex(nodeNumber, 2);
    emit dataChanged(a, b);
}
