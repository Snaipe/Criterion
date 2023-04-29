#include <stddef.h>
#include <string.h>

#include "criterion/criterion.h"
#include "string/fmt.h"

#define ARRAY_ELEMENTS(arr) (sizeof (arr) / sizeof ((arr)[0]))

typedef struct test_escape_params {
    const char *input;
    const cri_escape_char *escape;
    size_t escape_size;
    const char *expected_output;
} test_escape_params;

void assert_escaped(const char *input, const cri_escape_char *escape,
        size_t escape_size, const char *expected_output)
{
    char *escaped_str = cri_escape_str(input, strlen(input), escape, escape_size);
    cr_assert_not_null(escaped_str);
    cr_assert_str_eq(escaped_str, expected_output);
    free(escaped_str);
}

Test(escape, escape_astr)
{
    cri_escape_char escape[] = {
        { '"', "\\\"" },
        { '\\', "\\\\" },
    };
    cri_escape_char escape2[] = {
        { '<', "&lt;" },
        { '&', "&amp;" },
        { '"', "&quot;" },
    };

    assert_escaped("", escape, ARRAY_ELEMENTS(escape), "");
    assert_escaped("nothing", escape, ARRAY_ELEMENTS(escape), "nothing");
    assert_escaped("\"abc\"", escape, ARRAY_ELEMENTS(escape), "\\\"abc\\\"");
    assert_escaped("Lorem ipsum dolor sit amet\\, consectetur adipiscing elit."
                   "Phasellus feugiat \"metus\" nec velit dictum finibus.",
                   escape, ARRAY_ELEMENTS(escape),
                   "Lorem ipsum dolor sit amet\\\\, consectetur adipiscing elit."
                   "Phasellus feugiat \\\"metus\\\" nec velit dictum finibus.");

    assert_escaped("<<&&&&<\"", escape2, ARRAY_ELEMENTS(escape2), "&lt;&lt;&amp;&amp;&amp;&amp;&lt;&quot;");

    assert_escaped("<\t\r\ncontrol\x01\\", escape2, ARRAY_ELEMENTS(escape2), "&lt;\\t\\r\\ncontrol\\u0001\\\\");
}
