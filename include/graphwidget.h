#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

#include "graphlogic.h"

class MainWindow;
class GraphLogic;
class QGestureEvent;

/** Responsibilities:
  * - Handle scene zoom in/out events
  * - Close scene (clean), new scene (clean & add first node)
  * - Pass key events to GraphLogic
  */
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:

    GraphWidget(MainWindow *parent = 0);

    void newScene();
    void closeScene();

    GraphLogic *graphLogic() const;

public slots:
    void zoomIn();
    void zoomOut();

signals:

    void  notification(const QString &msg);

protected:
    /**
     * @brief Переопределяем для обработки жестов
     */
    bool event(QEvent* _event);

    /**
     * @brief Обрабатываем жест увеличения масштаба
     */
    void gestureEvent(QGestureEvent* _event);

    void keyPressEvent(QKeyEvent* _event);
    void wheelEvent(QWheelEvent* _event);
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    void scaleView(qreal factor);

private:
    MainWindow *m_parent;
    QGraphicsScene *m_scene;
    GraphLogic *m_graphlogic;

    /**
     * @brief Инерционный тормоз масштабирования при помощи жестов
     */
    int m_gestureZoomInertionBreak;
};

#endif // GRAPHWIDGET_H
