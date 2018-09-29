#ifndef MY_DIAGRAM_PATH_ITEM_H
#define MY_DIAGRAM_PATH_ITEM_H

namespace Ui
{
    class My_diagram_path_item;
}
class My_diagram_arrow;

#include"QGraphicsPathItem"

class My_diagram_path_item : public QGraphicsPathItem
{
public:
    enum { Type = UserType + 17 };//Перечисление Type является уникальным идентификатором класса. Он используется qgraphicsitem_cast(), который делает динамическое приведение графических элементов. Константа UserType является минимальным значением, которым может быть пользовательский тип графического элемента.
    enum DiagramType { Actor = 11 };

    My_diagram_path_item(DiagramType diagramType, QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);


    void removeArrow(My_diagram_arrow *arrow);
    void removeArrows();
    DiagramType diagramType() const
        { return myDiagramType; }
    void addArrow(My_diagram_arrow *arrow);

    QPainterPath get_path() const
        { return myPath;      }
    QPixmap image() const;
    int type() const
        { return Type;}
    QPolygonF poligon();
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    DiagramType myDiagramType;//Тип элемета (Квадрат, ромб и т.д.)
    QPainterPath myPath;
    QMenu *myContextMenu;
    QList<My_diagram_arrow *> arrows;// список стрелок которые с ним соединены
};

#endif // MY_DIAGRAM_PATH_ITEM_H
