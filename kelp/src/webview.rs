use core::pin::Pin;
use cxx_qt_lib::QQuickItem;
use cxx_qt_lib::QString;
use cxx_qt_lib::QUrl;

/// The bridge definition for our QObject
#[cxx_qt::bridge]
pub mod qobject {

    unsafe extern "C++" {
        include!("cxx-qt-lib/qstring.h");
        /// An alias to the QString type
        type QString = cxx_qt_lib::QString;

        include!("cxx-qt-lib/qurl.h");
        type QUrl = cxx_qt_lib::QUrl;
    }

    extern "RustQt" {
        #[qobject]
        #[qml_element]
        #[qproperty(QString, title)]
        #[qproperty(QUrl, url)]
        #[base = QQuickItem]
        type ServoWebView = super::QServoWebViewRust;
    }
}

#[derive(Default)]
pub struct QServoWebViewRust {
    title: QString,
    url: QUrl,
}

impl qobject::ServoWebView {}
