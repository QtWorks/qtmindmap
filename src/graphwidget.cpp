#include "include/graphwidget.h"

#include <QApplication>
#include <QDebug>
#include <QPinchGesture>

#include "include/node.h"
#include "include/edge.h"
#include "include/mainwindow.h"

#include <math.h>

GraphWidget::GraphWidget(MainWindow *parent) :
    QGraphicsView(parent),
    m_parent(parent),
    m_gestureZoomInertionBreak(0)
{
    //
    // Настраиваем сцену
    //
    m_scene = new QGraphicsScene(this);
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_scene->setSceneRect(-5000, -5000, 10000, 10000);
    setScene(m_scene);

    //
    // Отслеживаем жесты
    //
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::SwipeGesture);

    //
    // Остальные настройки
    //
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setMinimumSize(400, 400);

    m_graphlogic = new GraphLogic(this);
}

void GraphWidget::newScene()
{
    m_graphlogic->removeAllNodes();
    m_graphlogic->addFirstNode();

    this->show();
}

void GraphWidget::closeScene()
{
    m_graphlogic->removeAllNodes();
    this->hide();

}

GraphLogic *GraphWidget::graphLogic() const
{
    return m_graphlogic;
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(0.1));
}

void GraphWidget::zoomOut()
{
    scaleView(qreal(-0.1));
}

bool GraphWidget::event(QEvent *_event)
{
    bool result = true;

    //
    // Определяем особый обработчик для жестов
    //
    if (_event->type() == QEvent::Gesture) {
        gestureEvent(static_cast<QGestureEvent*>(_event));
    }
    //
    // Прочие стандартные обработчики событий
    //
    else {
        result = QGraphicsView::event(_event);
    }

    return result;
}

void GraphWidget::gestureEvent(QGestureEvent *_event)
{
    //
    // Жест масштабирования
    //
    if (QGesture* gesture = _event->gesture(Qt::PinchGesture)) {
        if (QPinchGesture* pinch = qobject_cast<QPinchGesture *>(gesture)) {
            //
            // При масштабировании за счёт жестов приходится немного притормаживать
            // т.к. события приходят слишком часто и при обработке каждого события
            // пользователю просто невозможно корректно настроить масштаб
            //

            const int INERTION_BREAK_STOP = 4;
            qreal zoomDelta = 0;
            if (pinch->scaleFactor() > 1) {
                if (m_gestureZoomInertionBreak < 0) {
                    m_gestureZoomInertionBreak = 0;
                } else if (m_gestureZoomInertionBreak >= INERTION_BREAK_STOP) {
                    m_gestureZoomInertionBreak = 0;
                    zoomDelta = 0.1;
                } else {
                    ++m_gestureZoomInertionBreak;
                }
            } else if (pinch->scaleFactor() < 1) {
                if (m_gestureZoomInertionBreak > 0) {
                    m_gestureZoomInertionBreak = 0;
                } else if (m_gestureZoomInertionBreak <= -INERTION_BREAK_STOP) {
                    m_gestureZoomInertionBreak = 0;
                    zoomDelta = -0.1;
                } else {
                    --m_gestureZoomInertionBreak;
                }
            } else {
                //
                // При обычной прокрутке часто приходит событие с scaledFactor == 1,
                // так что просто игнорируем их
                //
            }

            //
            // Если необходимо масштабируем и перерисовываем представление
            //
            const bool needZoomIn = pinch->scaleFactor() > pinch->lastScaleFactor();
            const bool needZoomOut = pinch->scaleFactor() < pinch->lastScaleFactor();
            if (zoomDelta > 0 && needZoomIn) {
                zoomIn();
            } else if (zoomDelta < 0 && needZoomOut) {
                zoomOut();
            }

            _event->accept();
        }
    }
}

// MainWindow::keyPressEvent passes all keyevent to here, except
// Ctrl + m (show/hide mainToolBar) and Ctrl + i (show/hide statusIconsToolbar)
void GraphWidget::keyPressEvent(QKeyEvent *_event)
{
    // if GraphLogic handles the event then stop.
    if (m_graphlogic->processKeyEvent(_event))
        return;

    if (_event->key() == Qt::Key_Plus)
    {
        zoomIn();
        return;
    }

    if (_event->key() == Qt::Key_Minus)
    {
        zoomOut();
        return;
    }

    QGraphicsView::keyPressEvent(_event);
}

void GraphWidget::wheelEvent(QWheelEvent* _event)
{
#ifdef Q_OS_MAC
    const qreal ANGLE_DIVIDER = 2.;
#else
    const qreal ANGLE_DIVIDER = 120.;
#endif
    //
    // Собственно масштабирование
    //
    if (_event->modifiers() & Qt::ControlModifier) {
        if (_event->orientation() == Qt::Vertical) {
            //
            // zoomRange > 0 - масштаб увеличивается
            // zoomRange < 0 - масштаб уменьшается
            //
            const qreal zoom = _event->angleDelta().y() / ANGLE_DIVIDER;
            if (zoom > 0) {
                zoomIn();
            } else if (zoom < 0) {
                zoomOut();
            }

            _event->accept();
        }
    }
    //
    // В противном случае прокручиваем редактор
    //
    else {
        QGraphicsView::wheelEvent(_event);
    }
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    painter->fillRect(m_scene->sceneRect(), QApplication::palette().base());
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(m_scene->sceneRect());
}

void GraphWidget::scaleView(qreal factor)
{
    scale(1 + factor, 1 + factor);
}
