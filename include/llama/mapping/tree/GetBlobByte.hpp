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

#include "GetBlobSize.hpp"
#include "Operations.hpp"

namespace llama::mapping::tree
{
    namespace internal
    {
        template<typename T_Tree, std::size_t T_Pos>
        LLAMA_FN_HOST_ACC_INLINE auto summarizeTreeSmallerPos(
            typename T_Tree::Type const & childs,
            decltype(T_Tree::count) const & count) -> std::size_t
        {
            if constexpr(T_Pos == 0)
                return 0;
            else
                return getTreeBlobSize(childs.first)
                    + summarizeTreeSmallerPos<
                           typename TreePopFrontChild<T_Tree>::ResultType,
                           T_Pos - 1>(childs.rest, count);
        }

        template<typename T_Tree, typename T_LastCoord>
        LLAMA_FN_HOST_ACC_INLINE auto getTreeBlobByteImpl(
            const T_Tree & tree,
            const Tuple<T_LastCoord> & treeCoord) -> std::size_t
        {
            return sizeof(typename T_Tree::Type) * treeCoord.first.runtime;
        }

        template<typename T_Tree, typename T_TreeCoord>
        LLAMA_FN_HOST_ACC_INLINE auto
        getTreeBlobByteImpl(const T_Tree & tree, const T_TreeCoord & treeCoord)
            -> std::size_t
        {
            return getTreeBlobSize(
                       tree.childs, LLAMA_DEREFERENCE(treeCoord.first.runtime))
                + summarizeTreeSmallerPos<
                       T_Tree,
                       decltype(T_TreeCoord::FirstElement::compiletime)::value>(
                       tree.childs, LLAMA_DEREFERENCE(tree.count))
                + getTreeBlobByteImpl(
                       getTupleElementRef<
                           T_TreeCoord::FirstElement::compiletime>(tree.childs),
                       treeCoord.rest);
        }
    }

    template<typename T_Tree, typename T_TreeCoord>
    LLAMA_FN_HOST_ACC_INLINE auto
    getTreeBlobByte(T_Tree const & tree, T_TreeCoord const & treeCoord)
        -> std::size_t
    {
        return internal::getTreeBlobByteImpl(tree, treeCoord);
    }
}
