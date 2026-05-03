#ifndef BATIMENT_HPP
#define BATIMENT_HPP

#include <string>
#include <vector>
#include <map> //dicos

class Villageois;
class Ressource;

enum class StatutBatiment {
    INACTIF,           
    PRODUIT,          
    ENATTENTE,        
    STOCKAGEPLEIN,
};

class Batiment {
    protected:
        //infos de base
        std::string nom;
        StatutBatiment statut;
        bool actif;
        int id;

        // villageois actuellement dans le bâtiment
        std::vector<Villageois*> occupantsActuels; // relation d'association

        // ressources
        std::map<std::string, Ressource*> stockpile;     // ressources stockées dans le bâtiment
        std::map<std::string, int> ressourcesRequises;   // inputs de production recquis
        std::map<std::string, int> ressourcesProduites;  // ce que le bâtiment produit

    public:
        // constructeur
        Batiment(const std::string& nom, int id);
        virtual ~Batiment();

        // getters 

        // infos de base
        std::string getNom() const;
        StatutBatiment getStatut() const; // reourne un bool
        std::string getStatutString() const;  // retourne un string
        bool estActif() const;

        // occupants
        const std::vector<Villageois*>& getOccupants() const;
        int getNombreOccupants() const;
        bool estVide() const;

        // ressources
        const std::map<std::string, Ressource*>& getStockpile() const; // affiche tout ce qu'il y a dans le bâtiment
        int getQuantiteRessource(const std::string& nomRessource) const; // comme getStockpile mais avec une ressource spécifique ? 

        const std::map<std::string, int>& getRessourcesRequises() const;
        const std::map<std::string, int>& getRessourcesProduites() const;
        bool peutProduire() const; // si les ressources sont dispo 

        // économie
        // setters
        void setNom(const std::string& n);
        void setStatut(StatutBatiment s);
        void setActif(bool a);

        // méthodes 
        bool ajouterOccupant(Villageois* v);
        bool retirerOccupant(Villageois* v);
        void viderOccupants();
        bool contientOccupant(const Villageois* v) const; 
        void ajouterRessourceStockpile(const std::string& nom, int quantite);
        bool consommerRessource(const std::string& nom, int quantite);
        void transfererRessource(Batiment* destination, Villageois* expediteur, const std::string& nomRessource, int quantite);
        void definirRessourceRequise(const std::string& nom, int quantite);
        void definirRessourceProduite(const std::string& nom, int quantite);


        // polymorphisme
        virtual void afficherInfos() const = 0;
        virtual std::string getType() const = 0;
        virtual void produire() = 0;  // production spécifique à chaque bâtiment
        virtual void consommer() = 0; // consommation spécifique

        void mettreAJourStatut();  // met à jour le statut selon l'état interne
        void simulerCycle();       // simule un cycle (tick)
};

#endif
