use cxx_qt_build::{CxxQtBuilder, QmlModule};

fn main() {
    unsafe {
        CxxQtBuilder::new_qml_module(
            QmlModule::new("org.plankton.browser").qml_file("src/qml/Main.qml"),
        )
        .cc_builder(|cc| {
            cc.link_lib_modifier("+whole-archive");
        })
        .build();
    }
}
