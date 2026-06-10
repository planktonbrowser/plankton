#[cxx_qt::bridge]
pub(crate) mod qobject {
    unsafe extern "C++" {

        include!("cxx-qt-lib/qstring.h");
        type QString = cxx_qt_lib::QString;

        include!("cxx-qt-lib/qurl.h");
        type QUrl = cxx_qt_lib::QUrl;
    }

    unsafe extern "RustQt" {
        #[qobject]
        #[qml_element]
        #[qproperty(QString, title)]
        #[qproperty(QUrl, url)]
        type ServoWebView = super::QServoWebViewRust;
    }
}
