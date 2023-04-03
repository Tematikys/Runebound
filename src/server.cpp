#include <iostream>
#include <boost/asio.hpp>

#include <deque>
#include <set>
#include <memory>
#include <map>

#include "game.hpp"
#include "json.hpp"
#include "json_fwd.hpp"

using boost::asio::ip::tcp;
using json = nlohmann::json;


class Session;

runebound::game::Game test_game();

std::map<std::string, runebound::game::Game> games;
std::set<Session*> sessions;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket) : socket_(std::move(socket)) {}

    void start()
    {
        std::cout<<"Some connected\n";
        sessions.insert(this);
//        write("Hello from server\n");
        do_read();

    }

    void write(const std::string& message)
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(message),
                                 [this, self, message](boost::system::error_code ec, std::size_t length)
                                 {
                                     if (!ec)
                                     {
                                         std::cout << "Sent:{ \n " << message << "\n}\n";
                                     }
                                     else
                                     {
                                         sessions.erase(this);
                                         std::cout << "Disconnected" << std::endl;
                                     }
                                 });
    }

    tcp::socket& socket() { return socket_; }

private:
    void do_read()
    {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, 1024),
                                [this, self](boost::system::error_code ec,  std::size_t length)
                                {
                                    if (!ec)
                                    {
                                        std::string message(data_, length);
                                        std::cout << "Received: " << message;
                                        write("Message getted\n");
                                        json data=json::parse(message);
                                        std::cout<<data;

                                        if (data["action type"]=="reverse token") {
                                            std::cout<<data["x"]<<' '<<data["y"];
//                                            test_game.reverse_token(data["x"],data["y"]);
                                        }
                                        do_read();
                                    }
                                    else
                                    {
                                        sessions.erase(this);
                                        std::cout << "Disconnected" << std::endl;
                                    }
                                });
    }
    runebound::game::Game* game_=nullptr;
    tcp::socket socket_;
    char data_[1024];
};

class Server
{
public:
    Server(boost::asio::io_context& io_context, short port)
            : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    {
        std::cout<<"Server started\n";
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket)
                {
                    if (!ec)
                    {
                        std::make_shared<Session>(std::move(socket))->start();
                    }

                    do_accept();
                });
    }

    tcp::acceptor acceptor_;
};

int main()
{
    try
    {
        boost::asio::io_context io_context;
        Server server(io_context, 4444);
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}