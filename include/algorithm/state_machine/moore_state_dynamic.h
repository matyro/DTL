#pragma once

#include <vector>
#include <utility>

#include "algorithm/state_machine/moore_state.h"

namespace algorithm
{
    namespace state_machine
    {
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

    inline long size() const override
    {
        return m_edges.size();
    }

    MooreState<TIn>* transfer(TIn in) override
    {
        for (const auto& itr : m_edges)
        {
            if (itr.first == in)
            {
                itr.second->input(in);
                return itr.second;
            }
         }
         this->input(in);
         return this;
    }

    virtual void input(TIn in) {};
};
}
}
