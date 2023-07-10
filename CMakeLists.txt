cmake_minimum_required(VERSION 3.26)

project(prometheus_app VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR, ON)
set(CMAKE_PREFIX_PATH "C:/Qt/6.5.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Quick SerialPort)

qt_standard_project_setup()

qt_add_executable(prometheus WIN32 src/main.cpp src/RtMidi.h src/RtMidi.cpp src/backend.h src/backend.cpp)

qt_add_qml_module(prometheus
    URI prometheus_app
    VERSION 1.0
    QML_FILES ui/Main.qml ui/Fader.qml
)

add_definitions(-D__WINDOWS_MM__)

target_link_libraries(prometheus PRIVATE Qt6::Quick Qt6::SerialPort Winmm)

install(TARGETS prometheus
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET prometheus
    OUTPUT_SCRIPT deployApp
)
install(SCRIPT "${deployApp}")