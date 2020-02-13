cmake_minimum_required(VERSION 3.10)

project(jetlog_cpp_full)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -g)
endif()

## Jetlog C++ full library
add_library(jetlog_cpp_full INTERFACE)

#target_include_directories(jetlog_cpp_full INTERFACE include)
target_include_directories(jetlog_cpp_full INTERFACE 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS jetlog_cpp_full
    EXPORT jetlog_cpp_fullTargets
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT jetlog_cpp_fullTargets
  FILE jetlog_cpp_fullTargets.cmake
  DESTINATION lib/cmake/jetlog_cpp_full
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/jetlog_cpp_fullConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/example"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )
# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/jetlog_cpp_fullConfigVersion.cmake"
  VERSION "${Tutorial_VERSION_MAJOR}.${Tutorial_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# install the configuration file
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/jetlog_cpp_fullConfig.cmake
  DESTINATION lib/cmake/jetlog_cpp_full
)

## Jetlog C++ full example
add_executable(jetlog_cpp_full_example src/example.cpp)
target_link_libraries(jetlog_cpp_full_example PRIVATE jetlog_cpp_full)
