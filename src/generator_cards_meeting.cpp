#include "card_meeting.hpp"
#include <fstream>
#include "nlohmann/json.hpp"

namespace runebound::generator {
namespace {
void write_to_file(
    const ::runebound::cards::CardMeeting &card,
    const std::string &file
) {
    nlohmann::json json;
    std::ofstream out(file);
    to_json(json, card);
    out << json.dump(4);
}
}  // namespace

void generate_cards_meeting() {
    write_to_file(
        cards::CardMeeting("Coven of Necromancers", Characteristic::BODY, 2, -2,
                           Characteristic::SPIRIT, 1, 0),
        "data/json/cards/cards_meeting/coven_of_necromancers.json"
    );
}
}  // namespace runebound::generator