#include "graphview.h"

GraphView::GraphView(Graph *g) : QGraphicsView(g), g(g)
{
    setSceneRect(-8192, -8192, 16384, 16384);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void GraphView::resetScale()
{
    s = 1.0;
    resetMatrix();
    ensureVisible(this->scene()->itemsBoundingRect());
}

void GraphView::wheelEvent(QWheelEvent *event)
{
    qreal scaleFactor = 1.15;
    if(event->delta() > 0 && s < 8)
    {
        s *= scaleFactor;
        scale(scaleFactor, scaleFactor);
    }
    else if(event->delta() < 0 && s > 0.125)
    {
        s *= 1.0 / scaleFactor;
         scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

}

void GraphView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    //fitInView(sceneRect(), Qt::KeepAspectRatio);
}
