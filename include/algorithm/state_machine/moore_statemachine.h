/* moore_statemachine.h
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
#include <utility>

#include "meta/index_sequence.h"

#include "algorithm/state_machine/moore_state.h"

namespace algorithm
{
    namespace state_machine
    {

template <class TIn, class ... TArgs>
class MooreStateMachine
{
    static_assert(sizeof...(TArgs) > 0, "Number of States must be at least 1!");

    private:
    std::tuple<TArgs...> m_states;

    MooreState<TIn>* m_current;

    template<std::size_t... I>
    std::tuple<TArgs* ...> _getStates( meta::integer_sequence<I...> )
    {
        return std::make_tuple( &std::get<I>(m_states)... );
    }

    public:

        /** Constructor takes one to N different States and stores them in this class
        *   You should not access the States after they were transfered into this class
        *   use getStates() to access and mofify each state
        */
        constexpr MooreStateMachine(TArgs&&... states)
        : m_states{ states... }, m_current( &std::get<0>(m_states) )
        {
            //static_assert(std::is_convertible<, "Must inherit from class MooreState");
        }

        /** Recieve pointers to access and modifie stored States
        *   User std::tie( pointers ) to access the states
        */
        std::tuple<TArgs* ...> getStates()
        {
            return _getStates(meta::index_sequence_from< sizeof...(TArgs) >() );
        }

        /** Transfer State into the next
        *  Inputs data into the StateMachine and transfers to the next state
        */
        void call(TIn in)
        {
            m_current = m_current->transfer(in);
            m_current->operator()();
        }

        inline long size() const
        {
            return sizeof...(TArgs);
        }
};

}
}
