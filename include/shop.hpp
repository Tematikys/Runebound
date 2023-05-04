#ifndef SHOP_HPP_
#define SHOP_HPP_

#include "runebound_fwd.hpp"
#include "product.hpp"
#include <set>

namespace runebound::trade {

struct NoProductException : std::runtime_error {
    NoProductException() : std::runtime_error("This product is not in the shop") {}
};

struct Shop {
private:
    std::set <Product> m_products;
public:
    [[nodiscard]] std::set <Product> get_products() const {
        return m_products;
    }

    void add_product(const Product &product) {
        m_products.insert(product);
    }

    void erase_product(const Product &product) {
        if (m_products.count(product) == 0) {
            throw NoProductException();
        }
        m_products.erase(product);
    }
};
} // namespace runebound::trade
#endif SHOP_HPP_
