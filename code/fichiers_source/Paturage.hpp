#ifndef PATURAGE_H
#define PATURAGE_H

#include "Batiment.hpp"
#include <string>

class Paturage : public Batiment {
public:
    // constructeur
    Paturage(const std::string& nom, int id);

    void afficherInfos() const override;
    std::string getType() const override;
    void produire() override;
    void consommer() override;
private:

};

#endif
