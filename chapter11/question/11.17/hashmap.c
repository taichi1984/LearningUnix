#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct __node {
  char *key;
  char *value;
  struct __node *next;
} Node;

int put_node(Node **Map, int size, const char *key, const char *value);
Node *get_node(Node **Map, int size, char *key);
void free_map(Node **map, int size);
unsigned long hash(const char *str);
/* テスト用
int main(void) {
  Node *map[256];
  for (int i = 0; i < 256; i++) {
    map[i] = NULL;
  }

  Node *a = malloc(sizeof(Node));
  a->key = strdup("test");
  a->value = strdup("testvalue");
  a->next = NULL;

  put_node(map, 256, "test", "testvalue");

  printf("%s \n", get_node(map, 256, "test")->value);

  return 0;
}
*/
int put_node(Node **table, int size, const char *key, const char *value)
/*
 *  第１引数は格納するMapをさすポインタ
 *  第２引数は追加したいNode
 *
 *  戻り値は、成功時は1
 *  エラー時は -1を返す。
 */
{
  int index = hash(key) % size;

  Node *tmp = table[index];
  while (tmp != NULL) {
    if (strcmp(tmp->key, key) == 0) {
      free(tmp->value);
      tmp->value = strdup(value);

      return 1;
    }
    tmp = tmp->next;
  }
  Node *node = malloc(sizeof(Node));
  if (!node)
    return -1;

  node->key = strdup(key);
  node->value = strdup(value);
  if (!node->key || !node->value) {
    free(node->key);
    free(node->value);
    free(node);
    return -1;
  }

  node->next = table[index];
  table[index] = node;

  return 1;
}

Node *get_node(Node **table, int size, char *key)
/*
 *  第１引数は取得するnodeが格納されたMapをさすポインタ
 *  第２引数は欲しいnodeのsize
 *  第３引数は欲しいnodeのkey
 *
 *  戻り値は欲しいNodeを返す。
 *  失敗時（エラー時、もしくは該当するNodeがない場合)はNULLを返す。
 */
{
  int index = 0;
  index = hash(key) % size;

  Node *tmp = table[index];
  while (tmp != NULL) {
    if (strcmp(tmp->key, key) == 0) {
      return tmp;
    }
    tmp = tmp->next;
  }
  return NULL;
}

void free_map(Node **map, int size) {
  for (int i = 0; i < size; i++) {
    Node *tmp = map[i];
    while (tmp != NULL) {
      Node *next = tmp->next;

      free(tmp->key);
      free(tmp->value);
      free(tmp);

      tmp = next;
    }
    map[i] = NULL;
  }
}

unsigned long hash(const char *str) {
  unsigned long h = 5381;
  int c;

  while ((c = *str++))
    h = h * 33 + c;

  return h;
}
