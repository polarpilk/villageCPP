#include "Ressource.hpp"
#include "Logger.hpp"

// constructeur
Ressource::Ressource(const std::string& nom, int quantite, double prix,
                     const std::string& unite)
    : nom(nom), quantite(quantite), prixUnitaire(prix), nomUnite(unite) {

    // validation des valeurs
    if (quantite < 0) {
        this->quantite = 0;
        Logger::log(LogCategory::DEBUG, "erreur: quantité négative de " + nom);
    }

    if (prix < 0) {
        this->prixUnitaire = 0.0;
        Logger::log(LogCategory::DEBUG, "erreur: prix négatif pour " + nom);
    }

    Logger::log(LogCategory::RESSOURCES,
                "Ressource '" + nom + "' créée (" + std::to_string(quantite) + " " + unite + ")");
}

// getters
std::string Ressource::getNom() const {
    return nom;
}

int Ressource::getQuantite() const {
    return quantite;
}

double Ressource::getPrixUnitaire() const {
    return prixUnitaire;
}

std::string Ressource::getNomUnite() const {
    return nomUnite;
}


// setters
void Ressource::setQuantite(int q) {
    if (q < 0) {
        Logger::log(LogCategory::DEBUG, "erreur: quantité négative pour " + nom);
        quantite = 0;
        return;
    }

    quantite = q;
}

void Ressource::setPrixUnitaire(double p) {
    if (p < 0) {
        Logger::log(LogCategory::DEBUG, "erreur: prix négatif pour " + nom);
        prixUnitaire = 0.0;
        return;
    }

    prixUnitaire = p;
}

// méthodes
void Ressource::ajouter(int q) {
    if (q <= 0) {
        Logger::log(LogCategory::DEBUG, "erreur: quantite nulle ou négative");
        return;
    }
    quantite += q;
    Logger::log(LogCategory::RESSOURCES,
                "+" + std::to_string(q) + " " + nom + " (total: " + std::to_string(quantite) + ")");
}

bool Ressource::retirer(int q) {
    if (q <= 0) {
        Logger::log(LogCategory::DEBUG, "erreur: quantite nulle ou négative");
        return false;
    }

    if (quantite < q) {
        Logger::log(LogCategory::RESSOURCES,
                    "impossible de retirer " + std::to_string(q) + " " + nom +
                        " (disponible: " + std::to_string(quantite) + ")");
        return false;
    }

    quantite -= q;
    Logger::log(LogCategory::RESSOURCES,
                "-" + std::to_string(q) + " " + nom + " (restant: " + std::to_string(quantite) + ")");
    return true;
}

double Ressource::getValeurTotale() const {
    return quantite * prixUnitaire;
}

bool Ressource::estDisponible(int quantiteRequise) const {
    return quantite >= quantiteRequise;
}


// sera utilisé quand les villageois vont devoir mettre des ressources requises etc
bool Ressource::operator==(const Ressource& autre) const {
    // deux ressources sont égales si elles ont le même nom
    return nom == autre.nom;
}

bool Ressource::operator>(const Ressource& autre) const {
    // compare les quantités
    return quantite > autre.quantite;
}
