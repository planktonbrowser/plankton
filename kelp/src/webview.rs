use crate::renderer::qobject::QServoRenderer;
use core::pin::Pin;
use cxx_qt_lib::QString;
use cxx_qt_lib::QUrl;

/// The bridge definition for our QObject
#[cxx_qt::bridge]
pub(crate) mod qobject {

    unsafe extern "C++" {
        include!("cxx-qt-lib/qstring.h");
        /// An alias to the QString type
        type QString = cxx_qt_lib::QString;

        include!("cxx-qt-lib/qurl.h");
        type QUrl = cxx_qt_lib::QUrl;

        include!(<QQuickFramebufferObject>);
        type QQuickFramebufferObject;

        include!("helpers.h");
        type QQuickFramebufferObjectRenderer;

    }

    unsafe extern "RustQt" {
        #[qobject]
        #[qml_element]
        #[qproperty(QString, title)]
        #[qproperty(QUrl, url)]
        #[base = QQuickFramebufferObject]
        type ServoWebView = super::QServoWebViewRust;

        #[cxx_override]
        #[cxx_name = "createRenderer"]
        fn create_renderer(self: &ServoWebView) -> *mut QQuickFramebufferObjectRenderer;

        #[inherit]
        fn update(self: Pin<&mut ServoWebView>);
    }

    impl cxx_qt::Constructor<()> for ServoWebView {}
    // impl cxx_qt::Threading for ServoWebView {}
}

#[derive(Default)]
pub struct QServoWebViewRust {
    title: QString,
    url: QUrl,
}

impl qobject::ServoWebView {
    fn create_renderer(&self) -> *mut qobject::QQuickFramebufferObjectRenderer {
        QServoRenderer::new().into_raw() as *mut qobject::QQuickFramebufferObjectRenderer
    }
}

impl cxx_qt::Initialize for qobject::ServoWebView {
    fn initialize(mut self: Pin<&mut Self>) {
        // TODO: we don't support QFlags so just enable everything for now
        // self.as_mut()
        //     .set_accepted_mouse_buttons(QMouseEventButton::AllButtons);
        //
        // self.as_mut().set_accept_touch_events(true);
        // self.as_mut().set_mirror_vertically(true);
        //
        // When the URL changes trigger QQuickFramebufferObject::update
        // which then triggers QQuickFramebufferObject::Renderer::synchronize
        self.on_url_changed(|qobject| {
            qobject.update();
        })
        .release();
    }
}
