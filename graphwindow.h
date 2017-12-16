#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>

#include "graph.h"

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent = 0);
    ~GraphWindow();
private slots:
    void addNode();
    void addEdge();
    void remove();
    void move();
    void open();
    void save();
    void newGraph();

private:
    Ui::GraphWindow *ui;
    Graph* graph;

    void populateGraph();
};

#endif // GRAPHWINDOW_H
