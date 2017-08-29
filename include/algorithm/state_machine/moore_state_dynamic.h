#pragma once

#include <vector>
#include <utility>

#include "algorithm/state_machine/moore_state.h"


template<class TIn>
class MooreStateDynamic : public MooreState<TIn>
{
    private:

    std::vector< std::pair<TIn, MooreState<TIn>* > > m_edges;

    public:

    template<class ... TArgs>
    MooreStateDynamic(TArgs ... args)
    : m_edges( args... )
    {}

    void addEdge(TIn i, MooreState<TIn>* const e)
    {
        m_edges.push_back( std::make_pair( i, e ) );
    }

    MooreState<TIn>* transfer(TIn in) override
    {
        for (const auto& itr : m_edges)
        {
            if (itr.first == in)
            {
                   return itr.second;
            }
         }
         return this;
    }
};
