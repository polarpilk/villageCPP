#include "Boulangerie.hpp"
#include "Logger.hpp"

// constructeur
Boulangerie::Boulangerie(const std::string& nom, int id)
    : Batiment(nom, id)
{
    definirRessourceRequise("Farine", 10);
    definirRessourceProduite("Pain", 5);
}

void Boulangerie::afficherInfos() const {
    std::string info = "=== " + getNom() + " ===\n";
    info += "Type: Boulangerie\n";
    info += "Statut: " + getStatutString() + "\n";
    info += "Occupants: " + std::to_string(getNombreOccupants());

    Logger::log(LogCategory::SYSTEME, info);
}

std::string Boulangerie::getType() const {
    return "Boulangerie";
}

void Boulangerie::produire() {
    Logger::log(LogCategory::PRODUCTION, + " La boulangerie cuit du pain..." );
    ajouterRessourceStockpile("Pain", 5);
}

void Boulangerie::consommer() {
    Logger::log(LogCategory::PRODUCTION, + " La boulangerie consomme de la farine.." );
    consommerRessource("Farine", 10);
}


