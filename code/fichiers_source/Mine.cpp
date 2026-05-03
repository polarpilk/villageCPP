#include "Mine.hpp"
#include "Logger.hpp"

// constructeur
Mine::Mine(const std::string& nom, int id)
    : Batiment(nom, id)
{
    definirRessourceProduite("Acier", 5);
}

void Mine::afficherInfos() const {
    std::string info = "=== " + getNom() + " ===\n";
    info += "Type: Mine\n";
    info += "Statut: " + getStatutString() + "\n";
    info += "Occupants: " + std::to_string(getNombreOccupants());

    Logger::log(LogCategory::SYSTEME, info);
}

std::string Mine::getType() const {
    return "Mine";
}

void Mine::produire() {
    Logger::log(LogCategory::PRODUCTION, + " La mine produit de l'acier..." );
    ajouterRessourceStockpile("Acier", 5);
}

void Mine::consommer() {
    Logger::log(LogCategory::PRODUCTION, + " Les mineurs reçoivent des outils..." );
}

