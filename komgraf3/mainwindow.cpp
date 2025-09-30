#include "mainwindow.h"
#include <QVBoxLayout>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Convex Hull Comparison");
    resize(800, 600);

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    // canvas
    canvas = new Canvas(this);
    canvas->setMinimumSize(600, 400);

    // tombol
    fastButton = new QPushButton("Run Fast Convex Hull", this);
    slowButton = new QPushButton("Run Slow Convex Hull", this);
    clearButton = new QPushButton("Clear", this);

    connect(fastButton, &QPushButton::clicked, this, &MainWindow::runFastConvexHull);
    connect(slowButton, &QPushButton::clicked, this, &MainWindow::runSlowConvexHull);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearCanvas);

    layout->addWidget(canvas, 1);
    layout->addWidget(fastButton);
    layout->addWidget(slowButton);
    layout->addWidget(clearButton);

    setCentralWidget(central);
}

MainWindow::~MainWindow() {}

void MainWindow::runFastConvexHull()
{
    if (canvas->getPoints().size() < 3) return;
    long long iterations = 0;
    auto hull = convexHull(canvas->getPoints(), iterations);
    canvas->setFastHull(hull, iterations);
}

void MainWindow::runSlowConvexHull()
{
    if (canvas->getPoints().size() < 3) return;
    long long iterations = 0;
    auto hull = slowConvexHull(canvas->getPoints(), iterations);
    canvas->setSlowHull(hull, iterations);
}

void MainWindow::clearCanvas()
{
    canvas->clearPoints();
}

double MainWindow::cross(const QPointF &o, const QPointF &a, const QPointF &b)
{
    return (a.x()-o.x())*(b.y()-o.y()) - (a.y()-o.y())*(b.x()-o.x());
}

// Fast convex hull (Monotone Chain)
QVector<QPointF> MainWindow::convexHull(const QVector<QPointF> &pts, long long &iterations)
{
    iterations = 0;
    QVector<QPointF> a = pts;
    std::sort(a.begin(), a.end(), [](const QPointF &p1, const QPointF &p2){
        return (p1.x()==p2.x()) ? p1.y()<p2.y() : p1.x()<p2.x();
    });

    QVector<QPointF> lower, upper;

    for (auto &p : a) {
        while (lower.size() >= 2 && cross(lower[lower.size()-2], lower[lower.size()-1], p) <= 0) {
            lower.pop_back(); iterations++;
        }
        lower.push_back(p);
    }

    for (int i = a.size()-1; i >= 0; --i) {
        auto p = a[i];
        while (upper.size() >= 2 && cross(upper[upper.size()-2], upper[upper.size()-1], p) <= 0) {
            upper.pop_back(); iterations++;
        }
        upper.push_back(p);
    }

    lower.pop_back();
    upper.pop_back();

    return lower + upper;
}

// Slow convex hull (Brute Force)
QVector<QPointF> MainWindow::slowConvexHull(const QVector<QPointF> &pts, long long &iterations)
{
    iterations = 0;
    QVector<QPointF> hull;

    int n = pts.size();
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            bool valid = true;
            int pos = 0, neg = 0;
            for (int k = 0; k < n; k++) {
                if (k == i || k == j) continue;
                double c = cross(pts[i], pts[j], pts[k]);
                iterations++;
                if (c > 0) pos++;
                if (c < 0) neg++;
                if (pos && neg) { valid = false; break; }
            }
            if (valid) {
                hull.push_back(pts[i]);
                hull.push_back(pts[j]);
            }
        }
    }

    // Hapus duplikat + rapikan dengan convexHull cepat
    std::sort(hull.begin(), hull.end(), [](const QPointF &a, const QPointF &b){
        return (a.x()==b.x()) ? a.y()<b.y() : a.x()<b.x();
    });
    hull.erase(std::unique(hull.begin(), hull.end()), hull.end());

    long long dummy = 0;
    return convexHull(hull, dummy);
}
