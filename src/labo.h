#pragma once

#include <stdint.h>
#include <stdbool.h>

void* allocate(size_t size);

typedef struct Node Node;
struct Node {
	int x, y;
	uint8_t* pixel;
	Node* adj[4];
	int adj_len;
	float cost;
	float estimate;
	Node* parent;
	bool visited;
};

typedef struct Graph Graph;
struct Graph {
	Node** nodes;          // Tableau de nœuds (indexé par x * width + y)
	int width, height;     // Dimensions de l'image
};

// Structure pour la file de priorité
typedef struct HeapNode HeapNode;
struct HeapNode {
	Node* node;            // Pointeur vers le nœud
	float f;               // Valeur f = cost + estimate
};

typedef struct PriorityQueue PriorityQueue;
struct PriorityQueue {
	HeapNode* nodes;       // Tableau de nœuds
	int capacity;          // Capacité maximale
	int size;              // Taille actuelle
};

// Crée un nœud pour un pixel blanc avec sa position en x et y
Node* create_node(uint8_t* pixel, int x, int y);

// Crée le graphe pour la recherche des adjacents
Graph* create_graph(unsigned char* img, int width, int height, int channels);

// Crée une file de priorité
PriorityQueue* create_priority_queue(int capacity);

// Sert trier la PriorityQueue. Il fait un trie au cas un noeud ajouter à la fin à un petit f que son parent. 
// Il est dépacer vers le haut de l'abre jusqu'à sa bonne position
void heapify_up(PriorityQueue* pq, int idx);

// Insère un nœud dans la file de priorité
void pq_insert(PriorityQueue* pq, Node* node, float f);

// Sert à trier la PriorityQueue au cas ou on extrait la racine pour toujours avoir un bon ordre dans le graph
void heapify_down(PriorityQueue* pq, int idx);

// Extrait le nœud avec le plus petit f
Node* pq_extract_min(PriorityQueue* pq);

// Algorithme A*
Node* a_star(Graph* graph, Node* start, Node* goal);
void draw_path(Graph* graph, Node* goal, unsigned char* img, int channels);