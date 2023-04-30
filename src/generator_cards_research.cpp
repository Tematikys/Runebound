#include <fstream>
#include "card_research.hpp"
#include "nlohmann/json.hpp"

namespace runebound::generator {
namespace {
void write_to_file(
    const ::runebound::cards::CardResearch &card,
    const std::string &file
) {
    nlohmann::json json;
    std::ofstream out(file);
    to_json(json, card);
    out << json.dump(4);
}
}  // namespace

void generate_cards_research() {
    write_to_file(
        cards::CardResearch(
            "Prowess of the Dwarves", "Valley of Souls",
            {cards::CardResearch::Outcome(1, 0, 0, {map::TypeCell::HILLS}),
             cards::CardResearch::Outcome(
                 2, 0, 0, {map::TypeCell::HILLS, map::TypeCell::HILLS}
             ),
             cards::CardResearch::Outcome(
                 0, 0, 1, {map::TypeCell::HILLS, map::TypeCell::MOUNTAINS}
             )}
        ),
        "data/json/cards/cards_research/prowess_of_the_dwarves.json"
    );

}
}  // namespace runebound::generator