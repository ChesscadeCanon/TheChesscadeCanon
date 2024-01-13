cmake_minimum_required(VERSION 3.13.0)
project(Chesscade)
set(SOLIB ${PROJECT_BINARY_DIR}/libChesscadeLib.so)
set(WINLIB ${PROJECT_BINARY_DIR}/ChesscadeLib.dll)
include_directories(src/core)
add_library(ChesscadeLib SHARED)
target_include_directories(ChesscadeLib PUBLIC export)
target_sources(ChesscadeLib PUBLIC src/export/api.cpp PRIVATE src/core/game.c rules.h src/core/histotrie.c src/core/board.c)
file(COPY assets DESTINATION ${PROJECT_SOURCE_DIR}/pygame_frontend)
add_custom_command(
	TARGET ChesscadeLib
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ChesscadeLib> ${PROJECT_SOURCE_DIR}/pygame_frontend/bin/
)
IF(false) # Command line app currently only works on Windows, and is turned off because the .exe gives Windows Defender a false positive. It's more of a debugging tool than a game anyway.
	IF(WIN32) 
		add_executable(ChesscadeApp)
		IF(WIN32)
		ELSE()
			find_package(Curses REQUIRED)
			include_directories(${CURSES_INCLUDE_DIR})
			target_link_libraries(ChesscadeApp ${CURSES_LIBRARIES})
		ENDIF()
		target_include_directories(ChesscadeApp PRIVATE src/standalone)
		target_sources(ChesscadeApp PRIVATE src/standalone/main.c src/core/game.c src/standalone/view.c src/standalone/control.c src/standalone/platform.c rules.h src/core/histotrie.c src/core/board.c)
		install(TARGETS ChesscadeApp DESTINATION bin)
	ENDIF()
ENDIF()
