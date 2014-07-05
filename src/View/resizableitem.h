#ifndef RESIZABLEITEM_H
#define RESIZABLEITEM_H
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class ResizableItem: public QGraphicsRectItem
{
public:
    ResizableItem(QGraphicsItem* parent = 0);
    void setText(QString str){_text = str;}
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    QRectF boundingRect() const;
private:
    QString _text;
    qreal _adjust;
    qreal _x, _adj_h;
    qreal _y, _adj_w;
    enum ResizeMode{Right=0,Bottom,RightBottom, NoOne} _mode;

//    int kostyl = {0};
};

#endif // RESIZABLEITEM_H
