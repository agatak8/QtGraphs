#include "graphview.h"

GraphView::GraphView(Graph *g) : QGraphicsView(g), g(g)
{
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void GraphView::resetScale()
{
    resetMatrix();
    ensureVisible(this->scene()->itemsBoundingRect());
}

void GraphView::wheelEvent(QWheelEvent *event)
{
    qreal scaleFactor = 1.15;
    if(event->delta() > 0)
    {
        scale(scaleFactor, scaleFactor);
    }
    else
    {
         scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

}

void GraphView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    //fitInView(sceneRect(), Qt::KeepAspectRatio);
}
