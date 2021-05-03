add_library(mancala STATIC mancala.cc mancala.h)

add_executable(adv-mancala main.cc main.h mancala)
target_link_libraries(adv-mancala PUBLIC mancala)

# Tests
add_executable(test_mancala EXCLUDE_FROM_ALL test_mancala.cc)
target_link_libraries(test_mancala PUBLIC mancala)
