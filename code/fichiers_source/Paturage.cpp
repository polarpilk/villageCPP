#include "Paturage.hpp"
#include "Logger.hpp"

// constructeur
Paturage::Paturage(const std::string& nom, int id)
    : Batiment(nom, id)
{
    definirRessourceProduite("Moutons", 1);
}

void Paturage::afficherInfos() const {
    std::string info = "=== " + getNom() + " ===\n";
    info += "Type: Pâturage\n";
    info += "Statut: " + getStatutString() + "\n";
    info += "Occupants: " + std::to_string(getNombreOccupants());

    Logger::log(LogCategory::SYSTEME, info);
}

std::string Paturage::getType() const {
    return "Pâturage";
}

void Paturage::produire() {
    Logger::log(LogCategory::PRODUCTION, + " Le pâturage produit des moutons..." );
    ajouterRessourceStockpile("Moutons", 3);
}

void Paturage::consommer() {
    Logger::log(LogCategory::PRODUCTION, + " Le pâturage consomme de l'herbe ..." );
}

