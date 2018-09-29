#include "my_diagram_arrow.h"
#include "my_diagram_text_item.h"
#include <QtGui>

#include <math.h>

const qreal Pi = 3.14;


My_diagram_arrow::My_diagram_arrow(ArrowType arrowType,My_diagram_item *startItem, My_diagram_item *endItem,QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene)
{
    myArrowType = arrowType;
    myStartItem = startItem;
    myEndItem = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::black;
    setPen(QPen(myColor,2));
    if(arrowType == Dependency || arrowType == Include || arrowType == Extend )
        setPen(QPen(myColor, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF My_diagram_arrow::boundingRect() const
{
    qreal extra = (pen().width() + 40) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath My_diagram_arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void My_diagram_arrow::updatePosition()
{
        QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
        setLine(line);
}

void My_diagram_arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
        if (myStartItem->collidesWithItem(myEndItem))
            return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 20;


    painter->setPen(myPen);
    painter->setBrush(myColor);
//    Надо найти позицию, где мы отрисуем наконечник стрелки.
//    Наконечник должен быть отрисован где линия и конечный элемент пересекаются.
//    Это делается путём взятия линии между каждыми точками полигона и проверкой не пересекается ли она с линией стрелки.
//    Так как начальная и конечная точки установлены в центр элементов, линия должна пересекать одну и только одну линию полигона.
//    Заметьте, что точки полигона относительны к локальной координатной системе элемента.
//    Поэтому нам надо добавить позицию конечного элемента что бы получить координаты в отношении сцены.
//================================================

 //=================================================
    if(myArrowType == Dependency)
    {
        QLineF centerLine(myStartItem->pos(), myEndItem->pos());//делаем линию от одного элемента до другого
        QPolygonF endPolygon = myEndItem->polygon();//берем ограничивающий многоугольник последнего элемента
        QPointF p1 = endPolygon.first() + myEndItem->pos();//берем первую точку ограничивающего многоугольника + координата самого элемента для перехода от локальных координат к координатам сцены
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        for (int i = 1; i < endPolygon.count(); ++i)
        {
            p2 = endPolygon.at(i) + myEndItem->pos();//берем координаты следующей точки ограничивающего многоугольника
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);// смотрим соединяется ли линия, построенная на точках ограничивающего прямоугольника элемента с линией идущей от центра огр прямоугольника первого элемента к центру огран прямоугольника второго элемента
            if (intersectType == QLineF::BoundedIntersection)//intersectPoint - хранит координаты точки пересечения
                break;// если нашли пересечение то выходим ))
            p1 = p2; // иначе переходим к следующему элементу
        }



        setLine(QLineF(intersectPoint, myStartItem->pos()));
    //    Мы вычисляем угол между осью Х и линией стрелки.
    //    Нам необходимо получить наконечник стрелки на этот угол чтобы он следовал направлению стрелки.
    //    Если угол отрицательный, мы должны повернуть направление стрелки.
        QPen Asa(myColor,pen().width(),Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
        double angle = ::acos(line().dx() / line().length());//угол между линией соединяющей 2 - а элемента и осью Х
        if (line().dy() >= 0)
            angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,cos(angle + Pi / 3) * arrowSize);//arrowSize - выступает в роли гипотинузы
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,cos(angle + Pi - Pi / 3) * arrowSize);
    //    Затем мы можем вычислить три точки полигона наконечника стрелки.
    //    Одна из этих точек это конец линии, которая теперь равна точке пересечения между стрелкой и конечным полигоном.
    //    Затем мы очищаем полигон arrowHead от ранее вычисленного наконечника стрелки и устанавливаем эти новые точки.
        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;

        painter->drawLine(line());
        painter->setPen(Asa);
        painter->drawPolygon(arrowHead);
        painter->setPen(myPen);
    }

    if(myArrowType == Association)
    {
        QLineF centerLine(myStartItem->pos(), myEndItem->pos());//делаем линию от одного элемента до другого
        QPolygonF endPolygon = myEndItem->polygon();//берем ограничивающий многоугольник последнего элемента
        QPointF p1 = endPolygon.first() + myEndItem->pos();//берем первую точку ограничивающего многоугольника + координата самого элемента для перехода от локальных координат к координатам сцены
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        for (int i = 1; i < endPolygon.count(); ++i)
        {
            p2 = endPolygon.at(i) + myEndItem->pos();//берем координаты следующей точки ограничивающего многоугольника
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);// смотрим соединяется ли линия, построенная на точках ограничивающего прямоугольника элемента с линией идущей от центра огр прямоугольника первого элемента к центру огран прямоугольника второго элемента
            if (intersectType == QLineF::BoundedIntersection)//intersectPoint - хранит координаты точки пересечения
                break;// если нашли пересечение то выходим ))
            p1 = p2; // иначе переходим к следующему элементу
        }

        setLine(QLineF(intersectPoint, myStartItem->pos()));
        arrowHead.clear();
        painter->drawLine(line());
    }

    if(myArrowType == Transaction)
    {
        QLineF centerLine(myStartItem->pos(), myEndItem->pos());//делаем линию от одного элемента до другого
        QPolygonF endPolygon = myEndItem->polygon();//берем ограничивающий многоугольник последнего элемента
        QPointF p1 = endPolygon.first() + myEndItem->pos();//берем первую точку ограничивающего многоугольника + координата самого элемента для перехода от локальных координат к координатам сцены
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        for (int i = 1; i < endPolygon.count(); ++i)
        {
            p2 = endPolygon.at(i) + myEndItem->pos();//берем координаты следующей точки ограничивающего многоугольника
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);// смотрим соединяется ли линия, построенная на точках ограничивающего прямоугольника элемента с линией идущей от центра огр прямоугольника первого элемента к центру огран прямоугольника второго элемента
            if (intersectType == QLineF::BoundedIntersection)//intersectPoint - хранит координаты точки пересечения
                break;// если нашли пересечение то выходим ))
            p1 = p2; // иначе переходим к следующему элементу
        }
        QPolygonF beginPolygon = myStartItem->polygon();
        p1 = beginPolygon.first() + myStartItem->pos();
        //p2 = 0;
        QPointF intersectPoint2;
        for (int i = 1; i < beginPolygon.count(); ++i)
        {
            p2 = beginPolygon.at(i) + myStartItem->pos();//берем координаты следующей точки ограничивающего многоугольника
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint2);// смотрим соединяется ли линия, построенная на точках ограничивающего прямоугольника элемента с линией идущей от центра огр прямоугольника первого элемента к центру огран прямоугольника второго элемента
            if (intersectType == QLineF::BoundedIntersection)//intersectPoint - хранит координаты точки пересечения
                break;// если нашли пересечение то выходим ))
            p1 = p2; // иначе переходим к следующему элементу
        }
        setLine(QLineF(intersectPoint, intersectPoint2));
        //setLine(QLineF(intersectPoint, myStartItem->pos()));

        QPen Asa(myColor,pen().width(),Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
        double angle = ::acos(line().dx() / line().length());//угол между линией соединяющей 2 - а элемента и осью Х
        if (line().dy() >= 0)
            angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,cos(angle + Pi / 3) * arrowSize);//arrowSize - выступает в роли гипотинузы
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,cos(angle + Pi - Pi / 3) * arrowSize);
    //    Затем мы можем вычислить три точки полигона наконечника стрелки.
    //    Одна из этих точек это конец линии, которая теперь равна точке пересечения между стрелкой и конечным полигоном.
    //    Затем мы очищаем полигон arrowHead от ранее вычисленного наконечника стрелки и устанавливаем эти новые точки.
        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;

        painter->drawLine(line());
        painter->setPen(Asa);
        painter->drawPolygon(arrowHead);
        painter->setPen(myPen);
    }


    if(myArrowType == Aggregation)
    {
        QLineF centerLine(myStartItem->pos(), myEndItem->pos());//делаем линию от одного элемента до другого
        QPolygonF startPolygon = myStartItem->polygon();//берем ограничивающий многоугольник последнего элемента
        QPointF p1 = startPolygon.first() + myStartItem->pos();//берем первую точку ограничивающего многоугольника + координата самого элемента для перехода от локальных координат к координатам сцены
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        for (int i = 1; i < startPolygon.count(); ++i)
        {
            p2 = startPolygon.at(i) + myStartItem->pos();//берем координаты следующей точки ограничивающего многоугольника
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);// смотрим соединяется ли линия, построенная на точках ограничивающего прямоугольника элемента с линией идущей от центра огр прямоугольника первого элемента к центру огран прямоугольника второго элемента
            if (intersectType == QLineF::BoundedIntersection)//intersectPoint - хранит координаты точки пересечения
                break;// если нашли пересечение то выходим ))
            p1 = p2; // иначе переходим к следующему элементу
        }
        setLine(QLineF(intersectPoint, myEndItem->pos()));
//===============================================================
        double angle = ::acos(line().dx() / line().length());
        if (line().dy() >= 0)
            angle = (Pi * 2) - angle;
        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,cos(angle + Pi - Pi / 3) * arrowSize);
        arrowHead.clear();


        QPointF arrowP3 = (arrowP1+arrowP2)/2;
        QPointF dxs =  arrowP3 - line().p1();
        arrowP3 = line().p1() + dxs*2;

        arrowHead << line().p1() << arrowP1  << arrowP3 << arrowP2; // arrowP1 - нижняя

        painter->drawLine(QLineF(arrowP3, myEndItem->pos()));

        painter->setBrush(Qt::NoBrush);
        painter->drawPolygon(arrowHead);
    }

    if(myArrowType == Composition)
    {
        QLineF centerLine(myStartItem->pos(), myEndItem->pos());//делаем линию от одного элемента до другого
        QPolygonF startPolygon = myStartItem->polygon();//берем ограничивающий многоугольник последнего элемента
        QPointF p1 = startPolygon.first() + myStartItem->pos();//берем первую точку ограничивающего многоугольника + координата самого элемента для перехода от локальных координат к координатам сцены
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        for (int i = 1; i < startPolygon.count(); ++i)
        {
            p2 = startPolygon.at(i) + myStartItem->pos();//берем координаты следующей точки ограничивающего многоугольника
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);// смотрим соединяется ли линия, построенная на точках ограничивающего прямоугольника элемента с линией идущей от центра огр прямоугольника первого элемента к центру огран прямоугольника второго элемента
            if (intersectType == QLineF::BoundedIntersection)//intersectPoint - хранит координаты точки пересечения
                break;// если нашли пересечение то выходим ))
            p1 = p2; // иначе переходим к следующему элементу
        }
        setLine(QLineF(intersectPoint, myEndItem->pos()));
//===============================================================
        double angle = ::acos(line().dx() / line().length());
        if (line().dy() >= 0)
            angle = (Pi * 2) - angle;
        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,cos(angle + Pi - Pi / 3) * arrowSize);
        arrowHead.clear();


        QPointF arrowP3 = (arrowP1+arrowP2)/2;
        QPointF dxs =  arrowP3 - line().p1();
        arrowP3 = line().p1() + dxs*2;

        arrowHead << line().p1() << arrowP1  << arrowP3 << arrowP2; // arrowP1 - нижняя

        //painter->drawLine(QLineF(arrowP3, myEndItem->pos()));
        painter->drawLine(line());

        //painter->setBrush(Qt::NoBrush);
        painter->drawPolygon(arrowHead);
    }

    if(myArrowType == Generalization)
    {
        QLineF centerLine(myStartItem->pos(), myEndItem->pos());//делаем линию от одного элемента до другого
        QPolygonF endPolygon = myEndItem->polygon();//берем ограничивающий многоугольник последнего элемента
        QPointF p1 = endPolygon.first() + myEndItem->pos();//берем первую точку ограничивающего многоугольника + координата самого элемента для перехода от локальных координат к координатам сцены
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        for (int i = 1; i < endPolygon.count(); ++i)
        {
            p2 = endPolygon.at(i) + myEndItem->pos();//берем координаты следующей точки ограничивающего многоугольника
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);// смотрим соединяется ли линия, построенная на точках ограничивающего прямоугольника элемента с линией идущей от центра огр прямоугольника первого элемента к центру огран прямоугольника второго элемента
            if (intersectType == QLineF::BoundedIntersection)//intersectPoint - хранит координаты точки пересечения
                break;// если нашли пересечение то выходим ))
            p1 = p2; // иначе переходим к следующему элементу
        }

        setLine(QLineF(intersectPoint, myStartItem->pos()));
    //    Мы вычисляем угол между осью Х и линией стрелки.
    //    Нам необходимо получить наконечник стрелки на этот угол чтобы он следовал направлению стрелки.
    //    Если угол отрицательный, мы должны повернуть направление стрелки.

        double angle = ::acos(line().dx() / line().length());//угол между линией соединяющей 2 - а элемента и осью Х
        if (line().dy() >= 0)
            angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,cos(angle + Pi / 3) * arrowSize);//arrowSize - выступает в роли гипотинузы
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,cos(angle + Pi - Pi / 3) * arrowSize);
        QPointF P3 = (arrowP1+arrowP2)/2;
    //    Затем мы можем вычислить три точки полигона наконечника стрелки.
    //    Одна из этих точек это конец линии, которая теперь равна точке пересечения между стрелкой и конечным полигоном.
    //    Затем мы очищаем полигон arrowHead от ранее вычисленного наконечника стрелки и устанавливаем эти новые точки.
        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;

        painter->drawLine(QLineF(myStartItem->pos(),P3));
       // painter->drawLine(line());
        painter->setBrush(Qt::NoBrush);
        painter->drawPolygon(arrowHead);
    }
    if(myArrowType == Include)
    {
        QLineF centerLine(myStartItem->pos(), myEndItem->pos());//делаем линию от одного элемента до другого
        QPolygonF endPolygon = myEndItem->polygon();//берем ограничивающий многоугольник последнего элемента
        QPointF p1 = endPolygon.first() + myEndItem->pos();//берем первую точку ограничивающего многоугольника + координата самого элемента для перехода от локальных координат к координатам сцены
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        for (int i = 1; i < endPolygon.count(); ++i)
        {
            p2 = endPolygon.at(i) + myEndItem->pos();//берем координаты следующей точки ограничивающего многоугольника
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);// смотрим соединяется ли линия, построенная на точках ограничивающего прямоугольника элемента с линией идущей от центра огр прямоугольника первого элемента к центру огран прямоугольника второго элемента
            if (intersectType == QLineF::BoundedIntersection)//intersectPoint - хранит координаты точки пересечения
                break;// если нашли пересечение то выходим ))
            p1 = p2; // иначе переходим к следующему элементу
        }

        setLine(QLineF(intersectPoint, myStartItem->pos()));
    //    Мы вычисляем угол между осью Х и линией стрелки.
    //    Нам необходимо получить наконечник стрелки на этот угол чтобы он следовал направлению стрелки.
    //    Если угол отрицательный, мы должны повернуть направление стрелки.
        QPen Asa(myColor,pen().width(),Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
        double angle = ::acos(line().dx() / line().length());//угол между линией соединяющей 2 - а элемента и осью Х
        if (line().dy() >= 0)
            angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,cos(angle + Pi / 3) * arrowSize);//arrowSize - выступает в роли гипотинузы
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,cos(angle + Pi - Pi / 3) * arrowSize);
    //    Затем мы можем вычислить три точки полигона наконечника стрелки.
    //    Одна из этих точек это конец линии, которая теперь равна точке пересечения между стрелкой и конечным полигоном.
    //    Затем мы очищаем полигон arrowHead от ранее вычисленного наконечника стрелки и устанавливаем эти новые точки.
        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;
        painter->drawText(line().pointAt(0.4),"<<include>>");
        painter->drawLine(line());
        painter->setPen(Asa);
        painter->drawPolygon(arrowHead);
        painter->setPen(myPen);
    }
    if(myArrowType == Extend)
    {
        QLineF centerLine(myStartItem->pos(), myEndItem->pos());//делаем линию от одного элемента до другого
        QPolygonF endPolygon = myEndItem->polygon();//берем ограничивающий многоугольник последнего элемента
        QPointF p1 = endPolygon.first() + myEndItem->pos();//берем первую точку ограничивающего многоугольника + координата самого элемента для перехода от локальных координат к координатам сцены
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        for (int i = 1; i < endPolygon.count(); ++i)
        {
            p2 = endPolygon.at(i) + myEndItem->pos();//берем координаты следующей точки ограничивающего многоугольника
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);// смотрим соединяется ли линия, построенная на точках ограничивающего прямоугольника элемента с линией идущей от центра огр прямоугольника первого элемента к центру огран прямоугольника второго элемента
            if (intersectType == QLineF::BoundedIntersection)//intersectPoint - хранит координаты точки пересечения
                break;// если нашли пересечение то выходим ))
            p1 = p2; // иначе переходим к следующему элементу
        }

        setLine(QLineF(intersectPoint, myStartItem->pos()));
    //    Мы вычисляем угол между осью Х и линией стрелки.
    //    Нам необходимо получить наконечник стрелки на этот угол чтобы он следовал направлению стрелки.
    //    Если угол отрицательный, мы должны повернуть направление стрелки.
        QPen Asa(myColor,pen().width(),Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
        double angle = ::acos(line().dx() / line().length());//угол между линией соединяющей 2 - а элемента и осью Х
        if (line().dy() >= 0)
            angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,cos(angle + Pi / 3) * arrowSize);//arrowSize - выступает в роли гипотинузы
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,cos(angle + Pi - Pi / 3) * arrowSize);
    //    Затем мы можем вычислить три точки полигона наконечника стрелки.
    //    Одна из этих точек это конец линии, которая теперь равна точке пересечения между стрелкой и конечным полигоном.
    //    Затем мы очищаем полигон arrowHead от ранее вычисленного наконечника стрелки и устанавливаем эти новые точки.
        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;

        painter->drawText(line().pointAt(0.5),"<<extend>>");
        painter->drawLine(line());
        painter->setPen(Asa);
        painter->drawPolygon(arrowHead);
        painter->setPen(myPen);
    }
    if (isSelected())
    {
       painter->setPen(QPen(myColor, 1, Qt::DashLine));
       QLineF myLine = line();
       myLine.translate(0, 4.0);//смещаем нашу нарисованную линию на 4 пикселя и рисуем пунктиром
       painter->drawLine(myLine);
       myLine.translate(0,-8.0);//аналогично
       painter->drawLine(myLine);
    }
}

QDataStream &operator<<(QDataStream &out, const My_diagram_arrow &boxItem)
{
    out << boxItem.getArrowType() << boxItem.startItem()->pos() << boxItem.endItem()->pos();
    return out;
}
QDataStream &operator>>(QDataStream &in, My_diagram_arrow *boxItem)
{
    int ads;
    My_diagram_arrow::ArrowType arrowtype;
    QPointF start;
    QPointF end;

    in >> ads;
    arrowtype =  My_diagram_arrow::ArrowType(ads);
    in >> start >> end;

    My_diagram_item * startItem =
        qgraphicsitem_cast<My_diagram_item *>(boxItem->scene()->itemAt(start));

    My_diagram_item * endItem =
        qgraphicsitem_cast<My_diagram_item *>(boxItem->scene()->itemAt(end));

    My_diagram_arrow *Temp = new My_diagram_arrow(arrowtype,startItem,endItem);
    startItem->addArrow(Temp);
    endItem->addArrow(Temp);
    Temp->setZValue(-1000.0);
    boxItem->scene()->addItem(Temp);
    Temp->updatePosition();
    return in;
}
