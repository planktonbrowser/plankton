#pragma once
#include <QQuickItem>

class ServoWebView : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(QList<QVector2D> vertices READ vertices WRITE setVertices NOTIFY
                 verticesChanged)
  QML_ELEMENT

public:
  explicit ServoWebView(QQuickItem *parent = nullptr);

  QList<QVector2D> vertices() const;
  void setVertices(const QList<QVector2D> &newVertices);

signals:
  void verticesChanged();

protected:
  QSGNode *updatePaintNode(QSGNode *old, UpdatePaintNodeData *) override;

private:
  QList<QVector2D> m_vertices;
};
