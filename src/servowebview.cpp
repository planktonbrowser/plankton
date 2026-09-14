#include "servowebview.h"
#include "rust_backend/src/lib.rs.h"
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

static const float quadVertices[] = {
    // x,y, u, v
    -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
};

ServoRenderNode::ServoRenderNode(QQuickWindow *window) : m_window(window) {
  QFile file;
  file.setFileName(QStringLiteral(":/plankton/shaders/texturedquad.vert.qsb"));
  if (!file.open(QFile::ReadOnly)) {
    qFatal("Failed to load vertex shader");
  }
  m_shaders.append(QRhiShaderStage(QRhiShaderStage::Vertex,
                                   QShader::fromSerialized(file.readAll())));
  file.close();
  file.setFileName(QStringLiteral(":/plankton/shaders/texturedquad.frag.qsb"));
  if (!file.open(QFile::ReadOnly)) {
    qFatal("Failed to load fragment shader");
  }
  m_shaders.append((QRhiShaderStage(QRhiShaderStage::Fragment,
                                    QShader::fromSerialized(file.readAll()))));
}

void ServoRenderNode::releaseResources() {
  m_vertexBuffer.reset();
  m_uniformBuffer.reset();
  m_pipeline.reset();
  m_resourceBindings.reset();
  m_wrappedTex.reset();
  m_sampler.reset();
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

  if (!m_vertexBuffer) {
    m_vertexBuffer.reset(rhi->newBuffer(
        QRhiBuffer::Immutable, QRhiBuffer::VertexBuffer, sizeof(quadVertices)));
    m_vertexBuffer->create();
    resourceUpdates->uploadStaticBuffer(m_vertexBuffer.get(), quadVertices);
  }

  if (!m_uniformBuffer) {
    m_uniformBuffer.reset(
        rhi->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, 68));
    m_uniformBuffer->create();
  }

  if (!m_wrappedTex) {
    uint64_t vkImageHandle = hal_texture();

    QRhiTexture::NativeTexture nt{vkImageHandle,
                                  VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
    m_wrappedTex.reset(rhi->newTexture(QRhiTexture::RGBA8, QSize(512, 512), 1,
                                       QRhiTexture::Flags{}));
    m_wrappedTex->createFrom(nt);
    m_sampler.reset(rhi->newSampler(QRhiSampler::Linear, QRhiSampler::Linear,
                                    QRhiSampler::None, QRhiSampler::ClampToEdge,
                                    QRhiSampler::ClampToEdge));
    m_sampler->create();
  }

  if (!m_resourceBindings) {
    m_resourceBindings.reset(rhi->newShaderResourceBindings());
    m_resourceBindings->setBindings({
        QRhiShaderResourceBinding::uniformBuffer(
            0,
            QRhiShaderResourceBinding::VertexStage |
                QRhiShaderResourceBinding::FragmentStage,
            m_uniformBuffer.get()),
        QRhiShaderResourceBinding::sampledTexture(
            1, QRhiShaderResourceBinding::FragmentStage, m_wrappedTex.get(),
            m_sampler.get()),
    });
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
    inputLayout.setBindings({4 * sizeof(float)});
    inputLayout.setAttributes({
        {0, 0, QRhiVertexInputAttribute::Float2, 0},                 // position
        {0, 1, QRhiVertexInputAttribute::Float2, 2 * sizeof(float)}, // uv
    });
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
  cb->draw(4);
}

ServoWebView::ServoWebView(QQuickItem *parent) : QQuickItem(parent) {
  setFlag(ItemHasContents, true);
}

QSGNode *ServoWebView::updatePaintNode(QSGNode *old, UpdatePaintNodeData *) {
  ServoRenderNode *node = static_cast<ServoRenderNode *>(old);

  if (!node) {
    node = new ServoRenderNode(window());
  }

  // node->setVertices(m_vertices);
  return node;
}