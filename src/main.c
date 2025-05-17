//Bonne pratique, pour les includes systemes toujours utiliser <> et "" pour les includes de votre projet.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "utils.h"
#include "labo.h"

#include "stb_image.h"
#include "stb_image_write.h"

#define HEAP_SIZE 2048 * 2048 * 24
static uint8_t* heap = NULL;
static size_t heap_top = 0;
void* allocate(size_t size) {
	size_t old_top = heap_top;
	heap_top += size;
	assert(heap_top <= HEAP_SIZE);
	return &heap[old_top];
}

const char* img_names[] = {
	"31.bmp",
	"64.bmp",
	"128.bmp",
	"512.bmp",
	"45565.bmp",
};

const size_t img_name_len = sizeof(img_names) / sizeof(img_names[0]);

int main(int argc, char** argv) {
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
	
	char const* choose_img = img_names[0]; //****

	int width, height, channels; // channels c'est le rgb
	unsigned char* img = stbi_load(choose_img, &width, &height, &channels, 0);
	if (img == NULL) {
		printf("Error in loading the image\n");
	}
	printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

	// Créer le graphe
	Graph* graph = create_graph(img, width, height, channels);

	// Je cherche la position du premier pixel blanc de la premiere ligne
	Node* start = NULL;
	for (int x = 0; x < width; x++) {
		if (graph->nodes[x] && img[x * channels] == 255) {
			start = graph->nodes[x];
			break;
		}
	}

	// Je cherche la position du deuxième pixel blanc de la derniere ligne
	Node* goal = NULL;
	for (int x = 0; x < width; x++) {
		int idx = ((height - 1) * width + x);
		if (graph->nodes[idx] && img[idx * channels] == 255) {
			goal = graph->nodes[idx];
			break;
		}
	}

	//si je trouve le premier et le dernier point blanc je les passes en paramètre
	if (start && goal) {
		// Exécuter A*
		Node* path_end = a_star(graph, start, goal);
		if (path_end) {
			// Dessiner le chemin
			draw_path(graph, path_end, img, channels);
		}
	}

	char filepath[_MAX_PATH] = { 0 };
	snprintf(filepath, _MAX_PATH, "solution_%s", choose_img);
	stbi_write_bmp(filepath, width, height, channels, img);

	return 0;
}