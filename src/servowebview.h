#pragma once

#include <qcolor.h>
#include <qqmlintegration.h>
#include <qquickitem.h>
#include <qsgrendernode.h>
#include <qtmetamacros.h>
#include <rhi/qrhi.h>

class ServoWebView : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(QList<QVector2D> vertices READ vertices WRITE setVertices NOTIFY
                 verticesChanged)
  QML_ELEMENT

public:
  explicit ServoWebView(QQuickItem *parent = nullptr);

  QList<QVector2D> vertices() const;
  void setVertices(const QList<QVector2D> &newVertices);

Q_SIGNALS:
  void verticesChanged();

protected:
  QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

private:
  QList<QVector2D> m_vertices;
};

class ServoRenderNode : public QSGRenderNode {
public:
  ServoRenderNode(QQuickWindow *window);

  void setVertices(const QList<QVector2D> &vertices);

  void prepare() override;
  void render(const RenderState *state) override;
  void releaseResources() override;
  RenderingFlags flags() const override;
  QSGRenderNode::StateFlags changedStates() const override;

protected:
  QQuickWindow *m_window;
  std::unique_ptr<QRhiBuffer> m_vertexBuffer;
  std::unique_ptr<QRhiBuffer> m_uniformBuffer;
  std::unique_ptr<QRhiShaderResourceBindings> m_resourceBindings;
  std::unique_ptr<QRhiGraphicsPipeline> m_pipeline;
  QList<QRhiShaderStage> m_shaders;
  bool m_verticesDirty = true;
  QList<QVector2D> m_vertices;
};