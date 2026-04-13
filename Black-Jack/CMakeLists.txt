# CMakeList.txt : CMake project for Black-Jack, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Black-Jack" VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

qt_standard_project_setup()

# Add source to this project's executable.
qt_add_executable(Black-Jack
header/card.h
header/deck.h
header/deck_controller.h
header/dialog.h
header/hand_class.h
header/hand_controller.h
header/mainwindow.h
src/card.cpp
src/deck.cpp
src/deck_controller.cpp
src/dialog.cpp
src/hand_class.cpp
src/hand_controller.cpp
src/mainwindow.cpp
src/mainwindow.ui
src/main.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/header)

target_link_libraries(Black-Jack PRIVATE Qt6::Widgets)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Black-Jack PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# For Windows: undgå at overskrive runtime settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(UnitTests
    unit_tests/test_hand.cpp
    unit_tests/test_deck.cpp
    header/card.h
    header/deck.h
    header/hand_class.h
    header/hand_controller.h
    src/card.cpp
    src/deck.cpp
    src/hand_class.cpp
    src/hand_controller.cpp
)

target_link_libraries(UnitTests
    PRIVATE
    gtest_main
    Qt6::Widgets
)

include(GoogleTest)
gtest_discover_tests(UnitTests)