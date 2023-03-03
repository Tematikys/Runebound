#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using std::string;
using std::cout;
using std::endl;

string read_(ip::tcp::socket & socket) {
    boost::asio::streambuf buf;
    boost::asio::read_until( socket, buf, "\n" );
    return boost::asio::buffer_cast<const char*>(buf.data());
}
void send_(ip::tcp::socket & socket, const string& message) {
    boost::asio::write( socket, boost::asio::buffer(message) );
}

int main() {
    boost::asio::io_service io_service;
    ip::tcp::acceptor acceptor_(io_service, ip::tcp::endpoint(ip::tcp::v4(), 1234 ));
    ip::tcp::socket socket_(io_service);
    acceptor_.accept(socket_);
    cout <<  read_(socket_);
    send_(socket_, "Pong");
    return 0;
}