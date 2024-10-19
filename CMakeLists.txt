cmake_minimum_required(VERSION 3.13.0)
project(Chesscade)
set(SOLIB ${PROJECT_BINARY_DIR}/libChesscadeLib.so)
set(WINLIB ${PROJECT_BINARY_DIR}/ChesscadeLib.dll)
include_directories(src/core src)
add_library(ChesscadeLib SHARED)
target_include_directories(ChesscadeLib PUBLIC export)
target_sources(ChesscadeLib PUBLIC src/export/api.cpp PRIVATE src/core/game.c rules.h src/core/histotrie.c src/core/board.c)
file(COPY assets DESTINATION ${PROJECT_SOURCE_DIR}/godot_frontend/app)
file(COPY assets DESTINATION ${PROJECT_SOURCE_DIR}/godot_43_frontend/app)
file(COPY assets DESTINATION ${PROJECT_SOURCE_DIR}/pygame_frontend)
add_custom_command(
	TARGET ChesscadeLib
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ChesscadeLib> ${PROJECT_SOURCE_DIR}/pygame_frontend/bin/
	COMMENT "Copied library to pygame_frontend/bin"
)
add_executable(ChesscadeApp)
target_include_directories(ChesscadeApp PRIVATE src/standalone)
IF(WIN32)
	target_sources(ChesscadeApp PRIVATE src/standalone/main.c src/core/game.c src/standalone/view.c src/standalone/control.c rules.h src/core/histotrie.c src/core/board.c src/standalone/application.c src/standalone/main.c src/standalone/tests.c src/core/simulator.c)
ELSE()
	target_sources(ChesscadeApp PRIVATE src/standalone/main.c src/core/game.c rules.h src/core/histotrie.c src/core/board.c src/standalone/main.c src/standalone/tests.c src/core/simulator.c)
ENDIF()
install(TARGETS ChesscadeApp DESTINATION bin)
