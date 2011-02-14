PROJECT(QPass)

cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(Qt4 REQUIRED)

SET(QPASS_VERSION 1.0.2)
SET(QPASS_WIN_VERSION 1,0,2,0)

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
	PredefinedSettings.cpp
	TrayIcon.cpp
	dialogs/PreviousPasswordDialog.cpp
	dialogs/NewDatabaseDialog.cpp
	dialogs/AboutDialog.cpp
	dialogs/DatabaseExportDialog.cpp
	dialogs/DatabaseImportDialog.cpp
	dialogs/PasswordChangeDialog.cpp
	dialogs/PasswordGeneratorDialog.cpp
)
set(QPass_HEADERS 
	MainWindow.h 
	DataModel.h 
	TrayIcon.h
	dialogs/PreviousPasswordDialog.h
	dialogs/NewDatabaseDialog.h
	dialogs/AboutDialog.h
	dialogs/DatabaseExportDialog.h
	dialogs/DatabaseImportDialog.h
	dialogs/PasswordChangeDialog.h
	dialogs/PasswordGeneratorDialog.h
)
set(QPass_UI 
	MainWindow.ui
	dialogs/PreviousPasswordDialog.ui
	dialogs/NewDatabaseDialogPage1.ui
	dialogs/NewDatabaseDialogPage2.ui
	dialogs/AboutDialog.ui
	dialogs/DatabaseExportDialog.ui
	dialogs/DatabaseImportDialog.ui
	dialogs/PasswordChangeDialog.ui
	dialogs/PasswordGeneratorDialogPage1.ui
	dialogs/PasswordGeneratorDialogPage3.ui
)
set(QPass_TS
	pl_PL.ts
)
include_directories(${INCLUDE_DIR})
QT4_ADD_TRANSLATION(QPass_QM translations/${QPass_TS})
QT4_WRAP_UI(QPass_UI_HDRS ${QPass_UI})
QT4_WRAP_CPP(QPass_HEADERS_MOC ${QPass_HEADERS})
QT4_ADD_RESOURCES(QPass_RCC_SRCS images/qpass.qrc)

#if you are going to change below values look and change values in PredefinedSettings.cpp if necessary
if(WIN32)
	set(QPASS_BIN_DIR ".")
	set(QPASS_DATA_DIR ".")
else(WIN32)
	set(QPASS_BIN_DIR "bin")
	set(QPASS_DATA_DIR "share/qpass")
endif(WIN32)

configure_file (${CMAKE_SOURCE_DIR}/qpass-config.h.cmake ${CMAKE_BINARY_DIR}/qpass-config.h)

ADD_DEFINITIONS(${QT_DEFINITIONS})

if(WIN32)
	if(MINGW)
	ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/win_res.o
	COMMAND windres.exe -I"${CMAKE_CURRENT_BINARY_DIR}"
	-i ${CMAKE_CURRENT_SOURCE_DIR}/win_res.rc
	-o ${CMAKE_CURRENT_BINARY_DIR}/win_res.o)
	add_executable(qpass WIN32 ${QPass_SOURCES} ${QPass_HEADERS_MOC} ${QPass_UI_HDRS} ${QPass_RCC_SRCS} ${QPass_QM} ${CMAKE_CURRENT_BINARY_DIR}/win_res.o)
	endif(MINGW)
else(WIN32)
ADD_EXECUTABLE(qpass ${QPass_SOURCES} ${QPass_HEADERS_MOC} ${QPass_UI_HDRS} ${QPass_RCC_SRCS} ${QPass_QM})
endif(WIN32)

TARGET_LINK_LIBRARIES(qpass ${QT_LIBRARIES} gcrypt gpg-error)


install(TARGETS qpass DESTINATION ${QPASS_BIN_DIR})
install(FILES ${QPass_QM} DESTINATION ${QPASS_DATA_DIR}/translations)
install(FILES COPYING DESTINATION ${QPASS_DATA_DIR})
install(FILES README DESTINATION ${QPASS_DATA_DIR})

if (UNIX AND NOT APPLE)
	install (FILES images/hi16-app-qpass.png DESTINATION "share/icons/hicolor/16x16/apps" RENAME qpass.png)
	install (FILES images/hi22-app-qpass.png DESTINATION "share/icons/hicolor/22x22/apps" RENAME qpass.png)
	install (FILES images/hi24-app-qpass.png DESTINATION "share/icons/hicolor/24x24/apps" RENAME qpass.png)
	install (FILES images/hi32-app-qpass.png DESTINATION "share/icons/hicolor/32x32/apps" RENAME qpass.png)
	install (FILES images/hi48-app-qpass.png DESTINATION "share/icons/hicolor/48x48/apps" RENAME qpass.png)
	install (FILES images/hi48-app-qpass.png DESTINATION "share/pixmaps" RENAME qpass.png)
	install (FILES qpass.desktop DESTINATION "share/applications" RENAME qpass.desktop)
endif(UNIX AND NOT APPLE)
