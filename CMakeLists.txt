cmake_minimum_required(VERSION 3.15)

# --- VCPKG BOOTSTRAP LOGIC ---
set(VCPKG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg")

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

if(NOT EXISTS "${VCPKG_ROOT}/README.md")
    message(STATUS "Initializing vcpkg submodule...")
    execute_process(COMMAND git submodule update --init --recursive
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
endif()

if(WIN32)
    set(VCPKG_EXEC "${VCPKG_ROOT}/vcpkg.exe")
    set(BOOTSTRAP_SCRIPT "${VCPKG_ROOT}/bootstrap-vcpkg.bat")
else()
    set(VCPKG_EXEC "${VCPKG_ROOT}/vcpkg")
    set(BOOTSTRAP_SCRIPT "${VCPKG_ROOT}/bootstrap-vcpkg.sh")
endif()

if(NOT EXISTS "${VCPKG_EXEC}")
    message(STATUS "Bootstrapping vcpkg...")
    execute_process(COMMAND "${BOOTSTRAP_SCRIPT}" -disableMetrics
                    WORKING_DIRECTORY "${VCPKG_ROOT}"
                    RESULT_VARIABLE BOOTSTRAP_RESULT)
    if(NOT BOOTSTRAP_RESULT EQUAL "0")
        message(FATAL_ERROR "Bootstrapping vcpkg failed")
    endif()
endif()

# --- PROJECT SETUP ---
project(InvTrendAnalysis)

# --- FORCE STATIC RUNTIME ---
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- ENVIRONMENT CHECKS ---
if(NOT DEFINED ENV{ipapiAPIKey})
    message(FATAL_ERROR "ipapiAPIKey environment variable not set. See the Readme.")
endif()

if(NOT DEFINED ENV{OpenWeatherAPIKey})
    message(FATAL_ERROR "OpenWeatherAPIKey environment variable not set. See the Readme.")
endif()

# --- DEPENDENCIES (VCPKG) ---
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)
find_package(libmysql CONFIG REQUIRED)
find_package(wxWidgets REQUIRED COMPONENTS net core base)

# --- INCLUDE DIRECTORIES ---
include_directories(${wxWidgets_INCLUDE_DIRS})

# --- TARGET CONFIGURATION ---
add_executable(${PROJECT_NAME} WIN32 MainFrame.cpp MainPage.cpp)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/images"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/images"
)

# --- COMPILER DEFINITIONS ---
target_compile_definitions(${PROJECT_NAME} PRIVATE
    UNICODE _UNICODE
    $<$<CONFIG:Debug>:__WXDEBUG__>
)

# --- LINKING ---
target_link_libraries(${PROJECT_NAME} PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
    unofficial::mysql-connector-cpp::connector
    unofficial::mysql-connector-cpp::connector-jdbc
    mysqlclient
    ${wxWidgets_LIBRARIES}
)