/* client_test.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/



#include "io/network/client.h"
#include "io/network/socket.h"

#include "algorithm/char_to_byte.h"

#include <thread>
#include <future>
#include <chrono>



namespace test
{

    namespace network
    {
        class NetworkClient : public ::testing::Test
        {
        public:

            static ::io::network::Client* client;
            static std::future<bool> trReturn;

            NetworkClient()
            {
                // initialization code here
            }

            static void SetUpTestCase()
            {
                client = new ::io::network::Client();

                trReturn = std::async(std::launch::async, []() -> bool {
                    ::io::network::Socket server;
                    server.create();

                    server.bind(9557);

                    server.listen();

                    ::io::network::Socket acc;
                    server.accept(acc);

                    auto a = ::io::network::Socket::waitFor_read({ &acc }, 1000000 );

                    if(a.size() == 0)
                    {
                    	return false;
                    }

                    auto msg = a[0]->recv(true);
                    acc.send(msg.data(), msg.size());

                    acc.close();
                    server.close();

                    return true;
                });
            }

            static void TearDownTestCase()
            {
            	delete client;
            }

            void SetUp( )
            {
            }

            void TearDown( )
            {
            }

            ~NetworkClient()
            {
                // cleanup any pending stuff, but no exceptions allowed
            }
        };

        ::io::network::Client* NetworkClient::client = nullptr;
        std::future<bool> NetworkClient::trReturn;


        TEST_F(NetworkClient, Connect)
        {
        	std::this_thread::sleep_for(std::chrono::milliseconds(100));

            ASSERT_TRUE( client->init("127.0.0.1", 9557) );
        }

        TEST_F(NetworkClient, SendRecv)
        {
            client->send( ::algorithm::char_to_byte("Test\0").data(), 5);
            auto data = client->recv();

            ASSERT_EQ(data.size(), 5);
            ASSERT_EQ(data[0], 'T');
            ASSERT_EQ(data[1], 'e');
            ASSERT_EQ(data[2], 's');
            ASSERT_EQ(data[3], 't');
            ASSERT_EQ(data[4], '\0');
        }

        TEST_F(NetworkClient, Close)
        {
        	ASSERT_TRUE(trReturn.get()) << "Control Server recieved wrong message";

            client->close();
        }


    }
}
