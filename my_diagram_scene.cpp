#include <QtGui>

#include "my_diagram_scene.h"
#include "my_diagram_arrow.h"


My_diagram_scene::My_diagram_scene(QMenu *itemMenu, QObject *parent): QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = My_diagram_item::Class_;                         // Ёто значит мы вставл€ем либо треугольник квадратик и т.д.
    line = 0;
    textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
    ismodified = false;
}

void My_diagram_scene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if (isItemChange(My_diagram_arrow::Type))
    {
        My_diagram_arrow *item =
            qgraphicsitem_cast<My_diagram_arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}

void My_diagram_scene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(My_diagram_item::Type))
    {
        My_diagram_text_item *item =
            qgraphicsitem_cast<My_diagram_text_item *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}

void My_diagram_scene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(My_diagram_item::Type))
    {
        My_diagram_item *item =
            qgraphicsitem_cast<My_diagram_item *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}

void My_diagram_scene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(My_diagram_text_item::Type))
    {
        QGraphicsTextItem *item =
            qgraphicsitem_cast<My_diagram_text_item *>(selectedItems().first());

        if(item!=0 && item->parentItem()!=0)
            return;
        if (item)
            item->setFont(myFont);
    }
}


void My_diagram_scene::setMode(Mode mode)
{
    myMode = mode;
}

void My_diagram_scene::setArrows(Arrows arrows)
{
    myArrow = arrows;
}

void My_diagram_scene::setItemType(My_diagram_item::DiagramType type)
{
    myItemType = type;
}

void My_diagram_scene::editorLostFocus(My_diagram_text_item *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();                    //очищаем текущий элемент когда он тер€ет фокус
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty())
    {
        removeItem(item);
        item->deleteLater();
    }
}

void My_diagram_scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
//    My_diagram_item * my = new My_diagram_item(myItemType,myItemMenu);
//    addItem(my);
My_diagram_item* item;
My_diagram_text_item* One;
My_diagram_text_item* Two;
My_diagram_text_item* Three;
    switch (myMode)
    {
    case InsertItem:
        {
            if(myItemType == My_diagram_item::Class_)
          {
            item = new My_diagram_item(myItemType, myItemMenu);
            item->setBrush(myItemColor);

            One = new My_diagram_text_item(item, 0,My_diagram_text_item::Margin, My_diagram_text_item::Class);
            One->moveBy(-100,-100);
            connect(One, SIGNAL(lostFocus(My_diagram_text_item*)),
                    this, SLOT(editorLostFocus(My_diagram_text_item*)));
            connect(One, SIGNAL(selectedChange(QGraphicsItem*)),
                    this, SIGNAL(itemSelected(QGraphicsItem*)));
            connect(One, SIGNAL(items_changed()),
                    this, SLOT(isModified()));
            addItem(item);
            item->setPos(mouseEvent->scenePos());

//==================================================================
            Two = new My_diagram_text_item(item, 0,My_diagram_text_item::Margin, My_diagram_text_item::Attribute);
            Two->moveBy(-100,-65);
            connect(Two, SIGNAL(lostFocus(My_diagram_text_item*)),
                    this, SLOT(editorLostFocus(My_diagram_text_item*)));
            connect(Two, SIGNAL(selectedChange(QGraphicsItem*)),
                    this, SIGNAL(itemSelected(QGraphicsItem*)));
            connect(Two, SIGNAL(items_changed()),
                    this, SLOT(isModified()));
//==================================================================
            Three = new My_diagram_text_item(item, 0,My_diagram_text_item::Margin, My_diagram_text_item::Operation);
            Three->moveBy(-100,25);
            connect(Three, SIGNAL(lostFocus(My_diagram_text_item*)),
                    this, SLOT(editorLostFocus(My_diagram_text_item*)));
            connect(Three, SIGNAL(selectedChange(QGraphicsItem*)),
                    this, SIGNAL(itemSelected(QGraphicsItem*)));
            connect(Three, SIGNAL(items_changed()),
                    this, SLOT(isModified()));
            emit itemInserted(item);
            break;
          }
            if(myItemType == My_diagram_item::Interface)
            {
                item = new My_diagram_item(myItemType, myItemMenu);
                item->setBrush(myItemColor);

                One = new My_diagram_text_item(item, 0,My_diagram_text_item::Margin, My_diagram_text_item::Interface);
                One->moveBy(-115,-100);
                connect(One, SIGNAL(lostFocus(My_diagram_text_item*)),
                        this, SLOT(editorLostFocus(My_diagram_text_item*)));
                connect(One, SIGNAL(selectedChange(QGraphicsItem*)),
                        this, SIGNAL(itemSelected(QGraphicsItem*)));
                connect(One, SIGNAL(items_changed()),
                        this, SLOT(isModified()));

                addItem(item);
                item->setPos(mouseEvent->scenePos());

                Three = new My_diagram_text_item(item, 0,My_diagram_text_item::Margin, My_diagram_text_item::Operation);
                Three->setTextWidth(230);
                Three->moveBy(-115,10);
                connect(Three, SIGNAL(lostFocus(My_diagram_text_item*)),
                        this, SLOT(editorLostFocus(My_diagram_text_item*)));
                connect(Three, SIGNAL(selectedChange(QGraphicsItem*)),
                        this, SIGNAL(itemSelected(QGraphicsItem*)));
                connect(Three, SIGNAL(items_changed()),
                        this, SLOT(isModified()));
                emit itemInserted(item);
                break;
            }
            if(myItemType == My_diagram_item::Option)
            {
                item = new My_diagram_item(myItemType, myItemMenu);
                item->setBrush(myItemColor);

                One = new My_diagram_text_item(item, 0,My_diagram_text_item::Margin, My_diagram_text_item::Option_disc);
                One->moveBy(-115,-60);
                connect(One, SIGNAL(lostFocus(My_diagram_text_item*)),
                        this, SLOT(editorLostFocus(My_diagram_text_item*)));
                connect(One, SIGNAL(selectedChange(QGraphicsItem*)),
                        this, SIGNAL(itemSelected(QGraphicsItem*)));
                connect(One, SIGNAL(items_changed()),
                        this, SLOT(isModified()));

                addItem(item);
                item->setPos(mouseEvent->scenePos());
                emit itemInserted(item);
                break;
            }
            if(myItemType == My_diagram_item::State)
            {
                item = new My_diagram_item(myItemType, myItemMenu);
                item->setBrush(myItemColor);

                One = new My_diagram_text_item(item, 0,My_diagram_text_item::Margin, My_diagram_text_item::State_name);
                One->moveBy(-115,-80);
                connect(One, SIGNAL(lostFocus(My_diagram_text_item*)),
                        this, SLOT(editorLostFocus(My_diagram_text_item*)));
                connect(One, SIGNAL(selectedChange(QGraphicsItem*)),
                        this, SIGNAL(itemSelected(QGraphicsItem*)));
                connect(One, SIGNAL(items_changed()),
                        this, SLOT(isModified()));

                Two = new My_diagram_text_item(item, 0,My_diagram_text_item::Margin, My_diagram_text_item::State_disc);
                Two->moveBy(-115,-60);
                connect(Two, SIGNAL(lostFocus(My_diagram_text_item*)),
                        this, SLOT(editorLostFocus(My_diagram_text_item*)));
                connect(Two, SIGNAL(selectedChange(QGraphicsItem*)),
                        this, SIGNAL(itemSelected(QGraphicsItem*)));
                connect(One, SIGNAL(items_changed()),
                        this, SLOT(isModified()));

                addItem(item);
                item->setPos(mouseEvent->scenePos());
                emit itemInserted(item);
                break;
            }
            if(myItemType == My_diagram_item::BeginState)
            {

                item = new My_diagram_item(myItemType, myItemMenu);
                item->setBrush(QBrush(Qt::SolidPattern));
                addItem(item);
                item->setPos(mouseEvent->scenePos());
                emit itemInserted(item);
                break;
            }
            if(myItemType == My_diagram_item::EndState)
            {

                item = new My_diagram_item(myItemType, myItemMenu);
                //item->setBrush(Qt::white);
                addItem(item);
                item->setPos(mouseEvent->scenePos());

                update();
                emit itemInserted(item);
                break;
            }
            if(myItemType == My_diagram_item::SyncState)
            {

                item = new My_diagram_item(myItemType, myItemMenu);
                //item->setBrush(QBrush(Qt::SolidPattern));
//                item->update();
//                item->setBrush(Qt::white);
                addItem(item);
                item->setPos(mouseEvent->scenePos());

                emit itemInserted(item);
                break;
            }
            if(myItemType == My_diagram_item::Shallow_history_state)
            {

                item = new My_diagram_item(myItemType, myItemMenu);
               // item->setBrush(QBrush(Qt::SolidPattern));
                //item->setBrush(Qt::white);
                addItem(item);
                item->setPos(mouseEvent->scenePos());

                emit itemInserted(item);
                break;
            }
            if(myItemType == My_diagram_item::Deep_history_state)
            {

                item = new My_diagram_item(myItemType, myItemMenu);
               // item->setBrush(QBrush(Qt::SolidPattern));
                //item->setBrush(Qt::white);
                addItem(item);
                item->setPos(mouseEvent->scenePos());

                emit itemInserted(item);
                break;
            }
            if(myItemType == My_diagram_item::Transition)
            {

                item = new My_diagram_item(myItemType, myItemMenu);
                item->setBrush(QBrush(Qt::SolidPattern));

                addItem(item);
                item->setPos(mouseEvent->scenePos());

                emit itemInserted(item);
                break;
            }
            if(myItemType == My_diagram_item::Actor)
            {
                item = new My_diagram_item(myItemType, myItemMenu);
                //item->setBrush(QBrush(Qt::SolidPattern));
                One = new My_diagram_text_item(item, 0,My_diagram_text_item::Margin, My_diagram_text_item::Actor_name);
                One->moveBy(-45,100);
                connect(One, SIGNAL(lostFocus(My_diagram_text_item*)),
                                       this, SLOT(editorLostFocus(My_diagram_text_item*)));
                connect(One, SIGNAL(selectedChange(QGraphicsItem*)),
                                       this, SIGNAL(itemSelected(QGraphicsItem*)));
                connect(One, SIGNAL(items_changed()),
                        this, SLOT(isModified()));
                item->setBrush(Qt::white);
                addItem(item);
                item->setPos(mouseEvent->scenePos());

                emit itemInserted(item);
                break;
            }

        }

        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;

        case InsertText:
            textItem = new My_diagram_text_item();
            textItem->setFont(myFont);           
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, SIGNAL(lostFocus(My_diagram_text_item*)),
                    this, SLOT(editorLostFocus(My_diagram_text_item*)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                    this, SIGNAL(itemSelected(QGraphicsItem*)));
            connect(textItem, SIGNAL(items_changed()),
                    this, SLOT(isModified()));
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);

        default:
            ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
    QList<QGraphicsItem*> ASA = this->items();

        for(int i=0;i!=ASA.count();i++)
        {
            if(qgraphicsitem_cast<My_diagram_text_item *>(ASA.at(i))!=0)
                normal_text_without_focus(qgraphicsitem_cast<My_diagram_text_item *>(ASA.at(i)));
        }
}

void My_diagram_scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)//вызываетс€ только в том случае если только одна из кнопок мыши нажата
{
    if (myMode == InsertLine && line != 0)
    {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem)
            {
                QGraphicsScene::update();
                QGraphicsScene::mouseMoveEvent(mouseEvent);
            }
}

void My_diagram_scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0 && myMode == InsertLine)
    {
        QList<QGraphicsItem *> startItems = items(line->line().p1());

        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());

        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;
//—начала нам надо получить элементы (если такие есть) под начальной и конечной точками.
//—ама лини€ это первый элемент на этих координатах, поэтому мы удал€ем ее из списка.
//¬ качестве предосторожности мы провер€ем не пустой ли список, но это не должно никогда произойти.

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == My_diagram_item::Type &&
            endItems.first()->type() == My_diagram_item::Type &&
            startItems.first() != endItems.first())
        {
            My_diagram_item *startItem =
                qgraphicsitem_cast<My_diagram_item *>(startItems.first());
            My_diagram_item *endItem =
                qgraphicsitem_cast<My_diagram_item *>(endItems.first());
            My_diagram_arrow *arrow = 0;

            if(myArrow == Dependency)
                arrow = new My_diagram_arrow(My_diagram_arrow::Dependency, startItem, endItem);

            if(myArrow == Association)
                arrow = new My_diagram_arrow(My_diagram_arrow::Association, startItem, endItem);

            if(myArrow == Aggregation)
                arrow = new My_diagram_arrow(My_diagram_arrow::Aggregation, startItem, endItem);

            if(myArrow == Composition)
                arrow = new My_diagram_arrow(My_diagram_arrow::Composition, startItem, endItem);

            if(myArrow == Generalization)
                arrow = new My_diagram_arrow(My_diagram_arrow::Generalization, startItem, endItem);
            if(myArrow == Include)
                arrow = new My_diagram_arrow(My_diagram_arrow::Include, startItem, endItem);
            if(myArrow == Extend)
                arrow = new My_diagram_arrow(My_diagram_arrow::Extend, startItem, endItem);
            if(myArrow == Transaction)
                arrow = new My_diagram_arrow(My_diagram_arrow::Transaction, startItem, endItem);

            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool My_diagram_scene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems())
    {
        if (item->type() == type)
            return true;
    }
    return false;
}
void My_diagram_scene::normal_text_without_focus(My_diagram_text_item* item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);
    if(!item->isSelected())
        item->setTextInteractionFlags(Qt::NoTextInteraction);
}
void My_diagram_scene::isModified()
{
    ismodified = true;
}
