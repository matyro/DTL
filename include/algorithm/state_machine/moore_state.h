#pragma once

template<class TIn>
class MooreState
{
private:

public:
    MooreState() { }
    virtual ~MooreState() { }
    virtual void operator()(void) = 0;

    virtual MooreState<TIn>* transfer(TIn) = 0;

};
