#include <iostream>
#include <boost/asio.hpp>
#include <chrono>
#include <thread>
#include "json.hpp"
#include "json_fwd.hpp"

using boost::asio::ip::tcp;
using json = nlohmann::json;

class Client {
public:
    Client(boost::asio::io_context &io_context, const std::string &host, int port)
            : socket_(io_context) {
        tcp::resolver resolver(io_context);
        auto endpoints = tcp::endpoint(boost::asio::ip::address::from_string(host), port);
        socket_.async_connect(endpoints,
                              [this](boost::system::error_code ec) {
                                  if (!ec) {
                                      do_read();
                                  } else {
                                      std::cerr << "Connect failed: " << ec.message() << std::endl;
                                  }
                              });
    }

    void do_read() {
        socket_.async_read_some(boost::asio::buffer(read_buffer_, 1024),
                                [this](boost::system::error_code ec, std::size_t length) {
                                    if (!ec) {
                                        std::string_view stringView(read_buffer_, length);
                                        std::cout << stringView << std::endl;
                                        do_read();
                                    } else {
                                        std::cerr << "Read failed: " << ec.message() << std::endl;
                                        socket_.close();
                                    }
                                });
    }

    void do_write(std::string str) {
        socket_.async_write_some(boost::asio::buffer(str),
                                 [this, str](boost::system::error_code ec, std::size_t length) {
                                     if (!ec) {
                                         std::cout << "Sent: " << str << std::endl;
                                     } else {
                                         std::cerr << "Write failed: " << ec.message() << std::endl;
                                         socket_.close();
                                     }
                                 });
    }

private:
    tcp::socket socket_;
    char read_buffer_[1024];
};

int main() {
    boost::asio::io_context io_context;
    auto work_guard = boost::asio::make_work_guard(io_context);
    Client client(io_context, "127.0.0.1", 4444);
    while (!io_context.stopped()) {
        try {
            json data;
            std::string str;
            std::cin >> str;
            std::cout<<str;
            if (str == "reverse_token") {
                std::cout<<"ok";
                int x, y;
                std::cin >> x >> y;

                data["action type"] = "reverse token";
                data["x"] = x;
                data["y"] = y;
                client.do_write(data.dump());
            }
//            if (str == "add game") {
//                data["add game"] = "reverse token";
//            }
//            if (str == "get games") {
//
//            }
//            if (str == "connect") {
//
//            }


            std::this_thread::sleep_for(std::chrono::milliseconds (100));
            io_context.poll();
        }
        catch (std::exception &e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }
    return 0;
}