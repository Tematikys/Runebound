#include <fstream>
#include "product.hpp"
#include "nlohmann/json.hpp"

namespace runebound::generator {
namespace {
void write_to_file(
    const ::runebound::trade::Product &product,
    const std::string &file
) {
    nlohmann::json json;
    std::ofstream out(file);
    to_json(json, product);
    out << json.dump(4);
}
}  // namespace

void generate_products() {
    write_to_file(
        trade::Product("Relics", 2, 5, map::SpecialTypeCell::SANCTUARY),
        "data/json/products/relics.json"
    );
}
}  // namespace runebound::generator