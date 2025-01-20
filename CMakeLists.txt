cmake_minimum_required(VERSION 3.16)

project(TestBinance VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(IOS)
    set(CMAKE_SYSTEM_NAME iOS)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "16.0")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    # Отключаем создание bundle для iOS
    set(CMAKE_MACOSX_BUNDLE TRUE)
    # Устанавливаем тип компоновки
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2 WebSockets Sql Network)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appTestBinance
    main.cpp
)

qt_add_qml_module(appTestBinance
    URI TestBinance
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES websocker_worker.h websocker_worker.cpp
        SOURCES websocket_manager.h websocket_manager.cpp
        QML_FILES QML/App.qml
        SOURCES databasemanager.h databasemanager.cpp
        SOURCES d_account.h d_account.cpp
        SOURCES db_mapper.h db_mapper.cpp
        SOURCES m_account.h m_account.cpp
        QML_FILES QML/AppSelector.qml
        QML_FILES QML/Accounts.qml
        QML_FILES QML/AccountMain.qml
        QML_FILES QML/AccountForm.qml
        RESOURCES diaaccount.qmodel
        SOURCES BinanceTask.h
        SOURCES binance_manager.h binance_manager.cpp
        SOURCES service_manager.h service_manager.cpp
        QML_FILES QML/Trade.qml
        SOURCES d_asset.h d_asset.cpp
        SOURCES app_namespaces.h
        QML_FILES QML/Controls/MMT_Button.qml
        QML_FILES QML/const.js
)

# Добавляем qtquickcontrols2.conf в ресурсы
qt_add_resources(appTestBinance "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appTestBinance PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appTestBinance
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appTestBinance
    PRIVATE Qt6::Quick
    PRIVATE Qt6::QuickControls2
    PRIVATE Qt6::WebSockets
    PRIVATE Qt6::Core
    PRIVATE Qt6::Sql
    PRIVATE Qt6::Network
)

include(GNUInstallDirs)
install(TARGETS appTestBinance
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
