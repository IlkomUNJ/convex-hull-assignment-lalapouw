#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QVector>
#include <QPointF>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

    void addPoint(const QPointF &p);
    void clearPoints();

    void setFastHull(const QVector<QPointF> &h, long long iter);
    void setSlowHull(const QVector<QPointF> &h, long long iter);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void pointAdded();

private:
    QVector<QPointF> points;
    QVector<QPointF> fastHull;
    QVector<QPointF> slowHull;

    long long fastIterations;
    long long slowIterations;

public:
    const QVector<QPointF>& getPoints() const { return points; }
};

#endif // CANVAS_H
