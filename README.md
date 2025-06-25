# 🎮 Jeu du Pendu - C++/Qt

Un jeu du pendu moderne développé en C++ avec Qt, offrant une interface graphique intuitive et un système de scores avancé.

## 🎯 Aperçu

Ce projet est un jeu du pendu développé en C++/Qt . Il implémente toutes les fonctionnalités d'un jeu du pendu classique avec des améliorations modernes comme la sauvegarde des scores et une interface utilisateur soignée.

## ✨ Fonctionnalités

### Fonctionnalités principales
- 🎲 **Jeu du pendu complet** avec interface graphique Qt
- 🏆 **Système de scores** avec sauvegarde des 10 meilleurs scores
- 📱 **Interface utilisateur moderne** et responsive
- 🎨 **Ressources graphiques** intégrées (images du pendu)
- 📚 **Dictionnaire de mots** extensible
- 💾 **Sauvegarde automatique** des données

### Fonctionnalités avancées
- **Menu complet** : Nouveau jeu, Scores, Règles, À propos
- **Page d'accueil** accueillante
- **Système de points** basé sur les mots trouvés et les erreurs
- **Gestion du top 10** avec demande de pseudo
- **Règles intégrées** pour les nouveaux joueurs

## 🔧 Prérequis

Avant de compiler et exécuter le projet, assurez-vous d'avoir :

- **Qt 5.12+** ou **Qt 6.x**
- **Compilateur C++** compatible (GCC, Clang, MSVC)
- **CMake 3.16+** ou **qmake**
- **Git** pour cloner le projet

### Systèmes supportés
- 🖥️ Windows 10/11
- 🐧 Linux (Ubuntu 20.04+, Debian, Fedora)
- 🍎 macOS 10.15+

## 🚀 Installation

### 1. Cloner le repository
```bash
git clone https://github.com/votre-username/pendu-cpp-qt.git
cd pendu-cpp-qt
```

### 2. Compilation avec Qt Creator
1. Ouvrez Qt Creator
2. Dézipper le fichier des images
2. Ouvrez le fichier `CMakeLists.txt` 
4. Configurez le projet pour votre kit de développement
5. Compilez et exécutez

### 3. Compilation en ligne de commande

#### Avec CMake
```bash
mkdir build
cd build
cmake ..
make
./pendu
```

#### Avec qmake
```bash
qmake
make
./pendu
```

## 🎮 Utilisation

### Démarrage rapide
1. Lancez l'application
2. Cliquez sur "Nouveau jeu" depuis le menu
3. Devinez les lettres du mot mystère
4. Évitez de faire trop d'erreurs !

### Navigation
- **Nouveau** : Démarre une nouvelle partie
- **Scores** : Affiche le classement des meilleurs joueurs
- **Règles** : Explique les règles du jeu
- **À propos** : Informations sur l'application


## 📖 Règles du jeu

### Objectif
Deviner le mot mystère en proposant des lettres, tout en évitant de faire trop d'erreurs.

### Déroulement
1. Un mot est choisi aléatoirement dans le dictionnaire
2. Vous proposez des lettres une par une
3. Si la lettre est dans le mot, elle s'affiche
4. Si la lettre n'est pas dans le mot, une partie du pendu se dessine
5. Vous gagnez en trouvant toutes les lettres
6. Vous perdez si le dessin du pendu est terminé (6 erreurs)

### Système de points
- **Mot trouvé** : Points basés sur la longueur du mot
- **Erreurs** : Réduction des points selon le nombre d'erreurs
- **Bonus** : Multiplicateur pour les mots difficiles

