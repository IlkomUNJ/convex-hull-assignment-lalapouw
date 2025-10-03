#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "canvas.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void runFastConvexHull();
    void runSlowConvexHull();
    void clearCanvas();

private:
    Canvas *canvas;
    QPushButton *fastButton;
    QPushButton *slowButton;
    QPushButton *clearButton;

    // helper
    static double cross(const QPointF &o, const QPointF &a, const QPointF &b);
    QVector<QPointF> convexHull(const QVector<QPointF> &pts, long long &iterations);
    QVector<QPointF> slowConvexHull(const QVector<QPointF> &pts, long long &iterations);
};

#endif // MAINWINDOW_H
