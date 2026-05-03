#ifndef VILLAGE_HPP
#define VILLAGE_HPP

#include <string>
#include <vector>
#include <map>

class Batiment;
class Villageois;


class Village {
private:
    std::string nomVillage;
    int tickActuel;
    int ticksParJour;      // 24 ticks = 1 jour ?
    int jour;
    
    // composition
    std::vector<Batiment*> batiments;         
    std::vector<Villageois*> population;

    
    double tresorVillage;
    int niveauProsperite;
    
    // stats
    std::vector<double> historiqueTresor;

public:

    // constructeur
    Village(const std::string& nom);
    ~Village();  // doit libérer toute la mémoire allouée
    
    // getters
    std::string getNomVillage() const;

        // système de ticks
    int getTickActuel() const;
    int getTicksParJour() const;
    int getJour() const;
    
        // collections
    const std::vector<Batiment*>& getBatiments() const;
    const std::vector<Villageois*>& getPopulation() const;
    
        // économie
    double getTresorVillage() const;
    const std::map<std::string, int>& getRessourcesTotales() const;
    int getNiveauProsperite() const;
    
        // statistiques
    int getNombreHabitants() const;
    int getNombreBatiments() const;
    int getNombreRoutes() const;
    const std::vector<double>& getHistoriqueTresor() const;
    
    // setters
    void setNomVillage(const std::string& nom);
    void setTicksParJour(int ticks);
    void setNiveauProsperite(int niveau);
    void ajouterAuTresor(double montant);
    void retirerDuTresor(double montant);
    
        // bâtiments
    void ajouterBatiment(Batiment* b);
    bool supprimerBatiment(const std::string& nom);
    Batiment* trouverBatiment(const std::string& nom) const;
    int compterBatimentsType(const std::string& type) const;
    
        // villageois
    void ajouterVillageois(Villageois* v);
    bool supprimerVillageois(const std::string& nom);
    Villageois* trouverVillageois(const std::string& nom) const;
    std::vector<Villageois*> trouverVillageoisParProfession(const std::string& prof) const;
    
    
    // méthodes
    
    void tick(); // avance le temps
    
    // Actions effectuées à certains moments
    void updateJour();           // appelé quand on veut passer au jour suivant
    void tickBatiments();             // met à jour tous les bâtiments
    void tickVillageois();            // met à jour tous les villageois
    void calculerNiveauProsperite();

    // sauvegarde/chargement (lecture/écriture fichiers)
    void sauvegarder(const std::string& nomFichier) const;
    bool importerVillageois(const std::string& nomFichier);
    void exporterStatistiques(const std::string& nomFichier) const;
    
    void afficherStatistiques() const;
    void afficherEtatBatiments() const;
    void afficherRessources() const;
};

#endif

