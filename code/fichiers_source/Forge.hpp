#ifndef FORGE_HPP
#define FORGE_HPP

#include "Batiment.hpp"
#include <string>

class Forge : public Batiment {
public:
    // constructeur
    Forge(const std::string& nom, int id);

    void afficherInfos() const override;
    std::string getType() const override;
    void produire() override;
    void consommer() override;
private:
};

#endif
