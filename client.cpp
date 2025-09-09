#include "message.hpp"
#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/impl/write.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/system/detail/error_code.hpp>
#include <future>
#include <iostream>
#include <istream>
#include <memory>
#include <string>




using boost::asio::ip::tcp;

void async_read(tcp::socket &socket) {
    auto buffer = std::make_shared<boost::asio::streambuf>();
    boost::asio::async_read_until(socket, *buffer, "\n", [&socket, buffer](boost::system::error_code ec, std::size_t Length){
        if(!ec) {
            std::istream is(buffer.get());
            std::string recieved;
            std::getline(is, recieved);
            std::cout<<"Server: "<<recieved<<std::endl;
            async_read(socket);
        }
    });

}


int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cerr << "Provide port too as a second argument" << std::endl;
        return 1;
    }
    boost::asio::io_context io_context;
    tcp::socket socket(io_context);
    tcp::resolver resolver(io_context);
    boost::asio::connect(socket, resolver.resolve("127.0.0.1", argv[1]));
    async_read(socket);
    std::thread t([&io_context, &socket](){
        while(true) {
            std::string data;
            std::cout << "Enter the Message: ";
            std::getline(std::cin, data);
            data+="\n";
            boost::asio::post(io_context, [&, data](){
                boost::asio::write(socket, boost::asio::buffer(data));
            });
        }
    });
    io_context.run();
    t.join();
    return 0;
}



// Purpose of io_context::post:
// The primary purpose of post is to safely enqueue a task for execution within the io_context's thread pool, ensuring that the handler is invoked only by a thread that is currently running io_context::run(), run_one(), poll(), or poll_one(). This is particularly useful for:
