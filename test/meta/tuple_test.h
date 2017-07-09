/* tuple_test.h
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

#include "meta/tuple/tuple_algorithm.h"

#include <type_traits>



namespace test
{

    namespace meta
    {
        using namespace ::meta::tuple;

        TEST(Tuple, FilterType)
        {
            auto a1 = std::make_tuple('a', "bc", 1, 'b', 2, 2.3f);

            /// Test tuple correctness
            //char
            ASSERT_TRUE( (std::is_same<char, std::tuple_element<0, decltype(a1)>::type>::value) );

            //char[]
            ASSERT_FALSE( (std::is_convertible<std::tuple_element<1, decltype(a1)>::type, char>::value) );
            ASSERT_FALSE( (std::is_same<char, std::tuple_element<1, decltype(a1)>::type>::value) );

            //int
            ASSERT_TRUE( (std::is_convertible<std::tuple_element<2, decltype(a1)>::type, char>::value) );
            ASSERT_FALSE( (std::is_same<char, std::tuple_element<2, decltype(a1)>::type>::value) );

            //char
            ASSERT_TRUE( (std::is_same<char, std::tuple_element<3, decltype(a1)>::type>::value) );

            //int
            ASSERT_TRUE( (std::is_convertible<std::tuple_element<4, decltype(a1)>::type, char>::value) );
            ASSERT_FALSE( (std::is_same<char, std::tuple_element<4, decltype(a1)>::type>::value) );

            //float
            ASSERT_TRUE( (std::is_convertible<std::tuple_element<2, decltype(a1)>::type, char>::value) );   // Yes an implicit cast from float to char is possible ...
            ASSERT_FALSE( (std::is_same<char, std::tuple_element<5, decltype(a1)>::type>::value) );

            /// Check filter function
            auto b1 = filterTuple<char, decltype(a1) >::filter_same(a1);
            ASSERT_EQ( std::tuple_size<decltype(b1)>::value, 2);
            ASSERT_TRUE( (std::is_same<char, std::tuple_element<0, decltype(b1)>::type>::value) );
            ASSERT_TRUE( (std::is_same<char, std::tuple_element<1, decltype(b1)>::type>::value) );

            ASSERT_EQ( std::get<0>(b1), 'a' );
            ASSERT_EQ( std::get<1>(b1), 'b' );
        }

        TEST(Tuple, FilterEmpty)
        {
            auto a2 = std::make_tuple();

            ASSERT_EQ( (filterTuple<char, decltype(a2) >::filter_same(a2)), std::tuple<>() );
        }





        const std::string l_fold(const std::string str, const char c)
        {            
            std::stringstream sstr;
            sstr << str;
            sstr << c;
            return sstr.str();
        }


        TEST(Tuple, Foldl)
        {
            auto a1 = std::make_tuple('a', 'b', 'c', 'd', 'e');
            ASSERT_STREQ( (foldTuple::foldl<decltype(l_fold), std::string, decltype(a1)>(l_fold, a1,  std::string("-")).c_str() ), "-abcde" );
        }

        TEST(Tuple, FoldlLambda)
        {
            auto Lambda = [](const std::string str, const char c) -> const std::string{
                std::stringstream sstr;
                sstr << str;
                sstr << c;
                return sstr.str();
            };

            auto a1 = std::make_tuple('a', 'b', 'c', 'd', 'e');
            ASSERT_STREQ( (foldTuple::foldl<decltype(l_fold), std::string, decltype(a1)>(Lambda, a1,  std::string("-")).c_str() ), "-abcde" );
        }

        TEST(Tuple, FoldlEmpty)
        {
            auto a2 = std::make_tuple();
            ASSERT_STREQ( (foldTuple::foldl<decltype(l_fold), std::string, decltype(a2)>(l_fold, a2, std::string("-")).c_str() ), "-");
        }

        const std::string r_fold(const char c, const std::string str)
        {
            std::stringstream sstr;
            sstr << str;
            sstr << c;
            return sstr.str();
        }

        TEST(Tuple, Foldr)
        {
            auto a1 = std::make_tuple('a', 'b', 'c', 'd', 'e');
            ASSERT_STREQ( (foldTuple::foldr<decltype(r_fold), std::string, decltype(a1)>(r_fold, a1, std::string("-")).c_str() ), "-edcba");
        }

        TEST(Tuple, FoldrLambda)
        {
            auto Lambda = [](const char c, const std::string str) -> const std::string{
                std::stringstream sstr;
                sstr << str;
                sstr << c;
                return sstr.str();
            };
            auto a1 = std::make_tuple('a', 'b', 'c', 'd', 'e');
            ASSERT_STREQ( (foldTuple::foldr<decltype(r_fold), std::string, decltype(a1)>(Lambda, a1, std::string("-")).c_str() ), "-edcba");
        }

        TEST(Tuple, FoldrEmpty)
        {
            auto a2 = std::make_tuple();
            ASSERT_STREQ( (foldTuple::foldr<decltype(r_fold), std::string, decltype(a2)>(r_fold, a2, std::string("-")).c_str() ), "-");
        }




        int map(const int c) { return c - 3; }

        TEST(Tuple, MapFunction)
        {
            auto in = std::make_tuple(4, 5, 6, 7, 8);

            auto out = mapTuple::map<decltype(map), decltype(in)>(map, in);

            ASSERT_EQ( std::tuple_size<decltype(out)>::value, 5);

            ASSERT_EQ( std::get<0>(out), 1);
            ASSERT_EQ( std::get<1>(out), 2);
            ASSERT_EQ( std::get<2>(out), 3);
            ASSERT_EQ( std::get<3>(out), 4);
            ASSERT_EQ( std::get<4>(out), 5);
        }

        TEST(Tuple, MapLambda)
        {
            auto in = std::make_tuple(4, 5, 6, 7, 8);

            auto mapLambda = [](const int c){return c - 3;};
            auto out = mapTuple::map<decltype(map), decltype(in)>(mapLambda, in) ;

            ASSERT_EQ( std::tuple_size<decltype(out)>::value, 5);

            ASSERT_EQ( std::get<0>(out), 1);
            ASSERT_EQ( std::get<1>(out), 2);
            ASSERT_EQ( std::get<2>(out), 3);
            ASSERT_EQ( std::get<3>(out), 4);
            ASSERT_EQ( std::get<4>(out), 5);
        }

        TEST(Tuple, Print)
        {
            auto in = std::make_tuple(4, 5, 6, 7, 8);

            std::stringstream sstr;
            sstr << in;
            ASSERT_STREQ(sstr.str().c_str(), "(4,5,6,7,8)");
            ASSERT_STREQ(print(in).c_str(), "(4,5,6,7,8)");
        }

        TEST(Tuple, ForEach)
        {
            auto in = std::make_tuple(1, 2, 3, 4, 5);
            int sum = 0;
            auto Lambda = [&sum](int data){sum += data;};

            for_each(in, Lambda);
            ASSERT_EQ(sum, 15);
        }

    }
}
