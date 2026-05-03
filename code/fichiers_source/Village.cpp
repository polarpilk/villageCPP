#include "Village.hpp"
#include "Batiment.hpp"
#include "Villageois.hpp"
#include "Ressource.hpp"
#include "Logger.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <qobject.h>
#include <fstream>

// constructeur
Village::Village(const std::string& nom)
    : nomVillage(nom), tickActuel(0), ticksParJour(24), jour(1),
    tresorVillage(1000.0), niveauProsperite(50) {

    Logger::log(LogCategory::SYSTEME, "Village '" + nom + "' créé");
    historiqueTresor.push_back(tresorVillage);
}

// destructeur
Village::~Village() {
    // libérer tous les batiments
    for (auto* batiment : batiments) {
        delete batiment;
    }
    batiments.clear();

    // liberer tous les villageois
    for (auto* villageois : population) {
        delete villageois;
    }
    population.clear();

    Logger::log(LogCategory::SYSTEME, "Village '" + nomVillage + "' détruit");
}


// getters
std::string Village::getNomVillage() const {
    return nomVillage;
}

int Village::getTickActuel() const {
    return tickActuel;
}

int Village::getTicksParJour() const {
    return ticksParJour;
}

int Village::getJour() const {
    return jour;
}

const std::vector<Batiment*>& Village::getBatiments() const {
    return batiments;
}

const std::vector<Villageois*>& Village::getPopulation() const {
    return population;
}

double Village::getTresorVillage() const {
    return tresorVillage;
}

const std::map<std::string, int>& Village::getRessourcesTotales() const {
    static std::map<std::string, int> ressourcesTotales;
    ressourcesTotales.clear();

    // Sommer toutes les ressources de tous les batiments
    for (const auto* batiment : batiments) {
        for (const auto& [nomRes, ressource] : batiment->getStockpile()) {
            ressourcesTotales[nomRes] += ressource->getQuantite();
        }
    }

    return ressourcesTotales;
}

int Village::getNiveauProsperite() const {
    return niveauProsperite;
}

int Village::getNombreHabitants() const {
    return population.size();
}

int Village::getNombreBatiments() const {
    return batiments.size();
}

// setters

void Village::setNomVillage(const std::string& nom) {
    nomVillage = nom;
}

void Village::setTicksParJour(int ticks) {
    if (ticks > 0) {
        ticksParJour = ticks;
    }
}

void Village::setNiveauProsperite(int niveau) {
    niveauProsperite = std::max(0, std::min(100, niveau));
}

void Village::ajouterAuTresor(double montant) {
    if (montant > 0) {
        tresorVillage += montant;
        Logger::log(LogCategory::ECONOMIE,
                    "+" + std::to_string(montant) + " pièces d'or au trésor (Total: " +
                        std::to_string(tresorVillage) + ")");
    }
}

void Village::retirerDuTresor(double montant) {
    if (montant > 0 && tresorVillage >= montant) {
        tresorVillage -= montant;
        Logger::log(LogCategory::ECONOMIE,
                    "-" + std::to_string(montant) + " pièces d'or du trésor (Restant: " +
                        std::to_string(tresorVillage) + ")");
    }
}


// gestion des bâtiments
void Village::ajouterBatiment(Batiment* b) {
    if (!b) {
        Logger::log(LogCategory::DEBUG, "Erreur: tentative d'ajouter un bâtiment nul");
        return;
    }

    batiments.push_back(b);
    Logger::log(LogCategory::SYSTEME, "Bâtiment '" + b->getNom() + "' ajouté au village");
}

bool Village::supprimerBatiment(const std::string& nom) {
    auto it = std::find_if(batiments.begin(), batiments.end(),
                           [&nom](const Batiment* b) { return b->getNom() == nom; });

    if (it != batiments.end()) {
        Logger::log(LogCategory::SYSTEME, "Suppression du bâtiment '" + (*it)->getNom() + "'");
        delete *it;
        batiments.erase(it);
        return true;
    }

    Logger::log(LogCategory::DEBUG, "Bâtiment '" + nom + "' non trouvé");
    return false;
}

Batiment* Village::trouverBatiment(const std::string& nom) const {
    auto it = std::find_if(batiments.begin(), batiments.end(),
                           [&nom](const Batiment* b) { return b->getNom() == nom; });

    return (it != batiments.end()) ? *it : nullptr;
}

int Village::compterBatimentsType(const std::string& type) const {
    int count = 0;
    for (const auto* batiment : batiments) {
        if (batiment->getType() == type) {
            count++;
        }
    }
    return count;
}


// gestion villageois
void Village::ajouterVillageois(Villageois* v) {
    if (!v) {
        Logger::log(LogCategory::DEBUG, "Erreur: tentative d'ajouter un villageois nul");
        return;
    }

    population.push_back(v);
    Logger::log(LogCategory::SYSTEME, "Villageois '" + v->getNom() + "' ajouté au village");
}

bool Village::supprimerVillageois(const std::string& nom) {
    auto it = std::find_if(population.begin(), population.end(),
                           [&nom](const Villageois* v) { return v->getNom() == nom; });

    if (it != population.end()) {
        Logger::log(LogCategory::SYSTEME, "Suppression du villageois '" + (*it)->getNom() + "'");
        delete *it;
        population.erase(it);
        return true;
    }

    Logger::log(LogCategory::DEBUG, "Villageois '" + nom + "' non trouvé");
    return false;
}

Villageois* Village::trouverVillageois(const std::string& nom) const {
    auto it = std::find_if(population.begin(), population.end(),
                           [&nom](const Villageois* v) { return v->getNom() == nom; });

    return (it != population.end()) ? *it : nullptr;
}

std::vector<Villageois*> Village::trouverVillageoisParProfession(const std::string& prof) const {
    std::vector<Villageois*> resultat;

    for (auto* villageois : population) {
        if (villageois->getLieuTravail() &&
            villageois->getLieuTravail()->getType() == prof) {
            resultat.push_back(villageois);
        }
    }

    return resultat;
}


// SYSTEME DE TICKS
void Village::tick() {
    tickActuel++;

    int heureActuelle = tickActuel % ticksParJour;

    Logger::log(LogCategory::SYSTEME, "\n=== Jour " + std::to_string(jour) + ", Heure " + std::to_string(heureActuelle) + " ===");

    // D'ABORD les villageois (ils agissent selon leur routine)
    for (auto* villageois : population) {
        villageois->faireAction(heureActuelle);
    }

    // ENSUITE les batiments (production basée sur la présence des villageois)
    tickBatiments();

    // nouveau jour
    if (tickActuel % ticksParJour == 0) {
        updateJour();
    }
}

void Village::tickBatiments() {
    for (auto* batiment : batiments) {
        if (!batiment->estActif()) continue;

        batiment->simulerCycle();
        Logger::log(LogCategory::PRODUCTION, batiment->getNom() + " " + batiment->getStatutString());

        int heureActuelle = tickActuel % ticksParJour;

        // bonus de fin de journée
        if (heureActuelle == 17) {
            for (auto* occupant : batiment->getOccupants()) {
                for (const auto& [ressource, quantite] : batiment->getRessourcesProduites()) {
                    occupant->ajouterRessource(ressource, 2);}
            }
        }
    }

}

void Village::updateJour() {
    jour++;
    Logger::log(LogCategory::SYSTEME, "\n========================================\n"
                                      "   NOUVEAU JOUR : Jour " + std::to_string(jour) + "\n"
                                      "========================================");
    calculerNiveauProsperite();
    historiqueTresor.push_back(tresorVillage);

    // stats du jour écoulé ?
    afficherStatistiques();
}


// CALCULS
void Village::calculerNiveauProsperite() {
    int score = 0;

    // facteur 1: trésor (max 30 points)
    score += std::min(30, static_cast<int>(tresorVillage / 100));

    // facteur 2: population (max 20 points)
    score += std::min(20, static_cast<int>(population.size() * 2));

    // facteur 3: bâtiments (max 25 points)
    score += std::min(25, static_cast<int>(batiments.size() * 3));

    // facteur 4: ressources totales (max 25 points)
    int totalRessources = 0;
    for (const auto& [nom, quantite] : getRessourcesTotales()) {
        totalRessources += quantite;
    }
    score += std::min(25, totalRessources / 20);

    niveauProsperite = std::max(0, std::min(100, score));

    Logger::log(LogCategory::SYSTEME, "Niveau de prosperité: " + std::to_string(niveauProsperite) + "/100");
}


// AFFICHAGE
void Village::afficherStatistiques() const {
    Logger::log(LogCategory::SYSTEME,
                "\n=== STATISTIQUES DU VILLAGE ===\n"
                "Nom: " + nomVillage + "\n"
                "Jour: " + std::to_string(jour) + "\n"
                "Population: " + std::to_string(population.size()) + " habitants\n"
                "Bâtiments: " + std::to_string(batiments.size()) + "\n"
                "Trésor: " + std::to_string(tresorVillage) + " pieces d'or\n"
                "Prospérité: " + std::to_string(niveauProsperite) + "/100\n"
                "================================");
}

void Village::afficherEtatBatiments() const {
    Logger::log(LogCategory::SYSTEME, "\n=== ÉTAT DES BATIMENTS ===");

    for (const auto* batiment : batiments) {
        std::string info = "\n" + batiment->getNom() + " (" + batiment->getType() + ")\n";
        info += "  Statut: " + batiment->getStatutString() + "\n";
        info += "  Occupants: " + std::to_string(batiment->getNombreOccupants()) + "\n";
        info += "  Actif: " + std::string(batiment->estActif() ? "Oui" : "Non");

        // affichage stockpile
        const auto& stockpile = batiment->getStockpile();
        if (!stockpile.empty()) {
            info += "\n  Ressources:";
            for (const auto& [nom, ressource] : stockpile) {
                info += "\n    - " + nom + ": " + std::to_string(ressource->getQuantite());
            }
        }

        Logger::log(LogCategory::SYSTEME, info);
    }

    Logger::log(LogCategory::SYSTEME, "\n===========================");
}

void Village::afficherRessources() const {
    Logger::log(LogCategory::SYSTEME, "\n=== RESSOURCES TOTALES ===");

    const auto& ressources = getRessourcesTotales();

    if (ressources.empty()) {
        Logger::log(LogCategory::SYSTEME, "Aucune ressource disponible");
    } else {
        for (const auto& [nom, quantite] : ressources) {
            Logger::log(LogCategory::SYSTEME, "  " + nom + ": " + std::to_string(quantite));
        }
    }

    Logger::log(LogCategory::SYSTEME, "==========================");
}

// sauvegarde/chargement ?

void Village::sauvegarder(const std::string& nomFichier) const {
    std::ofstream fichier(nomFichier);

    if (!fichier.is_open()) {
        Logger::log(LogCategory::DEBUG, "Erreur: impossible d'ouvrir le fichier " + nomFichier);
        return;
    }

    // Écrire les données basiques
    fichier << "VILLAGE:" << nomVillage << std::endl;
    fichier << "JOUR:" << jour << std::endl;
    fichier << "TICK:" << tickActuel << std::endl;
    fichier << "TRESOR:" << tresorVillage << std::endl;
    fichier << "PROSPERITE:" << niveauProsperite << std::endl;

    // Sauvegarder les villageois (juste leurs noms pour l’instant)
    fichier << "HABITANTS:" << population.size() << std::endl;
    for (const auto* v : population) {
        fichier << "  " << v->getNom() << std::endl;
    }


    fichier.close();
    Logger::log(LogCategory::SYSTEME, "Village sauvegardé dans " + nomFichier);
}


bool Village::importerVillageois(const std::string& nomFichier) {
    std::ifstream fichier(nomFichier);
    if (!fichier.is_open()) {
        Logger::log(LogCategory::DEBUG, "Erreur: impossible d'ouvrir le fichier " + nomFichier);
        return false;
    }

    std::string ligneId, ligneName, ligneTravail;
    while (std::getline(fichier, ligneId) &&
           std::getline(fichier, ligneName) &&
           std::getline(fichier, ligneTravail)) {

        // stoi: string to integer
        int id = std::stoi(ligneId);
        std::string nom = ligneName;

        Batiment* lieuTravail = trouverBatiment(ligneTravail);

        Villageois* v = new Villageois(id, nom);
        if (lieuTravail) v->setLieuTravail(lieuTravail);

        // init avec 0 règlesLivraison pour le moment pour éviter les bugs
        v->regleLivraison = {"", 0, nullptr};
        ajouterVillageois(v);
        Logger::log(LogCategory::SYSTEME, "Villageois importé: " + nom + " (ID: " + std::to_string(id) + ")");
    }
    fichier.close();
    return true;
}


