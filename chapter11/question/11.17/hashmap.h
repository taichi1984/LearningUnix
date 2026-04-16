#ifndef MAP_H
#define MAP_H

typedef struct __node {
  char *key;
  char *value;
  struct __node *next;
} Node;

int put_node(Node **Map, int size, const char *key, const char *value);
Node *get_node(Node **Map, int size, char *key);
void free_map(Node **map, int size);

#endif 
