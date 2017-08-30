#pragma once

#include <string>

#include "gtest/gtest.h"
#include "algorithm/state_machine/moore_state.h"
#include "algorithm/state_machine/moore_statemachine.h"
#include "algorithm/state_machine/moore_state_static.h"

namespace test
{
    namespace algorithm
    {
        using namespace ::algorithm::state_machine;

        TEST(MooreStateMachine, State)
        {
            struct test1 : public MooreStateStatic<char,0>
            {
            public:
                void operator()(void){ return; }
            };


            test1 t1;
            ASSERT_EQ(t1.size(), 0);

            struct test2 : public MooreStateStatic<std::string,1>
            {
            public:
                void operator()(void){ return; }
            };

            test2 t2;
            ASSERT_EQ(t2.size(), 1);

        }

        TEST(MooreStateMachine, Construct)
        {
            struct test1 : public MooreStateStatic<char,0>
            {
            public:
                void operator()(void){ return; }
            };

            struct test2 : public MooreStateStatic<std::string,0>
            {
            public:
                void operator()(void){ return; }
            };

            MooreStateMachine<char, test1> t1( test1() );
            MooreStateMachine<char, test1, test1> t1( test1(), test1() );

            MooreStateMachine<std::string, test2> t2( test2() );


        }


        struct test1 : public MooreStateStatic<char,1>
        {
        public:
            std::vector<char> vec;

            void operator()(void){ return; }

            void input(char in) override { vec.push_back(in); };
        };

        struct test2 : public MooreStateStatic<char,1>
        {
        public:
            std::vector<char> vec;

            void operator()(void){ return; }

            void input(char in) override { vec.push_back(in); };
        };

        TEST(MooreStateMachine, StaticMachineTest)
        {


            auto m1 = MooreStateMachine<char, test1, test2>( test1(), test2() );

            test1* t1;
            test2* t2;

            std::tie(t1, t2) = m1.getStates();

            t1->setEdge<0>('1', t2);
            t2->setEdge<0>('2', t1);

            m1.call('T');
            m1.call('1');
            m1.call('1');
            m1.call('2');

            t1->vec.push_back('\0');
            t2->vec.push_back('\0');

            ASSERT_STREQ(t1->vec.data(), "T2");
            ASSERT_STREQ(t2->vec.data(), "11");
        }



    }
}
