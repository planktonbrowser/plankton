use cxx_qt_build::{CxxQtBuilder, QmlModule};

fn main() {
    CxxQtBuilder::new_qml_module(QmlModule::new("org.plankton.kelp"))
        .file("src/webview.rs")
        .build();
}
