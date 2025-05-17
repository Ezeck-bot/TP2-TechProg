#include "labo.h"
#include <assert.h>
#include <string.h>
#include <float.h>
#include <math.h>

// Crée un nœud pour un pixel blanc avec sa position en x et y
Node* create_node(uint8_t* pixel, int x, int y) {
    Node* node = allocate(sizeof(Node));
    node->x = x;
    node->y = y;
    node->pixel = pixel;
    node->adj_len = 0;
    node->cost = FLT_MAX;
    node->estimate = 0.0f;
    node->parent = NULL;
    node->visited = false;
    for (int i = 0; i < 4; i++) {
        node->adj[i] = NULL;
    }
    return node;
}

// Crée le graphe à partir de l'image
Graph* create_graph(unsigned char* img, int width, int height, int channels) {
    Graph* graph = allocate(sizeof(Graph));
    graph->width = width;
    graph->height = height;
    graph->nodes = allocate(sizeof(Node*) * width * height);
    memset(graph->nodes, 0, sizeof(Node*) * width * height);

    // Créer les nœuds pour les pixels blancs
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * channels;
            if (img[idx] == 255) { // Pixel blanc
                graph->nodes[y * width + x] = create_node(&img[idx], x, y);
            }
        }
    }

    // Ajouter les voisins
    int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} }; // Gauche, Droite, Haut, Bas
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Node* node = graph->nodes[y * width + x];
            if (!node) continue; // Pas un chemin
            for (int i = 0; i < 4; i++) {
                int nx = x + directions[i][0];
                int ny = y + directions[i][1];
                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    Node* neighbor = graph->nodes[ny * width + nx];
                    if (neighbor) { // Si le voisin est un chemin on l'ajoute
                        node->adj[node->adj_len++] = neighbor;
                    }
                }
            }
        }
    }

    return graph;
}

// Crée une file de priorité
PriorityQueue* create_priority_queue(int capacity) {
    PriorityQueue* pq = allocate(sizeof(PriorityQueue));
    pq->nodes = allocate(sizeof(HeapNode) * capacity);
    pq->capacity = capacity;
    pq->size = 0;
    return pq;
}

// Sert trier la PriorityQueue. Il fait un trie au cas un noeud ajouter à la fin à un petit f que son parent. 
// Il est dépacer vers le haut de l'abre jusqu'à sa bonne position
void heapify_up(PriorityQueue* pq, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (pq->nodes[parent].f <= pq->nodes[idx].f) {
            break;
        }
        // Échanger
        HeapNode temp = pq->nodes[parent];
        pq->nodes[parent] = pq->nodes[idx];
        pq->nodes[idx] = temp;
        idx = parent;
    }
}

// Insère un nœud dans la file de priorité
void pq_insert(PriorityQueue* pq, Node* node, float f) {
    if (pq->size >= pq->capacity) {
        return; // Éviter le débordement
    }
    pq->nodes[pq->size].node = node;
    pq->nodes[pq->size].f = f;
    heapify_up(pq, pq->size);
    pq->size++;
}

// Sert à trier la PriorityQueue au cas ou on extrait la racine pour toujours avoir un bon ordre dans le graph
void heapify_down(PriorityQueue* pq, int idx) {
    while (idx < pq->size) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;

        if (left < pq->size && pq->nodes[left].f < pq->nodes[smallest].f) {
            smallest = left;
        }
        if (right < pq->size && pq->nodes[right].f < pq->nodes[smallest].f) {
            smallest = right;
        }
        if (smallest == idx) {
            break;
        }
        // Échanger
        HeapNode temp = pq->nodes[idx];
        pq->nodes[idx] = pq->nodes[smallest];
        pq->nodes[smallest] = temp;
        idx = smallest;
    }
}

// Extrait le nœud avec le plus petit f
Node* pq_extract_min(PriorityQueue* pq) {
    if (pq->size == 0) {
        return NULL;
    }
    Node* min_node = pq->nodes[0].node;
    pq->nodes[0] = pq->nodes[--pq->size];
    heapify_down(pq, 0);
    return min_node;
}

// Algorithme A*
Node* a_star(Graph* graph, Node* start, Node* goal) {
    // Initialiser les nœuds
    for (int i = 0; i < graph->width * graph->height; i++) {
        if (graph->nodes[i]) {
            graph->nodes[i]->cost = FLT_MAX;
            graph->nodes[i]->estimate = 0.0f;
            graph->nodes[i]->parent = NULL;
            graph->nodes[i]->visited = false;
        }
    }

    // Initialiser le nœud de départ
    start->cost = 0.0f;
    start->estimate = (float)(abs(start->x - goal->x) + abs(start->y - goal->y));

    // Créer la file de priorité
    PriorityQueue* pq = create_priority_queue(graph->width * graph->height);
    pq_insert(pq, start, start->cost + start->estimate);

    while (pq->size > 0) {
        Node* current = pq_extract_min(pq);
        if (current == goal) {
            return current; // Chemin trouvé
        }

        current->visited = true;

        // Explorer les voisins
        for (int i = 0; i < current->adj_len; i++) {
            Node* neighbor = current->adj[i];
            if (neighbor->visited) {
                continue;
            }

            float new_g = current->cost + 1.0f; // Coût de 1 par mouvement
            if (new_g < neighbor->cost) {
                neighbor->cost = new_g;
                neighbor->estimate = (float)(abs(neighbor->x - goal->x) + abs(neighbor->y - goal->y));
                neighbor->parent = current;
                pq_insert(pq, neighbor, neighbor->cost + neighbor->estimate);
            }
        }
    }

    return NULL; // Pas de chemin trouvé
}

// Dessine le chemin trouvé en rouge sur l'image
void draw_path(Graph* graph, Node* goal, unsigned char* img, int channels) {
    Node* current = goal;
    while (current) {
        int idx = (current->y * graph->width + current->x) * channels;
        if (channels >= 3) {
            img[idx] = 255;     // Rouge
            img[idx + 1] = 0;   // Vert
            img[idx + 2] = 0;   // Bleu
        }
        else {
            img[idx] = 128;     // Gris pour niveaux de gris
        }
        current = current->parent;
    }
}