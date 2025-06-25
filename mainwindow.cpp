#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gameWindow(nullptr)
{
    scoreManager = new ScoreManager(this);
    setupUI();
    setupMenus();
}

MainWindow::~MainWindow()
{
    if (gameWindow)
    {
        delete gameWindow;
    }
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    // Titre principal
    titleLabel = new QLabel("Bienvenue dans le jeu du PENDU", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 28px; font-weight: bold; color:rgb(106, 110, 114); margin: 20px; }");

    // Message de bienvenue
    welcomeLabel = new QLabel("Vous avez 7 coups pour trouver le mot caché ! Et si vous réussissez : on recommence ! Plus vous avez trouvé des mots, plus votre score grandira !!! Alors à vous de jouer!\n\nJe vous souhaite bien du plaisir ….Et, si vous pensez pouvoir trouver un mot en un coup, c’est que vous pensez que le dictionnaire est petit ! Hors, pour votre information, il comprend plus de 336 000 mots… Donc bonne chance!! \n\n Cliquez sur 'Nouveau Jeu' pour commencer ou utilisez le menu pour accéder aux autres options.", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("QLabel { font-size: 14px; color:rgb(237, 238, 240); margin: 20px; line-height: 1.6; }");
    welcomeLabel->setWordWrap(true);

    // Bouton nouveau jeu
    newGameButton = new QPushButton("Nouveau Jeu", this);
    newGameButton->setStyleSheet("QPushButton { font-size: 16px; padding: 15px 30px; background-color: #3498db; color: white; border: none; border-radius: 8px; } QPushButton:hover { background-color: #2980b9; }");

    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(welcomeLabel);
    mainLayout->addWidget(newGameButton, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::newGame);

    setWindowTitle("Jeu du Pendu");
    resize(600, 400);
}

void MainWindow::setupMenus()
{
    QMenuBar *menuBar = this->menuBar();

    // Menu Nouveau
    QMenu *newMenu = menuBar->addMenu("&Nouveau");
    QAction *newGameAction = newMenu->addAction("&Nouvelle partie");
    newGameAction->setShortcut(QKeySequence::New);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);

    // Menu Scores
    QMenu *scoresMenu = menuBar->addMenu("&Scores");
    QAction *showScoresAction = scoresMenu->addAction("&Voir les scores");
    connect(showScoresAction, &QAction::triggered, this, &MainWindow::showScores);

    // Menu Règles
    QMenu *rulesMenu = menuBar->addMenu("&Règles");
    QAction *rulesAction = rulesMenu->addAction("&Voir les règles");
    connect(rulesAction, &QAction::triggered, this, &MainWindow::showRules);

    // Menu À propos
    QMenu *aboutMenu = menuBar->addMenu("À &propos");
    QAction *aboutAction = aboutMenu->addAction("&À propos");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::newGame()
{
    // Ferme l'ancienne fenêtre de jeu si elle existe
    if (gameWindow) {
        gameWindow->close();
        gameWindow->deleteLater();
    }

    // Crée une nouvelle fenêtre de jeu
    gameWindow = new GameWindow(this);
    gameWindow->setAttribute(Qt::WA_DeleteOnClose); // Gestion automatique de la mémoire

    // Connecte le signal de fin de jeu
    connect(gameWindow, &GameWindow::gameFinished, this, &MainWindow::onGameFinished);

    // Cache la fenêtre principale et montre la fenêtre de jeu
    this->hide();
    gameWindow->show();
    gameWindow->activateWindow(); // Donne le focus
}

void MainWindow::showScores()
{
    QDialog *scoresDialog = new QDialog(this);
    scoresDialog->setWindowTitle("Top 10 des meilleurs scores");
    scoresDialog->resize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(scoresDialog);

    QLabel *titleLabel = new QLabel("🏆 TOP 10 DES MEILLEURS SCORES 🏆");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #2c3e50; margin: 10px; }");

    QTableWidget *table = new QTableWidget(scoresDialog);
    table->setColumnCount(4); // Changement à 4 colonnes
    QStringList headers;
    headers << "Rang" << "Pseudo" << "Score" << "Mots trouvés"; // Ajout de la colonne mots trouvés
    table->setHorizontalHeaderLabels(headers);

    // Utilisation du bon type de retour
    auto scores = scoreManager->getTopScores();
    table->setRowCount(scores.size());

    for (int i = 0; i < scores.size(); ++i)
    {
        const auto& entry = scores[i];
        table->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        table->setItem(i, 1, new QTableWidgetItem(entry.first)); // Pseudo
        table->setItem(i, 2, new QTableWidgetItem(QString::number(entry.second.first))); // Score
        table->setItem(i, 3, new QTableWidgetItem(QString::number(entry.second.second))); // Mots trouvés
    }

    table->horizontalHeader()->setStretchLastSection(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    QPushButton *closeButton = new QPushButton("Fermer", scoresDialog);
    connect(closeButton, &QPushButton::clicked, scoresDialog, &QDialog::accept);

    layout->addWidget(titleLabel);
    layout->addWidget(table);
    layout->addWidget(closeButton);

    scoresDialog->exec();
}

void MainWindow::showRules()
{
    QDialog *rulesDialog = new QDialog(this);
    rulesDialog->setWindowTitle("Règles du jeu");
    rulesDialog->resize(600, 500);

    QVBoxLayout *layout = new QVBoxLayout(rulesDialog);

    QLabel *titleLabel = new QLabel("RÈGLES DU JEU DU PENDU");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #2c3e50; margin: 10px; }");

    QTextEdit *rulesText = new QTextEdit(rulesDialog);
    rulesText->setReadOnly(true);
    rulesText->setHtml(
        "<h3>🎯 Objectif :</h3>"
        "<p>Deviner le mot caché en proposant des lettres une par une.</p>"

        "<h3>Règles :</h3>"
        "<p> Vous avez 7 coups pour trouver le mot caché ! Et si vous réussissez : on recommence ! Plus vous avez trouvé des mots, plus votre score grandira !!! Alors à vous de jouer!</p>"

        "<h3>Système de points :</h3>"
        "<ul>"
        "<li><b>Mot trouvé sans erreur :</b> 100 points</li>"
        "<li><b>Mot trouvé avec 1 erreur :</b> 50pts</li>"
        "<li><b>Mot trouvé avec 2 erreur :</b> 35pts</li>"
        "<li><b>Mot trouvé avec 3 erreur :</b> 25pts</li>"
        "<li><b>Mot trouvé avec 4 erreur :</b> 15pts</li>"
        "<li><b>Mot trouvé avec 5 erreur :</b> 10pts</li>"
        "<li><b>Mot trouvé avec 6 erreur :</b> 5pts</li>"
        "</ul>");

    QPushButton *closeButton = new QPushButton("Fermer", rulesDialog);
    connect(closeButton, &QPushButton::clicked, rulesDialog, &QDialog::accept);

    layout->addWidget(titleLabel);
    layout->addWidget(rulesText);
    layout->addWidget(closeButton);

    rulesDialog->exec();
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "À propos",
                       "<b>Jeu du Pendu</b><br><br>"
                       "Version 1.0<br><br>"
                       "Un jeu classique du pendu développé avec Qt.<br><br>"
                       "Fonctionnalités :<br>"
                       "• Système de scores<br>"
                       "• Sauvegarde des 10 meilleurs scores<br>"
                       "• Interface intuitive<br>"
                       "• Dictionnaire de mots français<br>"
                       "• Développé par nous<br><br>"
                       );
}

void MainWindow::onGameFinished(int score, int wordsFound, bool won)
{
    this->show();
    
    if (won && wordsFound > 0) {
        checkAndSaveScore(score, wordsFound);
    } else {
        QString message = QString("Partie terminée\nScore: %1\nMots trouvés: %2")
                            .arg(score).arg(wordsFound);
        QMessageBox::information(this, "Résultat", message);
    }
}

void MainWindow::checkAndSaveScore(int score, int wordsFound)
{
    if (scoreManager->isTopScore(score))
    {
        bool ok;
        QString pseudo = QInputDialog::getText(this, "Nouveau record !",
                                               QString("Félicitations ! Vous avez trouvé %1 mots avec %2 points.\n\nEntrez votre pseudo :")
                                                   .arg(wordsFound)
                                                   .arg(score),
                                               QLineEdit::Normal, "", &ok);

        if (ok && !pseudo.isEmpty())
        {
            scoreManager->addScore(pseudo, score, wordsFound);
            showScores();
        }
    }
    else
    {
        QMessageBox::information(this, "Partie terminée",
                                 QString("Bien joué ! Vous avez trouvé %1 mots avec %2 points.\n\nContinuez à jouer pour améliorer votre score !")
                                     .arg(wordsFound)
                                     .arg(score));
    }
}