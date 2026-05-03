#include "Champ.hpp"
#include "Logger.hpp"

// constructeur
Champ::Champ(const std::string& nom, int id)
    : Batiment(nom, id)
{
    definirRessourceProduite("Blé", 10);
}

void Champ::afficherInfos() const {
    std::string info = "=== " + getNom() + " ===\n";
    info += "Type: Champ\n";
    info += "Statut: " + getStatutString() + "\n";
    info += "Occupants: " + std::to_string(getNombreOccupants());

    Logger::log(LogCategory::SYSTEME, info);
}

std::string Champ::getType() const {
    return "ChampBlé";
}

void Champ::produire() {
    Logger::log(LogCategory::PRODUCTION, + " Le champ produit du blé..." );
    ajouterRessourceStockpile("Blé", 10);
}

void Champ::consommer() {
    Logger::log(LogCategory::PRODUCTION, + " Le champ consomme de l'eau..." );
}

