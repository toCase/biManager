cmake_minimum_required(VERSION 3.16)

project(TestBinance VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(IOS)
    set(CMAKE_SYSTEM_NAME iOS)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "17.0")
    # Устанавливаем тип компоновки
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(QT_NO_FFMPEG_XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY ON)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2 WebSockets Sql Network)

qt_standard_project_setup(REQUIRES 6.8)

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
        RESOURCES ios/Info.plist
)

# Добавляем qtquickcontrols2.conf в ресурсы
qt_add_resources(appTestBinance "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

if(IOS)
    qt_add_ios_ffmpeg_libraries(appTestBinance)        
endif()

set_target_properties(appTestBinance PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.mmtool.appTestBinance"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.mmtool.appTestBinance"
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/ios/Info.plist"
    XCODE_ATTRIBUTE_INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ios/Info.plist"
    XCODE_ATTRIBUTE_SKIP_INSTALL NO
    XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Добавьте эти строки для принудительного использования вашего Info.plist
set_target_properties(appTestBinance PROPERTIES
    XCODE_ATTRIBUTE_GENERATE_INFOPLIST_FILE NO
    XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS NO
    XCODE_ATTRIBUTE_COPY_PHASE_STRIP NO
    XCODE_ATTRIBUTE_ENABLE_BITCODE NO
    XCODE_ATTRIBUTE_ENABLE_TESTABILITY NO
    XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS YES
    XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH YES
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
