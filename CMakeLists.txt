cmake_minimum_required(VERSION 3.3)
# Project Name
project(async_udp)

# Find Packages
find_package(Boost)

# Check presence of boost
if(Boost_FOUND)
    message("*** Boost Dependancy Found ***")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_library(
    ${PROJECT_NAME} STATIC
    ${PROJECT_SOURCE_DIR}/src/async_udp.cc
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    
)
