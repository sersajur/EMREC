#include "resizableitem.h"
#include <QCursor>
#include <QGraphicsScene>
#include <QPainter>
ResizableItem::ResizableItem(QGraphicsItem *parent):
    QGraphicsRectItem(parent)
{
    _mode = NoOne;
    _adjust = 8;
    setFlag(QGraphicsItem::ItemIsMovable,true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setZValue(3);
}

void ResizableItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if((event->pos().x() > boundingRect().right() - _adjust) && (event->pos().y() < boundingRect().bottom() - _adjust))
        {
            _adj_w = event->pos().x();
            _mode = Right;
            return;
        }
    if((event->pos().x() < boundingRect().right() - _adjust) && (event->pos().y() > boundingRect().bottom() - _adjust))
        {
            _adj_h = event->pos().y();
            _mode = Bottom;
            return;
        }
    if((event->pos().x() > boundingRect().right() - _adjust) && (event->pos().y() > boundingRect().bottom() - _adjust))
        {
            _adj_w = event->pos().x();
            _adj_h = event->pos().y();
            _mode = RightBottom;
            return;
        }
    _x = event->pos().x();
    _y = event->pos().y();

    QGraphicsRectItem::mousePressEvent(event);
    update();
}

void ResizableItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    prepareGeometryChange();
    switch(_mode){
    case Right:{
        setRect(rect().x(), rect().y(), rect().width() + event->pos().x() - _adj_w, rect().height());
        _mode = NoOne;
        break;
    }
    case Bottom:{
        setRect(rect().x(), rect().y(), rect().width(), rect().height() + event->pos().y() - _adj_h);
        _mode = NoOne;
        break;
    }
    case RightBottom:{
        setRect(rect().x(), rect().y(), rect().width() + event->pos().x() - _adj_w, rect().height() + event->pos().y() - _adj_h);
        _mode = NoOne;
        break;
    }
    case NoOne:{
        setPos(x() + event->pos().x() - _x, y() + event->pos().y() - _y);
        break;
    }
    default:
        break;
    }
    update();
    QGraphicsRectItem::mouseReleaseEvent(event);
}

void ResizableItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::SizeAllCursor);
    if((event->pos().x() > boundingRect().right() - _adjust) && (event->pos().y() < boundingRect().bottom() - _adjust))
        setCursor(Qt::SizeHorCursor);
    if((event->pos().x() < boundingRect().right() - _adjust) && (event->pos().y() > boundingRect().bottom() - _adjust))
        setCursor(Qt::SizeVerCursor);
    if((event->pos().x() > boundingRect().right() - _adjust) && (event->pos().y() > boundingRect().bottom() - _adjust))
        setCursor(Qt::SizeFDiagCursor);

    QGraphicsRectItem::hoverMoveEvent(event);
}

void ResizableItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    prepareGeometryChange();
    switch(_mode){
    case Right:{
        setRect(rect().x(), rect().y(), rect().width() + event->pos().x() - _adj_w, rect().height());
        _adj_w = event->pos().x();
        break;
    }
    case Bottom:{
        setRect(rect().x(), rect().y(), rect().width(), rect().height() + event->pos().y() - _adj_h);
        _adj_h = event->pos().y();
        break;
    }
    case RightBottom:{
        setRect(rect().x(), rect().y(), rect().width() + event->pos().x() - _adj_w, rect().height() + event->pos().y() - _adj_h);
        _adj_w = event->pos().x();
        _adj_h = event->pos().y();
        break;
    }
    case NoOne:{
        setPos(x() + event->pos().x() - _x, y() + event->pos().y() - _y);
        break;
    }
    default:
        break;
    }
    update();
}

QRectF ResizableItem::boundingRect() const {
    QRectF tmp = QGraphicsRectItem::boundingRect();
    tmp.setTop(tmp.top()-15);
    return tmp;
}

void ResizableItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   (void)option;
   (void)widget;
   QRectF f(boundingRect());
 //  f.setTop(f.top()-15);
   painter->setPen(pen());
   painter->drawPath(shape());
   painter->setPen(Qt::green);
   painter->drawText(f,_text,QTextOption(Qt::AlignHCenter));
//   if(!kostyl) {
//       kostyl = 20;
//       scene()->update();
//   } else {
//       kostyl--;
//   }
//   scene()->update();
}

