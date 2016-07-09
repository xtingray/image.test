#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QCursor>

class NodeManager;

class Node : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    
    public:
        enum TypeNode { TopLeft  = 0, TopRight, BottomLeft, BottomRight, Center };
        enum ActionNode { NoAction = 0, Scale, Rotate };
        
        Node(TypeNode node, ActionNode action, const QPointF &pos=QPoint(0,0), NodeManager *manager=0, QGraphicsItem *parent=0, int zValue=0);
        ~Node();
        
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
        QRectF boundingRect() const;
        void resize(qreal factor);
        
        void setAction(ActionNode action);
        int actionNode();
        
        enum { Type = UserType + 1 };
        int type() const { return Type; }
        
        int typeNode() const;

    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
        
    private:
        struct Private;
        Private *const k;
};

#endif
