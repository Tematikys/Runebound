#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Client
{
public:
    Client(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
            : socket_(io_context)
    {
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(host, port);
        boost::asio::async_connect(socket_, endpoints,
                                   [this](boost::system::error_code ec, const tcp::endpoint&)
                                   {
                                       if (!ec)
                                       {
                                           do_read();
                                           do_write();
                                       }
                                       else
                                       {
                                           std::cerr << "Connect failed: " << ec.message() << std::endl;
                                       }
                                   });
    }

private:
    void do_read()
    {
        boost::asio::async_read(socket_, boost::asio::buffer(read_buffer_, max_length),
                                [this](boost::system::error_code ec, std::size_t length)
                                {
                                    if (!ec)
                                    {
                                        std::cout << "Received: " << read_buffer_ << std::endl;
                                        do_read();
                                    }
                                    else
                                    {
                                        std::cerr << "Read failed: " << ec.message() << std::endl;
                                        socket_.close();
                                    }
                                });
    }

    void do_write()
    {
        std::cout << "Enter message: ";
        std::cin.getline(write_buffer_, max_length);
        boost::asio::async_write(socket_, boost::asio::buffer(write_buffer_, std::strlen(write_buffer_)),
                                 [this](boost::system::error_code ec, std::size_t length)
                                 {
                                     if (!ec)
                                     {
                                         std::cout << "Sent: " << write_buffer_ << std::endl;
                                         do_write();
                                     }
                                     else
                                     {
                                         std::cerr << "Write failed: " << ec.message() << std::endl;
                                         socket_.close();
                                     }
                                 });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char read_buffer_[max_length];
    char write_buffer_[max_length];
};

int main()
{
    try
    {
        boost::asio::io_context io_context;
        Client client(io_context, "localhost", "4444");
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}