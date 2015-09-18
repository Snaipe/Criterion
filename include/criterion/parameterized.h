#ifndef CRITERION_PARAMETERIZED_H_
# define CRITERION_PARAMETERIZED_H_

# include "criterion.h"

# ifdef __cplusplus
#  define CR_PARAM_TEST_PROTOTYPE_(Param, Category, Name) \
    extern "C" void IDENTIFIER_(Category, Name, impl)(Param)
# else
#  define CR_PARAM_TEST_PROTOTYPE_(Param, Category, Name) \
    void IDENTIFIER_(Category, Name, impl)(Param)
# endif

# define ParameterizedTest(...) \
    CR_EXPAND(ParameterizedTest_(__VA_ARGS__, .sentinel_ = 0))

# define ParameterizedTest_(Param, Category, Name, ...)                        \
    CR_PARAM_TEST_PROTOTYPE_(Param, Category, Name);                           \
    struct criterion_test_extra_data IDENTIFIER_(Category, Name, extra) =      \
        CR_EXPAND(CRITERION_MAKE_STRUCT(struct criterion_test_extra_data,      \
            .kind_ = CR_TEST_PARAMETERIZED,                                    \
            .param_ = IDENTIFIER_(Category, Name, param),                      \
            .identifier_ = #Category "/" #Name,                                \
            .file_    = __FILE__,                                              \
            .line_    = __LINE__,                                              \
            __VA_ARGS__                                                        \
        ));                                                                    \
    SECTION_("cr_tst")                                                         \
    struct criterion_test IDENTIFIER_(Category, Name, meta) = {                \
        #Name,                                                                 \
        #Category,                                                             \
        (void(*)(void)) IDENTIFIER_(Category, Name, impl),                     \
        &IDENTIFIER_(Category, Name, extra)                                    \
    } SECTION_SUFFIX_;                                                         \
    CR_PARAM_TEST_PROTOTYPE_(Param, Category, Name)

# define ParameterizedTestParameters(Category, Name) \
    static struct criterion_test_params IDENTIFIER_(Category, Name, param)(void)

# ifdef __cplusplus
#  define cr_make_param_array(Type, Array, ...) \
    criterion_test_params(sizeof (Type), (Array), __VA_ARGS__)
# else
#  define cr_make_param_array(Type, Array, ...) \
    (struct criterion_test_params) { sizeof (Type), (void*)(Array), __VA_ARGS__ }
# endif

#endif /* !CRITERION_PARAMETERIZED_H_ */
