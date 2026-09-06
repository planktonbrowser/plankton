#include "servowebview.h"
#include <QDebug>
#include <qlogging.h>
#include <qquickitem.h>
#include <qtmetamacros.h>

ServoWebView::ServoWebView(QQuickItem *parent) : QQuickItem(parent) {}

QString ServoWebView::text() const { return m_text; }

void ServoWebView::setText(const QString &text) {
  if (m_text == text)
    return;
  m_text = text;
  Q_EMIT textChanged();
}

void ServoWebView::doSomething() { qWarning() << "Current text:" << m_text; }