#include <iostream>
#include <boost/asio.hpp>

#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>

using boost::asio::ip::tcp;
using json = nlohmann::json;

class Client {
public:
    Client(boost::asio::io_context &io_context, const std::string &host, int port, std::string user_name)
            : socket_(io_context), m_user_name(user_name) {
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
                                        json answer = json::parse(stringView);

                                        if (answer["change type"] == "game names") {
                                            game_names = answer["game names"];
                                        }
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
                                     } else {
                                         std::cerr << "Write failed: " << ec.message() << std::endl;
                                         socket_.close();
                                     }
                                 });
    }

    void reverse_token(int x, int y) {
        json data;
        data["action type"] = "reverse token";
        data["x"] = x;
        data["y"] = y;
        do_write(data.dump());
    }

    void add_game(std::string game_name) {
        json data;
        data["action type"] = "add game";
        data["game name"] = game_name;
        do_write(data.dump());
    }

    void join_game(std::string game_name) {
        json data;
        data["action type"] = "join game";
        data["game name"] = game_name;
        data["user name"] = m_user_name;
        do_write(data.dump());
    }


public:
    std::string m_user_name;
    std::vector<std::string> game_names;
private:
    tcp::socket socket_;
    char read_buffer_[1024];
};


int main() {
    std::string user_name;
    std::cout<<"Write your name:\n";
    std::cin>>user_name;

    boost::asio::io_context io_context;
    auto work_guard = boost::asio::make_work_guard(io_context);
    Client client(io_context, "127.0.0.1", 4444, user_name);
    try {
        //test commands begin
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        io_context.poll();

        std::cout<<"List of games:\n";
        for (auto e : client.game_names){
            std::cout<<e<<'\n';
        }
        std::cout<<'\n';

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        io_context.poll();

        client.add_game("My game");

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        io_context.poll();



        std::cout<<"List of games:\n";
        for (auto e : client.game_names){
            std::cout<<e<<'\n';
        }
        std::cout<<'\n';
        client.join_game("My game");
        // test commands end

        while (!io_context.stopped()) {

            //commands

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            io_context.poll();
        }
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }


    return 0;
}