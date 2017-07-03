/* unpack.h
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

        /// Unpacks tuple to class c'tor
        /**
        *   This class is used to split a normal tuple into a parameter pack and forward it to the constructor of a arbitary class
        *
        *    \tparam TForward Class who gets the unpacked tuple as constructor input
        */
        template<class TForward>
        class tupleUnpack : public TForward
        {
        private:

            /// Unpack tuple expansion
            /**
            *    This function uses an indice list and the default tuple get function to expand the tuple contents into a comma seperated list
            *    and perfect forward it to the constructor of the user defined class
            *    \param args Tuple with content
            *    \tparam TArgs Variadic template describing the tuple content types
            *    \tparam Is Variadic template containing the indice list from index_sequence to access all tuple members
            */
            template<class ... TArgs, std::size_t ... Is>
            tupleUnpack(std::tuple<TArgs...>&& args, meta::integer_sequence<Is...>)
            : TForward( std::forward<TArgs>( std::get<Is>(args) )... )
            {
            }

        public:

            /// Single parameter direct forward
            /**
            *    perfect forwards a single parameter to the inherit class
            */
            template<class TArg>
            tupleUnpack(TArg arg)
            : TForward( std::forward<TArg>(arg) )
            {}

                /// Single tuple element direct forward
                /**
                *    perfect forwards a single parameter to the inherit class
                */
                template<class TArg>
                tupleUnpack(std::tuple<TArg> arg)
                : TForward( std::forward<TArg>(std::get<0>(arg)) )
                {}


                    /// Unpack tuple interface
                    /**
                    *    Constructor that gets an tuple as parameter and creates an index sequence for it to forwad booth to the privat constructor
                    *    \param args Tuple with content
                    *    \tparam TArgs Variadic template describing the tuple content types
                    */
                    template<class ... TArgs>
                    tupleUnpack(std::tuple<TArgs...> args)
                    : tupleUnpack( std::forward<std::tuple<TArgs...>&& >(args), meta::index_sequence_from<sizeof...(TArgs)>())
                    {}




                    };

                }

            }
