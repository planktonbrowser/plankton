use kelp::qquick_servo_engine_view::load_html;

fn main() {
    load_html(
        String::from("<h1>Hello World!<h1>"),
        String::from("https://libremelon.com"),
    );
}
