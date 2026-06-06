use cxx_qt_build::{CxxQtBuilder, QmlModule};

fn main() {
    unsafe {
        CxxQtBuilder::new_qml_module(QmlModule::new("org.plankton.kelp"))
            .qt_module("OpenGL")
            .qt_module("Quick")
            .cc_builder(|cc| {
                cc.include("cpp");
                cc.file("cpp/helpers.cpp");
                println!("cargo:rerun-if-changed=cpp/helpers.cpp");
            })
            .file("src/webview.rs")
            .cpp_file("cpp/helpers.h")
            .build();
    }
}
