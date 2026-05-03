#ifndef MINE_HPP
#define MINE_HPP

#include "Batiment.hpp"
#include <string>

class Mine : public Batiment {

public:
    Mine(const std::string& nom, int id);

    void afficherInfos() const override;
    std::string getType() const override;
    void produire() override;
    void consommer() override;
};

#endif
