#include "shmo/strings.h"

int main(int argc, char *argv[]) {
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

    char *buffer = strview_join(views, array_count(views), strview_of(" "), stdalloc);
    printf("%s\n", buffer);
    allocator_free(stdalloc, buffer);
    return 0;
}
