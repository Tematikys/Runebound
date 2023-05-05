#include <fstream>
#include "card_meeting.hpp"
#include "generators.hpp"
#include "nlohmann/json.hpp"

namespace runebound::generator {

void generate_cards_meeting() {
    write_to_file(
        cards::CardMeeting(
            "Coven of Necromancers", Characteristic::BODY, 2, -2, 0,
            Characteristic::SPIRIT, 1, 0, 0
        ),
        "data/json/cards/cards_meeting/coven_of_necromancers.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Significant other", Characteristic::SPIRIT, 0, -2, 1,
            Characteristic::INTELLIGENCE, 0, 0, 0
        ),
        "data/json/cards/cards_meeting/significant_other.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Ancient burials", Characteristic::BODY, 0, -2, 0,
            Characteristic::INTELLIGENCE, 0, 0, 0
        ),
        "data/json/cards/cards_meeting/ancient_burials.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Strength exercises", Characteristic::BODY, 1, -1, 0,
            Characteristic::SPIRIT, 1, 0, 0
        ),
        "data/json/cards/cards_meeting/strength_exercises.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Shadows of the past", Characteristic::INTELLIGENCE, 0, 0, 1,
            Characteristic::BODY, 2, 0, 0
        ),
        "data/json/cards/cards_meeting/shadows_of_the_past.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Dirty beggar", Characteristic::INTELLIGENCE, -1, 0, 0,
            Characteristic::SPIRIT, 0, -1, 1
        ),
        "data/json/cards/cards_meeting/dirty_beggar.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Hidden gift", Characteristic::SPIRIT, 0, 0, 0,
            Characteristic::INTELLIGENCE, 1, 0, 0
        ),
        "data/json/cards/cards_meeting/hidden_gift.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Harvest festival", Characteristic::BODY, 1, 0, 0,
            Characteristic::INTELLIGENCE, 1, 0, 0
        ),
        "data/json/cards/cards_meeting/harvest_festival.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Runesmith", Characteristic::INTELLIGENCE, 3, -1, 0,
            Characteristic::SPIRIT, 0, 0, 0
        ),
        "data/json/cards/cards_meeting/runesmith.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Desperate Peasant", Characteristic::BODY, 0, 0, 0,
            Characteristic::BODY, 1, -1, 0
        ),
        "data/json/cards/cards_meeting/desperate_peasant.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Wandering Trader", Characteristic::BODY, 2, 0, 0,
            Characteristic::SPIRIT, -1, 0, 0
        ),
        "data/json/cards/cards_meeting/wandering_trader.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Devastated village", Characteristic::INTELLIGENCE, 0, 0, 0,
            Characteristic::BODY, 1, 0, 0
        ),
        "data/json/cards/cards_meeting/devastated_village.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Smuggling", Characteristic::SPIRIT, 3, -3, 0,
            Characteristic::INTELLIGENCE, 1, 0, 0
        ),
        "data/json/cards/cards_meeting/smuggling.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Faster than the wind", Characteristic::BODY, 0, 0, 0,
            Characteristic::SPIRIT, 0, 0, 1
        ),
        "data/json/cards/cards_meeting/faster_than_the_wind.json"
    );

    write_to_file(
        cards::CardMeeting(
            "Legends of antiquity", Characteristic::INTELLIGENCE, 0, 0, 0,
            Characteristic::BODY, 1, 0, 0
        ),
        "data/json/cards/cards_meeting/legends_of_antiquity.json"
    );
}
}  // namespace runebound::generator