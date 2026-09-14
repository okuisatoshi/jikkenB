#ifndef HASHMAP_H
#define HASHMAP_H

struct hashmap;

struct hashmap *hnew(void);

void hinsert(struct hashmap *map, const char *key, int value);

int hget(struct hashmap *map, const char *key);

void hdestroy(struct hashmap *map);

#endif
