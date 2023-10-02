#include "shmo/strings.h"

int main(void) {
    // Create a StringBuilder instance
    StringBuilder *builder = string_builder_create(NULL); // Passing NULL to use the default allocator
    if (!builder) {
        fprintf(stderr, "Failed to create StringBuilder\n");
        return 1;
    }

    // Push a character to StringBuilder
    if (!string_builder_push_char(builder, 'A')) {
        fprintf(stderr, "Failed to push character\n");
    }

    // Push a string to StringBuilder
    if (!string_builder_push_str(builder, "BCD")) {
        fprintf(stderr, "Failed to push string\n");
    }

    // Push a formatted string to StringBuilder
    if (!string_builder_push_fmt(builder, " %d %s", 123, "EFG")) {
        fprintf(stderr, "Failed to push formatted string\n");
    }

    // Print the constructed string
    printf("Constructed String: %s\n", string_builder_cstr(builder));

    // Insert a string at a specific position
    if (!string_builder_insert_str(builder, 4, " Inserted")) {
        fprintf(stderr, "Failed to insert string\n");
    }

    // Print the modified string
    printf("Modified String: %s\n", string_builder_cstr(builder));

    // Get and print a substring view from the StringBuilder
    Strview view = string_builder_get_view(builder, r1u(5, 13));
    if (strview_is_null(view)) {
        fprintf(stderr, "Failed to get string view\n");
    } else {
        printf("Substring View: %.*s\n", (int)view.length, view.data);
    }

    // Destroy the StringBuilder instance
    string_builder_destroy(builder);

    return 0;
}
