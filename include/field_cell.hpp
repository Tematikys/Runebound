#ifndef FIELD_CELL_HPP_
#define FIELD_CELL_HPP_

#include <memory>
#include <utility>
#include <vector>
#include "runebound_fwd.hpp"

namespace runebound {
namespace field {

    const int COUNT_NEIGHBOUR = 6;

    enum class TypeCell { WATER, FOREST, MOUNTAINS, HILLS, PLAIN };

    enum class Neighbour {
        TOPRIGHT,
        RIGHT,
        BOTTOMRIGHT,
        BOTTOMLEFT,
        LEFT,
        TOPLEFT
    };

    struct FieldCell {
    public:
        FieldCell(TypeCell type_cell) : m_type_cell(type_cell) {
            m_neighbours.resize(COUNT_NEIGHBOUR, nullptr);
            m_token = runebound::AdventureType::NOTHING;
            m_side_token = runebound::Side::FRONT;
        }

        void
        make_neighbour(std::shared_ptr<FieldCell> neighbour, Neighbour direction) {
            m_neighbours[static_cast<int>(direction)] = std::move(neighbour);
        }

        void make_token(runebound::AdventureType token) {
            m_token = token;
        }

        runebound::AdventureType get_token() const {
            return m_token;
        }

        void reverse_token() {
            m_side_token =
                static_cast<runebound::Side>(static_cast<int>(m_side_token) ^ 1);
        }

        runebound::Side get_side_token() const {
            return m_side_token;
        }

        TypeCell get_type_cell() const {
            return m_type_cell;
        }

    private:
        const TypeCell m_type_cell;
        runebound::AdventureType m_token;
        runebound::Side m_side_token;
        std::vector<std::shared_ptr<FieldCell>> m_neighbours;
    };

}  // namespace field
}  // namespace runebound
#endif  // FIELD_CELL_HPP_
