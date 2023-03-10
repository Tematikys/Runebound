#ifndef TOKENS_HPP_
#define TOKENS_HPP_
#include "nlohmann/json_fwd.hpp"
#include "runebound_fwd.hpp"

namespace runebound {
namespace token {

void to_json(nlohmann::json &json, const Token& token);
void from_json(const nlohmann::json &json, Token& token);

enum class TokenType { BATTLE, ADVENTURE, PLOT };

struct Token {
private:
    TokenType m_type;

public:
    Token() = default;

    explicit Token(TokenType type) : m_type(type) {
    }

    [[nodiscard]] TokenType get_type() const {
        return m_type;
    }

    friend void to_json(nlohmann::json &json, const Token& token);
    friend void from_json(const nlohmann::json &json, Token& token);

};
}  // namespace token
}  // namespace runebound
#endif  // TOKENS_HPP_
