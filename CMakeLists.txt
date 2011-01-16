project(qpass)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

set(QPASS_VERSION 0.3.2)
set(QPASS_WIN_VERSION 0,3,2,0)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(TRANSLATIONS_FILES locale/qpass_pl_PL.ts)
set(QPass_SRCS QPass.cpp database.cpp password-dialog.cpp export-dialog.cpp import-dialog.cpp about.cpp main.cpp)
qt4_automoc(${QPass_SRCS})
QT4_ADD_RESOURCES(QPass_RCC_SRCS qpass.rcc)

configure_file(${CMAKE_SOURCE_DIR}/qpass-config.h.cmake ${CMAKE_BINARY_DIR}/qpass-config.h)
configure_file(${CMAKE_SOURCE_DIR}/qpass.desktop.cmake ${CMAKE_BINARY_DIR}/qpass.desktop)

if(WIN32)
    if(MINGW)
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resource.o
    COMMAND windres.exe -I"${CMAKE_CURRENT_BINARY_DIR}"
    -i ${CMAKE_CURRENT_SOURCE_DIR}/resource.rc
    -o ${CMAKE_CURRENT_BINARY_DIR}/resource.o)
    add_executable(qpass WIN32 ${QPass_SRCS} ${QPass_RCC_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/resource.o)
    endif(MINGW)
else(WIN32)
  add_executable(qpass ${QPass_SRCS} ${QPass_RCC_SRCS})
endif(WIN32)

target_link_libraries(qpass ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} gcrypt gpg-error)

qt4_add_translation(QM_FILES ${TRANSLATIONS_FILES})
add_custom_target (translations DEPENDS ${QM_FILES})
add_dependencies (qpass translations)



if(WIN32)
	set(QPASS_BIN_DIR ".")
	set(QPASS_DATA_DIR ".")
else(WIN32)
	set(QPASS_BIN_DIR "bin")
	set(QPASS_DATA_DIR "share/qpass")
endif(WIN32)

install(TARGETS qpass DESTINATION ${QPASS_BIN_DIR})
install (FILES ${QM_FILES} DESTINATION ${QPASS_DATA_DIR}/translations)

if (UNIX AND NOT APPLE)
	install (FILES qpass.png DESTINATION "share/icons/hicolor/48x48/apps" RENAME qpass.png)
	install (FILES ${CMAKE_BINARY_DIR}/qpass.desktop DESTINATION "share/applications" RENAME qpass.desktop)
endif(UNIX AND NOT APPLE)
