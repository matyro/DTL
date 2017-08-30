#pragma once

#include <array>
#include <utility>

#include "algorithm/state_machine/moore_state.h"

namespace algorithm
{
    namespace state_machine
    {

template<class TIn, long TEdges>
class MooreStateStatic : public MooreState<TIn>
{
    private:

    std::array< std::pair<TIn, MooreState<TIn>* >, TEdges> m_edges;

    public:

    template<class ... TArgs>
    MooreStateStatic(TArgs ... args)
    : m_edges( args... )
    {}

    template<long TEdge>
    void setEdge(TIn i, MooreState<TIn>* const e)
    {
    	static_assert(TEdge < TEdges, "Edge is greater then the number of specified edges");
        m_edges[TEdge] = std::make_pair( i, e );
    }

    inline long size() const override
    {
        return TEdges;
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

    virtual void input(TIn in) { ((void)(in)); };
};

}
}
