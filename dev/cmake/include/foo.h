#ifndef FOO_H_
#define FOO_H_

#include <stddef.h>

size_t foo_strlen(const char *src);
char *foo_strcpy(char *restrict dst, const char *restrict src);
char *foo_strdup(const char *src);

#endif /* !FOO_H_ */
