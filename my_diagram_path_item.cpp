#include "my_diagram_path_item.h"
#include "my_diagram_arrow.h"
#include <QtGui>

My_diagram_path_item::My_diagram_path_item(DiagramType diagramType, QMenu *contextMenu,QGraphicsItem *parent , QGraphicsScene *scene )
        : QGraphicsPathItem(parent, scene)
{
    myDiagramType = diagramType;
    myContextMenu = contextMenu;
    switch (myDiagramType)
    {
        case Actor:
           {
                myPath.addEllipse(QPoint(0,-75),25,25);
                myPath.moveTo(0,-50);
                myPath.lineTo(0,20);
                myPath.lineTo(-45,100);
                myPath.moveTo(0,20);
                myPath.lineTo(45,100);
                myPath.moveTo(-45,-15);
                myPath.lineTo(45,-15);
                break;
            }
    }
    setPath(myPath);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable);
}
QPolygonF My_diagram_path_item::poligon()
{
    QPolygonF myPolygon;
              myPolygon << QPointF(-45, -100) << QPointF(45, -100)
                          << QPointF(45, 100) << QPointF(-45, 100)
                          << QPointF(-45, -100);
              return myPolygon;
}

void My_diagram_path_item::removeArrow(My_diagram_arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}
void My_diagram_path_item::removeArrows()
{
    foreach (My_diagram_arrow *arrow, arrows)
    {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
void My_diagram_path_item::addArrow(My_diagram_arrow *arrow)
{
    arrows.append(arrow);
}

QPixmap My_diagram_path_item::image() const
{
        QPixmap pixmap(250, 250);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setPen(QPen(Qt::black, 8));
        painter.translate(125, 125);
        painter.drawPath(myPath);
        return pixmap;
}

void My_diagram_path_item::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant My_diagram_path_item::itemChange(GraphicsItemChange change,const QVariant &value)//Если элемент переместился, нам надо обновить позицию стрелок, которые с ним соединены. Реализация QGraphicsItem ничего не делает, поэтому мы можем просто вернуть значение value.
{
    if (change == QGraphicsItem::ItemPositionChange)
    {
        foreach (My_diagram_arrow *arrow, arrows)
        {
            arrow->updatePosition();
        }
    }
    return value;
}
