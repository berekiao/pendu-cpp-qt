#include "gamewindow.h"
#include <QApplication>
#include <QDir>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QStringConverter>
#endif

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent),
      wrongGuesses(0),
      score(0),
      totalScore(0),
      wordsFound(0),
      gameInProgress(false),
      playerName("")
{
    setupUI();
    loadDictionary();
    startNewGame();
}

void GameWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    // Titre et bouton retour
    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *titleLabel = new QLabel("JEU DU PENDU");
    titleLabel->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color:rgb(237, 242, 247); }");

    returnButton = new QPushButton("← Retour au menu");
    returnButton->setStyleSheet("QPushButton { padding: 8px 16px; background-color: #95a5a6; color: white; border: none; border-radius: 4px; } QPushButton:hover { background-color: #7f8c8d; }");
    connect(returnButton, &QPushButton::clicked, this, &GameWindow::returnToMenu);

    topLayout->addWidget(titleLabel);
    topLayout->addStretch();
    topLayout->addWidget(returnButton);

    // Image du pendu
    hangmanLabel = new QLabel(this);
    hangmanLabel->setAlignment(Qt::AlignCenter);
    hangmanLabel->setMinimumSize(200, 200);
    hangmanLabel->setStyleSheet("QLabel { border: 2px solid #bdc3c7; border-radius: 8px; background-color: white; }");

    // Mot à deviner
    wordLabel = new QLabel(this);
    wordLabel->setAlignment(Qt::AlignCenter);
    wordLabel->setStyleSheet("QLabel { font-size: 32px; font-weight: bold; color: #2c3e50; margin: 20px; letter-spacing: 8px; }");

    // Informations de score et erreurs
    QHBoxLayout *infoLayout = new QHBoxLayout();
    scoreLabel = new QLabel("Score: 0");
    scoreLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: #27ae60; }");

    wrongLabel = new QLabel("Erreurs: 0/6");
    wrongLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: #27ae60; }"); // Commence en vert

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, MAX_WRONG_GUESSES);
    progressBar->setValue(0);
    progressBar->setStyleSheet("QProgressBar { border: 2px solid #bdc3c7; border-radius: 4px; background-color: #ecf0f1; } QProgressBar::chunk { background-color: #e74c3c; border-radius: 2px; }");

    infoLayout->addWidget(scoreLabel);
    infoLayout->addWidget(progressBar);
    infoLayout->addWidget(wrongLabel);

    // Grille des lettres
    letterWidget = new QWidget(this);
    letterGrid = new QGridLayout(letterWidget);
    letterGrid->setSpacing(5);

    // Créer les boutons pour chaque lettre
    QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < alphabet.length(); ++i)
    {
        QPushButton *button = new QPushButton(alphabet[i], this);
        button->setFixedSize(40, 40);
        button->setStyleSheet("QPushButton { font-size: 14px; font-weight: bold; background-color: #3498db; color: white; border: none; border-radius: 4px; } QPushButton:hover { background-color: #2980b9; } QPushButton:disabled { background-color: #bdc3c7; }");
        connect(button, &QPushButton::clicked, this, &GameWindow::letterClicked);
        letterButtons.append(button);
        letterGrid->addWidget(button, i / 9, i % 9);
    }

    // Assemblage du layout
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(hangmanLabel);
    mainLayout->addWidget(wordLabel);
    mainLayout->addLayout(infoLayout);
    mainLayout->addWidget(progressBar);
    mainLayout->addWidget(letterWidget);
    mainLayout->addStretch();

    setWindowTitle("Jeu du Pendu - Partie en cours");
    resize(800, 700);

    updateHangmanImage();
}

void GameWindow::loadDictionary()
{
    QFile file(":/dictionnaire.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        in.setCodec("UTF-8");
#else
        in.setEncoding(QStringConverter::Utf8);
#endif
        while (!in.atEnd())
        {
            QString word = in.readLine().trimmed().toUpper();
            if (word.length() >= 4)
            {
                dictionary.append(word);
            }
        }
        file.close();
    }
}

void GameWindow::startNewGame()
{
    wrongGuesses = 0;
    score = 100; // Score de base pour chaque nouveau mot

    // Choisir un nouveau mot aléatoire
    if (!dictionary.isEmpty())
    {
        currentWord = dictionary.at(QRandomGenerator::global()->bounded(dictionary.size()));
    }
    else
    {
        currentWord = "PENDU";
    }

    guessedWord = QString("_").repeated(currentWord.length());
    guessedLetters.clear();

    for (QPushButton *button : letterButtons)
    {
        button->setEnabled(true);
    }

    updateWordDisplay();
    updateHangmanImage();
    updateScoreDisplay();
    gameInProgress = true;
}

void GameWindow::letterClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button)
        return;

    QString letter = button->text();
    button->setEnabled(false);
    guessedLetters.append(letter);

    bool found = false;
    for (int i = 0; i < currentWord.length(); ++i)
    {
        if (currentWord[i] == letter[0])
        {
            guessedWord[i] = letter[0];
            found = true;
        }
    }

    if (!found)
    {
        wrongGuesses++;
        updateHangmanImage();
        progressBar->setValue(wrongGuesses);
    }

    updateWordDisplay();
    calculateScore();
    checkGameOver();
}

void GameWindow::updateWordDisplay()
{
    QString display = "";
    for (int i = 0; i < guessedWord.length(); ++i)
    {
        display += guessedWord[i];
        if (i < guessedWord.length() - 1)
        {
            display += " ";
        }
    }
    wordLabel->setText(display);
}

void GameWindow::updateHangmanImage()
{
    if (wrongGuesses == 0)
    {
        // Pas encore d'erreur → pas d'image
        hangmanLabel->clear();
        hangmanLabel->setText("🎯 Nouvelle partie !\nAucune erreur");
        hangmanLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: #27ae60; border: 2px solid #bdc3c7; border-radius: 8px; background-color: white; padding: 20px; }");
        wrongLabel->setText(QString("Erreurs: 0/%1").arg(MAX_WRONG_GUESSES));
        wrongLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: #27ae60; }");
        return;
    }

    int imageIndex = wrongGuesses - 1;
    QString imagePath = QString(":/images/Pendu-%1.png").arg(imageIndex);
    QPixmap pixmap(imagePath);

    if (pixmap.isNull())
    {
        hangmanLabel->setText(QString("❌ %1 erreur(s) sur %2").arg(wrongGuesses).arg(MAX_WRONG_GUESSES));
        hangmanLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: #e74c3c; border: 2px solid #bdc3c7; border-radius: 8px; background-color: white; padding: 20px; }");
    }
    else
    {
        pixmap = pixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        hangmanLabel->setPixmap(pixmap);
        hangmanLabel->setStyleSheet("QLabel { border: 2px solid #bdc3c7; border-radius: 8px; background-color: white; }");
    }

    QString errorStyle;
    if (wrongGuesses < 4)
    {
        errorStyle = "QLabel { font-size: 16px; font-weight: bold; color: #f39c12; }"; // Orange
    }
    else
    {
        errorStyle = "QLabel { font-size: 16px; font-weight: bold; color: #e74c3c; }"; // Rouge
    }

    wrongLabel->setText(QString("Erreurs: %1/%2").arg(wrongGuesses).arg(MAX_WRONG_GUESSES));
    wrongLabel->setStyleSheet(errorStyle);
}

void GameWindow::checkGameOver()
{
    bool won = !guessedWord.contains('_');
    bool lost = wrongGuesses >= MAX_WRONG_GUESSES;

    if (won || lost)
    {
        // Désactiver toutes les lettres
        for (QPushButton *button : letterButtons) {
            button->setEnabled(false);
        }

        if (won)
        {
            // Calculer le score final pour ce mot
            calculateScore();
            totalScore += score;
            wordsFound++;
            
            QTimer::singleShot(1500, this, [this]() {
                QMessageBox::information(this, "Mot trouvé!", 
                    QString("Bravo! Vous avez gagné %1 points pour ce mot\n\nScore total: %2 points")
                        .arg(score)
                        .arg(totalScore));
                continueGame();
            });
        }
        else
        {
            QTimer::singleShot(1500, this, [this]() {
                QMessageBox::information(this, "Perdu", 
                    QString("Dommage! Le mot était: %1\n\nScore total: %2 points")
                        .arg(currentWord)
                        .arg(totalScore));
                
                // Ne demander le nom que si au moins un mot a été trouvé
                if (wordsFound > 0) {
                    askPlayerName();
                } else {
                    emit gameFinished(totalScore, wordsFound, false);
                }
            });
        }
    }
}

void GameWindow::calculateScore()
{
    // Réinitialiser le score pour ce mot
    score = 0;

    // Calcul du score uniquement si le mot est complet
    if (!guessedWord.contains('_'))
    {
        // Appliquer le barème exact selon le nombre d'erreurs
        switch(wrongGuesses)
        {
            case 0: score = 100; break;  // Sans erreur
            case 1: score = 50;  break;
            case 2: score = 35;  break;
            case 3: score = 25;  break;
            case 4: score = 15;  break;
            case 5: score = 10;  break;
            case 6: score = 5;   break;
            default: score = 0;  break; // Plus de 6 erreurs = 0 point
        }
    }

    // Mise à jour de l'affichage
    scoreLabel->setText(QString("Score: %1").arg(score));
}

void GameWindow::continueGame()
{
    startNewGame();
    updateScoreDisplay();
}

void GameWindow::showFinalScore()
{
    QMessageBox::information(this, "Partie terminée",
                             QString("Score final: %1 points\nMots trouvés: %2")
                                 .arg(totalScore)
                                 .arg(wordsFound));
}

void GameWindow::askPlayerName()
{
    bool ok;
    QString name = QInputDialog::getText(this,
                                       "Nouveau record!",
                                       QString("Félicitations! Vous avez trouvé %1 mot(s) avec %2 points.\nEntrez votre pseudo:")
                                           .arg(wordsFound)
                                           .arg(totalScore),
                                       QLineEdit::Normal,
                                       "",
                                       &ok);

    if (ok && !name.isEmpty()) {
        playerName = name;
        saveScore();
        emit gameFinished(totalScore, wordsFound, true);
    } else {
        emit gameFinished(totalScore, wordsFound, false);
    }
}

void GameWindow::updateScoreDisplay()
{
    scoreLabel->setText(QString("Score: %1 (Total: %2)")
                            .arg(score)
                            .arg(totalScore));
    wrongLabel->setText(QString("Mots trouvés: %1").arg(wordsFound));
}

void GameWindow::saveScore()
{
    // À implémenter plus tard
    qDebug() << "Score à sauvegarder:" << totalScore << "par" << playerName;
}

void GameWindow::returnToMenu()
{
    if (gameInProgress)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Quitter",
                                      "Voulez-vous vraiment quitter? Votre score ne sera pas sauvegardé.",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            emit gameFinished(0, 0, false);
        }
    }
    else
    {
        emit gameFinished(totalScore, wordsFound, (wordsFound > 0));
    }
}