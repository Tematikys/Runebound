#ifndef TOKENS_HPP_
#define TOKENS_HPP_

namespace runebound {
namespace token {

enum class TokenType { BATTLE, ADVENTURE, PLOT };

struct Token {
private:
    TokenType m_type;

public:
    explicit Token(TokenType type) : m_type(type) {
    }

    [[nodiscard]] TokenType get_type() const {
        return m_type;
    }
};
}  // namespace token
}  // namespace runebound
#endif  // TOKENS_HPP_
