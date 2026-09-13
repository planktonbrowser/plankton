fn main() {
    cxx_build::bridge("src/lib.rs").compile("rust_backend");
    println!("cargo:rerun-if-changed=src/lib.rs");
}
