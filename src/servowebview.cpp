#include "servowebview.h"
#include <qdir.h>
#include <qlist.h>
#include <qlogging.h>
#include <qmatrix4x4.h>
#include <qquickitem.h>
#include <qquickwindow.h>
#include <qsgnode.h>
#include <qsgrendernode.h>
#include <qsize.h>
#include <qtmetamacros.h>
#include <qvectornd.h>
#include <rhi/qrhi.h>
#include <rhi/qshader.h>

ServoRenderNode::ServoRenderNode(QQuickWindow *window) : m_window(window) {
  QFile file;
  file.setFileName(QStringLiteral(":/plankton/shaders/customrender.vert.qsb"));
  if (!file.open(QFile::ReadOnly)) {
    qFatal("Failed to load vertex shader");
  }
  m_shaders.append(QRhiShaderStage(QRhiShaderStage::Vertex,
                                   QShader::fromSerialized(file.readAll())));
  file.close();
  file.setFileName(QStringLiteral(":/plankton/shaders/customrender.frag.qsb"));
  if (!file.open(QFile::ReadOnly)) {
    qFatal("Failed to load fragment shader");
  }
  m_shaders.append((QRhiShaderStage(QRhiShaderStage::Fragment,
                                    QShader::fromSerialized(file.readAll()))));
}

void ServoRenderNode::setVertices(const QList<QVector2D> &vertices) {
  if (m_vertices == vertices) {
    return;
  }

  m_verticesDirty = true;
  m_vertices = vertices;

  markDirty(QSGNode::DirtyGeometry);
}

void ServoRenderNode::releaseResources() {
  m_vertexBuffer.reset();
  m_uniformBuffer.reset();
  m_pipeline.reset();
  m_resourceBindings.reset();
}

QSGRenderNode::RenderingFlags ServoRenderNode::flags() const {
  return QSGRenderNode::NoExternalRendering |
         QSGRenderNode::DepthAwareRendering;
}

QSGRenderNode::StateFlags ServoRenderNode::changedStates() const {
  return QSGRenderNode::StateFlag::ViewportState |
         QSGRenderNode::StateFlag::CullState;
}

void ServoRenderNode::prepare() {
  QRhi *rhi = m_window->rhi();
  QRhiResourceUpdateBatch *resourceUpdates = rhi->nextResourceUpdateBatch();

  if (m_verticesDirty) {
    m_vertexBuffer.reset();
    m_verticesDirty = false;
  }

  if (!m_vertexBuffer) {
    m_vertexBuffer.reset(
        rhi->newBuffer(QRhiBuffer::Immutable, QRhiBuffer::VertexBuffer,
                       m_vertices.count() * sizeof(QVector2D)));
    m_vertexBuffer->create();
    resourceUpdates->uploadStaticBuffer(m_vertexBuffer.get(),
                                        m_vertices.constData());
  }

  if (!m_uniformBuffer) {
    m_uniformBuffer.reset(
        rhi->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, 68));
    m_uniformBuffer->create();
  }

  if (!m_resourceBindings) {
    m_resourceBindings.reset(rhi->newShaderResourceBindings());
    m_resourceBindings->setBindings({QRhiShaderResourceBinding::uniformBuffer(
        0,
        QRhiShaderResourceBinding::VertexStage |
            QRhiShaderResourceBinding::FragmentStage,
        m_uniformBuffer.get())});
    m_resourceBindings->create();
  }

  if (!m_pipeline) {
    m_pipeline.reset(rhi->newGraphicsPipeline());
    m_pipeline->setFrontFace(renderTarget()->resourceType() ==
                                         QRhiResource::TextureRenderTarget &&
                                     rhi->isYUpInFramebuffer()
                                 ? QRhiGraphicsPipeline::CW
                                 : QRhiGraphicsPipeline::CCW);
    m_pipeline->setCullMode(QRhiGraphicsPipeline::Back);
    m_pipeline->setTopology(QRhiGraphicsPipeline::TriangleStrip);
    QRhiGraphicsPipeline::TargetBlend blend;
    blend.enable = true;
    m_pipeline->setTargetBlends({blend});
    m_pipeline->setShaderResourceBindings(m_resourceBindings.get());
    m_pipeline->setShaderStages(m_shaders.cbegin(), m_shaders.cend());
    m_pipeline->setDepthTest(true);
    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings({2 * sizeof(float)});
    inputLayout.setAttributes({{0, 0, QRhiVertexInputAttribute::Float2, 0}});
    m_pipeline->setVertexInputLayout(inputLayout);
    m_pipeline->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    m_pipeline->create();
  }

  const QMatrix4x4 mvp = *projectionMatrix() * *matrix();
  const float opacity = inheritedOpacity();
  resourceUpdates->updateDynamicBuffer(m_uniformBuffer.get(), 0, 64,
                                       mvp.constData());
  resourceUpdates->updateDynamicBuffer(m_uniformBuffer.get(), 64, 4, &opacity);
  commandBuffer()->resourceUpdate(resourceUpdates);
}

void ServoRenderNode::render(const RenderState *) {
  QRhiCommandBuffer *cb = commandBuffer();
  cb->setGraphicsPipeline(m_pipeline.get());
  QSize renderTargetSize = renderTarget()->pixelSize();
  cb->setViewport(
      QRhiViewport(0, 0, renderTargetSize.width(), renderTargetSize.height()));
  cb->setShaderResources();
  QRhiCommandBuffer::VertexInput vertexBindings[] = {{m_vertexBuffer.get(), 0}};
  cb->setVertexInput(0, 1, vertexBindings);
  cb->draw(m_vertices.count());
}

ServoWebView::ServoWebView(QQuickItem *parent) : QQuickItem(parent) {
  setFlag(ItemHasContents, true);
  connect(this, &ServoWebView::verticesChanged, this, &ServoWebView::update);
}

QList<QVector2D> ServoWebView::vertices() const { return m_vertices; }

void ServoWebView::setVertices(const QList<QVector2D> &newVertices) {
  if (m_vertices == newVertices)
    return;

  m_vertices = newVertices;
  Q_EMIT verticesChanged();
}

QSGNode *ServoWebView::updatePaintNode(QSGNode *old, UpdatePaintNodeData *) {
  ServoRenderNode *node = static_cast<ServoRenderNode *>(old);

  if (!node) {
    node = new ServoRenderNode(window());
  }

  node->setVertices(m_vertices);
  return node;
}