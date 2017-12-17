#include "graphwindow.h"
#include "ui_graphwindow.h"
#include "graph.h"
#include "nodemodel.h"
#include "edgemodel.h"
#include "graphview.h"

#include <QDir>
#include <QFileDialog>
#include <QHeaderView>

GraphWindow::GraphWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);

    graph = new Graph(this);
    connect(ui->actionAdd_node, SIGNAL(triggered()), this, SLOT(addNode()));
    connect(ui->actionAdd_edge, SIGNAL(triggered()), this, SLOT(addEdge()));
    connect(ui->actionRemove_item, SIGNAL(triggered()), this, SLOT(remove()));
    connect(ui->actionMove, SIGNAL(triggered()), this, SLOT(move()));
    connect(ui->actionSelect, SIGNAL(triggered()), this, SLOT(select()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newGraph()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    NodeModel* nodeModel = new NodeModel(graph);
    ui->nodesView->setModel(nodeModel);
    ui->nodesView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    EdgeModel* edgeModel = new EdgeModel(graph);
    ui->edgesView->setModel(edgeModel);
    ui->edgesView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    graphView = new GraphView(graph);
    QGridLayout* gl = qobject_cast<QGridLayout*>(ui->centralWidget->layout());
    gl->addWidget(graphView, 1, 0, 3, 1);
    move();

}

GraphWindow::~GraphWindow()
{
    delete ui;
}

void GraphWindow::addNode()
{
    graphView->setDragMode(QGraphicsView::DragMode::NoDrag);
    graphView->setCursor(Qt::CrossCursor);
    graph->setMode(Graph::Mode::AddNode);
}

void GraphWindow::addEdge()
{
    graphView->setDragMode(QGraphicsView::DragMode::NoDrag);
    graphView->setCursor(Qt::SizeFDiagCursor);
    graph->setMode(Graph::Mode::AddEdge);
}

void GraphWindow::remove()
{
    graph->removeSelected();
}

void GraphWindow::move()
{
    graphView->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
    graph->setMode(Graph::Mode::Move);
}

void GraphWindow::select()
{
    graphView->setDragMode(QGraphicsView::DragMode::RubberBandDrag);
    graph->setMode(Graph::Mode::Select);
}

void GraphWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                      tr("Open graph"), "", tr("Text files (*.txt)"));
    if(fileName != QString())
    {
        graph->loadFromFile(fileName);
        //graphView->ensureVisible(graph->itemsBoundingRect());
        graphView->fitInView(graph->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
}

void GraphWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save graph"), "", tr("Text files (*.txt)"));
    if(fileName != QString())
        graph->saveToFile(fileName);
}

void GraphWindow::newGraph()
{
    graph->clear();
    graphView->resetScale();
}
