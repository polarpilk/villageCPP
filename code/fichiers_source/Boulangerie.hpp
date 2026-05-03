#pragma once
#include "Batiment.hpp"

class Boulangerie : public Batiment {
public:
    Boulangerie(const std::string& nom, int id);

    void afficherInfos() const override;
    std::string getType() const override;
    void produire() override;
    void consommer() override;
};
