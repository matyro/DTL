/* map.h
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
        class mapTuple
        {
        private:

            template<class TFunc, class TTuple, std::size_t ... Is>
            constexpr static auto _map(TFunc f, TTuple tup, meta::integer_sequence<Is...>)
            {
                return std::make_tuple(f(std::get<Is>(tup))...);
            }
            

            template<class TFunc, class TTuple, std::size_t ... Is>
            constexpr static auto _map_with_index(TFunc f, TTuple tup, meta::integer_sequence<Is...>)
            {
                return std::make_tuple(f(std::get<Is>(tup), Is)...);
            }

        public:

            template<class TFunc, class TTuple>
            constexpr static auto map(TFunc f, TTuple tup)
            {
                return _map<TFunc, TTuple>(f, tup, meta::index_sequence_from<std::tuple_size<TTuple>::value> { });
            }

            template<class TFunc, class TTuple>
            constexpr static auto map_with_index(TFunc f, TTuple tup)
            {
                return _map_with_index<TFunc, TTuple>(f, tup, meta::index_sequence_from<std::tuple_size<TTuple>::value> { });
            }
        };
    }

}
