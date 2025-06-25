#include "scoremanager.h"
#include <QDebug>

ScoreManager::ScoreManager(QObject *parent)
    : QObject(parent)
{
    settings = new QSettings(this);
    loadScores();
}

void ScoreManager::addScore(const QString &playerName, int score, int wordsFound)
{
    // Ajouter le nouveau score avec wordsFound
    scores.append(qMakePair(playerName, qMakePair(score, wordsFound)));
    
    // Trier par score décroissant
    std::sort(scores.begin(), scores.end(), [](const auto &a, const auto &b) {
        return a.second.first > b.second.first; // Compare les scores
    });
    
    // Garder seulement les 10 meilleurs
    if (scores.size() > MAX_SCORES) {
        scores.resize(MAX_SCORES);
    }
    
    saveScores();
}

QList<QPair<QString, QPair<int, int>>> ScoreManager::getTopScores() const
{
    return scores;
}

bool ScoreManager::isTopScore(int score) const
{
    if (scores.size() < MAX_SCORES) {
        return true;
    }
    return score > scores.last().second.first; // Accès au score via .first
}

void ScoreManager::clearScores()
{
    scores.clear();
    saveScores();
}

void ScoreManager::loadScores()
{
    scores.clear();
    
    int size = settings->beginReadArray("scores");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        QString name = settings->value("name").toString();
        int score = settings->value("score").toInt();
        int wordsFound = settings->value("wordsFound", 0).toInt(); // Valeur par défaut 0
        scores.append(qMakePair(name, qMakePair(score, wordsFound)));
    }
    settings->endArray();
}

void ScoreManager::saveScores()
{
    settings->beginWriteArray("scores");
    for (int i = 0; i < scores.size(); ++i) {
        settings->setArrayIndex(i);
        settings->setValue("name", scores[i].first);
        settings->setValue("score", scores[i].second.first); // Sauvegarde le score
        settings->setValue("wordsFound", scores[i].second.second); // Sauvegarde wordsFound
    }
    settings->endArray();
    settings->sync();
}