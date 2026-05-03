#ifndef RESSOURCE_HPP
#define RESSOURCE_HPP

#include <string>


class Ressource {
    private:
        std::string nom;
        int quantite;
        double prixUnitaire;
        std::string nomUnite; // kg, pieces, sacs, etc

    public:
        // Constructeur
        Ressource(const std::string& nom, int quantite = 0, double prix = 1.0, 
                const std::string& unite = "unite");
        
        // Getters
        std::string getNom() const;
        int getQuantite() const;
        double getPrixUnitaire() const;
        std::string getNomUnite() const;
        
        // Setters
        void setQuantite(int q);
        void setPrixUnitaire(double p);
        
        // Méthodes
        void ajouter(int q);
        bool retirer(int q);  // retourne false si quantité insuffisante
        double getValeurTotale() const;
        bool estDisponible(int quantiteRequise) const;
        
        // Opérateurs
        bool operator==(const Ressource& autre) const;
        bool operator>(const Ressource& autre) const;  // compare les quantités
};

#endif