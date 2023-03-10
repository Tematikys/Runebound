#ifndef TOKENS_HPP_
#define TOKENS_HPP_
#include "nlohmann/json_fwd.hpp"
#include "runebound_fwd.hpp"

namespace runebound {
namespace token {

nlohmann::json serialize_token(const Token& token);
Token deserialize_token(const nlohmann::json& json);

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

    friend nlohmann::json serialize_token(const Token& token);

    friend Token deserialize_token(const nlohmann::json& json);

};
}  // namespace token
}  // namespace runebound
#endif  // TOKENS_HPP_
