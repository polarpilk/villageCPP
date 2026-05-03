# villageCPP: Simulation de Village Médiéval avec C++/Qt
Projet final pour la matière Programmation orientée objet - L3 Informatique

Une simulation d'économie villageoise médiévale avec des chaînes de production, des routines autonomes pour les villageois, et une interface graphique Qt.

---

## Présentation

Ce projet simule la vie quotidienne d'un petit village médiéval. Les villageois suivent des routines horaires liées à leur profession, interagissent avec les bâtiments, produisent et consomment des ressources, et contribuent à l'économie locale. Le temps avance par **ticks** (un tick par heure en jeu), et le moteur du village coordonne l'ensemble des activités.

---

## Fonctionnalités

-  **Simulation par ticks** — les cycles horaires pilotent le comportement des villageois et la production des bâtiments
-  **Chaînes de production** — les ressources circulent entre les bâtiments (ex : blé → moulin → boulangerie)
-  **Villageois autonomes** — routines basées sur la profession, avec un système de livraison entre bâtiments
-  **Économie du village** — trésor, niveau de prospérité et suivi des ressources
-  **Sauvegarde / Import** — sauvegarde de l'état du village et import de nouveaux villageois via fichier texte
-  **Statistiques** — export et affichage de l'état des bâtiments et des ressources
-  **Système de logs** — messages catégorisés affichés dans l'interface Qt

---

## Architecture des classes

### `Village`
Moteur principal du programme. Gère le temps (ticks), l'économie, les bâtiments et les villageois.

**Attributs principaux :**
```cpp
std::vector<Batiment*> batiments
std::vector<Villageois*> population
int tickActuel
double tresorVillage
```

**Méthodes clés :** `updateJour()`, `tickBatiments()`, `tickVillageois()`, `sauvegarder()`, `importerVillageois()`

---

### `Batiment` *(classe abstraite)*
Représente les moyens de production et de consommation. Chaque bâtiment possède un inventaire de ressources et peut accueillir des villageois.

**Sous-classes :** `Abbaye`, `Boulangerie`, `Champ`, `Forge`, `Manoir`, `Mine`, `Moulin`, `Pâturage`, `Taverne`

**Méthodes virtuelles pures :**
```cpp
virtual void produire() = 0;
virtual void consommer() = 0;
virtual std::string getType() const = 0;
```

---

### `Villageois`
Chaque villageois a un lieu de travail et un accès à la taverne. Il suit une routine horaire selon sa profession et livre ses ressources aux bâtiments concernés lorsqu'il dépasse un certain seuil dans son inventaire.

**Méthodes clés :** `faireAction(int heure)`, `travailler()`, `deplacerVers()`

---

### `Ressource`
Unité échangeable possédée par les villageois et les bâtiments.

**Attributs :** `nom` (Blé, Pain, Acier, Manuscrits…), `quantite`, `prixUnitaire`, `nomUnite` (kg, pièces, sacs…)

---

## Relations entre les classes

```
Village      1 ◇─── 0..*  Batiment      (agrégation)
Batiment     1 ◆─── 0..*  Ressource     (composition)
Villageois 0..* ───    1  Batiment      (association)
Batiment   <|───           Champ, Mine, Moulin, Boulangerie, ...  (héritage)
```

---

## Format d'import d'un villageois

Fichier `importVillageois.txt` (3 lignes) :
```
10
Jean le Forgeron
Forge
```
La première ligne correspond à l'id, la deuxième au nom, la troisième au lieu de travail.

---

## Concepts C++ illustrés

- **Spécificateurs d'accès** — `private`, `protected`, `public` selon les classes
- **Méthodes virtuelles pures** — classe `Batiment` abstraite
- **Surcharge d'opérateurs** — `operator==` et `operator>` dans `Ressource`
- **Pointeurs & références** — agrégation avec `Villageois*`, transmission `const&`
- **Méthodes `const`** — getters non-mutants
- **Lecture/écriture de fichiers** — sauvegarde et import via `std::fstream`

---

## Difficultés rencontrées

- Migration de SFML vers Qt, refonte complète de l'interface graphique
- Gestion mémoire (fuites, segfaults)
- Logique métier couplée à l'interface faute de temps pour une refonte complète
- Filtres checkbox limités à deux types d'informations simultanés

---

## Pistes d'amélioration

- Icônes de production sur les bâtiments
- Log moins verbeux
- Comportement encore plus autonome pour les villageois
- Économie plus complexe
- Événements aléatoires (peste, guerre, récoltes abondantes…)

---

## Lancement

> L'exécutable est déjà compilé.

Lancer soit le raccourci dans le dossier `codeVillage`, soit l'exécutable dans `codeVillage/build`.

---

## Diagramme de classes

Voir [`Diagramme.pdf`](./Diagramme.pdf)
