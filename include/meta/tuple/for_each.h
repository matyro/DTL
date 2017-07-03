/* for_each.h
* this file is part of DTL
*
* Copyright (C) <2017> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once
#include <tuple>

#include "meta/index_sequence.h"

namespace meta
{
    namespace tuple
    {
        /// Map for tuples
        /**
        *   Call a function on every tuple element. The user must guarantee that the types inside the tuple are castable to the needed types
        */
        namespace forEach
        {
            template<typename TTuple, typename TFunc, size_t... TIs>
            void _for_each(TTuple&& tup, TFunc f, integer_sequence<TIs...>)
            {
                auto l = { (f(std::get<TIs>(tup)), 0)... };  // forces correct execution order
                (void)(l);
            }

            template<typename TTuple, typename TFunc, size_t... TIs>
            void _for_each_index(TTuple&& tup, TFunc f, integer_sequence<TIs...>)
            {
                auto l = { (f(std::get<TIs>(tup), TIs), 0)... };  // forces correct execution order
                (void)(l);
            }
        }

        template<typename... TArgs, typename TFunc>
        void for_each(const std::tuple<TArgs...>& tup, TFunc f)
        {
            forEach::_for_each(tup, f, index_sequence_from<sizeof...(TArgs)>() );
        }

        template<typename... TArgs, typename TFunc>
        void for_each_index(const std::tuple<TArgs...>& tup, TFunc f)
        {
            forEach::_for_each_index(tup, f, index_sequence_from<sizeof...(TArgs)>() );
        }
    }

}
