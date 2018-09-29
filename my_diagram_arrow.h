#ifndef MY_DIAGRAM_ARROW_H
#define MY_DIAGRAM_ARROW_H

#include <QGraphicsLineItem>
#include "my_diagram_item.h"
QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class My_diagram_arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    enum ArrowType { Dependency=0, Association, Aggregation, Composition, Generalization, Include, Extend, Transaction};
    // Зависимость (линия пунктир на конце стрелка),
    // Ассоциация (Сплошная линия),
    // Агрегация (Линия с пустым ромбом, который находится ближе к классу который является целым,
    //           т.е. который вмещает в себя все те агригуемые части)
    // Композиция (частный случай агрегации только его части не являются самостоятельными программами ЗАКРАШЕННЫЙ РОМБ)
    // Обобщение Сплошная линия с Пустой стрелкой

    My_diagram_arrow(ArrowType arrowType = Dependency,My_diagram_item *startItem = 0, My_diagram_item *endItem = 0,QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
        { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color)
        { myColor = color; }
    My_diagram_item *startItem() const
        { return myStartItem; }
    My_diagram_item *endItem() const
        { return myEndItem; }
    int getArrowType () const
        { return myArrowType; }
public slots:
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget = 0);

private:
    ArrowType myArrowType;
    My_diagram_item *myStartItem;
    My_diagram_item *myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
};

QDataStream &operator<<(QDataStream &out, const My_diagram_arrow &boxItem);
QDataStream &operator>>(QDataStream &in, My_diagram_arrow* boxItem);
#endif // MY_DIAGRAM_ARROW_H
