
set(RUST_TARGET_DIR "${CMAKE_SOURCE_DIR}/rust_backend/target")

add_custom_target(cargo_build ALL
    COMMAND cargo build --release
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/rust_backend"
    BYPRODUCTS "${RUST_TARGET_DIR}/release/librust_backend.a"
)


add_executable(plankton)

ecm_add_qml_module(plankton
    URI "org.plankton.browser"
)

target_sources(plankton
    PRIVATE
    main.cpp
    servowebview.h
    servowebview.cpp
)

ecm_target_qml_sources(plankton
    SOURCES
    Main.qml
)

qt6_add_shaders(plankton "plankton_shaders"
    # GLSL "300 es,130"
    PREFIX
        "/plankton"
    FILES
        "shaders/customrender.vert"
        "shaders/customrender.frag"
)

add_dependencies(plankton cargo_build)

target_include_directories(plankton PRIVATE
    "${RUST_TARGET_DIR}/cxxbridge/crate"
    "${RUST_TARGET_DIR}/cxxbridge"
)

target_link_libraries(plankton
    PRIVATE
        "${RUST_TARGET_DIR}/release/librust_backend.a"
        dl pthread m
        Qt6::Core
        Qt6::Gui
        Qt6::GuiPrivate
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Widgets
        KF6::CoreAddons
        KF6::I18n
        KF6::I18nQml
        KF6::IconThemes
        KF6::Kirigami
        Qt6::WebEngineQuick
)

