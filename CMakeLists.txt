cmake_minimum_required(VERSION 3.3)
# Project Name
project(async_udp)

# Find Packages
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED)

# Check presence of boost
if(Boost_FOUND)
    message("*** Boost Dependancy Found ***")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif(Boost_FOUND)

add_library(
    ${PROJECT_NAME} STATIC
    ${PROJECT_SOURCE_DIR}/src/async_udp.cc
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Client example this will need some runtime argument
add_executable(client ./example/client.cc)
if (WIN32)
# For windows specific applications
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    ${Boost_LIBRARIES}
    ws2_32
)

target_link_libraries(
        client
        PUBLIC
        ${Boost_LIBRARIES}
        ws2_32
    )
else()
# Anyother OS
    target_link_libraries(
        client
        PUBLIC
        ${Boost_LIBRARIES}
    )
endif (WIN32)

# Server example
add_executable(server ./example/server.cc)
target_link_libraries(
    server
    PUBLIC
    ${PROJECT_NAME}
)