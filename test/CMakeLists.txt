add_test(NAME MancalaTest COMMAND "$<TARGET_FILE:test_mancala>")
set_tests_properties(MancalaTest PROPERTIES
	PASS_REGULAR_EXPRESSION "  00\n01  13\n02  12\n03  11\n04  10\n05  09\n06  08\n  07")

add_test(NAME SaveTest COMMAND "$<TARGET_FILE:test_saving>")
