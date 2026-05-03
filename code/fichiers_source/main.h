#include <QMainWindow>
#include <qgroupbox.h>
#include <streambuf>

#include "Champ.hpp"
#include "Taverne.hpp"
#include "Village.hpp"
#include "qtstreambuf.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPushButtonClicked();
    void on_comboBox_currentIndexChanged(int index);
    void onCheckboxChanged();
    void onBoutonImporterClicked();

private:
    Champ* champ;
    Ui::MainWindow *ui;


    Village* village;
    Taverne* taverne;
    QtStreamBuf* qtCoutBuf;
    std::streambuf* oldCoutBuf;

    void updateUI();
    void updateVillageoisDisplay();

};
