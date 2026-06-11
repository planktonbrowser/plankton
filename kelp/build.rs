use cxx_qt_build::{CxxQtBuilder, QmlModule};

fn main() {
    unsafe {
        CxxQtBuilder::new_qml_module(QmlModule::new("org.plankton.kelp"))
            .qt_module("OpenGL")
            .qt_module("Quick")
            .qt_module("GuiPrivate")
            .files(["src/webview.rs"])
            .cc_builder(|cc| {
                cc.include("cpp");
                cc.files(["cpp/ServoRenderNode.cpp", "cpp/ServoWebView.cpp"]);
            })
            .cpp_file("cpp/ServoRenderNode.h")
            .cpp_file("cpp/ServoWebView.h")
            .build();
    }
}
