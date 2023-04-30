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
    write_to_file(
        cards::CardResearch(
            "Narrow Ledge", "Madness Pass",
            {cards::CardResearch::Outcome(1, 0, 0, {map::TypeCell::PLAIN, map::TypeCell::MOUNTAINS}),
             cards::CardResearch::Outcome(
                 2, 0, 0, {map::TypeCell::HILLS, map::TypeCell::MOUNTAINS}
             ),
             cards::CardResearch::Outcome(
                 4, 0, 0, {map::TypeCell::PLAIN, map::TypeCell::MOUNTAINS, map::TypeCell::MOUNTAINS}
             )}
        ),
        "data/json/cards/cards_research/narrow_ledge.json"
    );
    write_to_file(
        cards::CardResearch(
            "Winding Path", "Green Forest",
            {cards::CardResearch::Outcome(1, 0, 0, {map::TypeCell::FOREST}),
             cards::CardResearch::Outcome(
                 0, 0, 1, {map::TypeCell::FOREST, map::TypeCell::WATER}
             ),
             cards::CardResearch::Outcome(
                 3, 0, 0, {map::TypeCell::WATER, map::TypeCell::WATER, map::TypeCell::FOREST}
             )}
        ),
        "data/json/cards/cards_research/winding_path.json"
    );
    write_to_file(
        cards::CardResearch(
            "Goblin Raids", "Howling Dol",
            {cards::CardResearch::Outcome(1, 0, 0, {map::TypeCell::HILLS}),
             cards::CardResearch::Outcome(
                 0, 2, 0, {map::TypeCell::HILLS, map::TypeCell::MOUNTAINS}
             ),
             cards::CardResearch::Outcome(
                 3, 0, 0, {map::TypeCell::HILLS, map::TypeCell::HILLS, map::TypeCell::WATER}
             )}
        ),
        "data/json/cards/cards_research/goblin_raids.json"
    );
    write_to_file(
        cards::CardResearch(
            "Spring Festival", "Bright dol",
            {cards::CardResearch::Outcome(0, 0, 0, {map::TypeCell::PLAIN}),
             cards::CardResearch::Outcome(
                 0, 0, 1, {map::TypeCell::PLAIN, map::TypeCell::PLAIN, map::TypeCell::WATER}
             ),
             cards::CardResearch::Outcome(
                 2, 0, 0, {map::TypeCell::PLAIN, map::TypeCell::HILLS}
             )}
        ),
        "data/json/cards/cards_research/spring_festival.json"
    );
    write_to_file(
        cards::CardResearch(
            "Orc Renegades", "Fort Rodrik",
            {cards::CardResearch::Outcome(1, 0, 0, {map::TypeCell::HILLS}),
             cards::CardResearch::Outcome(
                 2, 0, 0, {map::TypeCell::HILLS, map::TypeCell::FOREST}
             ),
             cards::CardResearch::Outcome(
                 0, 0, 1, {map::TypeCell::WATER, map::TypeCell::WATER}
             )}
        ),
        "data/json/cards/cards_research/orc_renegades.json"
    );
    write_to_file(
        cards::CardResearch(
            "Dragon Bones", "Cursed ruins",
            {cards::CardResearch::Outcome(1, 0, 0, {map::TypeCell::HILLS}),
             cards::CardResearch::Outcome(
                 0, 0, 1, {map::TypeCell::HILLS, map::TypeCell::HILLS}
             ),
             cards::CardResearch::Outcome(
                 0, 0, 2, {map::TypeCell::HILLS, map::TypeCell::HILLS, map::TypeCell::WATER}
             )}
        ),
        "data/json/cards/cards_research/dragon_bones.json"
    );
    write_to_file(
        cards::CardResearch(
            "Secrets of the swamps", "The Quagmire of the Black Wing",
            {cards::CardResearch::Outcome(0, 0, 0, {map::TypeCell::WATER}),
             cards::CardResearch::Outcome(
                 3, 0, 0, {map::TypeCell::WATER, map::TypeCell::MOUNTAINS}
             ),
             cards::CardResearch::Outcome(
                 0, 0, 1, {map::TypeCell::WATER, map::TypeCell::WATER, map::TypeCell::PLAIN}
             )}
        ),
        "data/json/cards/cards_research/secrets_of_the_swamps.json"
    );
    write_to_file(
        cards::CardResearch(
            "Moonlight", "Moonlight Swamp",
            {cards::CardResearch::Outcome(1, 0, 0, {map::TypeCell::WATER}),
             cards::CardResearch::Outcome(
                 0, 0, 1, {map::TypeCell::WATER, map::TypeCell::PLAIN}
             ),
             cards::CardResearch::Outcome(
                 0, 0, 2, {map::TypeCell::WATER, map::TypeCell::WATER}
             )}
        ),
        "data/json/cards/cards_research/moonlight.json"
    );
    write_to_file(
        cards::CardResearch(
            "Marvelous people", "Moonlight Swamp",
            {cards::CardResearch::Outcome(0, 5, 0, {map::TypeCell::FOREST}),
             cards::CardResearch::Outcome(
                 0, 0, 1, {map::TypeCell::FOREST, map::TypeCell::FOREST}
             ),
             cards::CardResearch::Outcome(
                 0, 0, 2, {map::TypeCell::FOREST, map::TypeCell::FOREST, map::TypeCell::FOREST}
             )}
        ),
        "data/json/cards/cards_research/marvelous_people.json"
    );
    write_to_file(
        cards::CardResearch(
            "Magical Storm", "The Fangs of the Mennara",
            {cards::CardResearch::Outcome(0, -2, 0, {map::TypeCell::FOREST}),
             cards::CardResearch::Outcome(
                 0, 0, 1, {map::TypeCell::FOREST, map::TypeCell::WATER}
             ),
             cards::CardResearch::Outcome(
                 4, 0, 0, {map::TypeCell::FOREST, map::TypeCell::MOUNTAINS, map::TypeCell::WATER}
             )}
        ),
        "data/json/cards/cards_research/magical_storm.json"
    );
    write_to_file(
        cards::CardResearch(
            "Prince Of Thieves", "Robber freemen",
            {cards::CardResearch::Outcome(1, 0, 0, {map::TypeCell::FOREST}),
             cards::CardResearch::Outcome(
                 0, 3, 0, {map::TypeCell::FOREST, map::TypeCell::WATER}
             ),
             cards::CardResearch::Outcome(
                 3, 0, 0, {map::TypeCell::FOREST, map::TypeCell::MOUNTAINS, map::TypeCell::WATER}
             )}
        ),
            "data/json/cards/cards_research/prince_of_thieves.json"
    );
    write_to_file(
        cards::CardResearch(
            "Blood Stone", "Suvar's Haven",
            {cards::CardResearch::Outcome(0, 0, 0, {map::TypeCell::WATER}),
             cards::CardResearch::Outcome(
                 0, 0, 1, {map::TypeCell::WATER, map::TypeCell::WATER}
             ),
             cards::CardResearch::Outcome(
                 0, 0, 2, {map::TypeCell::WATER, map::TypeCell::WATER, map::TypeCell::WATER}
             )}
        ),
        "data/json/cards/cards_research/blood_stone.json"
    );

    write_to_file(
        cards::CardResearch(
            "Love you", "Telis Castle",
            {cards::CardResearch::Outcome(2, 0, 0, {map::TypeCell::FOREST}),
             cards::CardResearch::Outcome(
                 0, 0, 1, {map::TypeCell::FOREST, map::TypeCell::WATER}
             ),
             cards::CardResearch::Outcome(
                 0, 5, 0, {map::TypeCell::PLAIN, map::TypeCell::FOREST}
             )}
        ),
        "data/json/cards/cards_research/love_you.json"
    );
    write_to_file(
        cards::CardResearch(
            "My Kitty", "Itraitova Forge",
            {cards::CardResearch::Outcome(1, 0, 0, {map::TypeCell::PLAIN}),
             cards::CardResearch::Outcome(
                 0, 3, 0, {map::TypeCell::FOREST, map::TypeCell::MOUNTAINS}
             ),
             cards::CardResearch::Outcome(
                 3, 0, 0, {map::TypeCell::WATER, map::TypeCell::MOUNTAINS, map::TypeCell::FOREST}
             )}
        ),
        "data/json/cards/cards_research/my_kitty.json"
    );



}
}  // namespace runebound::generator