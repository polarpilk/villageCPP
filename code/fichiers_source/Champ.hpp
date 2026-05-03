#ifndef CHAMP_HPP
#define CHAMP_HPP

#include "Batiment.hpp"
#include <string>

class Champ : public Batiment {
public:
    Champ(const std::string& nom, int id);

    void afficherInfos() const override;
    std::string getType() const override;
    void produire() override;
    void consommer() override;
};

#endif
