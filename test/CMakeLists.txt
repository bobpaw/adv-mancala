message("test_mancala test file: $<TARGET_FILE:test_mancala>")

add_test(
	NAME
		MancalaTest
	COMMAND
		"${CMAKE_CTEST_COMMAND}"
			--build-and-test "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
			--build-generator "${CMAKE_GENERATOR}"
			--build-nocmake
			--build-target test_mancala
			--test-command "$<TARGET_FILE:test_mancala>"
)
