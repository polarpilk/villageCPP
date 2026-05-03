#ifndef ABBAYE_HPP
#define ABBAYE_HPP

#include "Batiment.hpp"
#include <string>

class Abbaye : public Batiment {
public:
    // constructeur
    Abbaye(const std::string& nom, int id);

    void afficherInfos() const override;
    std::string getType() const override;
    void produire() override;
    void consommer() override;
private:

};

#endif
