#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Logger.hpp"

#include "Village.hpp"
#include "Villageois.hpp"
#include "Champ.hpp"
#include "Taverne.hpp"
#include "Batiment.hpp"
#include "Paturage.hpp"
#include "Moulin.hpp"
#include "Boulangerie.hpp"
#include "Mine.hpp"
#include "Forge.hpp"
#include "Abbaye.hpp"
#include "Manoir.hpp"

#include "qtstreambuf.h"

#include <QPixmap>
#include <QStringConverter>
#include <QFileDialog>
#include <iostream>


// constructeur
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    village(nullptr),
    qtCoutBuf(nullptr),
    oldCoutBuf(nullptr)
{
    ui->setupUi(this);

    QPixmap img1(":/assets/assets/villageBG.png");
    ui->villageBG->setPixmap(img1.scaled(751, 656, Qt::KeepAspectRatio));

    QPixmap img2(":/assets/assets/enproduction.png");
    ui->iconeProdChamp->setPixmap(img2.scaled(35, 35, Qt::KeepAspectRatio));

   // filtres output
    connect(ui->checkBoxEconomie, &QCheckBox::clicked, this, &MainWindow::onCheckboxChanged);
    connect(ui->checkBoxProduction, &QCheckBox::clicked, this, &MainWindow::onCheckboxChanged);
    connect(ui->checkBoxRessources, &QCheckBox::clicked, this, &MainWindow::onCheckboxChanged);
    connect(ui->checkBoxSysteme, &QCheckBox::clicked, this, &MainWindow::onCheckboxChanged);
    connect(ui->checkBoxTout, &QCheckBox::clicked, this, &MainWindow::onCheckboxChanged);
    connect(ui->checkBoxVillageois, &QCheckBox::clicked, this, &MainWindow::onCheckboxChanged);

    // icones pour montrer qu'un bâtiment produit
    ui->iconeProdChamp->hide();


    // met les messages log dans le widget
    qtCoutBuf = new QtStreamBuf([this](const QString& msg) {ui->logView->append(QString::fromUtf8(msg.toUtf8()));});
    oldCoutBuf = std::cout.rdbuf(qtCoutBuf);


    Logger::log(LogCategory::SYSTEME, "DEMARRAGE DE LA SIMULATION" );

    // ----------------------------------------

    // init village
    village = new Village("Alenrac");

    Batiment* champ = new Champ("Champ de Bernard", 1);
    Batiment* moulin = new Moulin("Moulin de Otbert", 2);
    Batiment* boulangerie = new Boulangerie("Boulangerie du Village", 3);
    Batiment* taverne = new Taverne("Le Gobelet d'Argent", 4);
    Batiment* paturage = new Paturage("Troupeau de Moutons", 5);
    Batiment* mine = new Mine("Mine de la Montagne Noire", 6);
    Batiment* forge = new Forge("L'Atelier d'Adalind", 7);
    Batiment* abbaye = new Abbaye("Abbaye d'Alenrac", 8);
    Batiment* manoir = new Manoir("Demeure de la Baronne", 9);

    village->ajouterBatiment(champ);
    village->ajouterBatiment(moulin);
    village->ajouterBatiment(boulangerie);
    village->ajouterBatiment(taverne);
    village->ajouterBatiment(paturage);
    village->ajouterBatiment(mine);
    village->ajouterBatiment(forge);
    village->ajouterBatiment(abbaye);
    village->ajouterBatiment(manoir);

    if (!taverne) {
        std::cerr << "ERREUR CRITIQUE: taverne est NULL!" << std::endl;
    }

    Villageois* bernard = new Villageois(0, "Bernard le Fermier");
    Villageois* otto = new Villageois(1, "Otto le Fermier");
    Villageois* irmengard = new Villageois(2, "Irmengard la Bergère");
    Villageois* otbert = new Villageois(3, "Otbert le Meunier");
    Villageois* hildeburg = new Villageois(4, "Hildeburg la Boulangère");
    Villageois* bob = new Villageois(5, "Bob le Tavernier");
    Villageois* adalind = new Villageois(6, "Adalind la Forgeronne");
    Villageois* pippin = new Villageois(7, "Pippin le Mineur");
    Villageois* sygarius = new Villageois(8, "Père Sygarius");
    Villageois* mathildis = new Villageois(9, "Sr. Mathildis d'Alenrac");

    bernard->setLieuTravail(champ);
    otto->setLieuTravail(champ);
    irmengard->setLieuTravail(paturage);
    otbert->setLieuTravail(moulin);
    hildeburg->setLieuTravail(boulangerie);
    bob->setLieuTravail(taverne);
    adalind->setLieuTravail(forge);
    pippin->setLieuTravail(mine);
    sygarius->setLieuTravail(abbaye);
    mathildis->setLieuTravail(manoir);

    village->ajouterVillageois(bernard);
    village->ajouterVillageois(otto);
    village->ajouterVillageois(irmengard);
    village->ajouterVillageois(otbert);
    village->ajouterVillageois(hildeburg);
    village->ajouterVillageois(bob);
    village->ajouterVillageois(adalind);
    village->ajouterVillageois(pippin);
    village->ajouterVillageois(sygarius);
    village->ajouterVillageois(mathildis);

    for (Villageois* v : village->getPopulation()) {
        v->setEmplacementTaverne(taverne);
    }

    // gérer les chaines de prod
    // ex: quand bernard a plus de 5 blé il faut qu'il aille livrer ça au meunier
    bernard->regleLivraison = {"Blé", 5, moulin};
    otto->regleLivraison = {"Blé", 10, taverne};
    irmengard->regleLivraison = {"Moutons", 5, taverne};
    otbert->regleLivraison = {"Farine", 10, boulangerie};
    hildeburg->regleLivraison = {"Pain", 3, taverne};
    pippin->regleLivraison = {"Acier", 10, forge};

    // villageois qui ne livrent rien
    bob->regleLivraison = {"", 0, nullptr};
    adalind->regleLivraison = {"", 0, nullptr};
    sygarius->regleLivraison = {"", 0, nullptr};
    mathildis->regleLivraison = {"", 0, nullptr};

    std::cout << "Village initialisé avec succès.";

    // interface
    connect(ui->pushButton, &QPushButton::clicked,
            this, &MainWindow::onPushButtonClicked);
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboBox_currentIndexChanged);

    // SAUVEGARDE INFOS VILLAGE
    connect(ui->boutonSauvegarde, &QPushButton::clicked, [=]() {
        village->sauvegarder("DataVillage.txt");
    });

    // importer villageois
    connect(ui->boutonImporter, &QPushButton::clicked, this, &MainWindow::onBoutonImporterClicked);

    // remplir le ComboBox automatiquement avec les noms des villageois
    for (Villageois* v : village->getPopulation()) {
        ui->comboBox->addItem(QString::fromStdString(v->getNom()));
    }

    // affiche le premier villageois par défaut
    if (!village->getPopulation().empty()) {
        updateVillageoisDisplay();
    }
}

// destructeur
MainWindow::~MainWindow()
{
    std::cout.rdbuf(oldCoutBuf);
    delete qtCoutBuf;
    delete village;
    delete ui;
}

void MainWindow::updateUI()
{
    // indications visuelles quand les bâtiments produisent
    // TODO: à finir
    // if (champ->getStatut() == StatutBatiment::PRODUIT) {
    //     ui->iconeProdChamp->show();
    // } else {
    //     ui->iconeProdChamp->hide();
    // }
    // ...

    int jour = village->getJour();
    int heureActuelle = village->getTickActuel() % village->getTicksParJour();

    ui->jour->setText(QString::number(jour));
    ui->heure->setText(QString::number(heureActuelle));

    if (heureActuelle >= 6 && heureActuelle < 18) {
        ui->timebox->setTitle("🌞");
    } else {
        ui->timebox->setTitle("🌙");
    }

    updateVillageoisDisplay();
}


// avancer d'une heure/tick
void MainWindow::onPushButtonClicked()
{
    village->tick();
    std::cout << "Tick terminé.";
    updateUI();
}

// met à jour l'infobox des villageois
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    updateVillageoisDisplay();
}

void MainWindow::updateVillageoisDisplay()
{
    int index = ui->comboBox->currentIndex();

    if (index < 0 || index >= village->getPopulation().size()) {
        return;  // pas de sélection valide
    }

    // récupérer le villageois sélectionné
    Villageois* villageois = village->getPopulation()[index];

    // afficher l'image selon le villageois
    QString imagePath;
    switch (index) {
    case 0:
        imagePath = ":/assets/assets/bernard.jpg";
        break;
    case 1:
        imagePath = ":/assets/assets/otto.jpg";
        break;
    case 2:
        imagePath = ":/assets/assets/irmengard.jpg";
        break;
    case 3:
        imagePath = ":/assets/assets/otbert.jpg";
        break;
    case 4:
        imagePath = ":/assets/assets/hildeburg.jpg";
        break;
    case 5:
        imagePath = ":/assets/assets/bob.jpg";
        break;
    case 6:
        imagePath = ":/assets/assets/adalind.jpg";
        break;
    case 7:
        imagePath = ":/assets/assets/pippin.jpg";
        break;
    case 8:
        imagePath = ":/assets/assets/sygarius.jpg";
        break;
    case 9:
        imagePath = ":/assets/assets/mathildis.jpg";
        break;
    default:
        imagePath = ":/assets/assets/default.png";
        break;
    }

    QPixmap pixmap(imagePath);
    ui->iconeVillageois->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio));

    // affichage infos
    QString infoText;
    infoText += "Nom : " + QString::fromStdString(villageois->getNom()) + "\n";
    infoText += "ID : " + QString::number(villageois->getId()) + "\n";
    infoText += "Statut : " + QString::fromStdString(villageois->getStatutString()) + "\n";

    if (villageois->getLieuTravail()) {
        infoText += "Travail : " + QString::fromStdString(villageois->getLieuTravail()->getNom()) + "\n";
    }

    // inventaire
    infoText += "\nInventaire :\n";
    for (const auto& [ressource, quantite] : villageois->getInventaire()) {
        infoText += "  - " + QString::fromStdString(ressource) + " : " + QString::number(quantite) + "\n";
    }
    ui->infoViewVillageois->setText(infoText);
}

void MainWindow::onBoutonImporterClicked() {
    // ouvre une fenêtre 'parcourir'
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Choisir un fichier de villageois",
        "",
        "Fichiers texte (*.txt);;Tous les fichiers (*)"
        );

    if (!fileName.isEmpty()) {
        village->importerVillageois(fileName.toStdString());

        for (Villageois* v : village->getPopulation()) {
            if (ui->comboBox->findText(QString::fromStdString(v->getNom())) == -1) {
                ui->comboBox->addItem(QString::fromStdString(v->getNom()));
            }
        }
    }
}


void MainWindow::onCheckboxChanged()
{
    // si le bouton "tout" est séléctionné on séléctionne le reste automatiquement
    if (ui->checkBoxTout->isChecked()) {
        ui->checkBoxEconomie->setChecked(true);
        ui->checkBoxProduction->setChecked(true);
        ui->checkBoxRessources->setChecked(true);
        ui->checkBoxVillageois->setChecked(true);

        // mais on les disable
        ui->checkBoxEconomie->setEnabled(false);
        ui->checkBoxProduction->setEnabled(false);
        ui->checkBoxRessources->setEnabled(false);
        ui->checkBoxVillageois->setEnabled(false);

        Logger::setFilter(LogCategory::TOUT); }

        // et vice versa
        else {
            ui->checkBoxEconomie->setEnabled(true);
            ui->checkBoxProduction->setEnabled(true);
            ui->checkBoxRessources->setEnabled(true);
            ui->checkBoxVillageois->setEnabled(true);

            // si on déséléctionne un bouton, ça déséléctionne "tout"
            if (!ui->checkBoxEconomie->isChecked() ||
                !ui->checkBoxProduction->isChecked() ||
                !ui->checkBoxRessources->isChecked() ||
                !ui->checkBoxVillageois->isChecked()) {
                ui->checkBoxTout->setChecked(false);
            }

        if (ui->checkBoxTout->isChecked()) {
            Logger::setFilter(LogCategory::TOUT);
        } else if (ui->checkBoxVillageois->isChecked()) {
            Logger::setFilter(LogCategory::VILLAGEOIS);
        } else if (ui->checkBoxProduction->isChecked()) {
            Logger::setFilter(LogCategory::PRODUCTION);
        } else if (ui->checkBoxRessources->isChecked()) {
            Logger::setFilter(LogCategory::RESSOURCES);
        } else if (ui->checkBoxEconomie->isChecked()) {
            Logger::setFilter(LogCategory::ECONOMIE);
        } else if (ui->checkBoxSysteme->isChecked()) {
            Logger::setFilter(LogCategory::SYSTEME);
        }

        std::cout << "Filtre de log modifié. \n" << std::endl;
        }
}


