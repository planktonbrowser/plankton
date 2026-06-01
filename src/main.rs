use std::env;

use cxx_qt_lib::{QGuiApplication, QQmlApplicationEngine, QQuickStyle, QString, QUrl};
use cxx_qt_lib_extras::QApplication;
use kelp::qquick_servo_engine_view::load_html;

fn main() {
    // load_html(
    //     String::from("<h1>Hello World!<h1>"),
    //     String::from("https://libremelon.com"),
    // );

    let mut app = QApplication::new();
    QGuiApplication::set_desktop_file_name(&QString::from("org.plankton.browser"));

    if env::var("QT_QUICK_CONTROLS_STYLE").is_err() {
        QQuickStyle::set_style(&QString::from("org.kde.desktop"));
    }

    let mut engine = QQmlApplicationEngine::new();
    if let Some(engine) = engine.as_mut() {
        engine.load(&QUrl::from(
            "qrc:/qt/qml/org/plankton/browser/src/qml/Main.qml",
        ));
    }

    if let Some(app) = app.as_mut() {
        app.exec();
    }
}
