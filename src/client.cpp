#include <algorithm>
#include <fstream>
#include <iostream>
#include <thread>
#include <utility>


#include "game.hpp"
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

struct user {
    user(tcp::socket s) : s(s){
      character=character();
    };
    runebound::character::Character* character;
    tcp::socket* s;
};


user &start(tcp::iostream &client_io) {
    client_io << "What is your name?\n";
    std::string username;
    client_io >> username;
    client_io << "Hi " << username << "\n";
    return ledger.get_or_create_user(username);
}

void transfer(
        tcp::iostream &client_io,
        bank::ledger &ledger,
        bank::user &user
) {
    std::string counterpatry;
    std::string comment;
    int amount = 0;
    client_io >> counterpatry;
    client_io >> amount;

    std::getline(client_io, comment);
    user.transfer(
            ledger.get_or_create_user(counterpatry), amount, comment.substr(1)
    );
    client_io << "OK\n";
}

// NOLINTNEXTLINE (readability-function-cognitive-complexity)
int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr
                << "Usage: "
                << std::string(argv[0])
                << " <port> \n";
        return 1;
    }

    bank::ledger ledger = {};
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(
                io_context,
                tcp::endpoint(
                        tcp::v4(),
                        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                        static_cast<short>(std::stoi(argv[1]))
                )
        );
        std::string filename =
                // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                argv[2];

        std::cout << "Listening at " << acceptor.local_endpoint() << "\n";

        while (true) {
            tcp::socket s = acceptor.accept();
            std::thread([s = std::move(s), &ledger]() mutable {
                tcp::endpoint remote = s.remote_endpoint();
                tcp::endpoint local = s.local_endpoint();
                std::cout << "Connected " << remote << " --> " << local << "\n";
                tcp::iostream client_io(std::move(s));

                bank::user &user = start(client_io, ledger);

                while (client_io) {
                    auto print_transactions =
                            [&](const std::vector<bank::transaction> &transactions,
                                int balance_xts) {
                                client_io << "CPTY\tBAL\tCOMM\n";
                                int number_of_transactions = 0;
                                client_io >> number_of_transactions;
                                for (size_t i = std::max(
                                        0, static_cast<int>(transactions.size()) -
                                           number_of_transactions

                                );
                                     i < transactions.size(); i++) {
                                    bank::transaction t = transactions[i];
                                    if (t.counterparty != nullptr) {
                                        client_io << t.counterparty->name() << "\t";
                                    } else {
                                        client_io << "-\t";
                                    }

                                    client_io << t.balance_delta_xts << "\t"
                                              << t.comment << '\n';
                                }
                                client_io << "===== BALANCE: " << balance_xts
                                          << " XTS =====\n";
                            };
                    try {
                        std::string command;
                        if (!(client_io >> command)) {
                            break;
                        }
                        if (command == "balance") {
                            client_io << user.balance_xts() << "\n";
                            continue;
                        }
                        if (command == "transactions") {
                            user.snapshot_transactions(print_transactions);
                            continue;
                        }
                        if (command == "monitor") {
                            bank::user_transactions_iterator monitor =
                                    user.snapshot_transactions(print_transactions);

                            while (true) {
                                bank::transaction t =
                                        monitor.wait_next_transaction();
                                client_io << t.counterparty->name() << "\t"
                                          << t.balance_delta_xts << "\t"
                                          << t.comment << '\n';
                            }
                        }
                        if (command == "transfer") {
                            transfer(client_io, ledger, user);
                            continue;
                        }
                        client_io << "Unknown command: '" << command << "'\n";
                    } catch (std::exception &e) {
                        client_io << e.what() << "\n";
                    }
                }
                std::cout << "Disconnected " << remote << " --> " << local
                          << "\n";
            }).detach();
        }

    } catch (...) {
    }
    return 0;
}