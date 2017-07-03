/* fold.h
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
        /// Tuple fold methode
        /**
        *   Fold right and left for tuples. The user must guarantee that the types inside the tuple are castable to the needed types
        *
        *   Foldl: call function init and first element -> call function with return and second element -> ...
        *   Foldr: call function init and last element -> call function with return and second to last -> ...
        */
        class foldTuple
        {
        private:

            template<size_t TNum, class TFunc, class TReturn>
            struct _foldl
            {
                template<class ... TArgs>
                constexpr static TReturn fold(TFunc f, const std::tuple<TArgs...> tup, TReturn init)
                {
                    return f(_foldl<TNum - 1, TFunc, TReturn>::fold(f, tup, std::forward<TReturn>(init)), std::get<TNum - 1>(tup));
                }
            };

            template<class TFunc, class TReturn>
            struct _foldl<0, TFunc, TReturn>
            {
                template<class ... TArgs>
                constexpr static TReturn fold(TFunc f, const std::tuple<TArgs...> tup, TReturn init)
                {
                    (void)(f);
                    (void)(tup);
                    return init;
                }
            };

            template<size_t TNum, class TFunc, class TReturn>
            struct _foldr
            {
                template<class ... TArgs>
                constexpr static TReturn fold(TFunc f, const std::tuple<TArgs...> tup, TReturn init)
                {
                    return f(std::get<std::tuple_size<decltype(tup)>::value - TNum>(tup), _foldr<TNum - 1, TFunc, TReturn>::fold(f, tup, std::forward<TReturn>(init) ));
                }
            };

            template<class TFunc, class TReturn>
            struct _foldr<0, TFunc, TReturn>
            {
                template<class ... TArgs>
                constexpr static TReturn fold(TFunc f, const std::tuple<TArgs...> tup, TReturn init)
                {
                    (void)(f);
                    (void)(tup);
                    return init;
                }
            };


            public:

                template<class TFunc, class TReturn, class TTuple>
                constexpr static TReturn foldl(TFunc f, const TTuple tup, TReturn init)
                {
                    return _foldl<std::tuple_size<TTuple>::value, TFunc, TReturn>::fold(f, tup, std::forward<TReturn>(init) );
                }

                template<class TFunc, class TReturn, class TTuple>
                constexpr static TReturn foldr(TFunc f, const TTuple tup, TReturn init)
                {
                    return _foldr<std::tuple_size<TTuple>::value, TFunc, TReturn>::fold(f, tup, std::forward<TReturn>(init) );
                }

            };
        }

    }
