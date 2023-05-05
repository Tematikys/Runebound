#include <fstream>
#include "generators.hpp"
#include "nlohmann/json.hpp"
#include "product.hpp"

namespace runebound::generator {

void generate_products() {
    write_to_file(
        trade::Product("Relics", 2, 5, map::SpecialTypeCell::SANCTUARY),
        "data/json/products/relics.json"
    );
    write_to_file(
        trade::Product("Artifacts", 0, 2, map::SpecialTypeCell::SANCTUARY),
        "data/json/products/artifacts.json"
    );
    write_to_file(
        trade::Product("Provision", 0, 2, map::SpecialTypeCell::SETTLEMENT),
        "data/json/products/provision.json"
    );
    write_to_file(
        trade::Product("Ammunition", 0, 2, map::SpecialTypeCell::FORTRESS),
        "data/json/products/ammunition.json"
    );
    write_to_file(
        trade::Product("Values", 2, 5, map::SpecialTypeCell::SETTLEMENT),
        "data/json/products/values.json"
    );
    write_to_file(
        trade::Product("Armament", 2, 5, map::SpecialTypeCell::FORTRESS),
        "data/json/products/armament.json"
    );

    write_to_file(
        trade::Product(
            "Belt of the Alchemist", 2,
            {{Characteristic::BODY, 1},
             {Characteristic::INTELLIGENCE, 1},
             {Characteristic::SPIRIT, 1}},
            0, 0, 0
        ),
        "data/json/products/belt_alchemist.json"
    );
    write_to_file(
        trade::Product("Suma", 2, {}, 0, 0, 1), "data/json/products/suma.json"
    );
    write_to_file(
        trade::Product("Power Ring", 3, {}, 1, 0, 0),
        "data/json/products/power_ring.json"
    );
    write_to_file(
        trade::Product("Winged boots", 3, {}, 0, 1, 0),
        "data/json/products/winged_boots.json"
    );
    write_to_file(
        trade::Product(
            "Elven armor", 8,
            {{Characteristic::BODY, 2},
             {Characteristic::INTELLIGENCE, 2},
             {Characteristic::SPIRIT, 2}},
            1, 0, 1
        ),
        "data/json/products/elven_armor.json"
    );
    write_to_file(
        trade::Product("Thoroughbred stallion", 7, {}, 0, 2, 0),
        "data/json/products/thoroughbred_stallion.json"
    );
    write_to_file(
        trade::Product(
            "Lush camisole", 3,
            {{Characteristic::BODY, 1},
             {Characteristic::INTELLIGENCE, 1},
             {Characteristic::SPIRIT, 1}},
            0, 0, 0
        ),
        "data/json/products/lush_camisole.json"
    );

    write_to_file(
        trade::Product(
            "Leather armor", 3,
            fight::FightToken(
                fight::HandFightTokens::SHIELD, false, 1,
                fight::HandFightTokens::NOTHING, true, 1
            ),
            {}, 0, 0, 0
        ),
        "data/json/products/leather_armor.json"
    );
    write_to_file(
        trade::Product(
            "Trader's Attire", 4,
            fight::FightToken(
                fight::HandFightTokens::DEXTERITY, false, 1,
                fight::HandFightTokens::NOTHING, false, 1
            ),
            {}, 0, 1, 0
        ),
        "data/json/products/trader_attire.json"
    );
    write_to_file(
        trade::Product(
            "Short bow", 6,
            fight::FightToken(
                fight::HandFightTokens::PHYSICAL_DAMAGE, true, 1,
                fight::HandFightTokens::SHIELD, false, 1
            ),
            {}, 0, 0, 0
        ),
        "data/json/products/short_bow.json"
    );
    write_to_file(
        trade::Product(
            "Valorous Blade", 12,
            fight::FightToken(
                fight::HandFightTokens::PHYSICAL_DAMAGE, false, 3,
                fight::HandFightTokens::DEXTERITY, false, 1
            ),
            {}, 0, 0, 0
        ),
        "data/json/products/valorous_blade.json"
    );
    write_to_file(
        trade::Product(
            "Serrated Ax", 5,
            fight::FightToken(
                fight::HandFightTokens::PHYSICAL_DAMAGE, false, 2,
                fight::HandFightTokens::NOTHING, false, 1
            ),
            {{Characteristic::BODY, 1}}, 0, 0, 0
        ),
        "data/json/products/serrated_ax.json"
    );
    write_to_file(
        trade::Product(
            "Chain mail", 4,
            fight::FightToken(
                fight::HandFightTokens::SHIELD, false, 1,
                fight::HandFightTokens::NOTHING, false, 1
            ),
            {}, 1, 0, 0
        ),
        "data/json/products/chain_mail.json"
    );
    write_to_file(
        trade::Product(
            "Potions set", 4,
            fight::FightToken(
                fight::HandFightTokens::MAGICAL_DAMAGE, false, 1,
                fight::HandFightTokens::DEXTERITY, false, 1
            ),
            {}, 0, 0, 0
        ),
        "data/json/products/potions_set.json"
    );
    write_to_file(
        trade::Product(
            "Well-aimed bow", 12,
            fight::FightToken(
                fight::HandFightTokens::PHYSICAL_DAMAGE, true, 2,
                fight::HandFightTokens::SHIELD, false, 1
            ),
            {}, 0, 0, 0
        ),
        "data/json/products/well_aimed_bow.json"
    );
    write_to_file(
        trade::Product(
            "Ancient sword", 2,
            fight::FightToken(
                fight::HandFightTokens::PHYSICAL_DAMAGE, false, 1,
                fight::HandFightTokens::NOTHING, false, 1
            ),
            {}, 0, 0, 0
        ),
        "data/json/products/ancient_sword.json"
    );
    write_to_file(
        trade::Product(
            "Clawed Glove", 6,
            fight::FightToken(
                fight::HandFightTokens::PHYSICAL_DAMAGE, false, 1,
                fight::HandFightTokens::DEXTERITY, false, 1
            ),
            {{Characteristic::BODY, 1}}, 0, 0, 0
        ),
        "data/json/products/clawed_glove.json"
    );
    write_to_file(
        trade::Product(
            "Mana Armor", 11,
            fight::FightToken(
                fight::HandFightTokens::SHIELD, false, 2,
                fight::HandFightTokens::MAGICAL_DAMAGE, false, 1
            ),
            {{Characteristic::SPIRIT, 1}}, 2, 0, 0
        ),
        "data/json/products/mana_armor.json"
    );
    write_to_file(
        trade::Product(
            "Spiked armor", 9,
            fight::FightToken(
                fight::HandFightTokens::SHIELD, false, 2,
                fight::HandFightTokens::PHYSICAL_DAMAGE, false, 1
            ),
            {{Characteristic::BODY, 1}}, 1, 0, 0
        ),
        "data/json/products/spiked_armor.json"
    );
}
}  // namespace runebound::generator