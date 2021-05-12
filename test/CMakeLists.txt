add_test(NAME MancalaTest COMMAND "$<TARGET_FILE:test_mancala>")
set_tests_properties(MancalaTest PROPERTIES
	PASS_REGULAR_EXPRESSION "  02\n00  00\n02  00\n00  00\n05  00\n00  01\n01  01\n  00;00\n(06  06\n){6}  00")

add_test(NAME SaveTest COMMAND "$<TARGET_FILE:test_saving>")
