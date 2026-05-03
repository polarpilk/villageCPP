#include "Moulin.hpp"
#include "Logger.hpp"

// constructeur
Moulin::Moulin(const std::string& nom, int id)
    : Batiment(nom, id)
{
    definirRessourceRequise("Blé", 20);
    definirRessourceProduite("Farine", 5);
}

void Moulin::afficherInfos() const {
    std::string info = "=== " + getNom() + " ===\n";
    info += "Type: Moulin\n";
    info += "Statut: " + getStatutString() + "\n";
    info += "Occupants: " + std::to_string(getNombreOccupants());

    Logger::log(LogCategory::SYSTEME, info);
}

std::string Moulin::getType() const {
    return "Moulin";
}

void Moulin::produire() {
    Logger::log(LogCategory::PRODUCTION, + " Le moulin produit de la farine..." );
    ajouterRessourceStockpile("Farine", 5);
}

void Moulin::consommer() {
    Logger::log(LogCategory::PRODUCTION, + " Le moulin consomme du blé..." );
    consommerRessource("Ble", 20);
}


