#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

class Client {
public:
    Client(const std::string &host, const std::string &port)
            : io_context_(), socket_(io_context_) {
        tcp::resolver resolver(io_context_);
        tcp::resolver::results_type endpoints =
                resolver.resolve(host, port);
        boost::asio::connect(socket_, endpoints);
    }

    void start() {
        io_thread_ = std::thread([this]() {
            io_context_.run();
        });
    }

    std::string read() {
        char buffer[1024];
        std::size_t n = socket_.read_some(boost::asio::buffer(buffer, sizeof(buffer)));
        return std::string(buffer, n);
    }

    void read_to_out() {
        char buffer[1024];
        std::size_t n = socket_.read_some(boost::asio::buffer(buffer, sizeof(buffer)));
        std::string response(buffer, n);
        std::cout << "Received " << response.length() << " bytes:\n"
                << response << std::endl;
    }

    void write(const std::string &s) {
        boost::asio::write(socket_, boost::asio::buffer(s.data(), s.length()));
    }

private:
    boost::asio::io_context io_context_;
    tcp::socket socket_;
    std::thread io_thread_;
};


int main() {
    try {
        Client client("localhost", "1234");
        client.start();
        std::cout << "Connected\n";
        client.read_to_out();
        client.write("get games\n");
        client.read_to_out();
        client.write("add game Mygame\n");
        client.read_to_out();
        client.write("get games\n");
        client.read_to_out();
        client.write("enter game Mygame\n");
        client.read_to_out();



    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}