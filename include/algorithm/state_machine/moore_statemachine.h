#pragma once

#include <tuple>
#include <utility>

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
    std::tuple<TArgs* ...> _getStates( std::index_sequence<I...> )
    {
        return std::make_tuple( &std::get<I>(m_states)... );
    }

    public:
        constexpr MooreStateMachine(TArgs... states)
        : m_states{ states... }, m_current( &std::get<0>(m_states) )
        {
        }

        std::tuple<TArgs* ...> getStates()
        {
            return _getStates(std::make_index_sequence< sizeof...(TArgs) >() );
        }

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
