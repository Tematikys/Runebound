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
    write_to_file(
        trade::Product("Provision", 0, 2, map::SpecialTypeCell::SETTLEMENT),
        "data/json/products/provision.json"
    );
    write_to_file(
        trade::Product("Ammunition", 0, 2, map::SpecialTypeCell::FORTRESS),
        "data/json/products/ammunition.json"
    );
    write_to_file(
        trade::Product("Values", 2, 5, map::SpecialTypeCell::SETTLEMENT),
        "data/json/products/values.json"
    );
    write_to_file(
        trade::Product("Armament", 2, 5, map::SpecialTypeCell::FORTRESS),
        "data/json/products/armament.json"
    );
}
}  // namespace runebound::generator