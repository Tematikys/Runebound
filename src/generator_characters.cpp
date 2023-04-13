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
    out << json;
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
    write_to_file(master_thorn, "../data/json/master_thorn.json");
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
    write_to_file(elder_mok, "../data/json/elder_mok.json");
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
    write_to_file(lissa, "../data/json/lissa.json");
}
}  // namespace runebound::generator