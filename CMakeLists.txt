PROJECT(QPass)

cmake_minimum_required(VERSION 2.8)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Network REQUIRED)

SET(QPASS_VERSION "2.0.0")
SET(QPASS_WIN_VERSION 2,0,0,0)


FIND_LIBRARY(LIBGCRYPT "gcrypt")
FIND_LIBRARY(LIBGPGERROR "gpg-error")

SET(INCLUDE_DIR
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	dialogs
)

set(QPass_SOURCES 
	main.cpp 
	App.cpp
	MainWindow.cpp 
	DataModel.cpp 
	DataAccess.cpp
	PredefinedSettings.cpp
	TrayIcon.cpp
	UpdateChecker.cpp
	CsvFormat.cpp
	Benchmark.cpp
	dialogs/PreviousPasswordDialog.cpp
	dialogs/NewDatabaseDialog.cpp
	dialogs/AboutDialog.cpp
	dialogs/DatabaseExportDialog.cpp
	dialogs/DatabaseImportDialog.cpp
	dialogs/PasswordChangeDialog.cpp
	dialogs/PasswordGeneratorDialog.cpp
	dialogs/PreferencesDialog.cpp
	dialogs/UpdateCheckerDialog.cpp
    dialogs/OtherDatabaseDialog.cpp
    dialogs/PasswordViewer.cpp
	dialogs/ColumnOrganizationDialog.cpp
)
set(QPass_HEADERS 
	App.h
	MainWindow.h 
	DataModel.h 
	TrayIcon.h
	UpdateChecker.h
	Benchmark.h
	dialogs/PreviousPasswordDialog.h
	dialogs/NewDatabaseDialog.h
	dialogs/AboutDialog.h
	dialogs/DatabaseExportDialog.h
	dialogs/DatabaseImportDialog.h
	dialogs/PasswordChangeDialog.h
	dialogs/PasswordGeneratorDialog.h
	dialogs/PreferencesDialog.h
	dialogs/UpdateCheckerDialog.h
    dialogs/OtherDatabaseDialog.h
    dialogs/PasswordViewer.h
	dialogs/ColumnOrganizationDialog.h
)
set(QPass_UI 
	MainWindow.ui
	dialogs/PreviousPasswordDialog.ui
	dialogs/NewDatabaseDialog.ui
	dialogs/AboutDialog.ui
	dialogs/DatabaseExportDialog.ui
	dialogs/DatabaseImportDialog.ui
	dialogs/PasswordChangeDialog.ui
	dialogs/PasswordGeneratorDialogPage1.ui
	dialogs/PasswordGeneratorDialogPage3.ui
	dialogs/PreferencesDialog.ui
	dialogs/UpdateCheckerDialog.ui
    dialogs/OtherDatabaseDialog.ui
    dialogs/PasswordViewer.ui
	dialogs/ColumnOrganizationDialog.ui
)
set(QPass_TS
	translations/pl_PL.ts
   translations/qt_pl.ts
	translations/it_IT.ts
	translations/qt_it.ts
	translations/nl_NL.ts
)
include_directories(${INCLUDE_DIR})
QT5_CREATE_TRANSLATION(QPass_QM ${QPass_TS})
QT5_WRAP_UI(QPass_UI_HDRS ${QPass_UI})
QT5_WRAP_CPP(QPass_HEADERS_MOC ${QPass_HEADERS})
QT5_ADD_RESOURCES(QPass_RCC_SRCS images/qpass.qrc)

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
		set(CMAKE_RC_COMPILER_INIT windres)
		enable_language(RC)
		set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")

		ADD_EXECUTABLE(qpass WIN32 ${QPass_SOURCES} ${QPass_HEADERS_MOC} ${QPass_UI_HDRS} ${QPass_RCC_SRCS} ${QPass_QM} win_res.rc) 
	endif(MINGW)
else(WIN32)
	ADD_EXECUTABLE(qpass ${QPass_SOURCES} ${QPass_HEADERS_MOC} ${QPass_UI_HDRS} ${QPass_RCC_SRCS} ${QPass_QM})
endif(WIN32)

TARGET_LINK_LIBRARIES(qpass 
                      Qt5::Widgets
                      Qt5::Network
                      ${LIBGCRYPT}
                      ${LIBGPGERROR})

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
