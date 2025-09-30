#include "canvas.h"
#include <QPainter>
#include <QMouseEvent>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent), fastIterations(0), slowIterations(0)
{
    setStyleSheet("background:white;");
}

void Canvas::addPoint(const QPointF &p)
{
    points.append(p);
    fastHull.clear();
    slowHull.clear();
    fastIterations = 0;
    slowIterations = 0;
    update();
}

void Canvas::clearPoints()
{
    points.clear();
    fastHull.clear();
    slowHull.clear();
    fastIterations = 0;
    slowIterations = 0;
    update();
}

void Canvas::setFastHull(const QVector<QPointF> &h, long long iter)
{
    fastHull = h;
    fastIterations = iter;
    update();
}

void Canvas::setSlowHull(const QVector<QPointF> &h, long long iter)
{
    slowHull = h;
    slowIterations = iter;
    update();
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    // titik
    p.setBrush(Qt::black);
    for (auto &pt : points) {
        p.drawEllipse(pt, 4, 4);
    }

    // slow hull (biru)
    if (!slowHull.isEmpty()) {
        p.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        for (int i = 0; i < slowHull.size(); i++) {
            QPointF a = slowHull[i];
            QPointF b = slowHull[(i+1) % slowHull.size()];
            p.drawLine(a, b);
        }
    }

    // fast hull (merah)
    if (!fastHull.isEmpty()) {
        p.setPen(QPen(Qt::red, 2));
        for (int i = 0; i < fastHull.size(); i++) {
            QPointF a = fastHull[i];
            QPointF b = fastHull[(i+1) % fastHull.size()];
            p.drawLine(a, b);
        }
    }

    // teks info
    p.setPen(Qt::darkGreen);
    p.drawText(10, 20, QString("Fast Iterations: %1").arg(fastIterations));
    p.drawText(10, 40, QString("Slow Iterations: %1").arg(slowIterations));
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        addPoint(event->pos());
        emit pointAdded();
    }
}
