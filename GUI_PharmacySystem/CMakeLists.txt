cmake_minimum_required(VERSION 3.16)
project(PharmacyManagementSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============ ADD THESE LINES ============
# Tell CMake where to find Qt (adjust the path if needed)
set(CMAKE_PREFIX_PATH "C:/Qt/6.10.1/mingw_64")
# OR if you installed elsewhere, use the correct path
# set(CMAKE_PREFIX_PATH "C:/Qt/6.6.0/mingw_64")
# set(CMAKE_PREFIX_PATH "C:/Program Files/Qt/6.7.0/mingw_64")
# =========================================

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    main.cpp
    pharmacySystem.cpp
    loginwindow.cpp
    managerwindow.cpp
    pharmacistwindow.cpp
    medicineswindow.cpp
    billswindow.cpp
    reportswindow.cpp
)

set(HEADERS
    pharmacySystem.h
    loginwindow.h
    managerwindow.h
    pharmacistwindow.h
    medicineswindow.h
    billswindow.h
    reportswindow.h
)

add_executable(PharmacyManagementSystem ${SOURCES} ${HEADERS}
    fix_includes.bat)

target_link_libraries(PharmacyManagementSystem Qt6::Widgets)

# Copy stylesheet to build directory
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/styles.qss")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/styles.qss ${CMAKE_CURRENT_BINARY_DIR}/styles.qss COPYONLY)
endif()

# Installation
install(TARGETS PharmacyManagementSystem DESTINATION bin)
