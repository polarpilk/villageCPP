#include "Batiment.hpp"
#include "Villageois.hpp"
#include "Ressource.hpp"
#include "Logger.hpp"
#include <algorithm>

// definition des fonctions

// constructeur
Batiment::Batiment(const std::string& nom, int id)
    : nom(nom), statut(StatutBatiment::INACTIF), actif(true),
    id(id) {

    Logger::log(LogCategory::SYSTEME, "Batiment '" + nom + "' créé");
}

// destructeur
Batiment::~Batiment() {
    // liberer les ressources du stockpile
    for (auto& [nomRes, ressource] : stockpile) {
        delete ressource;
    }
    stockpile.clear();

    // ne pas delete occupantsActuels
    // (geres par Village.cpp)
    Logger::log(LogCategory::SYSTEME, "Batiment '" + nom + "' détruit");
}

// getters

std::string Batiment::getNom() const {
    return nom;
}

StatutBatiment Batiment::getStatut() const {
    return statut;
}

std::string Batiment::getStatutString() const {
    switch (statut) {
    case StatutBatiment::INACTIF:
        return "Inactif";
    case StatutBatiment::PRODUIT:
        return "Produit";
    case StatutBatiment::ENATTENTE:
        return "En attente";
    case StatutBatiment::STOCKAGEPLEIN:
        return "Stockage plein";
    default:
        return "Inconnu";
    }
}

bool Batiment::estActif() const {
    return actif;
}

const std::vector<Villageois*>& Batiment::getOccupants() const {
    return occupantsActuels;
}

int Batiment::getNombreOccupants() const {
    return occupantsActuels.size();
}

bool Batiment::estVide() const {
    return occupantsActuels.empty();
}

const std::map<std::string, Ressource*>& Batiment::getStockpile() const {
    return stockpile;
}

int Batiment::getQuantiteRessource(const std::string& nomRessource) const {
    auto it = stockpile.find(nomRessource);
    if (it != stockpile.end()) {
        return it->second->getQuantite();
    }
    return 0;
}

const std::map<std::string, int>& Batiment::getRessourcesRequises() const {
    return ressourcesRequises;
}

const std::map<std::string, int>& Batiment::getRessourcesProduites() const {
    return ressourcesProduites;
}

bool Batiment::peutProduire() const {
    // verifier que toutes les ressources requises sont disponibles
    for (const auto& [nomRes, quantiteRequise] : ressourcesRequises) {
        if (getQuantiteRessource(nomRes) < quantiteRequise) {
            return false;  // pas assez de cette ressource, on produit pas
        }
    }
    return true;  // toutes les ressources sont disponibles, on produit
}


// setters

void Batiment::setNom(const std::string& n) {
    nom = n;
}

void Batiment::setStatut(StatutBatiment s) {
    statut = s;
}

void Batiment::setActif(bool a) {
    actif = a;
    if (!a) {
        Logger::log(LogCategory::SYSTEME, nom + " est maintenant inactif");
    }
}

// methodes

bool Batiment::ajouterOccupant(Villageois* v) {
    if (!v) {
        Logger::log(LogCategory::DEBUG, "Erreur: tentative d'ajouter un villageois nul");
        return false;
    }

    // verifier s'il n'est pas dejà present
    if (contientOccupant(v)) {
        Logger::log(LogCategory::VILLAGEOIS, v->getNom() + " est déja dans " + nom);
        return false;
    }

    occupantsActuels.push_back(v);
    Logger::log(LogCategory::VILLAGEOIS, v->getNom() + " entre dans " + nom);
    return true;
}

bool Batiment::retirerOccupant(Villageois* v) {
    if (!v) return false;

    auto it = std::find(occupantsActuels.begin(), occupantsActuels.end(), v);
    if (it != occupantsActuels.end()) {
        occupantsActuels.erase(it);
        Logger::log(LogCategory::VILLAGEOIS, v->getNom() + " sort de " + nom);
        return true;
    }

    return false;
}

void Batiment::viderOccupants() {
    Logger::log(LogCategory::VILLAGEOIS, "Tous les occupants quittent " + nom);
    occupantsActuels.clear();
}

bool Batiment::contientOccupant(const Villageois* v) const {
    if (!v) return false;

    return std::find(occupantsActuels.begin(), occupantsActuels.end(), v)
           != occupantsActuels.end();
}

void Batiment::ajouterRessourceStockpile(const std::string& nomRes, int quantite) {
    if (quantite <= 0) return;

    // chercher si la ressource existe dejà
    auto index = stockpile.find(nomRes);

    if (index != stockpile.end()) {
        // ressource existe, ajouter la quantite
        index->second->ajouter(quantite);
    } else {
        // nouvelle ressource, creer l'objet
        Ressource* nouvelleRes = new Ressource(nomRes, quantite);
        stockpile[nomRes] = nouvelleRes;
    }

    Logger::log(LogCategory::RESSOURCES, nom + " a reçu " + std::to_string(quantite) + " " + nomRes);
}

bool Batiment::consommerRessource(const std::string& nomRes, int quantite) {
    if (quantite <= 0) return false;

    auto index = stockpile.find(nomRes);
    if (index == stockpile.end()) {
        Logger::log(LogCategory::PRODUCTION, nom + " n'a pas de " + nomRes);
        return false;
    }

    if (index->second->getQuantite() < quantite) {
        Logger::log(LogCategory::PRODUCTION,
                    nom + " n'a pas assez de " + nomRes +
                        " (a: " + std::to_string(index->second->getQuantite()) +
                        ", besoin: " + std::to_string(quantite) + ")");
        return false;
    }

    // retirer la quantite
    index->second->retirer(quantite);

    // si quantite = 0, supprimer l'entree
    if (index->second->getQuantite() <= 0) {
        delete index->second;
        stockpile.erase(index);
    }

    Logger::log(LogCategory::PRODUCTION, nom + " a consommé " + std::to_string(quantite) + " " + nomRes);
    return true;
}

void Batiment::transfererRessource(Batiment* destination, Villageois* expediteur,
                                   const std::string& nomRessource, int quantite) {
    if (!destination) {
        Logger::log(LogCategory::DEBUG, "Erreur: destination nulle");
        return;
    }

    if (quantite <= 0) return;

    // verifier si on a la ressource
    if (getQuantiteRessource(nomRessource) < quantite) {
        Logger::log(LogCategory::RESSOURCES, nom + " n'a pas assez de " + nomRessource + " a transférer");
        return;
    }

    // retirer de ce bâtiment
    if (consommerRessource(nomRessource, quantite)) {
        // ajouter à la destination
        destination->ajouterRessourceStockpile(nomRessource, quantite);

        if (expediteur) {
            Logger::log(LogCategory::RESSOURCES,
                        expediteur->getNom() + " a transféré " + std::to_string(quantite) + " " +
                            nomRessource + " de " + nom + " vers " + destination->getNom());
        } else {
            Logger::log(LogCategory::RESSOURCES,
                        "Transfert de " + std::to_string(quantite) + " " + nomRessource +
                            " de " + nom + " vers " + destination->getNom());
        }
    }
}

void Batiment::definirRessourceRequise(const std::string& nomRes, int quantite) {
    ressourcesRequises[nomRes] = quantite;
    Logger::log(LogCategory::SYSTEME,
                nom + " requiert " + std::to_string(quantite) + " " + nomRes + " pour produire");
}

void Batiment::definirRessourceProduite(const std::string& nomRes, int quantite) {
    ressourcesProduites[nomRes] = quantite;
    Logger::log(LogCategory::SYSTEME,
                nom + " produit " + std::to_string(quantite) + " " + nomRes + " par cycle");
}

void Batiment::mettreAJourStatut() {
    if (!actif) {
        statut = StatutBatiment::INACTIF;
        return;
    }

    // verifier si on peut produire
    if (!peutProduire() || estVide()) {
        statut = StatutBatiment::ENATTENTE;
        return;
    }

    // par defaut, prêt à produire
    statut = StatutBatiment::PRODUIT;
}

void Batiment::simulerCycle()
{
    if (!actif || estVide()) {
        statut = (actif ? StatutBatiment::ENATTENTE : StatutBatiment::INACTIF);
        return;
    }

    if (!peutProduire()) {
        statut = StatutBatiment::ENATTENTE;
        return;
    }

    consommer();
    produire();
    statut = StatutBatiment::PRODUIT;
}
