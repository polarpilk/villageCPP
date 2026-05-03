#ifndef TAVERNE_HPP
#define TAVERNE_HPP

#include "Batiment.hpp"
#include <string>

class Taverne : public Batiment {
public:
    Taverne(const std::string& nom, int id);

    void afficherInfos() const override;
    std::string getType() const override;
    void produire() override;
    void consommer() override;
};

#endif
