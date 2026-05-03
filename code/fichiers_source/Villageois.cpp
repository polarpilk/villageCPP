#include "Villageois.hpp"
#include "Batiment.hpp"
#include "Logger.hpp"
#include "Ressource.hpp"

// constructeur
Villageois::Villageois(int id, const std::string& nom)
    : id(id), nom(nom), statut(StatutVillageois::SEDEPLACE),
    emplacementActuel(nullptr), lieuTravail(nullptr),
    emplacementTaverne(nullptr), enDeplacement(false) {
    Logger::log(LogCategory::SYSTEME, "Villageois " + std::to_string(id) + " " + nom + " créé");
}

// destructeur
Villageois::~Villageois() {
    Logger::log(LogCategory::SYSTEME, "Villageois " + std::to_string(id) + " " + nom + " détruit");
}

// getters
int Villageois::getId() const {
    return id;
}

std::string Villageois::getNom() const {
    return nom;
}

StatutVillageois Villageois::getStatut() const {
    return statut;
}

std::string Villageois::getStatutString() const {
    switch (statut) {
    case StatutVillageois::SEDEPLACE:
        return "Se deplace";
    case StatutVillageois::TRAVAILLE:
        return "Travaille";
    case StatutVillageois::LIVRE:
        return "Livre";
    case StatutVillageois::PICOLE:
        return "Picole";
    case StatutVillageois::DORT:
        return "Dort";
    default:
        return "???";
    }
}

Batiment* Villageois::getLieuTravail() const {
    return lieuTravail;
}

const std::map<std::string, int>& Villageois::getInventaire() const {
    return inventaire;
}

int Villageois::getQuantiteRessource(const std::string& nomRessource) const {
    auto index = inventaire.find(nomRessource);
    if (index != inventaire.end()) {
        return index->second;
    }
    return 0;
}

bool Villageois::possede(const std::string& nomRessource, int quantite) const {
    return getQuantiteRessource(nomRessource) >= quantite;
}

// setters

void Villageois::setNom(const std::string& n) {
    nom = n;
}

void Villageois::setStatut(StatutVillageois s) {
    statut = s;
}

void Villageois::setEmplacement(Batiment* b) {
    emplacementActuel = b;
    enDeplacement = false;
}

void Villageois::setLieuTravail(Batiment* b) {
    if (!b) {
        Logger::log(LogCategory::DEBUG, "ERREUR: Tentative d'assigner un lieu de travail null a " + nom);
        return;
    }
    lieuTravail = b;
    Logger::log(LogCategory::SYSTEME, nom + " travaille maintenant à " + b->getNom());
}

void Villageois::setEmplacementTaverne(Batiment* t) {
    emplacementTaverne = t;
}

// méthodes

void Villageois::ajouterRessource(const std::string& nomRes, int quantite) {
    if (quantite <= 0) return;
    inventaire[nomRes] += quantite;
    Logger::log(LogCategory::RESSOURCES, nom + " a reçu " + std::to_string(quantite) + " " + nomRes);
}

bool Villageois::retirerRessource(const std::string& nomRes, int quantite) {
    if (quantite <= 0) return false;

    if (!possede(nomRes, quantite)) {
        Logger::log(LogCategory::RESSOURCES, nom + " n'a pas assez de " + nomRes);
        return false;
    }

    inventaire[nomRes] -= quantite;

    if (inventaire[nomRes] <= 0) {
        inventaire.erase(nomRes);
    }

    Logger::log(LogCategory::RESSOURCES, nom + " a deposé " + std::to_string(quantite) + " " + nomRes);
    return true;
}

void Villageois::viderInventaire() {
    inventaire.clear();
    Logger::log(LogCategory::RESSOURCES, nom + " a vidé son inventaire");
}

// déplacement

void Villageois::deplacerVers(Batiment* destination) {
    if (!destination) {
        Logger::log(LogCategory::DEBUG, "ERREUR: " + nom + " tente de se déplacer vers null");
        return;
    }

    enDeplacement = true;
    statut = StatutVillageois::SEDEPLACE;

    Logger::log(LogCategory::VILLAGEOIS, nom + " se déplace vers : " + destination->getNom());
    emplacementActuel = destination;
}

bool Villageois::estEnDeplacement() const {
    return enDeplacement;
}

void Villageois::faireAction(int heure)
{
    // validation lieu de travail pour éviter les segfaults
    if (!lieuTravail) {
        Logger::log(LogCategory::DEBUG, "[ERREUR] " + nom + " n'a PAS de lieu de travail!");
        setStatut(StatutVillageois::DORT);
        return;
    }

    // NUIT (0h-6h)
    if (heure >= 0 && heure <= 6) {
        setStatut(StatutVillageois::DORT);
        Logger::log(LogCategory::VILLAGEOIS, nom + " dort ");
        return;
    }

    // TRANSFERT DE RESSOURCES VERS BATIMENTS (7h-18h, si conditions remplies)
    if (heure >= 7 && heure < 18 &&
        regleLivraison.destination != nullptr &&
        !regleLivraison.ressource.empty() &&
        lieuTravail->getQuantiteRessource(regleLivraison.ressource) >= regleLivraison.seuil)
    {
        setStatut(StatutVillageois::LIVRE);

        // se déplacer au lieu de travail si pas déjà là
        if (emplacementActuel != lieuTravail) {
            deplacerVers(lieuTravail);
            return;
        }

        // sauvegarder les noms AVANT le transfert (encore une fois pour éviter les segfaults)
        std::string nomOrigine = lieuTravail->getNom();
        std::string nomDestination = regleLivraison.destination->getNom();

        // calculer le paiement avant le transfert
        double prixUnitaire = 1.0;  // Prix par défaut
        // pas le temps d'implémenter des prix différents pour chaque ressource
        const auto& stockpile = lieuTravail->getStockpile();
        auto it = stockpile.find(regleLivraison.ressource);
        if (it != stockpile.end()) {
            prixUnitaire = it->second->getPrixUnitaire();
        }

        // effectuer le transfert
        lieuTravail->transfererRessource(
            regleLivraison.destination,
            this,
            regleLivraison.ressource,
            regleLivraison.seuil
            );

        // afficher
        Logger::log(LogCategory::RESSOURCES, nom + " livre " + std::to_string(regleLivraison.seuil) + " " + regleLivraison.ressource + " de " + nomOrigine + " vers " + nomDestination);

        // payer le villageois (commission de 10% de la valeur)
        int paiement = static_cast<int>(regleLivraison.seuil * prixUnitaire * 0.3 + 2);
        if (paiement > 0) {
            this->ajouterRessource("Or", paiement);
            Logger::log(LogCategory::ECONOMIE, nom + " reçoit " + std::to_string(paiement) + " pièce(s) d'or pour la livraison");
        }

        return;
    }
    // TRAVAIL (7h-18h)
    if (heure >= 7 && heure < 18) {
        // se déplacer au travail si pas déjà là
        if (emplacementActuel != lieuTravail) {
            setStatut(StatutVillageois::SEDEPLACE);
            deplacerVers(lieuTravail);
            lieuTravail->ajouterOccupant(this);
            return;
        }

        // travailler
        setStatut(StatutVillageois::TRAVAILLE);
        Logger::log(LogCategory::VILLAGEOIS, nom + " travaille a " + lieuTravail->getNom());
        return;
    }

    // SOIR / TAVERNE (18h-20h)
    if (heure >= 18 && heure < 20) {
        // quitter le lieu de travail
        if (lieuTravail && emplacementActuel == lieuTravail) {
            lieuTravail->retirerOccupant(this);
        }

        // vérifier que la taverne existe
        if (!emplacementTaverne) {
            Logger::log(LogCategory::DEBUG, "ERREUR: " + nom + " n'a pas de taverne assignée!");
            setStatut(StatutVillageois::DORT);
            return;
        }

        // se déplacer à la taverne
        setStatut(StatutVillageois::SEDEPLACE);
        deplacerVers(emplacementTaverne);

        // si le villageois travaille à la taverne
        if (lieuTravail == emplacementTaverne) {
            Logger::log(LogCategory::VILLAGEOIS, nom + " sert les clients à la taverne");


            setStatut(StatutVillageois::TRAVAILLE);
            emplacementTaverne->ajouterOccupant(this);
        } else {
            Logger::log(LogCategory::VILLAGEOIS, nom + " picole à la taverne");

            // À 19h, payer pour la bière
            if (heure == 19) {
                int coutBiere = 2;
                if (this->possede("Or", coutBiere)) {
                    this->retirerRessource("Or", coutBiere);
                    Logger::log(LogCategory::ECONOMIE, nom + " dépense " + std::to_string(coutBiere) + " Or pour une bière");

                    for (auto* occupant : emplacementTaverne->getOccupants()) {
                        // l'argent va au tavernier
                        if (occupant->getLieuTravail() == emplacementTaverne) {
                            occupant->ajouterRessource("Or", coutBiere);
                            break;
                        }
                    }
                } else {
                    Logger::log(LogCategory::ECONOMIE,
                                nom + " n'a pas assez d'Or pour boire (" +
                                    std::to_string(this->getQuantiteRessource("Or")) + " Or)");
                }
            }

            setStatut(StatutVillageois::PICOLE);
        }
        return;
    }

    // FIN DE LA JOURNÉE (20h-23h)
    if (heure >= 20) {
        setStatut(StatutVillageois::DORT);
        Logger::log(LogCategory::VILLAGEOIS, nom + " rentre dormir");
        return;
    }
}
