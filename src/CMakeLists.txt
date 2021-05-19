include_directories(SYSTEM "${SFML_INCLUDE_DIR}")

add_library(mancala STATIC mancala.cc mancala.h)

add_executable(adv-mancala main.cc main.h)
target_link_libraries(adv-mancala PUBLIC mancala sfml-graphics sfml-window sfml-system "$<$<CXX_COMPILER_ID:GNU>:stdc++fs>")
add_custom_command(TARGET adv-mancala POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/src/FreeSans.ttf $<TARGET_FILE_DIR:adv-mancala>/FreeSans.ttf)
add_custom_command(TARGET adv-mancala POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/src/spritemap.png $<TARGET_FILE_DIR:adv-mancala>/spritemap.png)

# Tests
add_executable(test_mancala test_mancala.cc)
target_link_libraries(test_mancala PUBLIC mancala)

add_executable(test_saving test_saving.cc)
target_link_libraries(test_saving PUBLIC mancala)
