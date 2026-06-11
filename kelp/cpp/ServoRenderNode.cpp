#include <QSGRenderNode>
#include <ServoRenderNode.h>
#include <rhi/qrhi.h>

void ServoRenderNode::releaseResources() {
  m_vertexBuffer.reset();
  m_uniformBuffer.reset();
  m_pipeline.reset();
  m_resourceBindings.reset();
}

QSGRenderNode::RenderingFlags ServoRenderNode::flags() const {
  // We are rendering 2D content directly into the scene graph using QRhi, no
  // direct usage of a 3D API. Hence NoExternalRendering. This is a minor
  // optimization.

  // Additionally, the node takes the item transform into account by relying
  // on projectionMatrix() and matrix() (see prepare()) and never rendering at
  // other Z coordinates. Hence DepthAwareRendering. This is a potentially
  // bigger optimization.

  return QSGRenderNode::NoExternalRendering |
         QSGRenderNode::DepthAwareRendering;
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
