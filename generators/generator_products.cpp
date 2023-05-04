#include <fstream>
#include "product.hpp"
#include "nlohmann/json.hpp"
#include "generators.hpp"

namespace runebound::generator {

void generate_products() {
    write_to_file(
        trade::Product("Relics", 2, 5, map::SpecialTypeCell::SANCTUARY),
        "data/json/products/relics.json"
    );
    write_to_file(
        trade::Product("Artifacts", 0, 2, map::SpecialTypeCell::SANCTUARY),
        "data/json/products/artifacts.json"
    );
}
}  // namespace runebound::generator