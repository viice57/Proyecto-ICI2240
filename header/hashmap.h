//
//  HashMap.h
//  TablaHashLab
//
//  Created by Matias Barrientos on 11-08-18.
//  Copyright © 2018 Matias Barrientos. All rights reserved.
//

#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct HashMap HashMap;

typedef struct HashPair
{
	char * key;
    void * value;
} HashPair;

HashMap * createMap(long capacity);

int is_equal_hash(void* key1, void* key2);

void insertMap(HashMap * table, char * key, void * value);

void eraseMap(HashMap * table, char * key);

HashPair * searchMap(HashMap * table, char * key);

HashPair * firstMap(HashMap * table);

HashPair * nextMap(HashMap * table);

void enlarge(HashMap * map);

#endif /* HashMap_h */