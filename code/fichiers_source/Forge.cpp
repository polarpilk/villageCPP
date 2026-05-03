#include "Forge.hpp"
#include "Logger.hpp"
#include <iostream>


// constructeur
Forge::Forge(const std::string& nom, int id)
    : Batiment(nom, id)
{
    definirRessourceProduite("Épées", 2);
    definirRessourceRequise("Acier", 20);
}

void Forge::afficherInfos() const {
    std::string info = "=== " + getNom() + " ===\n";
    info += "Type: Forge\n";
    info += "Statut: " + getStatutString() + "\n";
    info += "Occupants: " + std::to_string(getNombreOccupants());

    Logger::log(LogCategory::SYSTEME, info);
}

std::string Forge::getType() const {
    return "Forge";
}

void Forge::produire() {
    Logger::log(LogCategory::PRODUCTION, + " La forge produit des épées..." );
    ajouterRessourceStockpile("Épées", 3);
}

void Forge::consommer() {
    Logger::log(LogCategory::PRODUCTION, + " La forge consomme de l'acier ..." );
    consommerRessource("Acier", 20);
}


