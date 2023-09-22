#include "shmo/strings.h"
#include <stdio.h>

int main(i32 argc, char *argv[]) {
    (void)argc; (void)argv;

    Strview views[] = {
            strview_of("Hello,"),
            strview_of("World!"),
            strview_of("This"),
            strview_of("is"),
            strview_of("a"),
            strview_of("test"),
            strview_of("of"),
            strview_of("the"),
            strview_of("strview_join"),
            strview_of("function!"),
    };

    // First calculate the size we'll need.
    usize buffer_size = strview_join(views, array_count(views), strview_of(" "), nullptr, 0);

    // Add one to account for a null-terminator
    buffer_size += 1;

    // Allocate enough memory.
    char *buffer = malloc(buffer_size);

    // Re-call the function with the new buffer to actually preform the operation.
    strview_join(views, array_count(views), strview_of(" "), buffer, buffer_size);

    printf("%s\n", buffer);
    free(buffer);

    return 0;
}
