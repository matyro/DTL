/* list_test.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include "gtest/gtest.h"
#include "meta/compile_time_list.h"

namespace test
{
    namespace meta
    {
        using namespace ::meta::list;

        TEST(CompileTimeList, Construct)
        {
            static constexpr int t1 = 1, t2 = 2, t3 = 3, t4 = 4;

            constexpr const int* p1 = &t1;
            constexpr const int* p2 = &t2;
            constexpr const int* p3 = &t3;
            constexpr const int* p4 = &t4;

            constexpr auto tup1 = make_list(t1, t2, t3, t4);
            constexpr auto tup2 = make_list(p1, p2, p3, p4);

            constexpr const List<const int, 4> tup3(t1, t2, t3, t4);
            constexpr const List<const int*, 4> tup4(p1, p2, p3, p4);

            constexpr auto tup5 = make_list(1,2,3,4);
            constexpr const List<const int, 4> tup6(1, 2, 3, 4);


            (void)(tup1);
            (void)(tup2);
            (void)(tup5);
        }

        TEST(CompileTimeList, CopyConstruct)
        {
            constexpr auto tup1 = make_list(1,2,3,4);

            constexpr auto tup2 = List<int, 4>(tup1);
            constexpr auto tup3 = tup1;
        }

        TEST(CompileTimeList, OperatorEqual)
        {
            constexpr auto tup1 = make_list(1,2,3,4);
            constexpr auto tup2 = make_list(1,2,3,4);
            constexpr auto tup3 = make_list(1.0,2.0,3.0,4.0);

            constexpr auto tup4 = make_list(1,2,3,5);
            constexpr auto tup5 = make_list(1,2,3,4,5);

            ASSERT_TRUE( tup1 == tup1);
            ASSERT_TRUE( tup1 == tup2);
            ASSERT_TRUE( tup1 == tup3);

            ASSERT_FALSE( tup1 == tup4);
            ASSERT_FALSE( tup1 == tup5);
        }

        TEST(CompileTimeList, CastTest)
        {
            List<int, 4> tup(1,2,3,4);

            List<float, 4> tupFloat = tup;

            ASSERT_EQ( tupFloat.get<0>(), 1.0f);
            ASSERT_EQ( tupFloat.get<1>(), 2.0f);
            ASSERT_EQ( tupFloat.get<2>(), 3.0f);
            ASSERT_EQ( tupFloat.get<3>(), 4.0f);
        }

        TEST(CompileTimeList, Get)
        {
            constexpr auto tup = make_list(1,2,3,4);

            ASSERT_EQ(tup.get(), 1);

            ASSERT_EQ( tup.get<0>(), 1);
            ASSERT_EQ( tup.get<1>(), 2);
            ASSERT_EQ( tup.get<2>(), 3);
            ASSERT_EQ( tup.get<3>(), 4);

            ASSERT_EQ( tup.get(0), 1);
            ASSERT_EQ( tup.get(1), 2);
            ASSERT_EQ( tup.get(2), 3);
            ASSERT_EQ( tup.get(3), 4);
        }

        TEST(CompileTimeList, Foldl)
        {
            auto concatStr = [](const int a, const std::string b) -> std::string
            {
                std::stringstream sstr;
                sstr << a;
                sstr << ",";
                sstr << b;
                return sstr.str();
            };

            constexpr auto tup = make_list(1,2,3,4);
            ASSERT_STREQ( tup.foldl(concatStr, std::string("-")).c_str(), "-1,2,3,4");
        }

        TEST(CompileTimeList, Foldr)
        {
            auto concatStr = [](const int a, std::string b)
            {
                std::stringstream sstr;
                sstr << a;
                sstr << ",";
                sstr << b;
                return sstr.str();
            };

            constexpr auto tup = make_list(1,2,3,4);
            ASSERT_STREQ( tup.foldr(concatStr, std::string("-")).c_str(), "-1,2,3,4");
        }

        TEST(CompileTimeList, Map)
        {
            auto m = [](const int a) -> int
            {
                return (a) + 5;
            };
            constexpr auto tup = make_list(1,2,3,4);
            auto newTup = tup.map<decltype(m), int>(m);

            ASSERT_EQ( newTup.get<0>(), 6);
            ASSERT_EQ( newTup.get<1>(), 7);
            ASSERT_EQ( newTup.get<2>(), 8);
            ASSERT_EQ( newTup.get<3>(), 9);
        }

        TEST(CompileTimeList, Zip)
        {
            ASSERT_TRUE(false);
        }

        TEST(CompileTimeList, Concat)
        {
            constexpr auto tup1 = make_list(1,2,3,4);
            constexpr auto tup2 = make_list(5,6,7,8);

            tup1.concat(tup2);
            (tup1 + tup2);
        }

        TEST(CompileTimeList, SubSection)
        {
            constexpr auto tup = make_list(1,2,3,4);
            tup.subSection<0, 2>();
        }

        TEST(CompileTimeList, Size)
        {
            constexpr auto tup = make_list(1,2,3,4);
            tup.size();
        }
    }
}
