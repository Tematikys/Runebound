#include <fstream>
#include "character.hpp"
#include "nlohmann/json.hpp"

namespace runebound::generator {
namespace {
void write_to_file(
    const ::runebound::character::Character &character,
    const std::string &file
) {
    nlohmann::json json;
    std::ofstream out(file);
    to_json(json, character);
    out << json.dump(4);
}
}  // namespace

void generate_characters() {
    ::runebound::character::Character master_thorn(
        2, 8, ::runebound::Point(5, 8), 4, 3, "Master Thorn",
        {::runebound::fight::FightToken(
             fight::HandFightTokens::PHYSICAL_DAMAGE, 0, 2,
             fight::HandFightTokens::MAGICAL_DAMAGE, 0, 1
         ),
         ::runebound::fight::FightToken(
             fight::HandFightTokens::DOUBLING, 0, 1,
             fight::HandFightTokens::MAGICAL_DAMAGE, 1, 1
         ),
         ::runebound::fight::FightToken(
             fight::HandFightTokens::DEXTERITY, 1, 1,
             fight::HandFightTokens::MAGICAL_DAMAGE, 0, 1
         )}
    );
    write_to_file(master_thorn, "data/json/characters/master_thorn.json");

    ::runebound::character::Character elder_mok(
        2, 9, ::runebound::Point(13, 6), 3, 3, "Elder Mok",
        {::runebound::fight::FightToken(
             fight::HandFightTokens::MAGICAL_DAMAGE, 0, 2,
             fight::HandFightTokens::DOUBLING, 0, 1
         ),
         ::runebound::fight::FightToken(
             fight::HandFightTokens::DEXTERITY, 1, 1,
             fight::HandFightTokens::SHIELD, 0, 1
         ),
         ::runebound::fight::FightToken(
             fight::HandFightTokens::MAGICAL_DAMAGE, 1, 1,
             fight::HandFightTokens::PHYSICAL_DAMAGE, 0, 1
         )}
    );
    write_to_file(elder_mok, "data/json/characters/elder_mok.json");

    ::runebound::character::Character lissa(
        1, 9, ::runebound::Point(8, 13), 3, 3, "Lissa",
        {::runebound::fight::FightToken(
             fight::HandFightTokens::PHYSICAL_DAMAGE, 0, 2,
             fight::HandFightTokens::SHIELD, 0, 1
         ),
         ::runebound::fight::FightToken(
             fight::HandFightTokens::MAGICAL_DAMAGE, 1, 1,
             fight::HandFightTokens::DEXTERITY, 0, 1
         ),
         ::runebound::fight::FightToken(
             fight::HandFightTokens::DEXTERITY, 1, 1,
             fight::HandFightTokens::PHYSICAL_DAMAGE, 1, 1
         )}
    );
    write_to_file(lissa, "data/json/characters/lissa.json");

    ::runebound::character::Character laurel_from_bloodwood(
        2, 8, ::runebound::Point(3, 1), 4, 3, "Laurel from Bloodwood",
        {::runebound::fight::FightToken(
             fight::HandFightTokens::PHYSICAL_DAMAGE, 0, 2,
             fight::HandFightTokens::MAGICAL_DAMAGE, 0, 1
         ),
         ::runebound::fight::FightToken(
             fight::HandFightTokens::SHIELD, 0, 1,
             fight::HandFightTokens::PHYSICAL_DAMAGE, 1, 1
         ),
         ::runebound::fight::FightToken(
             fight::HandFightTokens::PHYSICAL_DAMAGE, 1, 1,
             fight::HandFightTokens::DEXTERITY, 0, 1
         )}
    );
    write_to_file(
        laurel_from_bloodwood, "data/json/characters/laurel_from_bloodwood.json"
    );

    ::runebound::character::Character lord_hawthorne(
        3, 9, ::runebound::Point(13, 12), 3, 3, "Lord Hawthorne",
        {::runebound::fight::FightToken(
             fight::HandFightTokens::PHYSICAL_DAMAGE, 0, 2,
             fight::HandFightTokens::SHIELD, 0, 1
         ),
         ::runebound::fight::FightToken(
             fight::HandFightTokens::PHYSICAL_DAMAGE, 1, 1,
             fight::HandFightTokens::DEXTERITY, 0, 1
         ),
         ::runebound::fight::FightToken(
             fight::HandFightTokens::PHYSICAL_DAMAGE, 1, 1,
             fight::HandFightTokens::NOTHING, 0, 1
         )}
    );
    write_to_file(lord_hawthorne, "data/json/characters/lord_hawthorne.json");

    ::runebound::character::Character corbin(
        2, 10, ::runebound::Point(11, 6), 3, 3, "Corbin",
        {::runebound::fight::FightToken(
             fight::HandFightTokens::PHYSICAL_DAMAGE, 0, 2,
             fight::HandFightTokens::MAGICAL_DAMAGE, 0, 1
         ),
         ::runebound::fight::FightToken(
             fight::HandFightTokens::PHYSICAL_DAMAGE, 1, 1,
             fight::HandFightTokens::SHIELD, 0, 1
         ),
         ::runebound::fight::FightToken(
             fight::HandFightTokens::DEXTERITY, 0, 1,
             fight::HandFightTokens::PHYSICAL_DAMAGE, 0, 1
         )}
    );
    write_to_file(corbin, "data/json/characters/corbin.json");
}
}  // namespace runebound::generator