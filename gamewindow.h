#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QTimer>
#include <QProgressBar>

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);

signals:
    void gameFinished(int score, int wordsFound, bool won); 

private slots:
    void letterClicked();
    void returnToMenu();

private:
    void setupUI();
    void loadDictionary();
    void startNewGame();
    void updateWordDisplay();
    void updateHangmanImage();
    void checkGameOver();
    void calculateScore();
    void showFinalScore(); 
    void askPlayerName(); 
    void continueGame(); 
    void updateScoreDisplay();
    void saveScore();
    
    // Game data
    QStringList dictionary;
    QString currentWord;
    QString guessedWord;
    QStringList guessedLetters;
    int wrongGuesses;
    int score;
    int totalScore;
    int wordsFound; 
    bool gameInProgress; 
    QString playerName;

    
    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QLabel *hangmanLabel;
    QLabel *wordLabel;
    QLabel *scoreLabel;
    QLabel *wrongLabel;
    QGridLayout *letterGrid;
    QWidget *letterWidget;
    QPushButton *returnButton;
    QProgressBar *progressBar;
    
    QList<QPushButton*> letterButtons;
    
    static const int MAX_WRONG_GUESSES = 7;
};

#endif // GAMEWINDOW_H