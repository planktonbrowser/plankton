#pragma once

#include <qcolor.h>
#include <qqmlintegration.h>
#include <qquickitem.h>
#include <qsgrendernode.h>
#include <qtmetamacros.h>
#include <rhi/qrhi.h>

class ServoWebView : public QQuickItem {
  Q_OBJECT
  QML_ELEMENT

public:
  explicit ServoWebView(QQuickItem *parent = nullptr);

protected:
  QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
};

class ServoRenderNode : public QSGRenderNode {
public:
  ServoRenderNode(QQuickWindow *window);

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
  std::unique_ptr<QRhiTexture> m_wrappedTex;
  std::unique_ptr<QRhiSampler> m_sampler;
};