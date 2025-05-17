Mon hypothèse sur la structure la plus performante est que, la liste d'adjacence est meilleur à la matrice d'adjacence car elle coûte moins en mémoire.
La Matrice d'adjacence fonctennerait mais arrivé sur des graphes plus grand il consonmerait plus de mémoire. C'est toi qui nous à mis sur le chemin lorsqu'on te posait des question en classe.

Pour ce TP, je l'es décomposer en me disanr si j'ai le le premeir pixel et le dernier je pourrais sans doute mieux l'apréhender en cherchant le ou les chemins qui les relies.
Si je trouve plusieurs chemins je dois prende celui avec le moins coût et le dessiner. Je em suis un peu basé sur le TP de GameOfLife avec sa logique des voisins. 
Aussi pour des explication assez complexe j'ai demandé à Grok certaines explications sur par exemple le trie des PriorityQueue et aussi c'est quoi l'algorithme A* avec les concepts manathan et hereustique.

Lorsque je voulais faire pour les layrinthes plus grand comme 512.bmp, 45565.bmp j'ai du augmenter le HEAP_SIZE le multipler par 24 sinon j'avais des messages d'erreur de dépassement de mémoire.