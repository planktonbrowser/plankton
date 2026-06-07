use crate::webview::qobject::QUrl;
use std::pin::Pin;

#[cxx_qt::bridge]
pub(crate) mod qobject {
    unsafe extern "C++" {
        include!(<QQuickFramebufferObject>);
        type QQuickFramebufferObject;

        include!(<QOpenGLFramebufferObject>);
        type QOpenGLFramebufferObject;

        include!("helpers.h");
        type QQuickFramebufferObjectRenderer;

        #[cxx_name = "constructUniquePtr"]
        fn qservo_renderer_unique_ptr() -> UniquePtr<QServoRenderer>;
    }

    unsafe extern "RustQt" {
        #[base = QQuickFramebufferObjectRenderer]
        pub(crate) type QServoRenderer = super::QServoRendererRust;

        #[inherit]
        #[rust_name = "framebuffer_object"]
        fn framebufferObject(self: &QServoRenderer) -> *mut QOpenGLFramebufferObject;

        #[cxx_override]
        fn render(self: Pin<&mut QServoRenderer>);

        #[cxx_override]
        unsafe fn syncrhonize(self: Pin<&mut QServoRenderer>, item: *mut QQuickFramebufferObject);
    }
}

#[derive(Default)]
pub struct QServoRendererRust {
    url: QUrl,
}

impl qobject::QServoRenderer {
    pub(crate) fn new() -> cxx::UniquePtr<Self> {
        qobject::qservo_renderer_unique_ptr()
    }

    fn render(mut self: Pin<&mut Self>) {
        println!("Render start");
    }

    unsafe fn syncrhonize(mut self: Pin<&mut Self>, item: *mut qobject::QQuickFramebufferObject) {}
}
