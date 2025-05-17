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
	Node** nodes;          // Tableau de n�uds (index� par x * width + y)
	int width, height;     // Dimensions de l'image
};

// Structure pour la file de priorit�
typedef struct HeapNode HeapNode;
struct HeapNode {
	Node* node;            // Pointeur vers le n�ud
	float f;               // Valeur f = cost + estimate
};

typedef struct PriorityQueue PriorityQueue;
struct PriorityQueue {
	HeapNode* nodes;       // Tableau de n�uds
	int capacity;          // Capacit� maximale
	int size;              // Taille actuelle
};

// Cr�e un n�ud pour un pixel blanc avec sa position en x et y
Node* create_node(uint8_t* pixel, int x, int y);

// Cr�e le graphe pour la recherche des adjacents
Graph* create_graph(unsigned char* img, int width, int height, int channels);

// Cr�e une file de priorit�
PriorityQueue* create_priority_queue(int capacity);

// Sert trier la PriorityQueue. Il fait un trie au cas un noeud ajouter � la fin � un petit f que son parent. 
// Il est d�pacer vers le haut de l'abre jusqu'� sa bonne position
void heapify_up(PriorityQueue* pq, int idx);

// Ins�re un n�ud dans la file de priorit�
void pq_insert(PriorityQueue* pq, Node* node, float f);

// Sert � trier la PriorityQueue au cas ou on extrait la racine pour toujours avoir un bon ordre dans le graph
void heapify_down(PriorityQueue* pq, int idx);

// Extrait le n�ud avec le plus petit f
Node* pq_extract_min(PriorityQueue* pq);

// Algorithme A*
Node* a_star(Graph* graph, Node* start, Node* goal);
void draw_path(Graph* graph, Node* goal, unsigned char* img, int channels);