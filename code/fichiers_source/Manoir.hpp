#ifndef MANOIR_HPP
#define MANOIR_HPP

#include "Batiment.hpp"
#include <string>

class Manoir : public Batiment {
public:
    // constructeur
    Manoir(const std::string& nom, int id);

    void afficherInfos() const override;
    std::string getType() const override;
    void produire() override;
    void consommer() override;
private:

};
#endif
