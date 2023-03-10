#include "tokens.hpp"
#include "nlohmann/json.hpp"

namespace runebound {
namespace token {

void to_json(nlohmann::json &json, const Token& token) {
    json["m_type"] = token.m_type;
}
void from_json(const nlohmann::json &json, Token& token) {
    token.m_type = json["m_type"];
}

} // namespace token
} // namespace runebound