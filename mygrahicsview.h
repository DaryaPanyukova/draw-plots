#ifndef MYGRAHICSVIEW_H
#define MYGRAHICSVIEW_H

#include <QGraphicsView>
#include <QObject>

#include <QGraphicsView>
#include <QWidget>
#include <QtWidgets>
#include <QGraphicsScene>
#include <QPoint>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsItem>

class myGrahicsView : public QGraphicsView
{
    Q_OBJECT

public:
    myGrahicsView(QWidget* parent = 0);
    double getScaleFactor() { return factor; }

signals:
    void signal2();

private:
    void wheelEvent(QWheelEvent *event);

protected:
    double factor;
};


#endif // MYGRAHICSVIEW_H



