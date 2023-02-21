#ifndef FIELD_CELL_HPP_
#define FIELD_CELL_HPP_

#include <utility>
#include <vector>
#include <memory>

namespace runebound::field {

enum class TypeCell { Joker, Water, Forest, Mountains, Hills, Plain };

enum class Neighbour {
    TopRight,
    Right,
    BottomRight,
    BottomLeft,
    Left,
    TopLeft
};

struct FieldCell {
public:
    FieldCell(TypeCell type_cell) : m_type_cell(type_cell) {
        m_neighbours.resize(COUNT_NEIGHBOUR, nullptr);
    }

    void make_neighbour(std::shared_ptr <FieldCell> neighbour, Neighbour direction) {
        m_neighbours[static_cast<int>(direction)] = std::move(neighbour);
    }

private:
    const int COUNT_NEIGHBOUR = 6;
    const TypeCell m_type_cell;
    std::vector<std::shared_ptr<FieldCell>> m_neighbours;
};

}  // namespace runebound::field

#endif  // FIELD_CELL_HPP_
