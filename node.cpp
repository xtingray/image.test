#include "node.h"
#include "nodemanager.h"

struct Node::Private
{
    TypeNode typeNode;
    ActionNode action;
    bool notChange;
    ActionNode generalState; 
    QGraphicsItem *parent;
    NodeManager *manager;
    QSizeF size;
};

Node::Node(TypeNode node, ActionNode action, const QPointF &pos, NodeManager *manager, QGraphicsItem *parent, int zValue) : 
           QGraphicsItem(0), k(new Private)
{
    QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor));
    setFlag(ItemIsSelectable, false);
    setFlag(ItemIsMovable, true);
    setFlag(ItemIsFocusable, true);
    setPos(pos);

    k->typeNode = node;
    k->action = action;
    k->manager = manager;
    k->parent = parent;
    k->size = QSizeF(10, 10);
    k->generalState = Scale;

    setZValue(zValue);
}

Node::~Node()
{
    delete k;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
    Q_UNUSED(w);
    Q_UNUSED(option);
    
    QColor color;
   
    if (k->typeNode != Center) {
        if (k->action == Rotate) {
            color = QColor(255, 102, 0);
            color.setAlpha(180);
        } else {
            color = QColor("green");
            color.setAlpha(200);
        }
    } else {
        if (k->generalState == Scale) {
            color = QColor(150, 150, 150);
        } else {
           color = QColor(255, 0, 0);
        }
        color.setAlpha(150);
    }

    QRectF square = boundingRect();
    painter->setBrush(color);
    painter->drawRoundRect(square);

    if (k->typeNode == Center) {
        painter->save();
        color = QColor("white");
        color.setAlpha(220);
        painter->setPen(color);

        QPointF point1 = QPointF(square.topLeft().x() + 2, square.topLeft().y() + 2);
        QPointF point2 = QPointF(square.bottomRight().x() - 2, square.bottomRight().y() - 2);
        QPointF point3 = QPointF(square.bottomLeft().x() + 2, square.bottomLeft().y() - 2);
        QPointF point4 = QPointF(square.topRight().x() - 2, square.topRight().y() + 2);

        painter->drawLine(point1, point2);
        painter->drawLine(point3, point4);
        painter->restore();
    }
}

QRectF Node::boundingRect() const
{
    QRectF rect(QPointF(-k->size.width()/2, -k->size.height()/2), k->size);
    return rect;
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedChange) {
        setVisible(true);
        if (value.toBool())
            k->parent->setSelected(true);
        k->manager->show();
    }

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    k->manager->setPress(true);
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    k->parent->setSelected(true);
    k->manager->setPress(false);
    k->manager->syncNodes();
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF newPos(event->scenePos());

    if (k->notChange) {
        k->notChange = false;
    } else {
        if (k->action == Scale) {
            QRectF rect = k->parent->sceneBoundingRect();
            QRectF parentRect  = k->parent->sceneBoundingRect();
            QRectF parentSquare  = k->parent->boundingRect();
            
            switch (k->typeNode) {
                case TopRight:
                {
                     k->manager->setAnchor(parentSquare.bottomLeft());
                     rect.setTopRight(newPos);
                     break;
                }
                case BottomRight:
                {
                     k->manager->setAnchor(parentSquare.topLeft());
                     rect.setBottomRight(newPos);
                     break;
                }
                case TopLeft:
                {
                     k->manager->setAnchor(parentSquare.bottomRight());
                     rect.setTopLeft(newPos);
                     break;
                }
                case BottomLeft:
                {
                     k->manager->setAnchor(parentSquare.topRight());
                     rect.setBottomLeft(newPos);
                     break;
                }
                case Center:
                {
                     break;
                }
            };
            
            float sx = 1, sy = 1;
            sx = static_cast<float>(rect.width()) / static_cast<float>(parentRect.width());
            sy = static_cast<float>(rect.height()) / static_cast<float>(parentRect.height());

            if (k->manager->proportionalScale()) {
                k->manager->scale(sx, sx);
            } else {
                if (sx > 0 && sy > 0) {
                    k->manager->scale(sx, sy);
                } else {
                    if (sx > 0)
                        k->manager->scale(sx, 1);

                    if (sy > 0)
                        k->manager->scale(1, sy);
                }
            }
        } else if (k->action == Rotate) {
                   QPointF pRef; 
                   bool isCenter = false;
                   switch (k->typeNode) {
                       case TopRight:
                       {
                           pRef = k->parent->sceneBoundingRect().topRight();
                           break;
                       }
                       case BottomRight:
                       {
                           pRef = k->parent->sceneBoundingRect().bottomRight();
                           break;
                       }
                       case TopLeft:
                       {
                           pRef = k->parent->sceneBoundingRect().topLeft();
                           break;
                       }
                       case BottomLeft:
                       {
                           pRef = k->parent->sceneBoundingRect().bottomLeft();
                           break;
                       }
                       case Center:
                       {
                           isCenter = true;
                           break;
                       }
                   };

                   if (!isCenter) {
                       QPointF p1 = newPos;
                       QPointF p2 = k->parent->sceneBoundingRect().center();
                       k->manager->setAnchor(k->parent->boundingRect().center());

                       QLineF line(p2, p1);
                       QLineF lineRef(p2, pRef);
                       double angle = lineRef.angle() - line.angle();
                       k->manager->rotate(angle);
                   }
        }
    }

    if (k->typeNode == Center) {
        k->parent->moveBy(event->scenePos().x() - scenePos().x() , event->scenePos().y() - scenePos().y());
        event->accept();
    }
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    k->manager->toggleAction();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

int Node::typeNode() const
{
    return k->typeNode;
}

void Node::setAction(ActionNode action)
{
    if (k->typeNode != Center)
        k->action = action;
    else
        k->action = Scale;

    if (k->generalState == Scale)
        k->generalState = Rotate;
    else
        k->generalState = Scale;

    update();
}

int Node::actionNode()
{
    return k->action;
}

void Node::resize(qreal factor)
{
    setScale(factor);
}
