#include "nodemanager.h"

struct NodeManager::Private
{
    QHash<Node::TypeNode, Node *> nodes;
    QGraphicsItem *parent;
    QGraphicsScene *scene;

    QTransform origTransform;
    QPointF origPos;
    QPointF anchor;

    bool press;
    bool proportional;
    qreal rotation;
    qreal scaleX;
    qreal scaleY;
};

NodeManager::NodeManager(QGraphicsItem * parent, QGraphicsScene  *scene, int zValue): k(new Private)
{
    k->parent = parent;
    k->scene = scene;
    k->anchor = QPointF(0, 0);
    k->press = false;
    k->rotation = 0;

    QRectF rect = parent->sceneBoundingRect();
    Node *topLeft = new Node(Node::TopLeft, Node::Scale, rect.topLeft(), this, parent, zValue);
    Node *topRight = new Node(Node::TopRight, Node::Scale, rect.topRight(), this, parent, zValue);
    Node *bottomLeft = new Node(Node::BottomLeft, Node::Scale, rect.bottomLeft(), this, parent, zValue);
    Node *bottomRight = new Node(Node::BottomRight, Node::Scale, rect.bottomRight(), this, parent, zValue);
    Node *center = new Node(Node::Center, Node::Scale, rect.center(), this, parent, zValue);

    k->nodes.insert(Node::TopLeft, topLeft);
    k->nodes.insert(Node::TopRight, topRight);
    k->nodes.insert(Node::BottomLeft, bottomLeft);
    k->nodes.insert(Node::BottomRight, bottomRight);
    k->nodes.insert(Node::Center, center);

    k->proportional = false;
    
    beginToEdit();
}

NodeManager::~NodeManager()
{
}

void NodeManager::clear()
{
    foreach (Node *node, k->nodes) {
             if (node) {
                 QGraphicsScene *scene = node->scene();
                 if (scene)
                     scene->removeItem(node);
             }
             delete node;
             node = 0;
    }
    k->nodes.clear();
}

void NodeManager::syncNodes(const QRectF &rect)
{
    if (k->nodes.isEmpty())
        return;
    
    QHash<Node::TypeNode, Node *>::iterator it = k->nodes.begin();
    while (it != k->nodes.end()) {
           if ((*it)) {
               switch (it.key()) {
                       case Node::TopLeft:
                       {
                            if ((*it)->scenePos() != rect.topLeft())
                                (*it)->setPos(rect.topLeft());
                            break;
                       }
                       case Node::TopRight:
                       {
                            if ((*it)->scenePos() != rect.topRight())
                                (*it)->setPos(rect.topRight());
                            break;
                       }
                       case Node::BottomRight:
                       {
                            if ((*it)->scenePos() != rect.bottomRight())
                                (*it)->setPos(rect.bottomRight());
                            break;
                       }
                       case Node::BottomLeft:
                       {
                            if ((*it)->scenePos() != rect.bottomLeft())
                                (*it)->setPos(rect.bottomLeft() );
                            break;
                       }
                       case Node::Center:
                       {
                            if ((*it)->scenePos() != rect.center())
                                (*it)->setPos(rect.center());
                            break;
                       }
               }
           }
           ++it;
    }
}

void NodeManager::syncNodesFromParent()
{
    if (k->parent)
        syncNodes(k->parent->sceneBoundingRect());
}

QGraphicsItem *NodeManager::parentItem() const
{
    return k->parent;
}

bool NodeManager::isModified() const
{
    return !((k->parent->transform() == k->origTransform) && (k->parent->pos() == k->origPos));
}

void NodeManager::beginToEdit()
{
    k->origTransform = k->parent->transform();
    k->origPos = k->parent->pos();
}

void NodeManager::restoreItem()
{
    k->parent->setTransform(k->origTransform);
    k->parent->setPos(k->origPos);
}

void NodeManager::setAnchor(const QPointF& point)
{
    k->anchor = point;
}

QPointF NodeManager::anchor() const
{
    return k->anchor;
}

void NodeManager::scale(float sx, float sy)
{
    QTransform transform;
    QPointF point = k->parent->boundingRect().center();
    transform.translate(point.x(), point.y());
    transform.rotate(k->rotation);
    transform.scale(sx, sy);
    transform.translate(-point.x(), -point.y());
    k->parent->setTransform(transform);

    syncNodesFromParent();
    k->scaleX = sx;
    k->scaleY = sy;
}

void NodeManager::rotate(double angle)
{
    QTransform transform;
    QPointF point = k->parent->boundingRect().center();
    transform.translate(point.x(), point.y());
    transform.rotate(angle);
    transform.scale(k->scaleX, k->scaleY);
    transform.translate(-point.x(), -point.y());
    k->parent->setTransform(transform);

    syncNodesFromParent();
    k->rotation = angle;
}

void NodeManager::show()
{
    foreach (Node *node, k->nodes) {
             if (!node->scene())
                 k->scene->addItem(node);
    }
}

void NodeManager::setPress(bool press)
{
    k->press = press;
}

bool NodeManager::isPress()
{
    return k->press;
}

void NodeManager::toggleAction()
{
    foreach (Node *node, k->nodes) {
             if (node->actionNode() == Node::Scale) {
                 node->setAction(Node::Rotate);
             } else if (node->actionNode() == Node::Rotate) {
                        node->setAction(Node::Scale);
             }
    }
}

void NodeManager::setActionNode(Node::ActionNode action)
{
    foreach (Node *node, k->nodes)
             node->setAction(action);
}

void NodeManager::resizeNodes(qreal factor)
{
    foreach (Node *node, k->nodes)
             node->resize(factor);
}

void NodeManager::setVisible(bool visible)
{
    foreach (Node *node, k->nodes)
             node->setVisible(visible);
}

double NodeManager::rotation()
{
    return k->rotation;
}

void NodeManager::setProportion(bool flag)
{
    k->proportional = flag;
}

bool NodeManager::proportionalScale()
{
    return k->proportional;
}

void NodeManager::syncNodes()
{
    show();
    if (parentItem())
        syncNodesFromParent();
}
