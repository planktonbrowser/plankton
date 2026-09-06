#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <qquickitem.h>
#include <qtmetamacros.h>

class ServoWebView : public QQuickItem {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
  explicit ServoWebView(QQuickItem *parent = nullptr);

  QString text() const;
  void setText(const QString &text);

  Q_INVOKABLE void doSomething();

Q_SIGNALS:
  void textChanged();

private:
  QString m_text;
};