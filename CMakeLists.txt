cmake_minimum_required(VERSION 3.16)

project(fram_qml VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCV_DIR "E:/develop/opencv5.x/x64/vc17/staticlib")
set(ModelDeploy_DIR "E:/CLionProjects/ModelDeploy/build/install")
set(faiss_DIR "D:/faiss/install/share/faiss")
set(HuskarUI_DIR "D:/HuskarUI")

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Multimedia Concurrent)
find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)
find_package(faiss REQUIRED)
find_package(SQLite3 REQUIRED)

qt_standard_project_setup()

qt_add_executable(app_fram_qml
        main.cpp
        core/VectorSearch.cpp
        core/VectorSearch.h
        core/dbHelper.h
        schema/schema.h
        core/dbHelper.cpp
        task/ScheduledTask.cpp
        task/ScheduledTask.h
        task/ClearOldAttendInfoTask.cpp
        task/ClearOldAttendInfoTask.h
)

qt_add_qml_module(app_fram_qml
        URI fram_qml
        VERSION 1.0
        QML_FILES
        qml/Main.qml
        qml/MainPage.qml
        qml/AttendInfo.qml
        qml/RegisterPage.qml
        qml/Settings.qml
        qml/CameraManager.qml
        SOURCES
        videoanalyzer.h
        videoanalyzer.cpp
        utils.h
        utils.cpp
        registerface.h
        registerface.cpp
        workers/FaceDetectionWorker.h
        workers/FaceDetectionWorker.cpp
        workers/FaceRecognizerWorker.h
        workers/FaceRecognizerWorker.cpp


)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(app_fram_qml PROPERTIES
        #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appcamera
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)


target_include_directories(app_fram_qml PRIVATE
        "${ModelDeploy_DIR}/include"
        ${HuskarUI_DIR}/HuskarUI/include)

target_link_directories(app_fram_qml PRIVATE
        ${ModelDeploy_DIR}/lib
        ${HuskarUI_DIR}/HuskarUI/lib
)
target_link_libraries(app_fram_qml
        PRIVATE
        Qt6::Quick
        Qt6::Concurrent
        Qt6::Multimedia
        HuskarUIBasic
        ${OpenCV_LIBS}
        ModelDeploySDK
        faiss
        SQLite::SQLite3
)

include(GNUInstallDirs)
install(TARGETS app_fram_qml
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
