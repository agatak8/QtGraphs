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

private:
    Graph *g;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GRAPHVIEW_H
