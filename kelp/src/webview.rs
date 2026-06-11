use crate::renderer::{self, qobject};
use cxx_qt_lib::{QString, QUrl};

#[cxx_qt::bridge]
pub(crate) mod qobject {
    unsafe extern "C++" {
        include!("cpp/helpers.h");

        include!("cxx-qt-lib/qstring.h");
        type QString = cxx_qt_lib::QString;

        include!("cxx-qt-lib/qurl.h");
        type QUrl = cxx_qt_lib::QUrl;

        include!(<QQuickItem>);
        type QQuickItem;

        include!(<QSGNode>);
        type QSGNode;
    }

    unsafe extern "RustQt" {
        #[qobject]
        #[base = QQuickItem]
        #[qml_element]
        #[qproperty(QString, title)]
        #[qproperty(QUrl, url)]
        type ServoWebView = super::QServoWebViewRust;

        #[cxx_override]
        #[cxx_name = "updatePaintNode"]
        fn update_paint_node(self: &QSGNode, update_paint_node_data: *mut Pin<QSGNode>);
    }
}

#[derive(Default)]
pub struct QServoWebViewRust {
    title: QString,
    url: QUrl,
}

impl qobject::ServoWebView {
    fn update_paint_node(&self, update_paint_node_data: *mut Pin<QSGNode>) {}
}
