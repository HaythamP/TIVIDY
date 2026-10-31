cmake_minimum_required(VERSION 3.14)
project(GenericWorkflowSystem CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Disable Crow tests and examples
set(CROW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CROW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# Download Crow
FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG master
)
FetchContent_MakeAvailable(Crow)

# Include project headers
include_directories(src/include)

# Gather all source files except main.cpp
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")
list(REMOVE_ITEM ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Define executable
add_executable(workflow_server ${ENGINE_SOURCES})

# Link Crow
target_link_libraries(workflow_server PRIVATE Crow::Crow "-framework CoreFoundation" "-framework Security")

# Copy the UI on every normal build so HTML-only changes reach the server.
add_custom_target(copy_public ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_BINARY_DIR}/public
)
add_dependencies(workflow_server copy_public)
