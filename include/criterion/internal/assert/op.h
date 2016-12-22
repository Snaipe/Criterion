/*
 * The MIT License (MIT)
 *
 * Copyright © 2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#ifndef CRITERION_INTERNAL_ASSERT_OP_H_
#define CRITERION_INTERNAL_ASSERT_OP_H_

#define CRI_BINOP(Op, Actual, Ref)    Actual Op Ref
#define CRI_BINOP_EQ(Actual, Ref)     CRI_BINOP(==, Actual, Ref)
#define CRI_BINOP_NE(Actual, Ref)     CRI_BINOP(!=, Actual, Ref)
#define CRI_BINOP_LE(Actual, Ref)     CRI_BINOP(<=, Actual, Ref)
#define CRI_BINOP_LT(Actual, Ref)     CRI_BINOP(<, Actual, Ref)
#define CRI_BINOP_GE(Actual, Ref)     CRI_BINOP(>=, Actual, Ref)
#define CRI_BINOP_GT(Actual, Ref)     CRI_BINOP(>, Actual, Ref)

#ifdef __cplusplus

# include "op.hxx"

# define CRI_BINOP_T_EQ(Tag, Actual, Ref)    CRI_BINOP_EQ(Actual, Ref)
# define CRI_BINOP_T_NE(Tag, Actual, Ref)    CRI_BINOP_NE(Actual, Ref)
# define CRI_BINOP_T_LE(Tag, Actual, Ref)    CRI_BINOP_LE(Actual, Ref)
# define CRI_BINOP_T_LT(Tag, Actual, Ref)    CRI_BINOP_LT(Actual, Ref)
# define CRI_BINOP_T_GE(Tag, Actual, Ref)    CRI_BINOP_GE(Actual, Ref)
# define CRI_BINOP_T_GT(Tag, Actual, Ref)    CRI_BINOP_GT(Actual, Ref)

# define CRI_UNOP_ZERO(X)                    ::criterion::internal::operators::zero(X)
# define CRI_UNOP_T_ZERO(Tag, X)             ::criterion::internal::operators::zero(X)

#else /* !__cplusplus */

# define CRI_BINOP_EQ_TAG(Tag, Op, Actual, Ref)    (Op (CRI_USER_TAG_ID(eq, Tag)(&(Actual), &(Ref))))
# define CRI_BINOP_LE_TAG(Tag, Op, Actual, Ref)    (Op (CRI_USER_TAG_ID(le, Tag)(&(Actual), &(Ref))))

# define CRI_BINOP_T_EQ(Tag, Actual, Ref)          (CRI_BINOP_EQ_TAG(Tag, , Actual, Ref))
# define CRI_BINOP_T_NE(Tag, Actual, Ref)          (CRI_BINOP_EQ_TAG(Tag, !, Actual, Ref))
# define CRI_BINOP_T_LE(Tag, Actual, Ref)          (CRI_BINOP_LE_TAG(Tag, , Actual, Ref) || CRI_BINOP_EQ_TAG(Tag, , Actual, Ref))
# define CRI_BINOP_T_LT(Tag, Actual, Ref)          (CRI_BINOP_LE_TAG(Tag, , Actual, Ref))
# define CRI_BINOP_T_GE(Tag, Actual, Ref)          (CRI_BINOP_LE_TAG(Tag, !, Actual, Ref) || CRI_BINOP_EQ_TAG(Tag, , Actual, Ref))
# define CRI_BINOP_T_GT(Tag, Actual, Ref)          (CRI_BINOP_LE_TAG(Tag, !, Actual, Ref))

# define CRI_UNOP_ZERO(X)                          !(X)
# define CRI_UNOP_T_ZERO(Tag, X)                   CRI_USER_TAG_ID(zero, Tag)(&(X))

#endif /* !__cplusplus */

#define CRI_ASSERT_SPECIFIER_OPTAG(Op, Name, Tag, ...)     \
    CRI_IF_DEFINED_NODEFER(CRI_ASSERT_TEST_TAGC_ ## Tag(), \
            CRI_ASSERT_SPECIFIER_OPTAG_SCALAR, ,           \
            CRI_ASSERT_SPECIFIER_OPTAG_ARRAY,              \
            )(Op, Name, Tag, __VA_ARGS__)

#define CRI_ASSERT_OP_VAR_TAGGED(Type, Var, Name, MkNode, Val)          Var
#define CRI_ASSERT_OP_VAR_SINGLE(Type, Var, Name, MkNode, Val)          Var

#define CRI_ASSERT_OP_VAL_TAGGED(Type, Var, Name, MkNode, Val)          Val
#define CRI_ASSERT_OP_VAL_SINGLE(Type, Var, Name, MkNode, Val)          Val

#define CRI_ASSERT_OP_NAME_TAGGED(Type, Var, Name, MkNode, Val)         Name
#define CRI_ASSERT_OP_NAME_SINGLE(Type, Var, Name, MkNode, Val)         Name

#define CRI_ASSERT_OP_SUBSCRIPT_TAGGED(Type, Var, Name, MkNode, Val)    Var[cri_i]
#define CRI_ASSERT_OP_SUBSCRIPT_SINGLE(Type, Var, Name, MkNode, Val)    Var

#define CRI_ASSERT_OP_MKNODE_TAGGED(Type, Var, Name, MkNode, Val)       MkNode
#define CRI_ASSERT_OP_MKNODE_SINGLE(Type, Var, Name, MkNode, Val)       MkNode

#define CRI_ASSERT_OP_TYPE_TAGGED(Type, Var, Name, MkNode, Val)         Type
#define CRI_ASSERT_OP_TYPE_SINGLE(Type, Var, Name, MkNode, Val)         Type

#define CRI_ASSERT_OP_ARRTYPE_TAGGED(Type, Var, Name, MkNode, Val)      Type *
#define CRI_ASSERT_OP_ARRTYPE_SINGLE(Type, Var, Name, MkNode, Val)      Type

#define CRI_ASSERT_OPKIND(Kind, Type, Var, Name, MkNode, Val)           Kind
#define CRI_ASSERT_OPKIND_STRIP(Kind, Type, Var, Name, MkNode, Val)     (Type, Var, Name, MkNode, Val)

#define CRI_ASSERT_OPGET___(What, Kind)                                 What ## Kind
#define CRI_ASSERT_OPGET__(What, Kind)                                  CRI_ASSERT_OPGET___(What, Kind)
#define CRI_ASSERT_OPGET_(What, Kind)                                   CRI_ASSERT_OPGET__(What, Kind)
#define CRI_ASSERT_OPGET(What, Var)                                     CRI_ASSERT_OPGET_(CRI_ASSERT_OP_ ## What ## _, CRI_ASSERT_OPKIND Var) CR_EXPAND(CRI_ASSERT_OPKIND_STRIP Var)

#define CRI_ASSERT_IT_VAR(T, Var)                                        \
    CR_CHECKERROR(CRI_ASSERT_OPGET(T, Var)) CRI_ASSERT_OPGET(VAR, Var) = \
            CRI_VALUE_ESCAPE(decltype (CRI_ASSERT_OPGET(VAR, Var)), CRI_ASSERT_OPGET(VAL, Var));

#define CRI_ASSERT_IT_VAR_AUTO(_, Var)        \
    CRI_AUTOTYPE CRI_ASSERT_OPGET(VAR, Var) = \
            CRI_VALUE_ESCAPE(decltype (CRI_ASSERT_OPGET(VAL, Var)), CRI_ASSERT_OPGET(VAL, Var));

/* MSVC doesn't like obstructions, but it isn't necessary for their preprocessor
 * implementation */
#ifdef _MSC_VER
# define CRI_ASSERT_IT_MKNODE(Tag, Var)        CRI_ASSERT_OPGET(MKNODE, Var)(Tag, CRI_ASSERT_OPGET(VAR, Var));
# define CRI_ASSERT_IT_MKNODE_ARR(Tag, Var)    CRI_ASSERT_OPGET(MKNODE, Var)(Tag, CRI_ASSERT_OPGET(VAR, Var)[cri_i]);
#else
# define CRI_ASSERT_IT_MKNODE(Tag, Var)        CRI_OBSTRUCT_N(CRI_ASSERT_OPGET(MKNODE, Var))(Tag, CRI_ASSERT_OPGET(VAR, Var));
# define CRI_ASSERT_IT_MKNODE_ARR(Tag, Var)    CRI_OBSTRUCT_N(CRI_ASSERT_OPGET(MKNODE, Var))(Tag, CRI_ASSERT_OPGET(VAR, Var)[cri_i]);
#endif

#define CRI_ASSERT_IT_MKNODE_AUTO(Tag, Var)      CRI_MKNODE_UNPRINTABLE(, CRI_ASSERT_OPGET(VAR, Var));

#define CRI_ASSERT_IT_UNPACK(_, Var)             , CRI_ASSERT_OPGET(VAR, Var)
#define CRI_ASSERT_IT_VUNPACK(_, Var)            , CRI_ASSERT_OPGET(VAL, Var)
#define CRI_ASSERT_IT_SUNPACK(_, Var)            , CRI_ASSERT_OPGET(SUBSCRIPT, Var)

#define CRI_ASSERT_OP_APPLY(Op, ...)             Op CR_EXPAND((__VA_ARGS__))

#define CRI_ASSERT_MKLIST_1(Macro, L1, L2)       Macro(CR_VA_HEAD L1, CR_VA_HEAD L2)
#define CRI_ASSERT_MKLIST_2(Macro, L1, L2)       Macro(CR_VA_HEAD L1, CR_VA_HEAD L2) CRI_ASSERT_MKLIST_1(Macro, (CR_VA_TAIL L1), (CR_VA_TAIL L2))
#define CRI_ASSERT_MKLIST_3(Macro, L1, L2)       Macro(CR_VA_HEAD L1, CR_VA_HEAD L2) CRI_ASSERT_MKLIST_2(Macro, (CR_VA_TAIL L1), (CR_VA_TAIL L2))
#define CRI_ASSERT_MKLIST_4(Macro, L1, L2)       Macro(CR_VA_HEAD L1, CR_VA_HEAD L2) CRI_ASSERT_MKLIST_3(Macro, (CR_VA_TAIL L1), (CR_VA_TAIL L2))

#define CRI_ASSERT_MKLIST__(Macro, n, L1, L2)    CRI_ASSERT_MKLIST_ ## n(Macro, L1, L2)
#define CRI_ASSERT_MKLIST_(Macro, n, L1, L2)     CRI_ASSERT_MKLIST__(Macro, n, L1, L2)
#define CRI_ASSERT_MKLIST(Macro, L1, L2)         CRI_ASSERT_MKLIST_(Macro, CRITERION_ARG_LENGTH L1, L1, L2)

#define CRI_MKNODE_STR(Tag, Var)                                \
    do {                                                        \
        char *cri_str = CRI_USER_TOSTR(Tag, Var);               \
        cri_tmpn.params[cri_paramidx].type = CRI_ASSERT_RT_STR; \
        cri_tmpn.params[cri_paramidx].data = cri_str;           \
        cri_paramidx++;                                         \
    } while (0)

#define CRI_MKNODE_UNPRINTABLE(_, Var)                          \
    do {                                                        \
        char *cri_str = CRI_ASSERT_UNPRINTABLE(Var);            \
        cri_tmpn.params[cri_paramidx].type = CRI_ASSERT_RT_STR; \
        cri_tmpn.params[cri_paramidx].data = cri_str;           \
        cri_paramidx++;                                         \
    } while (0)

#ifdef CRI_CAPS_AUTOTYPE
# define CRI_ASSERT_SPECIFIER_OPTAGLESS(Op, Name, ...)                                                           \
    1; do {                                                                                                      \
        CRI_ASSERT_NAMESPACES;                                                                                   \
        CRITERION_APPLY(CRI_ASSERT_IT_VAR_AUTO, , __VA_ARGS__)                                                   \
        cri_cond_un = CRI_ASSERT_OP_APPLY CR_EXPAND((Op CRITERION_APPLY(CRI_ASSERT_IT_UNPACK, , __VA_ARGS__)));  \
        cri_assert_node_init(&cri_tmpn);                                                                         \
        cri_tmpn.repr = #Name "(" CR_STR CR_EXPAND((CRITERION_APPLY(CRI_ASSERT_IT_VUNPACK, , __VA_ARGS__))) ")"; \
        size_t cri_paramidx = 0;                                                                                 \
        CRITERION_APPLY(CRI_ASSERT_IT_MKNODE_AUTO, , __VA_ARGS__)                                                \
        cri_tmpn.pass = cri_cond_un;                                                                             \
        cri_assert_node_add(cri_node, &cri_tmpn);                                                                \
    } while (0)
#else
# define CRI_ASSERT_SPECIFIER_OPTAGLESS(Op, Name, ...) \
    1; CR_COMPILE_ERROR(Name without a tag parameter is unsupported on this compiler.)
#endif

#define CRI_ASSERT_SPECIFIER_OPTAG_SCALAR(Op, Name, Tag, ...)                                                         \
    1; do {                                                                                                           \
        CRI_ASSERT_NAMESPACES;                                                                                        \
        CRITERION_APPLY(CRI_ASSERT_IT_VAR, TYPE, __VA_ARGS__)                                                         \
        cri_cond_un = CRI_ASSERT_OP_APPLY(Op, Tag CRITERION_APPLY(CRI_ASSERT_IT_UNPACK, , __VA_ARGS__));              \
        cri_assert_node_init(&cri_tmpn);                                                                              \
        cri_tmpn.repr = #Name "(" #Tag CR_STR CR_EXPAND((CRITERION_APPLY(CRI_ASSERT_IT_VUNPACK, , __VA_ARGS__))) ")"; \
        size_t cri_paramidx = 0;                                                                                      \
        CRITERION_APPLY(CRI_ASSERT_IT_MKNODE, Tag, __VA_ARGS__)                                                       \
        cri_tmpn.pass = cri_cond_un;                                                                                  \
        cri_assert_node_add(cri_node, &cri_tmpn);                                                                     \
    } while (0)

#define CRI_ASSERT_SPECIFIER_OPTAG_ARRAY(Op, Name, Tag, ...)                                                                 \
    1; do {                                                                                                                  \
        CRI_ASSERT_NAMESPACES;                                                                                               \
        CRITERION_APPLY(CRI_ASSERT_IT_VAR, ARRTYPE, __VA_ARGS__)                                                             \
        size_t cri_size = CRI_ASSERT_TYPE_TAG_ARRLEN(Tag);                                                                   \
        cri_assert_node_init(&cri_tmpn);                                                                                     \
        const char *cri_repr = #Name "(" #Tag CR_STR CR_EXPAND((CRITERION_APPLY(CRI_ASSERT_IT_VUNPACK, , __VA_ARGS__))) ")"; \
        cri_tmpn.repr = cri_repr;                                                                                            \
        cri_tmpn.pass = 1;                                                                                                   \
        struct cri_assert_node *cri_tmp = cri_assert_node_add(cri_node, &cri_tmpn);                                          \
        struct cri_assert_node *cri_node = cri_tmp;                                                                          \
        for (size_t cri_i = 0; cri_node->pass && cri_i < cri_size; ++cri_i) {                                                \
            cri_assert_node_init(&cri_tmpn);                                                                                 \
            cr_asprintf((char **) &cri_tmpn.repr, "%s [" CR_STR(CRI_SIZE_T_FMT) "]", cri_repr, cri_i);                       \
            size_t cri_paramidx = 0;                                                                                         \
            CRITERION_APPLY(CRI_ASSERT_IT_MKNODE_ARR, Tag, __VA_ARGS__)                                                      \
            cri_tmpn.pass = CRI_ASSERT_OP_APPLY(Op, Tag CRITERION_APPLY(CRI_ASSERT_IT_SUNPACK, , __VA_ARGS__));              \
            cri_assert_node_add(cri_node, &cri_tmpn);                                                                        \
            cri_node->pass = cri_node->pass && cri_tmpn.pass;                                                                \
        }                                                                                                                    \
        cri_cond_un = cri_node->pass;                                                                                        \
    } while (0)

#define CRI_ASSERT_SPECIFIER_OP_HELPER(Op, N, ...)    CR_DEFER(CR_CONCAT)(CRI_ASSERT_SPECIFIER_ ## Op, N)(__VA_ARGS__)

#define CRI_ASSERT_SPEC_OPLEN_1(...)           \
    CR_EXPAND(CR_VA_TAIL_SELECT64(__VA_ARGS__, \
            1, 1, 1, 1,                        \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0))

#define CRI_ASSERT_SPEC_OPLEN_2(...)           \
    CR_EXPAND(CR_VA_TAIL_SELECT64(__VA_ARGS__, \
            1, 1, 1, 1,                        \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 0, 0, 0))

#define CRI_ASSERT_SPEC_OPLEN_3(...)           \
    CR_EXPAND(CR_VA_TAIL_SELECT64(__VA_ARGS__, \
            1, 1, 1, 1,                        \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      \
            1, 1, 1, 1, 1, 1, 0, 0, 0, 0))

#define CRI_ASSERT_SPEC_OPLEN__(N, Va) \
    CR_EXPAND(CRI_ASSERT_SPEC_OPLEN_ ## N Va)
#define CRI_ASSERT_SPEC_OPLEN_(N, Va) \
    CRI_ASSERT_SPEC_OPLEN__(N, Va)
#define CRI_ASSERT_SPEC_OPLEN(Params, Va) \
    CRI_ASSERT_SPEC_OPLEN_(CRITERION_ARG_LENGTH Params, Va)

#define CRI_ASSERT_IT_MERGEVALS(Def, Val)    , CR_EXPAND((CR_EAT Def, Val))

#define CRI_ASSERT_SPECIFIER_OP0(OpTag, OpTagless, OpName, Params, Va) \
    CRI_ASSERT_SPECIFIER_OPTAGLESS CR_EXPAND((OpTagless, OpName        \
            CRI_ASSERT_MKLIST(CRI_ASSERT_IT_MERGEVALS, Params, Va)     \
            ))
#define CRI_ASSERT_SPECIFIER_OP1(OpTag, OpTagless, OpName, Params, Va)                     \
    CRI_ASSERT_SPECIFIER_OPTAG CR_EXPAND((OpTag, OpName, CR_EXPAND(CR_VA_HEAD Va)          \
            CRI_ASSERT_MKLIST(CRI_ASSERT_IT_MERGEVALS, Params, CR_EXPAND((CR_VA_TAIL Va))) \
            ))

#define CRI_ASSERT_SPECIFIER_OPN__(Macro, Len)    Macro ## Len
#define CRI_ASSERT_SPECIFIER_OPN_(Len)            CRI_ASSERT_SPECIFIER_OPN__(CRI_ASSERT_SPECIFIER_OP, Len)
#define CRI_ASSERT_SPECIFIER_OPN(Params, Va) \
    CRI_ASSERT_SPECIFIER_OPN_(CRI_ASSERT_SPEC_OPLEN(Params, Va))

#define CRI_ASSERT_SPECIFIER_OP(OpTag, OpTagless, OpName, Params, ...) \
    CRI_ASSERT_SPECIFIER_OPN(Params, (__VA_ARGS__))(OpTag, OpTagless, OpName, Params, (__VA_ARGS__))

#define CRI_AS_GETTYPE(...)    CRI_ASSERT_TYPE_TAG(CR_VA_HEAD(__VA_ARGS__))

#endif /* !CRITERION_INTERNAL_ASSERT_OP_H_ */
