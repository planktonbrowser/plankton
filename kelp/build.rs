use cxx_qt_build::{CxxQtBuilder, QmlModule};

fn main() {
    CxxQtBuilder::new_qml_module(QmlModule::new("org.plankton.kelp"))
        .qt_module("OpenGL")
        .qt_module("Quick")
        .files(["src/webview.rs"])
        .build();
}
