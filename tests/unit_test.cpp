/* lib.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include <iostream>
#include <string>


#include "test/lib/data/particle_test.h"

#include "test/lib/io/socket_test.h"
#include "test/lib/io/dns_lookup_test.h"
#include "test/lib/io/client_test.h"
#include "test/lib/io/server_test.h"

#include "test/lib/meta/list_test.h"
#include "test/lib/meta/tuple_test.h"
#include "test/lib/meta/is_callable_test.h"


int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
