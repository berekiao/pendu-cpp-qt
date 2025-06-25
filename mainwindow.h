#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenuBar>
#include <QMessageBox>
#include <QTextEdit>
#include <QInputDialog>
#include <QTableWidget>
#include <QHeaderView>
#include "gamewindow.h"
#include "scoremanager.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGame();
    void showScores();
    void showRules();
    void showAbout();
    void onGameFinished(int score, int wordsFound, bool won);

private:
    void setupUI();
    void setupMenus();
    void checkAndSaveScore(int score, int wordsFound);
    
    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLabel *welcomeLabel;
    QPushButton *newGameButton;
    
    // Windows
    GameWindow *gameWindow = nullptr; // Initialisation à nullptr
    ScoreManager *scoreManager;
};

#endif // MAINWINDOW_H