project(teto)

cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

pkg_search_module(SDL2 sdl2)
pkg_search_module(SDL2_TTF SDL2_ttf)
pkg_search_module(SDL2_IMAGE SDL2_image)

function(add_cxx_flags flags)
    set(CMAKE_CXX_FLAGS "${flags} ${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
endfunction()

add_cxx_flags("-O2 -Wall -Wextra -std=c++11")

add_library(teto_teto Simulator.cc Printer.cc Onore.cc AI.cc)

add_executable(teto main.cc)

target_link_libraries(teto ${SDL2_LIBRARIES})
target_link_libraries(teto ${SDL2_TTF_LIBRARIES})
target_link_libraries(teto ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(teto gdi32)
target_link_libraries(teto teto_teto)
