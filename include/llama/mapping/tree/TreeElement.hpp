/* Copyright 2018 Alexander Matthes
 *
 * This file is part of LLAMA.
 *
 * LLAMA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * LLAMA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with LLAMA.  If not, see <www.gnu.org/licenses/>.
 */

#pragma once

#include <cstddef>
#include <type_traits>

#include "../../Tuple.hpp"

namespace llama
{

namespace mapping
{

namespace tree
{

template<
    typename T_Identifier,
    typename T_Type,
    typename T_CountType = std::size_t
>
struct TreeElement
{
	using IsTreeElementWithoutChilds = void;
    using Identifier = T_Identifier;
    using Type = T_Type;

    LLAMA_FN_HOST_ACC_INLINE
    TreeElement() :
        count( 1 )
    {}

    ~TreeElement() = default;

    LLAMA_FN_HOST_ACC_INLINE
    TreeElement( const T_CountType count ) : count(count) {}

    const T_CountType count;
};

template<
    typename T_Identifier,
    typename T_CountType,
    typename... T_Childs
>
struct TreeElement<
    T_Identifier,
    Tuple< T_Childs... >,
    T_CountType
>
{
	using IsTreeElementWithChilds = void;
    using Identifier = T_Identifier;
    using Type = Tuple< T_Childs... >;

    LLAMA_FN_HOST_ACC_INLINE
    TreeElement() :
        count( 1 )
    {}

    ~TreeElement() = default;

    LLAMA_FN_HOST_ACC_INLINE
    TreeElement(
        const T_CountType count,
        const Type childs
    ) :
        count(count),
        childs(childs)
    {}

    LLAMA_FN_HOST_ACC_INLINE
    TreeElement(
        const T_CountType count
    ) :
        count(count),
        childs()
    {}

    LLAMA_FN_HOST_ACC_INLINE
    TreeElement(
        const Type childs,
        const T_CountType count = 1
    ) :
        count(count),
        childs(childs)
    {}

    const T_CountType count;
    const Type childs;
};

template<
    typename T_Identifier,
    typename T_Type,
    typename T_CountType,
    T_CountType T_count
>
struct TreeElement<
    T_Identifier,
    T_Type,
    std::integral_constant< T_CountType, T_count >
>
{
	using IsTreeElementWithoutChilds = void;
    using Identifier = T_Identifier;
    using Type = T_Type;
    using CountType = std::integral_constant< T_CountType, T_count>;

    LLAMA_FN_HOST_ACC_INLINE
    TreeElement() {}

    LLAMA_FN_HOST_ACC_INLINE
    TreeElement( const T_CountType count ) {}

    LLAMA_IF_RELEASE( static constexpr ) CountType
    LLAMA_IF_DEBUG( const ) count = CountType();
};

template<
    typename T_Identifier,
    typename T_CountType,
    T_CountType T_count,
    typename... T_Childs
>
struct TreeElement<
    T_Identifier,
    Tuple< T_Childs... >,
    std::integral_constant< T_CountType, T_count>
>
{
	using IsTreeElementWithChilds = void;
    using Identifier = T_Identifier;
    using Type = Tuple< T_Childs... >;
    using CountType = std::integral_constant< T_CountType, T_count>;

    LLAMA_FN_HOST_ACC_INLINE
    TreeElement() : childs() {}

    LLAMA_FN_HOST_ACC_INLINE
    TreeElement(
        const T_CountType count,
        const Type childs = Type()
    ) : childs(childs) {}

    LLAMA_FN_HOST_ACC_INLINE
    TreeElement(
        const Type childs,
        const T_CountType count = T_CountType()
    ) : childs(childs) {}

    LLAMA_IF_RELEASE( static constexpr ) CountType
    LLAMA_IF_DEBUG( const ) count = CountType();

    const Type childs;
};

template<
    typename T_Identifier,
    typename T_Type,
    std::size_t T_count = 1
>
using TreeElementConst = TreeElement<
    T_Identifier,
    T_Type,
    std::integral_constant< std::size_t, T_count >
>;

template< typename T_Tree >
struct TreePopFrontChild
{
    using ResultType = TreeElement<
        typename T_Tree::Identifier,
        typename T_Tree::Type::RestTuple
    >;

    LLAMA_FN_HOST_ACC_INLINE
    auto
    operator()( T_Tree const & tree)
    -> ResultType
    {
        return ResultType(
            tree.count,
            tree.childs.rest
        );
    }
};

template<
    typename T_Identifier,
    typename T_Type,
    std::size_t T_count
>
struct TreePopFrontChild<
    TreeElementConst<
        T_Identifier,
        T_Type,
        T_count
    >
>
{
    using Tree = TreeElementConst<
        T_Identifier,
        T_Type,
        T_count
    >;
    using ResultType = TreeElementConst<
        typename Tree::Identifier,
        typename Tree::Type::RestTuple,
        T_count
    >;

    auto
    LLAMA_FN_HOST_ACC_INLINE
    operator()( Tree const & tree )
    -> ResultType
    {
        return ResultType( tree.childs.rest );
    }
};

template<
    typename T_Childs,
    typename T_CountType
>
struct TreeOptimalType
{
    using type = TreeElement<
        NoName,
        T_Childs
    >;
};

template<
    typename T_Childs,
    std::size_t T_count
>
struct TreeOptimalType<
    T_Childs,
    std::integral_constant<
        std::size_t,
        T_count
    >
>
{
    using type = TreeElementConst<
        NoName,
        T_Childs,
        T_count
    >;
};

} // namespace tree

} // namespace mapping

} // namespace llama

