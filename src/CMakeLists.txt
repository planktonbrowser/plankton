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

target_link_libraries(plankton
    PRIVATE
        Qt6::Core
        Qt6::Gui
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

