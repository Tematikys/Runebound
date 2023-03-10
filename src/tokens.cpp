#include "tokens.hpp"
#include "nlohmann/json.hpp"

namespace runebound {
namespace token {

nlohmann::json serialize_token(const Token& token) {
    nlohmann::json json_token;
    json_token["m_type"] = token.m_type;
    return json_token;
}

Token deserialize_token(const nlohmann::json& json) {
    Token result;
    result.m_type = json["m_type"];
    return result;
}

} // namespace token
} // namespace runebound