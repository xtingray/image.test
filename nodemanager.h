#ifndef NODEMANAGER_H
#define NODEMANAGER_H

#include "node.h"

#include <QHash>
#include <QGraphicsItem>
#include <QGraphicsScene>

class NodeManager : public QObject
{
    Q_OBJECT

    public:
        NodeManager(QGraphicsItem *parent, QGraphicsScene *scene, int zValue);
        ~NodeManager();
        void syncNodes(const QRectF &rect);
        void syncNodesFromParent();
        QGraphicsItem *parentItem() const;
        bool isModified() const;
        
        void beginToEdit();
        void restoreItem();
        
        void show();
        
        void setAnchor(const QPointF& point);
        QPointF anchor() const;
        
        void scale(float sx, float sy);
        void rotate(double angle);
        
        void setPress(bool press);
        bool isPress();
        
        void toggleAction();
        
        void setActionNode(Node::ActionNode action);
        
        void setVisible(bool visible);
        
        double rotation();

        void setProportion(bool flag);
        bool proportionalScale();
        void clear();

        void resizeNodes(qreal factor);
        void syncNodes();

    private:
        struct Private;
        Private *const k;
};

#endif
