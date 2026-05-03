#include "Taverne.hpp"
#include "Logger.hpp"

// constructeur
Taverne::Taverne(const std::string& nom, int id)
    : Batiment(nom, id)
{
    definirRessourceProduite("Bière", 10);
    definirRessourceRequise("Blé", 30);
}

void Taverne::afficherInfos() const {
    std::string info = "=== " + getNom() + " ===\n";
    info += "Type: Taverne\n";
    info += "Statut: " + getStatutString() + "\n";
    info += "Occupants: " + std::to_string(getNombreOccupants());

    Logger::log(LogCategory::SYSTEME, info);
}

std::string Taverne::getType() const {
    return "Taverne";
}

void Taverne::produire() {
    Logger::log(LogCategory::PRODUCTION, "La taverne brasse de la bière...");
    ajouterRessourceStockpile("Bière", 10);
}

void Taverne::consommer() {
    Logger::log(LogCategory::PRODUCTION, "La taverne consomme du blé...");
    consommerRessource("Blé", 30);
}


