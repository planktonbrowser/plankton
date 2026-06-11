#include <QQuickItem>
#include <ServoRenderNode.h>
#include <ServoWebView.h>

ServoWebView::ServoWebView(QQuickItem *parent) {
  setFlag(ItemHasContents, true);
  connect(this, &ServoWebView::verticesChanged(), this, &ServoWebView::update);
}

QSGNode *ServoWebView::updatePaintNode(QSGNode *old, UpdatePaintNodeData *) {
  ServoRenderNode *node = static_cast<ServoRenderNode *>(old);

  if (!node)
    node = new ServoRenderNode(window());

  node->setVertices(m_vertices);

  return node;
}
