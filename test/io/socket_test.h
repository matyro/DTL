/* SocketTest.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*    All rights reserved.
*
*   This software may be modified and distributed under the terms
*   of the LGPL license. See the LICENSE file for details.
*/


#include "algorithm/char_to_byte.h"
#include "io/network/socket.h"

#include "gtest/gtest.h"

#include <thread>
#include <chrono>


namespace test
{
    namespace network
    {

        class SocketTest : public ::testing::Test
        {
        public:

            static ::io::network::Socket* client;
            static ::io::network::Socket* server;
            static ::io::network::Socket* accept;

            SocketTest()
            {
                // initialization code here
            }

            static void SetUpTestCase()
            {
                client = new ::io::network::Socket();
                server = new ::io::network::Socket();
                accept = new ::io::network::Socket();
            }

            static void TearDownTestCase()
            {
                delete client;
                delete server;
                delete accept;
            }

            void SetUp( )
            {
                // code here will execute just before every test
            }

            void TearDown( )
            {
                // code here will be called just after the test completes
                // ok to through exceptions from here if need be
            }

            ~SocketTest()
            {
                // cleanup any pending stuff, but no exceptions allowed
            }
        };

        ::io::network::Socket* SocketTest::client = nullptr;
        ::io::network::Socket* SocketTest::server = nullptr;
        ::io::network::Socket* SocketTest::accept = nullptr;



        TEST_F(SocketTest, Server)
        {
            ASSERT_TRUE( server->create() );
            ASSERT_TRUE( server->bind(13337) );
            ASSERT_TRUE( server->listen() );
        }


        TEST_F(SocketTest, Client)
        {
            ASSERT_TRUE( client->create() );
        }

        TEST_F(SocketTest, Connect)
        {
            ASSERT_TRUE( client->connect("127.0.0.1", 13337) );

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            ASSERT_TRUE( server->accept(*accept) );
        }


        TEST_F(SocketTest, Message)
        {
            auto fServer = [](){
                auto a = ::io::network::Socket::waitFor_read({ &(*SocketTest::accept) }, 0);

                //ASSERT_EQUAL(a.size(), 1) << "no message recieved";

                auto msg = a[0]->recv(true);

                //ASSERT_EQUAL(msg.size(), 5) << "message not the correct size";

                //ASSERT_STREQ(msg.c_str(), "Test\0") << "message content not correct";

                accept->send(::algorithm::char_to_byte("Test2\0").data(), 6);
            };

            std::thread thread(fServer);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            client->send(::algorithm::char_to_byte("Test\0").data(), 5);

            auto msg = client->recv(false);

            ASSERT_EQ(msg.size(), 6) << "message not the correct size";
            ASSERT_STREQ(reinterpret_cast<const char*>( msg.data()  ), "Test2\0") << "message content not correct";

            thread.join();

        }



        TEST_F(SocketTest, Close)
        {
            server->close();
            client->close();
        }



        /*

        bool test_send()
        {
            auto fServer = [](){
                std::cout << "Start server" << std::endl;

                ::io::network::Socket server;

                std::cout << "Server create"<< std::endl;
                server.create();

                std::cout << "Server bind" << std::endl;
                server.bind(13338);

                std::cout << "Server listen" << std::endl;
                server.listen();

                ::io::network::Socket acc;
                server.accept(acc);

                std::cout << "Wait for client message:" << std::endl;
                auto a = acc.waitFor_read({ &acc }, 0);
                std::cout << "Get client message!" << std::endl;

                if (a.size() != 1)
                {
                    std::cerr << "Wait does not work correctly!" << std::endl;
                }

                auto msg = a[0]->recv(true);

                if (msg.size() != 5)
                {
                    std::cerr << "Error - Message size is not correct (Server)!" << std::endl;
                    std::cerr << "Its: " << msg.size() << "  Should: 5" << std::endl;
                }

                if (memcmp(msg.data(), "Test\0", 5) != 0)
                {
                    std::cerr << "Error - Message is not correct (Server)!" << std::endl;
                    std::cerr << "Should: " << "Test\0" << "  Is: ";
                    for (auto itr : msg)
                    {
                        std::cerr << itr;
                    }
                    std::cerr << std::endl;
                }

                acc.send(::algorithm::char_to_byte("Test2\0").data(), 6);

            };

            std::thread thread(fServer);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            std::cout << "Start client" << std::endl;
            ::io::network::Socket client;
            client.create();
            client.connect("127.0.0.1", 13338);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            std::cout << "Send" << std::endl;
            client.send(::algorithm::char_to_byte("Test\0").data(), 5);


            std::cout << "Recv" << std::endl;
            auto msg = client.recv(false);

            std::cout << "Check" << std::endl;
            if (msg.size() != 6)
            {
                std::cerr << "Error - Message size is not correct (Client)!" << std::endl;
                return false;
            }

            if (std::memcmp(msg.data(), "Test2\0", 6) != 0)
            {
                std::cerr << "Error - Message is not correct (Client)!" << std::endl;
                std::cerr << "Should: " << "Test\0" << "  Is: ";
                for (auto itr : msg)
                std::cerr << itr;
                std::cerr << std::endl;

                return false;
            }

            std::cout << "Join" << std::endl;
            thread.join();

            return true;
        }
*/
    }
}
