# Polytechnique Montréal
Département de génie informatique et génie logiciel
INF1900: Projet initial de système embarqué

## >>> Auteurs <<<
Sikouky, Abdelnour
Brossier, Sébastien
Maximciuc, Irina
Trudel, Jérémy

## >>> Description <<<
Ce projet est dans le cadre du cours INF1900,
Il a pour objectif : 
	- Identifier le coin.
	- Effectuer un trajet.

## >>> Compilation <<<
-make clean: 
	pour la librairie: va au dossier lib lance %make clean pour retirer tous les fichiers inutiles(.d et .o) présents dans la librairie 
-make : dans le dossier lib: lance %make clean puis %make pour compiler la librairie.
	dans le dossier app : lance %make clean puis make pour compiler le projet.

-make install:
	Dans le dossier app : lance %make install pour installer le programme dans le robot.

## >>> Classes <<<
- Can : Classe permettant de convertir des données analogiques en données numériques.
- Gps : Classe de l'algorithme djikstra pour effectuer un trajet.
- Interface : Classe qui gère l'affichage dans le LCD
- Led : Classe qui permet de gérer les lumières.
- Navigation: Classe permettant le déplacement du robot.
- Piezo : Classe permettant d'émettre un son.
- Robot : Classe qui rassemble toutes les classes et les challenges.
- SensorLine: Classe pour les directions du capteur de ligne.
- SharpIR: Classe pour le capteur Infrarouge pour détecter s'il y'a un poteau.
- Timer: Classe pour les deux timers 0 et 2.
- Timer1: Classe pour le timer 1.

