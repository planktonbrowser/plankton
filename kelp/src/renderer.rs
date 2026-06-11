#[cxx_qt::bridge]
pub(crate) mod qobject {
    unsafe extern "C++" {
        include!(<QSGNode>);
        type QSGNode;
    }

    unsafe extern "RustQt" {
        #[base = QSGNode]
        type QServoRenderer = super::QServoRendererRust;
    }
}

struct QServoRendererRust {}
