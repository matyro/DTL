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
#include "meta/type_traits/is_callable.h"

#include <type_traits>

namespace test
{

	namespace meta
	{


        bool testFunction1(const float* const t)
		{
			(void)(t);
			return true;
		}
		bool testFunction2(const int* const t)
		{
			(void)(t);
			return true;
		}

		struct testFunctor1
		{
			bool operator()(const float* const t) const
			{
				(void)(t);
				return true;
			}
		};

		struct testFunctor2
		{
			bool operator()(const float* const t)
			{
				(void)(t);
				return true;
			}
		};

		struct testFunctor3
		{
			bool operator()(const int* const t) const
			{
				(void)(t);
				return true;
			}
		};

		struct testStruct1
		{
			bool print()
			{
				return true;
			}
		};

        TEST(CustomTypeTraits, IsCallable)
		{
			auto testLambda1 = [](const float* const t)->bool
			{	(void)(t);return true;};
			auto testLambda2 = [](const int* const t)->bool
			{	(void)(t);return true;};

			ASSERT_EQ( (::meta::is_callable<decltype(&testFunction1), bool, const float* const>::value), true);
			ASSERT_EQ( (::meta::is_callable<decltype(&testFunction2), bool, const float* const>::value), false);
			ASSERT_EQ( (::meta::is_callable<testFunctor1, bool, const float* const>::value), true);
			ASSERT_EQ( (::meta::is_callable<testFunctor2, bool, const float* const>::value), true);
			ASSERT_EQ( (::meta::is_callable<testFunctor3, bool, const float* const>::value), false);
			ASSERT_EQ( (::meta::is_callable<decltype(testLambda1), bool, const float* const>::value), true);
		    ASSERT_EQ( (::meta::is_callable<decltype(testLambda2), bool, const float* const>::value), false);

            const int testVariable1 = 0;
			ASSERT_EQ( (::meta::is_callable<decltype(testVariable1), bool, const float* const>::value), false);
			ASSERT_EQ( (::meta::is_callable<testStruct1, bool, const float* const>::value), false);

		}

	}
}
