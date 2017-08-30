/* lib.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"

#include "algorithm/moore_test.h"

#include "io/socket_test.h"
#include "io/dns_lookup_test.h"
#include "io/client_test.h"
#include "io/server_test.h"

#include "meta/list_test.h"
#include "meta/tuple_test.h"
#include "meta/is_callable_test.h"


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
