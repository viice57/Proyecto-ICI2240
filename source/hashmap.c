#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../header/hashmap.h"

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap
{
  	HashPair ** buckets;
  	long size;
  	long capacity;
  	long current;
};

HashPair * createPair(char * key, void * value)
{
  	HashPair * newPair = (HashPair *) malloc(sizeof(HashPair));
  
  	newPair->key = key;
  	newPair->value = value;
  
  	return newPair;
}

long hash(char * key, long capacity)
{
  	unsigned long hash = 0;
  	char * ptr;
  
  	for(ptr = key; *ptr != '\0'; ptr++)
 	{
    	hash += hash * 32 + tolower(*ptr);
  	}
  	return hash%capacity;
}

int is_equal_hash(void* key1, void* key2)
{
  	if(!key1 || !key2) return 0;
  	if(strcmp((char*) key1, (char*) key2) == 0) return 1;
  
  	return 0;
}

void insertMap(HashMap * map, char * key, void * value)
{
  	HashPair * newPair = createPair(key, value);

  	size_t i = hash(key, map->capacity);

  	while(1)
  	{
    	if(!map->buckets[i] || !map->buckets[i]->key)
		{
      		map->buckets[i] = newPair;
      		map->size++;
      		map->current = i;
      		break;
    	}
		i++;

    	if(i == map->capacity) i = 0;
  	}
}

void enlarge(HashMap * map)
{
  	enlarge_called = 1;
  	HashPair ** oldBuckets = map->buckets;

  	size_t capacity = map->capacity;
  	size_t i;
  
  	map->capacity *= 2;
  	map->buckets = (HashPair **) calloc(map->capacity, sizeof(HashPair *));
  	map->size = 0;

  	for(i = 0; i < capacity; i++)
  	{
    	if(oldBuckets[i])
		{
      	insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
    	}
  	}
}

HashMap * createMap(long capacity)
{
  	HashMap * newMap = (HashMap *) malloc(sizeof(HashMap));

  	if(!newMap) return NULL;

  	newMap->buckets = (HashPair **) calloc(capacity, sizeof(HashPair *));
	newMap->size = 0;
	newMap->capacity = capacity;
	newMap->current = -1;
	  
	return newMap;
}

void eraseMap(HashMap * map,  char * key)
{
  	HashPair * eliminatePair = searchMap(map, key);

  	if(eliminatePair)
  	{
   		eliminatePair->key = NULL;
    	map->size--;
  	}
}

HashPair * searchMap(HashMap * map,  char * key)
{ 
  	size_t i = hash(key, map->capacity);
  	size_t cont = 0;

  	while(1)
  	{
    	if(!map->buckets[i] || !map->buckets[i]->key) break;

    	if(is_equal_hash(key, map->buckets[i]->key))
		{
      		map->current = i;
      		return map->buckets[i];
    	}
    	i++;
   	 	cont++;

    	if(cont == map->capacity) break;
    	if(i == map->capacity) i = 0;
 	}
  	return NULL;
}

HashPair * firstMap(HashMap * map)
{
  	size_t i;
  	if(!map->size) return NULL;

  	for(i = 0; i < map->capacity; i++)
  	{
    	if(map->buckets[i] && map->buckets[i]->key)
		{
      		map->current = i;
      		return map->buckets[i];
    	}
  	}
  	return NULL;
}

HashPair * nextMap(HashMap * map)
{
  	size_t i;
  
  	if(!map->size) return NULL;
  
  	for(i = map->current + 1; i < map->capacity; i++)
  	{
    	if(map->buckets[i] && map->buckets[i]->key)
		{
      		map->current = i;
      		return map->buckets[i];
    	}
  	}
  	return NULL;
}