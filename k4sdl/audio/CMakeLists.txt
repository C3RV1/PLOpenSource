cmake_minimum_required(VERSION 3.10)

target_sources(PLOpenSource PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Sound.cpp)
target_sources(PLOpenSource PRIVATE ${CMAKE_CURRENT_LIST_DIR}/SoundManager.cpp)
target_sources(PLOpenSource PRIVATE ${CMAKE_CURRENT_LIST_DIR}/SoundLoader.cpp)