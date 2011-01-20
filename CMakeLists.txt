PROJECT(QPass)

cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(Qt4 REQUIRED)

SET(INCLUDE_DIR
   ${CMAKE_CURRENT_BINARY_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}
   dialogs
   ${QT_INCLUDE_DIR}
)

INCLUDE(${QT_USE_FILE})

set(QPass_SOURCES 
   main.cpp 
   MainWindow.cpp 
   DataModel.cpp 
   DataAccess.cpp
   dialogs/PreviousPasswordDialog.cpp
   dialogs/NewDatabaseDialog.cpp
)
set(QPass_HEADERS 
   MainWindow.h 
   DataModel.h 
   dialogs/PreviousPasswordDialog.h
   dialogs/NewDatabaseDialog.h
)
set(QPass_UI 
   MainWindow.ui
   dialogs/PreviousPasswordDialog.ui
   dialogs/NewDatabaseDialogPage1.ui
   dialogs/NewDatabaseDialogPage2.ui
)
include_directories(${INCLUDE_DIR})
QT4_WRAP_UI(QPass_UI_HDRS ${QPass_UI})
QT4_WRAP_CPP(QPass_HEADERS_MOC ${QPass_HEADERS})


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_EXECUTABLE(QPass ${QPass_SOURCES} ${QPass_HEADERS_MOC} ${QPass_UI_HDRS})
TARGET_LINK_LIBRARIES(QPass ${QT_LIBRARIES} gcrypt gpg-error)
