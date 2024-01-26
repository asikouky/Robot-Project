# Description

Ce projet est dans le cadre du cours INF1900,
Il a pour objectif : 
- Identifier le coin où se situe le robot et vers quelle direction.
- Effectuer un trajet d'un point A a un point B.

# Compilation
pour la [librairie](https://github.com/asikouky/Robot-Project/tree/main/lib): va au dossier lib:
```bash
% cd lib
```
 lance  :

```bash
% make clean
```
pour retirer tous les fichiers inutiles(.d et .o) présents dans la librairie 

pour installer le programme dans le robot va au dossier [app](https://github.com/asikouky/Robot-Project/tree/main/app):
```bash
% cd app
```
lance  :

```bash
% make clean
```
pour retirer tous les fichiers inutiles(.d et .o) présents.

```bash
% make
```
pour compiler le programme.

```bash
% make install
```
pour installer le programme dans le robot.

Ou lance:

# Classes
- Can: 
```bash
Classe permettant de convertir des données analogiques en données numériques.
```
 - Gps :
```bash
Classe de l'algorithme djikstra pour effectuer un trajet.
``` 

- Interface : 
```bash
Classe qui gère l'affichage dans le LCD
``` 
- Led : 
```bash
Classe qui permet de gérer les lumières.
``` 
- Navigation:
```bash
Classe permettant le déplacement du robot.
```  
- Piezo : 
```bash
Classe permettant d'émettre un son.
``` 
- Robot : 
```bash
 Classe qui rassemble toutes les classes et les challenges.
```
- SensorLine: 
```bash
Classe pour les directions du capteur de ligne.
``` 
- SharpIR: 
```bash
Classe pour le capteur Infrarouge pour détecter un poteau.
``` 
- Timer: 
```bash
Classe pour les deux timers 0 et 2.
``` 
- Timer1: 
```bash
Classe pour le timer 1.
``` 

# Credits:
- Abdelnour Sikouky
- Sebastien Brossier
- Jeremy Trudel
- Irina Maximciuc



## License

[MIT](https://choosealicense.com/licenses/mit/)
