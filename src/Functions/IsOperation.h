#pragma once

namespace DB
{

/// These classes should be present in DB namespace (cannot place them into namelesspace)
template <typename> struct AbsImpl;
template <typename> struct BitCountImpl;
template <typename> struct NegateImpl;
template <typename> struct SignImpl;
template <typename, typename> struct PlusImpl;
template <typename, typename> struct MinusImpl;
template <typename, typename> struct MultiplyImpl;
template <typename, typename> struct DivideFloatingImpl;
template <typename, typename> struct DivideFloatingOrNullImpl;
template <typename, typename> struct DivideIntegralImpl;
template <typename, typename> struct DivideIntegralOrZeroImpl;
template <typename, typename> struct DivideIntegralOrNullImpl;
template <typename, typename> struct LeastBaseImpl;
template <typename, typename> struct GreatestBaseImpl;
template <typename, typename> struct ModuloImpl;
template <typename, typename> struct ModuloOrNullImpl;
template <typename, typename> struct PositiveModuloImpl;
template <typename, typename> struct PositiveModuloOrNullImpl;
template <typename, typename> struct EqualsOp;
template <typename, typename> struct NotEqualsOp;
template <typename, typename> struct LessOrEqualsOp;
template <typename, typename> struct GreaterOrEqualsOp;
template <typename, typename> struct BitHammingDistanceImpl;

template <template <typename, typename> typename Op1, template <typename, typename> typename Op2>
struct IsSameOperation
{
    static constexpr bool value = std::is_same_v<Op1<UInt8, UInt8>, Op2<UInt8, UInt8>>;
};

template <template <typename> typename Op>
struct IsUnaryOperation
{
    static constexpr bool abs = std::is_same_v<Op<Int8>, AbsImpl<Int8>>;
    static constexpr bool negate = std::is_same_v<Op<Int8>, NegateImpl<Int8>>;
    static constexpr bool sign = std::is_same_v<Op<Int8>, SignImpl<Int8>>;
    static constexpr bool bit_count = std::is_same_v<Op<Int8>, BitCountImpl<Int8>>;
};

template <template <typename, typename> typename Op>
struct IsOperation
{
    static constexpr bool equals = IsSameOperation<Op, EqualsOp>::value;
    static constexpr bool not_equals = IsSameOperation<Op, NotEqualsOp>::value;
    static constexpr bool less_or_equals = IsSameOperation<Op, LessOrEqualsOp>::value;
    static constexpr bool greater_or_equals = IsSameOperation<Op, GreaterOrEqualsOp>::value;

    static constexpr bool plus = IsSameOperation<Op, PlusImpl>::value;
    static constexpr bool minus = IsSameOperation<Op, MinusImpl>::value;
    static constexpr bool multiply = IsSameOperation<Op, MultiplyImpl>::value;
    static constexpr bool div_floating = IsSameOperation<Op, DivideFloatingImpl>::value;
    static constexpr bool div_floating_or_null = IsSameOperation<Op, DivideFloatingOrNullImpl>::value;
    static constexpr bool int_div = IsSameOperation<Op, DivideIntegralImpl>::value;
    static constexpr bool int_div_or_zero = IsSameOperation<Op, DivideIntegralOrZeroImpl>::value;
    static constexpr bool int_div_or_null = IsSameOperation<Op, DivideIntegralOrNullImpl>::value;
    static constexpr bool modulo = IsSameOperation<Op, ModuloImpl>::value;
    static constexpr bool modulo_or_null = IsSameOperation<Op, ModuloOrNullImpl>::value;
    static constexpr bool positive_modulo = IsSameOperation<Op, PositiveModuloImpl>::value;
    static constexpr bool positive_modulo_or_null = IsSameOperation<Op, PositiveModuloOrNullImpl>::value;
    static constexpr bool least = IsSameOperation<Op, LeastBaseImpl>::value;
    static constexpr bool greatest = IsSameOperation<Op, GreatestBaseImpl>::value;

    static constexpr bool bit_hamming_distance = IsSameOperation<Op, BitHammingDistanceImpl>::value;

    static constexpr bool division = div_floating || int_div || int_div_or_zero || modulo || positive_modulo;
    // NOTE: allow_decimal should not fully contain `division` because of divInt
    static constexpr bool allow_decimal = plus || minus || multiply || division || least || greatest;
    static constexpr bool division_or_null = modulo_or_null || positive_modulo_or_null || int_div_or_null || div_floating_or_null;
};

}
