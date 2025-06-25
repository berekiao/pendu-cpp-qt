#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <QObject>
#include <QSettings>
#include <QPair>
#include <QList>

class ScoreManager : public QObject
{
    Q_OBJECT

public:
    explicit ScoreManager(QObject *parent = nullptr);
    
    void addScore(const QString &playerName, int score, int wordsFound);
    QList<QPair<QString, QPair<int, int>>> getTopScores() const;
    bool isTopScore(int score) const;
    void clearScores();

private:
    void loadScores();
    void saveScores();
    
    QList<QPair<QString, QPair<int, int>>> scores;
    QSettings *settings;
    
    static const int MAX_SCORES = 10;
};

#endif // SCOREMANAGER_H