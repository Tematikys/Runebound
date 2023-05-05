#ifndef GENERATORS_HPP_
#define GENERATORS_HPP_

#include "nlohmann/json.hpp"
#include "runebound_fwd.hpp"

namespace runebound::generator {

template <typename T>
void write_to_file(const T &element, const std::string &file) {
    nlohmann::json json;
    std::ofstream out(file);
    to_json(json, element);
    out << json.dump(4);
}
}  // namespace runebound::generator

#endif  // GENERATORS_HPP_
