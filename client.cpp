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
#include <fstream>

using boost::asio::ip::tcp;

// ------------------
// Local login system
// ------------------

void SignUp() {
    std::string UserName, Email, Password;
    std::cin.ignore();
    std::cout << "Enter UserName: ";
    std::getline(std::cin, UserName);
    std::cout << "Enter Email: ";
    std::getline(std::cin, Email);
    std::cout << "Enter Password: ";
    std::getline(std::cin, Password);

    std::ofstream file("loginData.txt", std::ios::app);
    file << UserName << " " << Email << " " << Password << std::endl;
    file.close();
    std::cout << "Signup successful!\n";
}

bool Login() {
    std::string searchName, searchPass;
    std::cin.ignore();
    std::cout << "Enter UserName: ";
    std::getline(std::cin, searchName);
    std::cout << "Enter Password: ";
    std::getline(std::cin, searchPass);

    std::ifstream file("loginData.txt");
    std::string UserName, Email, Password;
    while (file >> UserName >> Email >> Password) {
        if (UserName == searchName) {
            if (Password == searchPass) {
                std::cout << "Login successful!\n";
                return true;
            } else {
                std::cout << "Incorrect password!\n";
                return false;
            }
        }
    }
    std::cout << "User not found!\n";
    return false;
}

// ------------------

void async_read(tcp::socket &socket) {
    auto buffer = std::make_shared<boost::asio::streambuf>();
    boost::asio::async_read_until(socket, *buffer, "\n", [&socket, buffer](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            std::istream is(buffer.get());
            std::string received;
            std::getline(is, received);
            std::cout << "Server: " << received << std::endl;
            async_read(socket);
        }
    });
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Provide port too as a second argument" << std::endl;
        return 1;
    }

    // Signup or Login before running chat
    char choice;
    std::cout << "1 - Signup\n2 - Login\nEnter choice: ";
    std::cin >> choice;

    if (choice == '1') {
        SignUp();
    }

    bool loggedIn = false;
    while (!loggedIn) {
        std::cout << "Login to continue:\n";
        loggedIn = Login();
    }

    // Proceed with chat connection
    boost::asio::io_context io_context;
    tcp::socket socket(io_context);
    tcp::resolver resolver(io_context);
    boost::asio::connect(socket, resolver.resolve("127.0.0.1", argv[1]));

    async_read(socket);

    std::thread t([&io_context, &socket]() {
        while (true) {
            std::string data;
            std::cout << "Enter the Message: ";
            std::getline(std::cin, data);
            data += "\n";
            boost::asio::post(io_context, [&, data]() {
                boost::asio::write(socket, boost::asio::buffer(data));
            });
        }
    });

    io_context.run();
    t.join();

    return 0;
}
