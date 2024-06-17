#include <stdio.h>
#include <stdlib.h>

// Structure pour stocker les coordonnées
typedef struct {
    int x;
    int y;
} Point;

// Fonction pour calculer le vecteur le plus court
Point shortest_vector(Point p1, Point p2, int map_size_x, int map_size_y) {
    Point vector;

    // Calcul des distances directes
    vector.x = p2.x - p1.x;
    vector.y = p2.y - p1.y;

    // Prendre en compte la circularité pour l'axe x
    if (vector.x > map_size_x / 2) {
        vector.x -= map_size_x;
    } else if (vector.x < -map_size_x / 2) {
        vector.x += map_size_x;
    }

    // Prendre en compte la circularité pour l'axe y
    if (vector.y > map_size_y / 2) {
        vector.y -= map_size_y;
    } else if (vector.y < -map_size_y / 2) {
        vector.y += map_size_y;
    }

    return vector;
}

// Fonction pour déterminer la case relative à P2 par laquelle le message va passer
Point relative_case(Point p2, Point vector, int map_size_x, int map_size_y) {
    Point relative = p2;

    // Mouvement le long de x
    if (vector.x != 0) {
        relative.x -= (vector.x > 0) ? 1 : -1;
        if (relative.x < 0) relative.x += map_size_x;
        if (relative.x >= map_size_x) relative.x -= map_size_x;
    }
    // Mouvement le long de y
    if (vector.y != 0) {
        relative.y -= (vector.y > 0) ? 1 : -1;
        if (relative.y < 0) relative.y += map_size_y;
        if (relative.y >= map_size_y) relative.y -= map_size_y;
    }

    return relative;
}

int main() {
    Point p1 = {1, 5};
    Point p2 = {4, 20};
    int map_size_x = 5;
    int map_size_y = 25;

    Point vector = shortest_vector(p1, p2, map_size_x, map_size_y);
    Point relative = relative_case(p2, vector, map_size_x, map_size_y);

    printf("P1: (%d, %d)\n", p1.x, p1.y);
    printf("P2: (%d, %d)\n", p2.x, p2.y);

    printf("Le vecteur le plus court entre P1 et P2 est: (%d, %d)\n", vector.x, vector.y);
    printf("La case relative à P2 par laquelle le message va arriver est: (%d, %d)\n", relative.x, relative.y);

    return 0;
}
