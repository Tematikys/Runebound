#include <iostream>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>


#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Session;

std::set <Session*> Sessions;
class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket) : socket_(std::move(socket))
    {
        Sessions.insert(this);
    }

    void Start()
    {
        DoRead([this](std::string data){
            // Handle received data
            std::cout << "Received: " << data << std::endl;

            // Echo back to client
            for (auto Session : Sessions){Session->DoWrite(data); }
        });
    }


private:
    void DoRead(std::function<void(std::string)> callback)
    {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                [this, self, callback](boost::system::error_code ec, std::size_t length)
                                {
                                    if (!ec)
                                    {
                                        callback(std::string(data_, length));
                                    } else { Sessions.erase(this);  std::cout<<"Disconected";}
                                });
    }


    void DoWrite(const std::string& message)
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(message),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                 {
                                     if (!ec)
                                     {
                                         DoRead([this](std::string data){
                                             // Handle received data
                                             std::cout << "Received: " << data << std::endl;

                                             // Echo back to client
                                             for (auto Session : Sessions){ Session->DoWrite(data);  }

                                         });
                                     } else { Sessions.erase(this);  std::cout<<"Disconected";}
                                 });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class Server
{
public:
    Server(boost::asio::io_context& io_context, short port)
            : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    {
        DoAccept();
    }

private:
    void DoAccept()
    {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket)
                               {
                                   if (!ec)
                                   {
                                       std::make_shared<Session>(std::move(socket))->Start();
                                   }

                                   DoAccept();
                               });
    }

    tcp::acceptor acceptor_;
};

int main()
{
    try
    {
        boost::asio::io_context io_context;
        Server server(io_context, 1234);
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
