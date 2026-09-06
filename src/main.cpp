#include <KIconTheme>
#include <KLocalization>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QUrl>
#include <QtQml>
#include <QtWebEngineQuick>
#include <klocalizedqmlcontext.h>

int main(int argc, char *argv[]) {
  KIconTheme::initTheme();

  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
  QtWebEngineQuick::initialize();

  QApplication app(argc, argv);
  KLocalizedString::setApplicationDomain("browser");
  QApplication::setOrganizationName(QStringLiteral("plankton"));
  QApplication::setOrganizationDomain(QStringLiteral("plankton.org"));
  QApplication::setApplicationName(QStringLiteral("Plankton Browser"));
  QApplication::setDesktopFileName(QStringLiteral("org.plankton.browser"));

  QApplication::setStyle(QStringLiteral("breeze"));
  if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
    QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
  }

  QQmlApplicationEngine engine;

  KLocalization::setupLocalizedContext(&engine);
  engine.loadFromModule("org.plankton.browser", "Main");

  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  return app.exec();
}
