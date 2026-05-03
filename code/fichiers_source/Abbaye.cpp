#include "Abbaye.hpp"
#include "Logger.hpp"

// constructeur
Abbaye::Abbaye(const std::string& nom, int id)
    : Batiment(nom, id)
{
    definirRessourceProduite("Manuscrits", 1);

}

void Abbaye::afficherInfos() const {
    std::string info = "=== " + getNom() + " ===\n";
    info += "Type: Abbaye\n";
    info += "Statut: " + getStatutString() + "\n";
    info += "Occupants: " + std::to_string(getNombreOccupants());

    Logger::log(LogCategory::SYSTEME, info);
}

std::string Abbaye::getType() const {
    return "Abbaye";
}

void Abbaye::produire() {
    Logger::log(LogCategory::PRODUCTION, + " Les moines travaillent au scriptorium" );
    ajouterRessourceStockpile("Manuscrits", 1);
}

void Abbaye::consommer() {
    Logger::log(LogCategory::PRODUCTION, + " Les moines préparent du parchermin..." );
}

