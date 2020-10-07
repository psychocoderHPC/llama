#include "common.h"

#include <catch2/catch.hpp>
#include <llama/llama.hpp>

// clang-format off
namespace tag {
    struct Pos {};
    struct X {};
    struct Y {};
    struct Z {};
    struct Momentum {};
    struct Weight {};
    struct Flags {};
}

// clang-format off
using Particle = llama::DS<
    llama::DE<tag::Pos, llama::DS<
        llama::DE<tag::X, double>,
        llama::DE<tag::Y, double>,
        llama::DE<tag::Z, double>
    >>,
    llama::DE<tag::Weight, float>,
    llama::DE<tag::Momentum, llama::DS<
        llama::DE<tag::X, double>,
        llama::DE<tag::Y, double>,
        llama::DE<tag::Z, double>
    >>,
    llama::DE<tag::Flags, llama::DA<bool, 4>>
>;
// clang-format on

TEST_CASE("prettyPrintType")
{
    auto str = prettyPrintType(Particle());
#ifdef _WIN32
    boost::replace_all(str, "__int64", "long");
#endif
    CHECK(str == R"(llama::DatumStruct<
    llama::DatumElement<
        tag::Pos,
        llama::DatumStruct<
            llama::DatumElement<
                tag::X,
                double
            >,
            llama::DatumElement<
                tag::Y,
                double
            >,
            llama::DatumElement<
                tag::Z,
                double
            >
        >
    >,
    llama::DatumElement<
        tag::Weight,
        float
    >,
    llama::DatumElement<
        tag::Momentum,
        llama::DatumStruct<
            llama::DatumElement<
                tag::X,
                double
            >,
            llama::DatumElement<
                tag::Y,
                double
            >,
            llama::DatumElement<
                tag::Z,
                double
            >
        >
    >,
    llama::DatumElement<
        tag::Flags,
        llama::DatumStruct<
            llama::DatumElement<
                std::integral_constant<
                    unsigned long,
                    0
                >,
                bool
            >,
            llama::DatumElement<
                std::integral_constant<
                    unsigned long,
                    1
                >,
                bool
            >,
            llama::DatumElement<
                std::integral_constant<
                    unsigned long,
                    2
                >,
                bool
            >,
            llama::DatumElement<
                std::integral_constant<
                    unsigned long,
                    3
                >,
                bool
            >
        >
    >
>)");
}

TEST_CASE("sizeOf")
{
    STATIC_REQUIRE(llama::sizeOf<Particle> == 56);
}
TEST_CASE("offsetOf")
{
    STATIC_REQUIRE(llama::offsetOf<Particle> == 0);
    STATIC_REQUIRE(llama::offsetOf<Particle, 0> == 0);
    STATIC_REQUIRE(llama::offsetOf<Particle, 0, 0> == 0);
    STATIC_REQUIRE(llama::offsetOf<Particle, 0, 1> == 8);
    STATIC_REQUIRE(llama::offsetOf<Particle, 0, 2> == 16);
    STATIC_REQUIRE(llama::offsetOf<Particle, 1> == 24);
    STATIC_REQUIRE(llama::offsetOf<Particle, 2> == 28);
    STATIC_REQUIRE(llama::offsetOf<Particle, 2, 0> == 28);
    STATIC_REQUIRE(llama::offsetOf<Particle, 2, 1> == 36);
    STATIC_REQUIRE(llama::offsetOf<Particle, 2, 2> == 44);
    STATIC_REQUIRE(llama::offsetOf<Particle, 3> == 52);
    STATIC_REQUIRE(llama::offsetOf<Particle, 3, 0> == 52);
    STATIC_REQUIRE(llama::offsetOf<Particle, 3, 1> == 53);
    STATIC_REQUIRE(llama::offsetOf<Particle, 3, 2> == 54);
    STATIC_REQUIRE(llama::offsetOf<Particle, 3, 3> == 55);
}