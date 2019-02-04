//
// Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: WebSocket server, asynchronous
//{% comment %} set( Boost_USE_STATIC_LIBS        OFF )
// set( Boost_USE_MULTITHREADED      ON )
// set( Boost_USE_STATIC_RUNTIME     OFF )
// set( BOOST_ALL_DYN_LINK           ON ) {% endcomment %}


//------------------------------------------------------------------------------



#include "util.h"

#include "listener.h"
#include "session.h"

int main(){
    auto const address = net::ip::make_address("0.0.0.0");
    auto const port = static_cast<unsigned short>(std::atoi("8080"));
    auto const threads = std::max<int>(1, std::atoi("1"));

    // The io_context is required for all I/O
    net::io_context ioc{threads};

    // Create and launch a listening port
    std::make_shared<listener>(ioc, tcp::endpoint{address, port})->run();

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i){
        v.emplace_back([&ioc]{
            ioc.run();
        });
    }
    ioc.run();

    return EXIT_SUCCESS;
}