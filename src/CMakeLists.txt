add_library(pockets STATIC pockets.cc pockets.h)

add_library(mancala STATIC mancala.cc mancala.h)
target_link_libraries(mancala PRIVATE pockets)

add_executable(adv-mancala main.cc main.h mancala)
target_link_libraries(adv-mancala PUBLIC mancala)

# Tests
add_executable(test_mancala test_mancala.cc)
target_link_libraries(test_mancala PUBLIC mancala)

add_executable(test_saving test_saving.cc)
target_link_libraries(test_saving PUBLIC mancala)
