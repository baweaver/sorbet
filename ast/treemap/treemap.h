#ifndef SORBET_TREEMAP_H
#define SORBET_TREEMAP_H

#include "ast/Trees.h"
#include "core/Context.h"
#include "core/GlobalState.h"
#include "core/errors/internal.h"
#include <memory>
#include <type_traits> // To use 'std::integral_constant'.
#include <typeinfo>

using std::make_unique;
using std::unique_ptr;

namespace sorbet {
namespace ast {

class FUNC_EXAMPLE {
public:
    // all members are optional, but METHOD NAMES MATTER
    // Not including the member will skip the branch
    // you may return the same pointer that you are given
    // caller is repsonsible to handle it
    unique_ptr<ClassDef> preTransformClassDef(core::MutableContext ctx, ClassDef *original);
    unique_ptr<Expression> postTransformClassDef(core::MutableContext ctx, unique_ptr<ClassDef> original);

    unique_ptr<MethodDef> preTransformMethodDef(core::MutableContext ctx, unique_ptr<MethodDef> original);
    unique_ptr<Expression> postTransformMethodDef(core::MutableContext ctx, unique_ptr<MethodDef> original);

    unique_ptr<If> preTransformIf(core::MutableContext ctx, unique_ptr<If> original);
    unique_ptr<Expression> postTransformIf(core::MutableContext ctx, unique_ptr<If> original);

    unique_ptr<While> preTransformWhile(core::MutableContext ctx, unique_ptr<While> original);
    unique_ptr<Expression> postTransformWhile(core::MutableContext ctx, unique_ptr<While> original);

    unique_ptr<Expression> postTransformBreak(core::MutableContext ctx, unique_ptr<Break> original);

    unique_ptr<Expression> postTransformRetry(core::MutableContext ctx, unique_ptr<Retry> original);

    unique_ptr<Expression> postTransformNext(core::MutableContext ctx, unique_ptr<Next> original);

    unique_ptr<Return> preTransformReturn(core::MutableContext ctx, unique_ptr<Return> original);
    unique_ptr<Expression> postTransformReturn(core::MutableContext ctx, unique_ptr<Return> original);

    unique_ptr<Yield> preTransformYield(core::MutableContext ctx, unique_ptr<Yield> original);
    unique_ptr<Expression> postTransformYield(core::MutableContext ctx, unique_ptr<Yield> original);

    unique_ptr<RescueCase> preTransformRescueCase(core::MutableContext ctx, unique_ptr<RescueCase> original);
    unique_ptr<Expression> postTransformRescueCase(core::MutableContext ctx, unique_ptr<RescueCase> original);

    unique_ptr<Rescue> preTransformRescue(core::MutableContext ctx, unique_ptr<Rescue> original);
    unique_ptr<Expression> postTransformRescue(core::MutableContext ctx, unique_ptr<Rescue> original);

    unique_ptr<Expression> postTransformField(core::MutableContext ctx, unique_ptr<Field> original);
    unique_ptr<Expression> postTransformUnresolvedIdent(core::MutableContext ctx, unique_ptr<UnresolvedIdent> original);

    unique_ptr<Assign> preTransformAssign(core::MutableContext ctx, unique_ptr<Assign> original);
    unique_ptr<Expression> postTransformAssign(core::MutableContext ctx, unique_ptr<Assign> original);

    unique_ptr<Send> preTransformSend(core::MutableContext ctx, unique_ptr<Send> original);
    unique_ptr<Expression> postTransformSend(core::MutableContext ctx, unique_ptr<Send> original);

    unique_ptr<Hash> preTransformHash(core::MutableContext ctx, unique_ptr<Hash> original);
    unique_ptr<Expression> postTransformHash(core::MutableContext ctx, unique_ptr<Hash> original);

    unique_ptr<Array> preTransformArray(core::MutableContext ctx, unique_ptr<Array> original);
    unique_ptr<Expression> postransformArray(core::MutableContext ctx, unique_ptr<Array> original);

    unique_ptr<Expression> postTransformUnresolvedConstantLit(core::MutableContext ctx,
                                                              unique_ptr<UnresolvedConstantLit> original);

    unique_ptr<Expression> postTransformSelf(core::MutableContext ctx, unique_ptr<Self> original);

    unique_ptr<Block> preTransformBlock(core::MutableContext ctx, unique_ptr<Block> original);
    unique_ptr<Expression> postTransformBlock(core::MutableContext ctx, unique_ptr<Block> original);

    unique_ptr<InsSeq> preTransformInsSeq(core::MutableContext ctx, unique_ptr<InsSeq> original);
    unique_ptr<Expression> postTransformInsSeq(core::MutableContext ctx, unique_ptr<InsSeq> original);
};

/**
 * GENERATE_HAS_MEMBER(att)  // Creates 'has_member_att'.
 * `HAS_MEMBER_att<C>::value` can be used to statically test if C has a member att
 */
#define GENERATE_HAS_MEMBER(X)                                                          \
    template <typename T> class HAS_MEMBER_##X {                                        \
        struct Fallback {                                                               \
            int X;                                                                      \
        };                                                                              \
        struct Derived : T, Fallback {};                                                \
                                                                                        \
        template <typename U, U> struct Check;                                          \
                                                                                        \
        typedef char ArrayOfOne[1];                                                     \
        typedef char ArrayOfTwo[2];                                                     \
                                                                                        \
        template <typename U> static ArrayOfOne &func(Check<int Fallback::*, &U::X> *); \
        template <typename U> static ArrayOfTwo &func(...);                             \
                                                                                        \
    public:                                                                             \
        typedef HAS_MEMBER_##X type;                                                    \
        enum { value = sizeof(func<Derived>(0)) == 2 };                                 \
    };

GENERATE_HAS_MEMBER(preTransformExpression);
GENERATE_HAS_MEMBER(preTransformClassDef);
GENERATE_HAS_MEMBER(preTransformMethodDef);
GENERATE_HAS_MEMBER(preTransformIf);
GENERATE_HAS_MEMBER(preTransformWhile);
GENERATE_HAS_MEMBER(preTransformBreak);
GENERATE_HAS_MEMBER(preTransformRetry);
GENERATE_HAS_MEMBER(preTransformNext);
GENERATE_HAS_MEMBER(preTransformReturn);
GENERATE_HAS_MEMBER(preTransformYield);
GENERATE_HAS_MEMBER(preTransformRescueCase);
GENERATE_HAS_MEMBER(preTransformRescue);
GENERATE_HAS_MEMBER(preTransformAssign);
GENERATE_HAS_MEMBER(preTransformSend);
GENERATE_HAS_MEMBER(preTransformHash);
GENERATE_HAS_MEMBER(preTransformArray);
GENERATE_HAS_MEMBER(preTransformBlock);
GENERATE_HAS_MEMBER(preTransformInsSeq);

// used to check for ABSENCE of method
GENERATE_HAS_MEMBER(preTransformField);
GENERATE_HAS_MEMBER(preTransformUnresolvedIdent);
GENERATE_HAS_MEMBER(preTransformLocal);
GENERATE_HAS_MEMBER(preTransformUnresolvedConstantLit);
GENERATE_HAS_MEMBER(preTransformConstantLit);
GENERATE_HAS_MEMBER(preTransformLiteral);
GENERATE_HAS_MEMBER(preTransformSelf);
GENERATE_HAS_MEMBER(preTransformCast);

GENERATE_HAS_MEMBER(postTransformClassDef);
GENERATE_HAS_MEMBER(postTransformMethodDef);
GENERATE_HAS_MEMBER(postTransformIf);
GENERATE_HAS_MEMBER(postTransformWhile);
GENERATE_HAS_MEMBER(postTransformBreak);
GENERATE_HAS_MEMBER(postTransformRetry);
GENERATE_HAS_MEMBER(postTransformNext);
GENERATE_HAS_MEMBER(postTransformReturn);
GENERATE_HAS_MEMBER(postTransformYield);
GENERATE_HAS_MEMBER(postTransformRescueCase);
GENERATE_HAS_MEMBER(postTransformRescue);
GENERATE_HAS_MEMBER(postTransformField);
GENERATE_HAS_MEMBER(postTransformUnresolvedIdent);
GENERATE_HAS_MEMBER(postTransformAssign);
GENERATE_HAS_MEMBER(postTransformSend);
GENERATE_HAS_MEMBER(postTransformHash);
GENERATE_HAS_MEMBER(postTransformLocal);
GENERATE_HAS_MEMBER(postTransformArray);
GENERATE_HAS_MEMBER(postTransformLiteral);
GENERATE_HAS_MEMBER(postTransformUnresolvedConstantLit);
GENERATE_HAS_MEMBER(postTransformConstantLit);
GENERATE_HAS_MEMBER(postTransformArraySplat);
GENERATE_HAS_MEMBER(postTransformHashSplat);
GENERATE_HAS_MEMBER(postTransformSelf);
GENERATE_HAS_MEMBER(postTransformBlock);
GENERATE_HAS_MEMBER(postTransformInsSeq);
GENERATE_HAS_MEMBER(postTransformCast);

#define GENERATE_POSTPONE_PRECLASS(X)                                                   \
                                                                                        \
    template <class FUNC, class CTX, bool has> class PostPonePreTransform_##X {         \
    public:                                                                             \
        static unique_ptr<X> call(CTX ctx, unique_ptr<X> cd, FUNC &what) {              \
            Error::raise("should never be called. Incorrect use of TreeMap?");          \
            return nullptr;                                                             \
        }                                                                               \
    };                                                                                  \
                                                                                        \
    template <class FUNC, class CTX> class PostPonePreTransform_##X<FUNC, CTX, true> {  \
    public:                                                                             \
        static unique_ptr<X> call(CTX ctx, unique_ptr<X> cd, FUNC &func) {              \
            return func.preTransform##X(ctx, move(cd));                                 \
        }                                                                               \
    };                                                                                  \
                                                                                        \
    template <class FUNC, class CTX> class PostPonePreTransform_##X<FUNC, CTX, false> { \
    public:                                                                             \
        static unique_ptr<X> call(CTX ctx, unique_ptr<X> cd, FUNC &func) {              \
            return cd;                                                                  \
        }                                                                               \
    };

#define GENERATE_POSTPONE_POSTCLASS(X)                                                   \
                                                                                         \
    template <class FUNC, class CTX, bool has> class PostPonePostTransform_##X {         \
    public:                                                                              \
        static unique_ptr<Expression> call(CTX ctx, unique_ptr<X> cd, FUNC &what) {      \
            Error::raise("should never be called. Incorrect use of TreeMap?");           \
            return nullptr;                                                              \
        }                                                                                \
    };                                                                                   \
                                                                                         \
    template <class FUNC, class CTX> class PostPonePostTransform_##X<FUNC, CTX, true> {  \
    public:                                                                              \
        static unique_ptr<Expression> call(CTX ctx, unique_ptr<X> cd, FUNC &func) {      \
            return func.postTransform##X(ctx, move(cd));                                 \
        }                                                                                \
    };                                                                                   \
                                                                                         \
    template <class FUNC, class CTX> class PostPonePostTransform_##X<FUNC, CTX, false> { \
    public:                                                                              \
        static unique_ptr<Expression> call(CTX ctx, unique_ptr<X> cd, FUNC &func) {      \
            return cd;                                                                   \
        }                                                                                \
    };

GENERATE_POSTPONE_PRECLASS(Expression);
GENERATE_POSTPONE_PRECLASS(ClassDef);
GENERATE_POSTPONE_PRECLASS(MethodDef);
GENERATE_POSTPONE_PRECLASS(If);
GENERATE_POSTPONE_PRECLASS(While);
GENERATE_POSTPONE_PRECLASS(Break);
GENERATE_POSTPONE_PRECLASS(Retry);
GENERATE_POSTPONE_PRECLASS(Next);
GENERATE_POSTPONE_PRECLASS(Return);
GENERATE_POSTPONE_PRECLASS(Yield);
GENERATE_POSTPONE_PRECLASS(RescueCase);
GENERATE_POSTPONE_PRECLASS(Rescue);
GENERATE_POSTPONE_PRECLASS(Assign);
GENERATE_POSTPONE_PRECLASS(Send);
GENERATE_POSTPONE_PRECLASS(Hash);
GENERATE_POSTPONE_PRECLASS(Array);
GENERATE_POSTPONE_PRECLASS(Block);
GENERATE_POSTPONE_PRECLASS(InsSeq);
GENERATE_POSTPONE_PRECLASS(Cast);

GENERATE_POSTPONE_POSTCLASS(ClassDef);
GENERATE_POSTPONE_POSTCLASS(MethodDef);
GENERATE_POSTPONE_POSTCLASS(If);
GENERATE_POSTPONE_POSTCLASS(While);
GENERATE_POSTPONE_POSTCLASS(Break);
GENERATE_POSTPONE_POSTCLASS(Retry);
GENERATE_POSTPONE_POSTCLASS(Next);
GENERATE_POSTPONE_POSTCLASS(Return);
GENERATE_POSTPONE_POSTCLASS(Yield);
GENERATE_POSTPONE_POSTCLASS(RescueCase);
GENERATE_POSTPONE_POSTCLASS(Rescue);
GENERATE_POSTPONE_POSTCLASS(Field);
GENERATE_POSTPONE_POSTCLASS(UnresolvedIdent);
GENERATE_POSTPONE_POSTCLASS(Assign);
GENERATE_POSTPONE_POSTCLASS(Send);
GENERATE_POSTPONE_POSTCLASS(Hash);
GENERATE_POSTPONE_POSTCLASS(Array);
GENERATE_POSTPONE_POSTCLASS(Local);
GENERATE_POSTPONE_POSTCLASS(Literal);
GENERATE_POSTPONE_POSTCLASS(UnresolvedConstantLit);
GENERATE_POSTPONE_POSTCLASS(ConstantLit);
GENERATE_POSTPONE_POSTCLASS(Self);
GENERATE_POSTPONE_POSTCLASS(Block);
GENERATE_POSTPONE_POSTCLASS(InsSeq);
GENERATE_POSTPONE_POSTCLASS(Cast);

/**
 * Given a tree transformer FUNC transform a tree.
 * Tree is guaranteed to be visited in the definition order.
 * FUNC may maintain internal state.
 * @tparam tree transformer, see FUNC_EXAMPLE
 */
template <class FUNC, class CTX> class TreeMapper {
private:
    friend class TreeMap;

    FUNC &func;
    bool locReported = false;

    static_assert(!HAS_MEMBER_preTransformField<FUNC>::value, "use post*Transform instead");
    static_assert(!HAS_MEMBER_preTransformUnresolvedIdent<FUNC>::value, "use post*Transform instead");
    static_assert(!HAS_MEMBER_preTransformLiteral<FUNC>::value, "use post*Transform instead");
    static_assert(!HAS_MEMBER_preTransformUnresolvedConstantLit<FUNC>::value, "use post*Transform instead");
    static_assert(!HAS_MEMBER_preTransformConstantLit<FUNC>::value, "use post*Transform instead");
    static_assert(!HAS_MEMBER_preTransformSelf<FUNC>::value, "use post*Transform instead");
    static_assert(!HAS_MEMBER_preTransformLocal<FUNC>::value, "use post*Transform instead");

    TreeMapper(FUNC &func) : func(func) {}

    unique_ptr<Expression> mapIt(unique_ptr<Expression> what, CTX ctx) {
        if (what == nullptr) {
            return what;
        }
        auto loc = what->loc;

        try {
            // TODO: reorder by frequency
            if constexpr (HAS_MEMBER_preTransformExpression<FUNC>::value) {
                what = PostPonePreTransform_Expression<FUNC, CTX, HAS_MEMBER_preTransformExpression<FUNC>::value>::call(
                    ctx, move(what), func);
            }

            if (isa_tree<EmptyTree>(what.get()) || isa_tree<ZSuperArgs>(what.get())) {
                return what;
            }

            if (ClassDef *u = cast_tree<ClassDef>(what.get())) {
                unique_ptr<ClassDef> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformClassDef<FUNC>::value) {
                    v = PostPonePreTransform_ClassDef<FUNC, CTX, HAS_MEMBER_preTransformClassDef<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                for (auto &def : v->rhs) {
                    def = mapIt(move(def), ctx.withOwner(v->symbol));
                }

                if constexpr (HAS_MEMBER_postTransformClassDef<FUNC>::value) {
                    return PostPonePostTransform_ClassDef<FUNC, CTX,
                                                          HAS_MEMBER_postTransformClassDef<FUNC>::value>::call(ctx,
                                                                                                               move(v),
                                                                                                               func);
                }

                return move(v);
            } else if (MethodDef *u = cast_tree<MethodDef>(what.get())) {
                unique_ptr<MethodDef> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformMethodDef<FUNC>::value) {
                    v = PostPonePreTransform_MethodDef<FUNC, CTX, HAS_MEMBER_preTransformMethodDef<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                for (auto &arg : v->args) {
                    // Only OptionalArgs have subexpressions within them.
                    if (auto *optArg = cast_tree<OptionalArg>(arg.get())) {
                        optArg->default_ = mapIt(move(optArg->default_), ctx.withOwner(v->symbol));
                    }
                }
                v->rhs = mapIt(move(v->rhs), ctx.withOwner(v->symbol));

                if constexpr (HAS_MEMBER_postTransformMethodDef<FUNC>::value) {
                    return PostPonePostTransform_MethodDef<
                        FUNC, CTX, HAS_MEMBER_postTransformMethodDef<FUNC>::value>::call(ctx, move(v), func);
                }

                return move(v);
            } else if (If *u = cast_tree<If>(what.get())) {
                unique_ptr<If> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformIf<FUNC>::value) {
                    v = PostPonePreTransform_If<FUNC, CTX, HAS_MEMBER_preTransformIf<FUNC>::value>::call(ctx, move(v),
                                                                                                         func);
                }
                v->cond = mapIt(move(v->cond), ctx);
                v->thenp = mapIt(move(v->thenp), ctx);
                v->elsep = mapIt(move(v->elsep), ctx);

                if constexpr (HAS_MEMBER_postTransformIf<FUNC>::value) {
                    return PostPonePostTransform_If<FUNC, CTX, HAS_MEMBER_postTransformIf<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                return move(v);
            } else if (While *u = cast_tree<While>(what.get())) {
                unique_ptr<While> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformWhile<FUNC>::value) {
                    v = PostPonePreTransform_While<FUNC, CTX, HAS_MEMBER_preTransformWhile<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                v->cond = mapIt(move(v->cond), ctx);
                v->body = mapIt(move(v->body), ctx);

                if constexpr (HAS_MEMBER_postTransformWhile<FUNC>::value) {
                    return PostPonePostTransform_While<FUNC, CTX, HAS_MEMBER_postTransformWhile<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                return move(v);
            } else if (Break *u = cast_tree<Break>(what.get())) {
                unique_ptr<Break> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformBreak<FUNC>::value) {
                    return PostPonePreTransform_Break<FUNC, CTX, HAS_MEMBER_preTransformBreak<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                v->expr = mapIt(move(v->expr), ctx);

                if constexpr (HAS_MEMBER_postTransformBreak<FUNC>::value) {
                    return PostPonePostTransform_Break<FUNC, CTX, HAS_MEMBER_postTransformBreak<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                return move(v);
            } else if (Retry *u = cast_tree<Retry>(what.get())) {
                unique_ptr<Retry> v(u);
                what.release();
                if constexpr (HAS_MEMBER_postTransformRetry<FUNC>::value) {
                    return PostPonePostTransform_Retry<FUNC, CTX, HAS_MEMBER_postTransformRetry<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                return move(v);
            } else if (Next *u = cast_tree<Next>(what.get())) {
                unique_ptr<Next> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformNext<FUNC>::value) {
                    return PostPonePreTransform_Next<FUNC, CTX, HAS_MEMBER_preTransformNext<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                v->expr = mapIt(move(v->expr), ctx);

                if constexpr (HAS_MEMBER_postTransformNext<FUNC>::value) {
                    return PostPonePostTransform_Next<FUNC, CTX, HAS_MEMBER_postTransformNext<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                return move(v);
            } else if (Return *u = cast_tree<Return>(what.get())) {
                unique_ptr<Return> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformReturn<FUNC>::value) {
                    v = PostPonePreTransform_Return<FUNC, CTX, HAS_MEMBER_preTransformReturn<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                v->expr = mapIt(move(v->expr), ctx);

                if constexpr (HAS_MEMBER_postTransformReturn<FUNC>::value) {
                    return PostPonePostTransform_Return<FUNC, CTX, HAS_MEMBER_postTransformReturn<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                return move(v);
            } else if (Yield *u = cast_tree<Yield>(what.get())) {
                unique_ptr<Yield> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformYield<FUNC>::value) {
                    v = PostPonePreTransform_Yield<FUNC, CTX, HAS_MEMBER_preTransformYield<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                for (auto &arg : v->args) {
                    arg = mapIt(move(arg), ctx);
                }

                if constexpr (HAS_MEMBER_postTransformYield<FUNC>::value) {
                    return PostPonePostTransform_Yield<FUNC, CTX, HAS_MEMBER_postTransformYield<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                return move(v);
            } else if (RescueCase *u = cast_tree<RescueCase>(what.get())) {
                unique_ptr<RescueCase> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformRescueCase<FUNC>::value) {
                    v = PostPonePreTransform_RescueCase<FUNC, CTX,
                                                        HAS_MEMBER_preTransformRescueCase<FUNC>::value>::call(ctx,
                                                                                                              move(v),
                                                                                                              func);
                }

                for (auto &el : v->exceptions) {
                    el = mapIt(move(el), ctx);
                }

                v->var = mapIt(move(v->var), ctx);

                v->body = mapIt(move(v->body), ctx);

                if constexpr (HAS_MEMBER_postTransformRescueCase<FUNC>::value) {
                    return PostPonePostTransform_RescueCase<
                        FUNC, CTX, HAS_MEMBER_postTransformRescueCase<FUNC>::value>::call(ctx, move(v), func);
                }

                return move(v);
            } else if (Rescue *u = cast_tree<Rescue>(what.get())) {
                unique_ptr<Rescue> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformRescue<FUNC>::value) {
                    v = PostPonePreTransform_Rescue<FUNC, CTX, HAS_MEMBER_preTransformRescue<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                v->body = mapIt(move(v->body), ctx);

                int i = 0;
                while (i < v->rescueCases.size()) {
                    auto &el = v->rescueCases[i];
                    auto oldRef = el.get();
                    auto narg = mapIt(move(el), ctx);
                    if (el.get() != narg.get()) {
                        auto nargCase = cast_tree<RescueCase>(narg.get());
                        ENFORCE(nargCase != nullptr, "rescue case was mapped into non-a rescue case");
                        el.reset(nargCase);
                        narg.release();
                    } else {
                        narg.release();
                        el.reset(oldRef);
                    }
                    i++;
                }

                v->else_ = mapIt(move(v->else_), ctx);
                v->ensure = mapIt(move(v->ensure), ctx);

                if constexpr (HAS_MEMBER_postTransformRescue<FUNC>::value) {
                    return PostPonePostTransform_Rescue<FUNC, CTX, HAS_MEMBER_postTransformRescue<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                return move(v);
            } else if (Field *u = cast_tree<Field>(what.get())) {
                unique_ptr<Field> v(u);
                what.release();
                if constexpr (HAS_MEMBER_postTransformField<FUNC>::value) {
                    return PostPonePostTransform_Field<FUNC, CTX, HAS_MEMBER_postTransformField<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                return move(v);
            } else if (UnresolvedIdent *u = cast_tree<UnresolvedIdent>(what.get())) {
                unique_ptr<UnresolvedIdent> v(u);
                what.release();
                if constexpr (HAS_MEMBER_postTransformUnresolvedIdent<FUNC>::value) {
                    return PostPonePostTransform_UnresolvedIdent<
                        FUNC, CTX, HAS_MEMBER_postTransformUnresolvedIdent<FUNC>::value>::call(ctx, move(v), func);
                }
                return move(v);
            } else if (Assign *u = cast_tree<Assign>(what.get())) {
                unique_ptr<Assign> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformAssign<FUNC>::value) {
                    v = PostPonePreTransform_Assign<FUNC, CTX, HAS_MEMBER_preTransformAssign<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                v->lhs = mapIt(move(v->lhs), ctx);
                v->rhs = mapIt(move(v->rhs), ctx);

                if constexpr (HAS_MEMBER_postTransformAssign<FUNC>::value) {
                    return PostPonePostTransform_Assign<FUNC, CTX, HAS_MEMBER_postTransformAssign<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                return move(v);
            } else if (Send *u = cast_tree<Send>(what.get())) {
                unique_ptr<Send> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformSend<FUNC>::value) {
                    v = PostPonePreTransform_Send<FUNC, CTX, HAS_MEMBER_preTransformSend<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                v->recv = mapIt(move(v->recv), ctx);
                for (auto &arg : v->args) {
                    arg = mapIt(move(arg), ctx);
                    ENFORCE(arg.get() != nullptr);
                }

                if (v->block) {
                    auto nblock = mapIt(move(v->block), ctx);
                    ENFORCE(isa_tree<Block>(nblock.get()), "block was mapped into not-a block");
                    v->block.reset(cast_tree<Block>(nblock.release()));
                }

                if constexpr (HAS_MEMBER_postTransformSend<FUNC>::value) {
                    return PostPonePostTransform_Send<FUNC, CTX, HAS_MEMBER_postTransformSend<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                return move(v);
            } else if (Hash *u = cast_tree<Hash>(what.get())) {
                unique_ptr<Hash> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformHash<FUNC>::value) {
                    v = PostPonePreTransform_Hash<FUNC, CTX, HAS_MEMBER_preTransformHash<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                for (auto &key : v->keys) {
                    key = mapIt(move(key), ctx);
                }

                for (auto &value : v->values) {
                    value = mapIt(move(value), ctx);
                }

                if constexpr (HAS_MEMBER_postTransformArray<FUNC>::value) {
                    return PostPonePostTransform_Hash<FUNC, CTX, HAS_MEMBER_postTransformHash<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                return move(v);
            } else if (Array *u = cast_tree<Array>(what.get())) {
                unique_ptr<Array> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformArray<FUNC>::value) {
                    v = PostPonePreTransform_Array<FUNC, CTX, HAS_MEMBER_preTransformArray<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                for (auto &elem : v->elems) {
                    elem = mapIt(move(elem), ctx);
                }

                if constexpr (HAS_MEMBER_postTransformArray<FUNC>::value) {
                    return PostPonePostTransform_Array<FUNC, CTX, HAS_MEMBER_postTransformArray<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                return move(v);
            } else if (Literal *u = cast_tree<Literal>(what.get())) {
                unique_ptr<Literal> v(u);
                what.release();
                if constexpr (HAS_MEMBER_postTransformLiteral<FUNC>::value) {
                    return PostPonePostTransform_Literal<FUNC, CTX, HAS_MEMBER_postTransformLiteral<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                return move(v);
            } else if (UnresolvedConstantLit *u = cast_tree<UnresolvedConstantLit>(what.get())) {
                unique_ptr<UnresolvedConstantLit> v(u);
                what.release();
                if constexpr (HAS_MEMBER_postTransformUnresolvedConstantLit<FUNC>::value) {
                    return PostPonePostTransform_UnresolvedConstantLit<
                        FUNC, CTX, HAS_MEMBER_postTransformUnresolvedConstantLit<FUNC>::value>::call(ctx, move(v),
                                                                                                     func);
                }
                return move(v);
            } else if (ConstantLit *u = cast_tree<ConstantLit>(what.get())) {
                unique_ptr<ConstantLit> v(u);
                what.release();
                if (v->typeAlias) {
                    v->typeAlias = mapIt(move(v->typeAlias), ctx);
                }
                if constexpr (HAS_MEMBER_postTransformConstantLit<FUNC>::value) {
                    return PostPonePostTransform_ConstantLit<
                        FUNC, CTX, HAS_MEMBER_postTransformConstantLit<FUNC>::value>::call(ctx, move(v), func);
                }
                return move(v);
            } else if (Self *u = cast_tree<Self>(what.get())) {
                unique_ptr<Self> v(u);
                what.release();
                if constexpr (HAS_MEMBER_postTransformSelf<FUNC>::value) {
                    return PostPonePostTransform_Self<FUNC, CTX, HAS_MEMBER_postTransformSelf<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                return move(v);
            } else if (Block *u = cast_tree<Block>(what.get())) {
                unique_ptr<Block> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformBlock<FUNC>::value) {
                    v = PostPonePreTransform_Block<FUNC, CTX, HAS_MEMBER_preTransformBlock<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                for (auto &arg : v->args) {
                    // Only OptionalArgs have subexpressions within them.
                    if (auto *optArg = cast_tree<OptionalArg>(arg.get())) {
                        optArg->default_ = mapIt(move(optArg->default_), ctx.withOwner(v->symbol));
                    }
                }
                v->body = mapIt(move(v->body), ctx.withOwner(v->symbol));

                if constexpr (HAS_MEMBER_postTransformBlock<FUNC>::value) {
                    return PostPonePostTransform_Block<FUNC, CTX, HAS_MEMBER_postTransformBlock<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                return move(v);
            } else if (InsSeq *u = cast_tree<InsSeq>(what.get())) {
                unique_ptr<InsSeq> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformInsSeq<FUNC>::value) {
                    v = PostPonePreTransform_InsSeq<FUNC, CTX, HAS_MEMBER_preTransformInsSeq<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                for (auto &stat : v->stats) {
                    stat = mapIt(move(stat), ctx);
                }

                v->expr = mapIt(move(v->expr), ctx);

                if constexpr (HAS_MEMBER_postTransformInsSeq<FUNC>::value) {
                    return PostPonePostTransform_InsSeq<FUNC, CTX, HAS_MEMBER_postTransformInsSeq<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                return move(v);
            } else if (Local *u = cast_tree<Local>(what.get())) {
                unique_ptr<Local> v(u);
                what.release();
                if constexpr (HAS_MEMBER_postTransformLocal<FUNC>::value) {
                    return PostPonePostTransform_Local<FUNC, CTX, HAS_MEMBER_postTransformLocal<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                return move(v);
            } else if (Cast *u = cast_tree<Cast>(what.get())) {
                unique_ptr<Cast> v(u);
                what.release();
                if constexpr (HAS_MEMBER_preTransformCast<FUNC>::value) {
                    v = PostPonePreTransform_Cast<FUNC, CTX, HAS_MEMBER_preTransformCast<FUNC>::value>::call(
                        ctx, move(v), func);
                }
                v->arg = mapIt(move(v->arg), ctx);

                if constexpr (HAS_MEMBER_postTransformCast<FUNC>::value) {
                    return PostPonePostTransform_Cast<FUNC, CTX, HAS_MEMBER_postTransformCast<FUNC>::value>::call(
                        ctx, move(v), func);
                }

                return move(v);
            } else {
                auto *ref = what.get();
                Error::raise("should never happen. Forgot to add new tree kind? ", demangle(typeid(*ref).name()));
            }
        } catch (SRubyException &) {
            if (!locReported) {
                locReported = true;
                if (auto e = ctx.state.beginError(loc, core::errors::Internal::InternalError)) {
                    e.setHeader("Failed to process tree (backtrace is above)");
                }
            }
            throw;
        }
    }
};

class TreeMap {
public:
    template <typename CTX, typename FUNC>
    static unique_ptr<Expression> apply(CTX ctx, FUNC &func, unique_ptr<Expression> to) {
        TreeMapper<FUNC, CTX> walker(func);
        return walker.mapIt(move(to), ctx);
    }
};

} // namespace ast
} // namespace sorbet

#endif // SORBET_TREEMAP_H
