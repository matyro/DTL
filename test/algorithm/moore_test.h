#pragma once

#include "gtest/gtest.h"
#include "algorithm/state_machine/moore_state.h"
#include "algorithm/state_machine/moore_statemachine.h"

namespace test
{
    namespace algorithm
    {

        TEST(MooreStateMachine, Construct)
        {
            algorithm::state_machine::MooreStateMachine<char> t1;

            algorithm::state_machine::MooreStateMachine<std::string> t2;
        }



        TEST(MooreStateMachine, State)
        {

            struct test1 : algorithm::state_machine::MooreStateStatic<char,0>
            {
            public:
                void operator()(void)
                {
                    return;
                }
            }

            test1 t1;

        }
    }
}
