#ifndef MY_DIAGRAM_ITEM_H
#define MY_DIAGRAM_ITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QLineEdit>
#include <QString>
namespace Ui
{
    class My_diagram_item;
}
class My_diagram_arrow;

class My_diagram_item : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 15 };//Перечисление Type является уникальным идентификатором класса. Он используется qgraphicsitem_cast(), который делает динамическое приведение графических элементов. Константа UserType является минимальным значением, которым может быть пользовательский тип графического элемента.
    enum DiagramType { Class_=0, Interface, Option, State, BeginState, EndState, SyncState, Shallow_history_state, Deep_history_state,Transition,Actor };

    My_diagram_item(DiagramType diagramType, QMenu *contextMenu = 0,QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    void removeArrow(My_diagram_arrow *arrow);
    void removeArrows();
    DiagramType diagramType() const
        { return myDiagramType; }
    QPolygonF polygon() const
        { return myPolygon; }
    void addArrow(My_diagram_arrow *arrow);
    QPixmap image() const;
    int type() const
        { return Type;}
    int getTypeElem() const
    {
        return myDiagramType;
    }
    QMenu * getMenu () const
    {
        return myContextMenu;
    }
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
    DiagramType myDiagramType;//Тип элемета (Квадрат, ромб и т.д.)
    QPolygonF myPolygon;
    QMenu *myContextMenu;
    QList<My_diagram_arrow *> arrows;// список стрелок которые с ним соединены
};

QDataStream &operator<<(QDataStream &out, const My_diagram_item &boxItem);
QDataStream &operator>>(QDataStream &in, My_diagram_item *boxItem);

#endif // MY_DIAGRAM_ITEM_H
