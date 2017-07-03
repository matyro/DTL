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
        class foldTupleExtended
        {
        private:

            template<size_t TNum, template <typename> class TFunc, class TReturn>
            struct _foldr_class
            {
                template<class ... TArgs>
                constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
                {
                    return static_cast<TReturn>(TFunc<decltype(std::get<std::tuple_size<decltype(tup)>::value - TNum>(tup))>(std::get<std::tuple_size<decltype(tup)>::value - TNum>(tup), _foldr_class<TNum - 1, TFunc, TReturn>::fold(tup, init)));
                }
            };

            template<template <typename> class TFunc, class TReturn>
            struct _foldr_class<0, TFunc, TReturn>
            {
                template<class ... TArgs>
                constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
                {
                    (void)(tup);
                    return init;
                }
            };

            template<size_t TNum, template <typename> class TFunc, class TReturn>
            struct _foldl_class
            {
                template<class ... TArgs>
                constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
                {
                    return static_cast<TReturn>(TFunc<decltype(std::get<TNum>(tup))>(std::get<TNum>(tup), _foldl_class<TNum - 1, TFunc, TReturn>::fold(tup, init)));
                }
            };

            template<template <typename> class TFunc, class TReturn>
            struct _foldl_class<0, TFunc, TReturn>
            {
                template<class ... TArgs>
                constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
                {
                    return static_cast<TReturn>(TFunc<decltype(std::get<0>(tup))>(std::get<0>(tup), init));
                }
            };






            /*template<size_t TNum, class TType, template <TType> class TFunc, class TReturn, TType ... TX>
            struct _foldl_t;

            template<size_t TNum, class TType, template <TType> class TFunc, class TReturn, TType T1, TType ... TX>
            struct _foldl_t<TNum, TType, TFunc, TReturn, T1, TX...>
            {
            template<class ... TArgs>
            constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
            {
            return static_cast<TReturn>(
            TFunc< T1 >(
            std::get<TNum>(tup),
            _foldl_t<TNum - 1, TType, TFunc, TReturn, TX...>::fold(tup, init))        );    }};

            template<class TType, template <TType> class TFunc, class TReturn, TType T1>
            struct _foldl_t<0, TType, TFunc, TReturn, T1>
            {
            template<class ... TArgs>
            constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
            {
            (void)(tup);
            return static_cast<TReturn>(
            TFunc<T1>(
            std::get<0>(tup), init)    );}};*/


            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            template<size_t TNum, class TType, template <TType> class TFunc, class TReturn, TType ... TX>
            struct _foldr_t;

            template<size_t TNum, class TType, template <TType> class TFunc, class TReturn, TType T1, TType ... TX>
            struct _foldr_t<TNum, TType, TFunc, TReturn, T1, TX...>
            {
                template<class ... TArgs>
                constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
                {
                    return static_cast<TReturn>(
                        TFunc< T1 >(
                            std::get<std::tuple_size<decltype(tup)>::value - TNum>(tup),
                            _foldr_t<TNum - 1, TType, TFunc, TReturn, TX...>::fold(tup, init))
                        );
                    }
                };

                template<class TType, template <TType> class TFunc, class TReturn>
                struct _foldr_t<0, TType, TFunc, TReturn>
                {
                    template<class ... TArgs>
                    constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
                    {
                        (void)(tup);
                        return init;
                    }
                };


            public:




                template<template <typename> class TFunc, class TReturn, class TTuple>
                constexpr static TReturn foldl_class(const TTuple tup, const TReturn init)
                {
                    return _foldl_class<std::tuple_size<TTuple>::value - 1, TFunc, TReturn>::fold(tup, init);
                }

                template<template <typename> class TFunc, class TReturn, class TTuple>
                constexpr static TReturn foldr_class(const TTuple tup, const TReturn init)
                {
                    return _foldr_class<std::tuple_size<TTuple>::value, TFunc, TReturn>::fold(tup, init);
                }


                // To make this work the template pack TX must be reversed
                /*template<class TType, template <TType> class TFunc, class TReturn, class TTuple, TType ... TX>
                constexpr static TReturn foldl_t(const TTuple tup, const TReturn init)
                {
                return _foldl_t<std::tuple_size<TTuple>::value - 1, TType, TFunc, TReturn, TX...>::fold(tup, init);            }*/

                template<class TType, template <TType> class TFunc, class TReturn, class TTuple, TType ... TX>
                constexpr static TReturn foldr_t(const TTuple tup, const TReturn init)
                {
                    return _foldr_t<std::tuple_size<TTuple>::value, TType, TFunc, TReturn, TX...>::fold(tup, init);
                }

            };
        }

    }
