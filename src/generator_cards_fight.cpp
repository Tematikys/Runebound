#include "card_fight.hpp"

namespace runebound::generator {
namespace {
void write_to_file(
    const ::runebound::cards::CardFight &card,
    const std::string &file
) {
    nlohmann::json json;
    std::ofstream out(file);
    to_json(json, card);
    out << json;
}
}  // namespace

void generate_cards_fight() {
    write_to_file(
        cards::CardFight(6, "Witch", 2),
        "../data/json/cards/cards_fight/witch.json"
    );
    write_to_file(
        cards::CardFight(7, "Terrifying", 3),
        "../data/json/cards/cards_fight/terrifying.json"
    );
    write_to_file(
        cards::CardFight(7, "Ettin", 3),
        "../data/json/cards/cards_fight/ettin.json"
    );
    write_to_file(
        cards::CardFight(8, "Ogre", 3),
        "../data/json/cards/cards_fight/ogre.json"
    );
    write_to_file(
        cards::CardFight(5, "Deep Elf", 2),
        "../data/json/cards/cards_fight/deep_elf.json"
    );
    write_to_file(
        cards::CardFight(3, "A horde of the Dead", 2),
        "../data/json/cards/cards_fight/horde_dead.json"
    );
    write_to_file(
        cards::CardFight(5, "The Rebels", 1),
        "../data/json/cards/cards_fight/the_rebels.json"
    );
    write_to_file(
        cards::CardFight(6, "Death Knight", 3),
        "../data/json/cards/cards_fight/death_knight.json"
    );
    write_to_file(
        cards::CardFight(6, "The Rogue Orc", 2),
        "../data/json/cards/cards_fight/the_rogue_ork.json"
    );
    write_to_file(
        cards::CardFight(5, "Razorwing", 2),
        "../data/json/cards/cards_fight/razorwing.json"
    );
    write_to_file(
        cards::CardFight(6, "The Beastman", 2),
        "../data/json/cards/cards_fight/the_beastman.json"
    );
    write_to_file(
        cards::CardFight(5, "The Great Dragon", 2),
        "../data/json/cards/cards_fight/the_great_dragon.json"
    );
    write_to_file(
        cards::CardFight(6, "Moon Cat", 2),
        "../data/json/cards/cards_fight/moon_cat.json"
    );
    write_to_file(
        cards::CardFight(6, "Dragon Hybrid", 2),
        "../data/json/cards/cards_fight/dragon_hybrid.json"
    );
    write_to_file(
        cards::CardFight(8, "The Restless Serpent", 3),
        "../data/json/cards/cards_fight/the_restless_serpent.json"
    );
}
}  // namespace runebound::generator