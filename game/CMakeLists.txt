cmake_minimum_required(VERSION 3.10)

add_subdirectory(TitleScreen)
target_sources(PLOpenSource PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Save.cpp)