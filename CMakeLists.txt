# Copyright (c) UnnamedOrange. Licensed under the MIT License.
# See the LICENSE file in the repository root for full license text.

cmake_minimum_required(VERSION 4.4)

project(visfacto
  VERSION 0.1.0
  LANGUAGES CXX
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*"
)

add_executable(${PROJECT_NAME}
  WIN32
  ${SOURCES}
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    UNICODE
    _UNICODE
)
if(MSVC)
  target_sources(${PROJECT_NAME}
    PRIVATE
      "app.manifest"
  )
  set_target_properties(${PROJECT_NAME}
    PROPERTIES
      MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
  )
  target_compile_options(${PROJECT_NAME}
    PRIVATE
      /W4
      /permissive-
      /utf-8
  )
else()
  message(WARNING "Compilers other than MSVC are not well supported yet")
  target_compile_options(${PROJECT_NAME}
    PRIVATE
      -Wall
      -Wextra
      -pedantic
  )
endif()
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    advapi32
    shell32
    user32
)
