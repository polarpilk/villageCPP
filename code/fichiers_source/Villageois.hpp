    #ifndef VILLAGEOIS_HPP
    #define VILLAGEOIS_HPP

    #include "Batiment.hpp"
    #include <string>
    #include <map>


    class Batiment;
    class Ressource;
    class Taverne;

    enum class StatutVillageois {
        SEDEPLACE,
        TRAVAILLE,
        LIVRE,
        PICOLE,
        DORT
    };

    struct RegleLivraison {
        std::string ressource;
        int seuil;
        Batiment* destination;
    };

    class Villageois {
        private:
            int id;
            std::string nom;            
            StatutVillageois statut;
            
            Batiment* emplacementActuel;
            Batiment* lieuTravail;
            Batiment* emplacementTaverne;
            bool enDeplacement;

            std::map<std::string, int> inventaire;


        public:
        // constructeur
        Villageois(int id, const std::string& nom);

        // destructeur
        ~Villageois();

        // getters
        int getId() const;
        std::string getNom() const;
        StatutVillageois getStatut() const;
        std::string getStatutString() const;  
        
        // associations
        Batiment* getLieuTravail() const;

        RegleLivraison regleLivraison;
        
        // inventaire
        const std::map<std::string, int>& getInventaire() const;
        int getQuantiteRessource(const std::string& nomRessource) const;
        bool possede(const std::string& nomRessource, int quantite) const;

        // setters
        void setNom(const std::string& n);
        void setStatut(StatutVillageois s);
        void setEmplacement(Batiment* o);
        void setLieuTravail(Batiment* b);
        void setEmplacementTaverne(Batiment* t);
        
        // méthodes
        void ajouterRessource(const std::string& nom, int quantite);
        bool retirerRessource(const std::string& nom, int quantite);
        void viderInventaire();
        
        
        // déplacement
        void deplacerVers(Batiment* destination);
        bool estEnDeplacement() const;

        // Actions spécifiques
        void faireAction(int heure);
        void travailler();
};

#endif

