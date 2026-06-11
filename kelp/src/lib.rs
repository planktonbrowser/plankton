pub(crate) mod renderer;
pub mod webview;

pub mod qquick_servo_engine_view {
    pub fn load_html(html: String, url: String) {
        println!("html: {}\nurl: {}", html, url);
    }
}
