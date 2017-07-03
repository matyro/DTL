/* filter.h
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

namespace meta
{
    namespace tuple
    {

        /**
        *    \file filter.h
        *    \brief Implementation of compile time function to reduce a tuple to specific types        
        *
        */

        namespace filterNamespace
        {
            template<class TTuple, class TFilter, size_t TNum, bool TMatch>
            struct _filter;

            template<class TTuple, class TFilter, size_t TNum>
            struct _filter<TTuple, TFilter, TNum, true>
            {
                constexpr static auto filter_same(const TTuple tup)
                {
                    return std::tuple_cat(_filter<TTuple, TFilter, TNum - 1, std::is_same<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_same(tup),
                    std::make_tuple(std::get<TNum>(tup)));
                }

                constexpr static auto filter_not_same(const TTuple tup)
                {
                    return std::tuple_cat(_filter<TTuple, TFilter, TNum - 1, !std::is_same<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_not_same(tup),
                    std::make_tuple(std::get<TNum>(tup)));
                }

                constexpr static auto filter_convertible(const TTuple tup)
                {
                    return std::tuple_cat(_filter<TTuple, TFilter, TNum - 1, std::is_convertible<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_convertible(tup),
                    std::make_tuple(std::get<TNum>(tup)));
                }

                constexpr static auto filter_not_convertible(const TTuple tup)
                {
                    return std::tuple_cat(_filter<TTuple, TFilter, TNum - 1, !std::is_convertible<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_not_convertible(tup),
                    std::make_tuple(std::get<TNum>(tup)));
                }
            };

            template<class TTuple, class TFilter, size_t TNum>
            struct _filter<TTuple, TFilter, TNum, false>
            {
                constexpr static auto filter_same(const TTuple tup)
                {
                    return _filter<TTuple, TFilter, TNum - 1, std::is_same<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_same(tup);
                }

                constexpr static auto filter_not_same(const TTuple tup)
                {
                    return _filter<TTuple, TFilter, TNum - 1, !std::is_same<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_not_same(tup);
                }

                constexpr static auto filter_convertible(const TTuple tup)
                {
                    return _filter<TTuple, TFilter, TNum - 1, std::is_convertible<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_convertible(tup);
                }

                constexpr static auto filter_not_convertible(const TTuple tup)
                {
                    return _filter<TTuple, TFilter, TNum - 1, !std::is_convertible<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_not_convertible(tup);
                }
            };

            template<class TTuple, class TFilter>
            struct _filter<TTuple, TFilter, 0, false>
            {
                constexpr static auto filter_same(const TTuple tup)
                {
                    (void)(tup);
                    return std::make_tuple();
                }

                constexpr static auto filter_not_same(const TTuple tup)
                {
                    (void)(tup);
                    return std::make_tuple();
                }

                constexpr static auto filter_convertible(const TTuple tup)
                {
                    (void)(tup);
                    return std::make_tuple();
                }

                constexpr static auto filter_not_convertible(const TTuple tup)
                {
                    (void)(tup);
                    return std::make_tuple();
                }
            };

            template<class TTuple, class TFilter>
            struct _filter<TTuple, TFilter, 0, true>
            {
                constexpr static auto filter_same(const TTuple tup)
                {
                    return std::make_tuple(std::get<0>(tup));
                }

                constexpr static auto filter_not_same(const TTuple tup)
                {
                    return std::make_tuple(std::get<0>(tup));
                }

                constexpr static auto filter_convertible(const TTuple tup)
                {
                    return std::make_tuple(std::get<0>(tup));
                }

                constexpr static auto filter_not_convertible(const TTuple tup)
                {
                    return std::make_tuple(std::get<0>(tup));
                }
            };
        }


        /// Tuple type filter
        /**
        *   Removes all parts of a tuple that does not match the given type
        *
        *   \tparam TFilter Filter the tuple with this specific type
        *	\tparam TTuple Type of tuple that should be filtered
        */
        template<class TFilter, class TTuple>
        class filterTuple
        {
        private:

        public:

            /// Keep all elements of exactly the type TFilter
            constexpr static auto filter_same(const TTuple tup)
            {
                return filterNamespace::_filter<TTuple, TFilter, std::tuple_size<TTuple>::value, false>::filter_same(tup);
            }

            /// Keep all elements that are not exactly TFilter
            constexpr static auto filter_not_same(const TTuple tup)
            {
                return filterNamespace::_filter<TTuple, TFilter, std::tuple_size<TTuple>::value, false>::filter_not_same(tup);
            }

            /// Keep all elements that are castable to TFilter
            constexpr static auto filter_convertible(const TTuple tup)
            {
                return filterNamespace::_filter<TTuple, TFilter, std::tuple_size<TTuple>::value, false>::filter_convertible(tup);
            }

            /// Keep all elements that are not convertible to TFilter
            constexpr static auto filter_not_convertible(const TTuple tup)
            {
                return filterNamespace::_filter<TTuple, TFilter, std::tuple_size<TTuple>::value, false>::filter_not_convertible(tup);
            }

        };
    }

}
