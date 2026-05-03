#include "Manoir.hpp"
#include "Logger.hpp"

// constructeur
Manoir::Manoir(const std::string& nom, int id)
    : Batiment(nom, id)
{
    definirRessourceRequise("Or", 5);
    definirRessourceProduite("Prestige", 2);
}

void Manoir::afficherInfos() const {
    std::string info = "=== " + getNom() + " ===\n";
    info += "Type: Manoir\n";
    info += "Statut: " + getStatutString() + "\n";
    info += "Occupants: " + std::to_string(getNombreOccupants());

    Logger::log(LogCategory::SYSTEME, info);
}

std::string Manoir::getType() const {
    return "Manoir";
}

void Manoir::produire() {
    Logger::log(LogCategory::PRODUCTION, "Le manoir génère du prestige...");
    ajouterRessourceStockpile("Prestige", 2);
}

void Manoir::consommer() {
    Logger::log(LogCategory::PRODUCTION, "Le manoir consomme de l'or...");
    consommerRessource("Or", 5);
}

