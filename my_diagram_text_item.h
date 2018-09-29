#ifndef MY_DIAGRAM_TEXT_ITEM_H
#define MY_DIAGRAM_TEXT_ITEM_H

#include <QGraphicsTextItem>
#include <QPen>
namespace Ui
{
    class My_diagram_text_item;
}

class My_diagram_text_item : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };
    enum ItemTextType { Label, Margin };
    enum NameTextType { Class, Attribute, Operation, Interface, Actor_name, Option_disc,State_name, State_disc};
    NameTextType NameTextType_() const
        { return myNameTextType; }
    ItemTextType ItemTextType_() const
        { return myItemTextType; }

    My_diagram_text_item(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0,ItemTextType itemTextType = Label, NameTextType nameTextType = Class );

    int type() const
        { return Type; }

signals:
    void lostFocus(My_diagram_text_item *item);
    void selectedChange(QGraphicsItem *item);
    void items_changed();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    ItemTextType myItemTextType;
    NameTextType myNameTextType;
};
QDataStream &operator<<(QDataStream &out, const My_diagram_text_item &boxItem);
QDataStream &operator>>(QDataStream &in, My_diagram_text_item &boxItem);

#endif // MY_DIAGRAM_TEXT_ITEM_H
