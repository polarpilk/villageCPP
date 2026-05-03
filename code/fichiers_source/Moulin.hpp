#pragma once
#include "Batiment.hpp"

class Moulin : public Batiment {
public:
    Moulin(const std::string& nom, int id);

    void afficherInfos() const override;
    std::string getType() const override;
    void produire() override;
    void consommer() override;
};
