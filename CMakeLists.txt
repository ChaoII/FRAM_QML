cmake_minimum_required(VERSION 3.16)

project(fram_qml VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(HuskarUI_DIR "D:/HuskarUI")


set(CMAKE_AUTORCC ON)

set(OpenCV_DIR "E:/CLionProjects/ModelDeploy/build/_deps/opencv-src/x64/vc17/staticlib")
set(ModelDeploy_DIR "E:/CLionProjects/ModelDeploy/build/install")

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Multimedia)
find_package(OpenCV)

qt_standard_project_setup()

qt_add_executable(appfram_qml
        src/main.cpp
        src/providers/CameraImageProvider.cpp
        src/providers/CameraImageProvider.h
        src/workers/FaceDetectionWorker.cpp
        src/workers/FaceDetectionWorker.h
        src/providers/FaceDetectionResultProvider.cpp
        src/providers/FaceDetectionResultProvider.h
        src/workers/CameraDecodeWorker.cpp
        src/workers/CameraDecodeWorker.h
        src/workers/WorkerBase.cpp
        src/workers/WorkerBase.h
        src/CameraManager.cpp
        src/CameraManager.h
        src/LocalCamera.cpp
        src/LocalCamera.h
        src/ThreadSafeQueue.h
        src/VideoProcessor.cpp
        src/VideoProcessor.h
)

qt_add_qml_module(appfram_qml
        URI fram_qml
        VERSION 1.0
        QML_FILES
        qml/Main.qml
        qml/AttendInfo.qml
        RESOURCES
        resources.qrc
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appfram_qml PROPERTIES
        #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appfram_qml
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

target_include_directories(appfram_qml PRIVATE
        ${HuskarUI_DIR}/HuskarUI/include
        ${ModelDeploy_DIR}/include
)

target_link_directories(appfram_qml PRIVATE
        ${HuskarUI_DIR}/HuskarUI/lib
        ${ModelDeploy_DIR}/lib
)


target_link_libraries(appfram_qml
        PRIVATE
        Qt6::Quick
        Qt6::Multimedia
        HuskarUIBasic
        ${OpenCV_LIBS}
        ModelDeploySDK
)

include(GNUInstallDirs)
install(TARGETS appfram_qml
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
