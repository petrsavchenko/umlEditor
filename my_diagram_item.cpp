#include <QtGui>
#include "my_diagram_item.h"
#include "my_diagram_arrow.h"
#include "my_diagram_text_item.h"

My_diagram_item::My_diagram_item(DiagramType diagramType, QMenu *contextMenu,QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPolygonItem(parent, scene)
{
    myDiagramType = diagramType;
    myContextMenu = contextMenu;
    QPainterPath path;
    switch (myDiagramType)
    {

    case Class_:
            {
            QPolygonF myPolygond;
            myPolygond << QPointF(-100, -100) << QPointF(100, -100) << QPointF(100,0)
                      << QPointF(100, 100) << QPointF(-100, 100) << QPointF(-100,0)
                      << QPointF(-100, -100);
            path.addPolygon(myPolygond);
            path.moveTo(-100,25);
            path.lineTo(100,25);
            path.moveTo(-100,-65);
            path.lineTo(100,-65);
            myPolygon = path.toFillPolygon();
            break;
        }
    case Interface:
            {
            QPolygonF myPolygond;
            myPolygond << QPointF(-115, -100) << QPointF(115, -100)
                      << QPointF(115, 100) << QPointF(-115, 100)
                      << QPointF(-115, -100);
            path.addPolygon(myPolygond);
            path.moveTo(-115,10);
            path.lineTo(115,10);
            myPolygon = path.toFillPolygon();
            break;
        }
    case Option:
        {
            QRect m(QPoint(-115, -100),QPoint(115, 100));
            path.addRoundRect(m,30);
            myPolygon = path.toFillPolygon();
            break;
        }
    case State:
        {
            QRect m(QPoint(-115, -100),QPoint(115, 60));
            path.addRoundRect(m,30);
            path.moveTo(-115,-60);
            path.lineTo(115,-60);
            myPolygon = path.toFillPolygon();
            break;
        }
    case BeginState:
        {
            path.addEllipse(-10,-10,30,30);
            myPolygon = path.toFillPolygon();
            break;
        }
    case EndState:
        {
            //path.addEllipse(0,0,30,30);
            path.addEllipse(-10,-10,50,50);
            myPolygon = path.toFillPolygon();
            break;
        }
    case SyncState:
         {
            path.addEllipse(-10,-10,50,50);
            myPolygon = path.toFillPolygon();
            break;
         }
    case Shallow_history_state:
        {
            path.addEllipse(-10,-10,50,50);
            myPolygon = path.toFillPolygon();
            break;
        }
    case Deep_history_state:
        {
            path.addEllipse(-10,-10,50,50);
            myPolygon = path.toFillPolygon();
            break;
        }
    case Transition:
        {
            myPolygon << QPointF(-5,-20 ) << QPointF(5, -20)
                    << QPointF(5, 20) << QPointF(-5, 20) << QPointF(-5,-20);break;

        }
    case Actor:
        myPolygon << QPointF(-45, -100) << QPointF(45, -100)
                    << QPointF(45, 100) << QPointF(-45, 100)
                    << QPointF(-45, -100);
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

void My_diagram_item::removeArrow(My_diagram_arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void My_diagram_item::removeArrows()
{
    foreach (My_diagram_arrow *arrow, arrows)
    {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void My_diagram_item::addArrow(My_diagram_arrow *arrow)
{
    arrows.append(arrow);
}

QPixmap My_diagram_item::image() const
{
    if(myDiagramType == BeginState)
    {
        QPixmap pixmap(250, 250);
        pixmap.load(":/begin_state.png");
        return pixmap.scaled(30,30);
    }
    if(myDiagramType == EndState)
    {
        QPixmap pixmap(250, 250);
        pixmap.load(":/final-state.png");       
        return pixmap.scaled(30,30);
    }
    if(myDiagramType == Actor)
    {
        QPixmap pixmap(250, 250);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setPen(QPen(Qt::black, 8));
        painter.translate(125, 125);
        QPainterPath * myPath = new QPainterPath();
        myPath->addEllipse(QPoint(0,-75),25,25);
        myPath->moveTo(0,-50);
        myPath->lineTo(0,20);
        myPath->lineTo(-45,100);
        myPath->moveTo(0,20);
        myPath->lineTo(45,100);
        myPath->moveTo(-45,-15);
        myPath->lineTo(45,-15);
        painter.drawPath(*myPath);
        return pixmap;
    }
    if(myDiagramType == SyncState)
    {
        QPixmap pixmap(250, 250);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setPen(QPen(Qt::black, 8));
        painter.translate(125, 125);
        painter.drawEllipse(QPoint(0,0),80,80);
        //painter.drawPolyline(myPolygon);
        painter.setFont(QFont("Times",50,QFont::Bold));
        painter.drawText(-8,50,"*");
        return pixmap;
    }
    if(myDiagramType == Shallow_history_state)
    {
        QPixmap pixmap(250, 250);
        pixmap.load(":/shallow-history-state.png");
        return pixmap.scaled(30,30);
    }
    if(myDiagramType == Deep_history_state)
    {
        QPixmap pixmap(250, 250);
        pixmap.load(":/deep-history-state.png");
        return pixmap.scaled(30,30);
    }
    if(myDiagramType == Transition)
    {
        QPixmap pixmap(250, 250);
        pixmap.load(":/join-state.png");
        return pixmap.scaled(30,30);
    }
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);
    return pixmap;
}

void My_diagram_item::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant My_diagram_item::itemChange(GraphicsItemChange change,const QVariant &value)//Если элемент переместился, нам надо обновить позицию стрелок, которые с ним соединены. Реализация QGraphicsItem ничего не делает, поэтому мы можем просто вернуть значение value.
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
void My_diagram_item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(myDiagramType == EndState)
    {
        painter->setBrush(Qt::SolidPattern);
        painter->drawEllipse(0,0,30,30);
        painter->setBrush(Qt::NoBrush);
    }
    if(myDiagramType == SyncState)
    {
        painter->setFont(QFont("Times",70,QFont::Bold));
        painter->drawText(-8,57,"*");
    }
    if(myDiagramType == Shallow_history_state)
    {
        painter->setFont(QFont("Times",30,QFont::Bold));
        painter->drawText(0,30,"H");
    }
    if(myDiagramType == Deep_history_state)
    {
        painter->setFont(QFont("Times",30,QFont::Bold));
        painter->drawText(-5,30,"H'");
    }
    if(myDiagramType == Actor)
    {
        //QGraphicsPolygonItem::paint(painter,option);
        painter->setBrush(Qt::white);
        painter->setPen(Qt::white);
        painter->drawPolygon(myPolygon);
        painter->setPen(Qt::black);
        QPainterPath * myPath = new QPainterPath();
        myPath->addEllipse(QPoint(0,-75),25,25);
        myPath->moveTo(0,-50);
        myPath->lineTo(0,20);
        myPath->lineTo(-45,100);
        myPath->moveTo(0,20);
        myPath->lineTo(45,100);
        myPath->moveTo(-45,-15);
        myPath->lineTo(45,-15);
        painter->drawPath(*myPath);
        //painter->setBrush();
        return;
    }
    QGraphicsPolygonItem::paint(painter,option);
}
QDataStream &operator<<(QDataStream &out, const My_diagram_item &boxItem)
{
    out << boxItem.pos() << boxItem.getTypeElem() << boxItem.zValue();
    QList<QGraphicsItem *> S =  boxItem.children();
    QList<QString> Pol;
    for(int i=0;i!=S.count();i++)
    {
       My_diagram_text_item * t = qgraphicsitem_cast<My_diagram_text_item *>(S.at(i));
       Pol.push_back(t->document()->toPlainText());
    }
    out << Pol;
    out << boxItem.brush();
    return out;
}
QDataStream &operator>>(QDataStream &in, My_diagram_item *boxItem)
{
      QPointF position;
      int a;
      QList<QString> Chldr;
      qreal z;
      QBrush AD;
      in >> position;
      in >> a;
      in >> z;

      in >> Chldr;


      My_diagram_item::DiagramType sas = My_diagram_item::DiagramType(a);


      My_diagram_item *Temp = new My_diagram_item(sas,boxItem->getMenu());
      Temp->setZValue(z);

      if(Chldr.size()!=0)
      {
          if(Temp->diagramType() == My_diagram_item::Class_)
          {
              My_diagram_text_item * One = new My_diagram_text_item(Temp,0,My_diagram_text_item::Margin);
              One->moveBy(-100,-100);
              One->setDocument(new QTextDocument(Chldr.at(0)));
              One->setTextWidth(200);

              My_diagram_text_item * Two = new My_diagram_text_item(Temp, 0,My_diagram_text_item::Margin, My_diagram_text_item::Attribute);
              Two->moveBy(-100,-65);
              Two->setDocument(new QTextDocument(Chldr.at(1)));
              Two->setTextWidth(200);

              My_diagram_text_item * Three = new My_diagram_text_item(Temp, 0,My_diagram_text_item::Margin, My_diagram_text_item::Operation);
              Three->moveBy(-100,25);
              Three->setDocument(new QTextDocument(Chldr.at(2)));
              Three->setTextWidth(200);
          }
          if(Temp->diagramType() == My_diagram_item::Interface)
          {
              My_diagram_text_item*  One = new My_diagram_text_item(Temp, 0,My_diagram_text_item::Margin, My_diagram_text_item::Interface);
              One->moveBy(-115,-100);
              One->setDocument(new QTextDocument(Chldr.at(0)));
              One->setTextWidth(230);
              My_diagram_text_item*  Three = new My_diagram_text_item(Temp, 0,My_diagram_text_item::Margin, My_diagram_text_item::Operation);
              Three->setTextWidth(230);
              Three->moveBy(-115,10);
              Three->setDocument(new QTextDocument(Chldr.at(1)));
          }
          if(Temp->diagramType() == My_diagram_item::Actor)
          {
              My_diagram_text_item*  One = new My_diagram_text_item(Temp, 0,My_diagram_text_item::Margin, My_diagram_text_item::Actor_name);
              One->moveBy(-45,100);
              One->setDocument(new QTextDocument(Chldr.at(0)));
              One->setTextWidth(90);
          }
          if(Temp->diagramType() == My_diagram_item::State)
          {
              My_diagram_text_item* One = new My_diagram_text_item(Temp, 0,My_diagram_text_item::Margin, My_diagram_text_item::State_name);
              One->moveBy(-115,-80);
              One->setDocument(new QTextDocument(Chldr.at(0)));
              One->setTextWidth(230);
              My_diagram_text_item* Two = new My_diagram_text_item(Temp, 0,My_diagram_text_item::Margin, My_diagram_text_item::State_disc);
              Two->moveBy(-115,-60);
              Two->setDocument(new QTextDocument(Chldr.at(1)));
              Two->setTextWidth(230);
          }
          if(Temp->diagramType() == My_diagram_item::Option)
          {
              My_diagram_text_item* One = new My_diagram_text_item(Temp, 0,My_diagram_text_item::Margin, My_diagram_text_item::Option_disc);
              One->moveBy(-115,-60);
              One->setDocument(new QTextDocument(Chldr.at(0)));
              One->setTextWidth(230);
          }
      }
      in >> AD;
      Temp->setBrush(AD);
      boxItem->scene()->addItem(Temp);
      Temp->setPos(position);
      return in;
}
