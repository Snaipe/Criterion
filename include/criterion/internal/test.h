/*
 * The MIT License (MIT)
 *
 * Copyright © 2015 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CRITERION_INTERNAL_TEST_H_
# define CRITERION_INTERNAL_TEST_H_

# include "designated-initializer-compat.h"
# include "common.h"

# ifdef __OBJC__
#import <Foundation/Foundation.h>
# endif

# ifdef __cplusplus
#  include <exception>
# endif

# define CR_IDENTIFIER_(Category, Name, Suffix) \
    Category ## _ ## Name ## _ ## Suffix

# ifdef __cplusplus
#  ifdef __OBJC__
#   define CR_LANG CR_LANG_OBJCXX
#  else
#   define CR_LANG CR_LANG_CXX
#  endif
# else
#  ifdef __OBJC__
#   define CR_LANG CR_LANG_OBJC
#  else
#   define CR_LANG CR_LANG_C
#  endif
# endif

# ifdef __cplusplus
#  define CR_TEST_PROTOTYPE_(Category, Name) \
    extern "C" void CR_IDENTIFIER_(Category, Name, impl)(void)
# else
#  define CR_TEST_PROTOTYPE_(Category, Name) \
    void CR_IDENTIFIER_(Category, Name, impl)(void)
# endif

# define CR_SUITE_IDENTIFIER_(Name, Suffix) \
    suite_ ## Name ## _ ## Suffix

CR_BEGIN_C_API

CR_API void criterion_internal_test_setup(void);
CR_API void criterion_internal_test_main(void (*fn)(void));
CR_API void criterion_internal_test_teardown(void);

CR_END_C_API

static const char *const cr_msg_test_init_std_exception = "Caught an unexpected exception during the test initialization: %s.";
static const char *const cr_msg_test_init_other_exception = "Caught some unexpected exception during the test initialization.";
static const char *const cr_msg_test_main_std_exception = "Caught an unexpected exception during the test execution: %s.";
static const char *const cr_msg_test_main_other_exception = "Caught some unexpected exception during the test execution.";
static const char *const cr_msg_test_fini_std_exception = "Caught an unexpected exception during the test finalization: %s.";
static const char *const cr_msg_test_fini_other_exception = "Caught some unexpected exception during the test finalization.";

# ifdef __cplusplus
# define CR_TEST_TRAMPOLINE_(Category, Name)                                                        \
    static inline void CR_IDENTIFIER_(Category, Name, jmp)(void) {                                  \
        try {                                                                                       \
            criterion_internal_test_setup();                                                        \
        } catch (const std::exception &e) {                                                         \
            criterion_test_die(cr_msg_test_init_std_exception, e.what());                           \
        } catch (...) {                                                                             \
            criterion_test_die(cr_msg_test_init_other_exception);                                   \
        }                                                                                           \
        try {                                                                                       \
            criterion_internal_test_main((void(*)(void)) CR_IDENTIFIER_(Category, Name, impl));     \
        } catch (const std::exception &e) {                                                         \
            criterion_test_die(cr_msg_test_main_std_exception, e.what());                           \
        } catch (...) {                                                                             \
            criterion_test_die(cr_msg_test_main_other_exception);                                   \
        }                                                                                           \
        try {                                                                                       \
            criterion_internal_test_teardown();                                                     \
        } catch (const std::exception &e) {                                                         \
            criterion_test_die(cr_msg_test_fini_std_exception, e.what());                           \
        } catch (...) {                                                                             \
            criterion_test_die(cr_msg_test_fini_other_exception);                                   \
        }                                                                                           \
    }
# else
#  if defined(__OBJC__) && defined(__EXCEPTIONS)
#   define CR_TEST_TRAMPOLINE_(Category, Name)                                                      \
    static inline void CR_IDENTIFIER_(Category, Name, jmp)(void) {                                  \
        @try {                                                                                      \
            criterion_internal_test_setup();                                                        \
        } @catch (NSException *e) {                                                                 \
            NSString *reason = [e reason];                                                          \
            criterion_test_die(cr_msg_test_init_std_exception, [reason UTF8String]);                \
        } @catch (...) {                                                                            \
            criterion_test_die(cr_msg_test_init_other_exception);                                   \
        }                                                                                           \
        @try {                                                                                      \
            criterion_internal_test_main((void(*)(void)) CR_IDENTIFIER_(Category, Name, impl));     \
        } @catch (NSException *e) {                                                                 \
            NSString *reason = [e reason];                                                          \
            criterion_test_die(cr_msg_test_main_std_exception, [reason UTF8String]);                \
        } @catch (...) {                                                                            \
            criterion_test_die(cr_msg_test_main_other_exception);                                   \
        }                                                                                           \
        @try {                                                                                      \
            criterion_internal_test_teardown();                                                     \
        } @catch (NSException *e) {                                                                 \
            NSString *reason = [e reason];                                                          \
            criterion_test_die(cr_msg_test_fini_std_exception, [reason UTF8String]);                \
        } @catch (...) {                                                                            \
            criterion_test_die(cr_msg_test_fini_other_exception);                                   \
        }                                                                                           \
    }
#  else
#   define CR_TEST_TRAMPOLINE_(Category, Name)                                                      \
    static inline void CR_IDENTIFIER_(Category, Name, jmp)(void) {                                  \
        criterion_internal_test_setup();                                                            \
        criterion_internal_test_main((void(*)(void)) CR_IDENTIFIER_(Category, Name, impl));         \
        criterion_internal_test_teardown();                                                         \
    }
#  endif
# endif

# define CR_TEST_BASE(Category, Name, ...)                                     \
    CR_TEST_PROTOTYPE_(Category, Name);                                        \
    CR_TEST_TRAMPOLINE_(Category, Name)                                        \
    struct criterion_test_extra_data CR_IDENTIFIER_(Category, Name, extra) =   \
        CR_EXPAND(CRITERION_MAKE_STRUCT(criterion_test_extra_data,             \
            .lang_ = CR_LANG,                                                  \
            .kind_ = CR_TEST_NORMAL,                                           \
            .param_ = (struct criterion_test_params(*)(void)) NULL,            \
            .identifier_ = #Category "/" #Name,                                \
            .file_    = __FILE__,                                              \
            .line_    = __LINE__,                                              \
            __VA_ARGS__                                                        \
        ));                                                                    \
    struct criterion_test CR_IDENTIFIER_(Category, Name, meta) = {             \
        #Name,                                                                 \
        #Category,                                                             \
        CR_IDENTIFIER_(Category, Name, jmp),                                   \
        &CR_IDENTIFIER_(Category, Name, extra)                                 \
    };                                                                         \
    CR_SECTION_("cr_tst")                                                      \
    struct criterion_test *CR_IDENTIFIER_(Category, Name, ptr)                 \
            = &CR_IDENTIFIER_(Category, Name, meta) CR_SECTION_SUFFIX_;        \
    CR_TEST_PROTOTYPE_(Category, Name)

# define CR_SUITE_BASE(Name, ...)                                              \
    struct criterion_test_extra_data CR_SUITE_IDENTIFIER_(Name, extra) =       \
        CR_EXPAND(CRITERION_MAKE_STRUCT(criterion_test_extra_data,             \
            .file_    = __FILE__,                                              \
            .line_    = 0,                                                     \
            __VA_ARGS__                                                        \
        ));                                                                    \
    struct criterion_suite CR_SUITE_IDENTIFIER_(Name, meta) = {                \
        #Name,                                                                 \
        &CR_SUITE_IDENTIFIER_(Name, extra),                                    \
    };                                                                         \
    CR_SECTION_("cr_sts")                                                      \
    struct criterion_suite *CR_SUITE_IDENTIFIER_(Name, ptr)                    \
	    = &CR_SUITE_IDENTIFIER_(Name, meta) CR_SECTION_SUFFIX_


#endif /* !CRITERION_INTERNAL_TEST_H_ */
