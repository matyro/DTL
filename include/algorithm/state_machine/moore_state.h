/* moore_state.h
* this file is part of DTL
*
* Copyright (C) <2017> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

namespace algorithm
{
    namespace state_machine
    {

template<class TIn>
class MooreState
{
private:

public:
    MooreState() { }
    virtual ~MooreState() { }

    /// Function to be called when this state switches to active
    virtual void operator()(void) = 0;

    /// Number of stored edges
    virtual inline long size() const = 0;

    /** Transfer to next state depending on the input
    *   This function can be modified by the user if a non static transfer function is desiered
    *   When you want to use the input in any way use the input function
    *   In normall cases unused input transfers into itself
    *   \return Returns the next state to work with. Nullptr leads to undefined behaviour
    */
    virtual MooreState<TIn>* transfer(TIn) = 0;

    /// This function is called with the input on the State after it was transfered
    virtual void input(TIn in) = 0;

};

}
}
