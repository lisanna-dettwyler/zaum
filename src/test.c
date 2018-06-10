#include <test.h>

#include <stdio.h>

enum {
	test_tab_width = 2,
	test_group_max_depth = 64,
	test_line_width = 80,
};

static U32 test_group_depth = 0;
static struct {
	cstr name;
	bool passed;
} test_groups [test_group_max_depth];

static void test_message_print (
	in ccstr header,
	in ccstr body,
	in ccstr tag
) {
	// Display test group progress message
	int header_length;
	printf("%*s%s: %n",
	       test_group_depth * test_tab_width,
	       "",
	       header,
	       &header_length
	);
	printf("%*s[%s]\n",
	       header_length - test_line_width,
	       body,
	       tag
	);
}

static ccstr test_pass_message () {
	return test_groups[test_group_depth].passed ? "PASSED" : "FAILED";
}

void test_group_start (
	in ccstr group_name
) {
	// Update group data
	assert(test_group_depth < test_group_max_depth);
	test_groups[test_group_depth].name = group_name;
	test_groups[test_group_depth].passed = true;

	// Display progress message
	test_message_print("GROUP", group_name, "START");

	// Prepare to update next group level
	test_group_depth++;
}

void test_group_end () {
	// Test group must be started to end
	assert(test_group_depth > 0);

	// Back down to the lower group level
	test_group_depth--;

	// Display progress message
	test_message_print("GROUP", test_groups[test_group_depth].name, test_pass_message());
}

void test_start (
	in ccstr test_name
) {
	// Tests must be in groups
	assert(test_group_depth > 0);

	// Update test passed marker
	test_groups[test_group_depth].passed = true;
	test_groups[test_group_depth].name = test_name;

	// Display progress message
	test_message_print("TEST", test_name, "START");
}

void test_end () {
	// Get test passed marker
	const bool passed = test_groups[test_group_depth].passed;

	// Update group passed marker
	if(test_group_depth > 0) {
		test_groups[test_group_depth - 1].passed &= passed;
	}

	// Display progress message
	test_message_print("TEST", test_groups[test_group_depth].name, test_pass_message());
}

void test_finish () {
	// Check for completion
	assert(test_group_depth == 0);

	printf("\n\nTesting complete.\n");
}

void test_assert_inner (
	in ccstr form,
	in bool value
) {
	// Asserts must be in tests
	assert(test_group_depth > 0);

	// Update passed marker
	test_groups[test_group_depth].passed &= value;

	// Display progress message
	test_message_print("  ASSERT", form, test_pass_message());
}
