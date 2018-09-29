#ifndef MY_DIAGRAM_SCENE_H
#define MY_DIAGRAM_SCENE_H

#include <QGraphicsScene>
#include "my_diagram_text_item.h"
#include "my_diagram_item.h"
//#include"my_diagram_path_item.h"
namespace Ui
{
    class My_diagram_scene;
}

QT_FORWARD_DECLARE_CLASS(QMenu)

class My_diagram_scene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };// В DiagramScene клик мыши может произвести три разных действия: элемент под курсором может быть перемещён, элемент может быть вставлен или стрелка может соединить элементы диаграммы.
    enum Arrows{ Dependency=4, Association, Aggregation, Composition, Generalization,Include,Extend,Transaction };

    My_diagram_scene(QMenu *itemMenu, QObject *parent = 0);
    QFont font() const
        { return myFont; }
    QColor textColor() const
        { return myTextColor; }
    QColor itemColor() const
        { return myItemColor; }
    QColor lineColor() const
        { return myLineColor; }
    void setLineColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setItemColor(const QColor &color);
    void setFont(const QFont &font);


public slots:
    void setMode(Mode mode);
    void setArrows(Arrows arrows);
    void setItemType(My_diagram_item::DiagramType type);
    void editorLostFocus(My_diagram_text_item *item);
    void isModified();

signals:
    void itemInserted(My_diagram_item *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    void normal_text_without_focus(My_diagram_text_item* item);
    bool isItemChange(int type);
    My_diagram_item::DiagramType myItemType;
    QMenu *myItemMenu;
    Mode myMode;
    Arrows myArrow;
    QPointF startPoint;
    QGraphicsLineItem *line;
    QFont myFont;
    My_diagram_text_item *textItem;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;
public:
    bool ismodified;
};
#endif // MY_DIAGRAM_SCENE_H
