/* print.h
* this file is part of DTL
*
* Copyright (C) <2017> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once
#include<tuple>
#include<sstream>

#include "map.h"
#include "fold.h"
#include "for_each.h"

namespace meta
{
    namespace tuple
    {


        template<class TStream, size_t TSize>
        struct _PrintTupleFunctor
        {
        private:
            TStream& os;

        public:
            constexpr _PrintTupleFunctor(TStream& _os) : os(_os) { }

            template<class TType>
            void operator()(const TType data, size_t i)
            {
                os << data << (i < TSize ? "," : "");
            }
        };

        /// Enable an easy ostream output for tuple
        /**
        *
        */
        template<class TStream, class ... TArgs>
        TStream& operator<<(TStream& os, std::tuple<TArgs...> tup)
        {
            os << "(";
            for_each_index(tup, _PrintTupleFunctor<TStream, sizeof...(TArgs)-1>(os) );
            os << ")";

            return os;
        }

        template<class ... TArgs>
        auto print(std::tuple<TArgs...> tup) -> std::string
        {
            std::stringstream sstr;
            sstr << tup;
            return sstr.str();
        }

    }

}
