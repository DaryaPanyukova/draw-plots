#include "mygrahicsview.h"


myGrahicsView::myGrahicsView(QWidget *parent ) : QGraphicsView(parent)
{
    factor = 50;
}

void myGrahicsView::wheelEvent(QWheelEvent *event)
{
    double scaleFactor = 1.5;
   if (event->delta() > 0) {
       if (factor > 200) {
           return;
       }
       factor *= scaleFactor;
       emit signal2();
      } else {
       if (factor < 25) {
           return;
       }
        factor /= scaleFactor;
        emit signal2();
      }
}



