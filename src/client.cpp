#include <iostream>
#include <boost/asio.hpp>
#include <chrono>
#include <thread>

using boost::asio::ip::tcp;

class Client {
public:
    Client(boost::asio::io_context &io_context, const std::string &host, int port)
            : socket_(io_context) {
        tcp::resolver resolver(io_context);
        auto endpoints = tcp::endpoint( boost::asio::ip::address::from_string(host), port );
        socket_.async_connect( endpoints,
                                   [this](boost::system::error_code ec) {
                                       if (!ec) {
                                           do_read();
                                       } else {
                                           std::cerr << "Connect failed: " << ec.message() << std::endl;
                                       }
                                   });
    }

    void do_read() {
        socket_.async_read_some( boost::asio::buffer(read_buffer_, max_length),
                                [this](boost::system::error_code ec, std::size_t length) {
                                    if (!ec) {
                                        std::string_view stringView(read_buffer_, length);
                                        std::cout << "Received: " << stringView << std::endl;
                                        do_read();
                                    } else {
                                        std::cerr << "Read failed: " << ec.message() << std::endl;
                                        socket_.close();
                                    }
                                });
    }

    void do_write(std::string str) {
        socket_.async_write_some( boost::asio::buffer(str),
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
    enum {
        max_length = 1024
    };
    char read_buffer_[max_length];
//    char write_buffer_[max_length];
};

int main() {
    boost::asio::io_context io_context;
    auto work_guard = boost::asio::make_work_guard(io_context);
    Client client(io_context, "127.0.0.1", 4444);
    while (!io_context.stopped()) {
        try {
            std::string str;
            std::cin>>str;
            client.do_write(str);
//            std::this_thread::sleep_for(std::chrono::milliseconds (100));
            io_context.poll();
        }
        catch (std::exception &e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }
    return 0;
}