#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include "graph.h"

#include <QGraphicsView>

class GraphView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphView(Graph* g);
    void resetScale();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Graph *g;
    qreal s = 1.0;
};

#endif // GRAPHVIEW_H
