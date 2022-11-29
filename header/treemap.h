#ifndef TREEMAP_H
#define TREEMAP_H

typedef struct TreeMap TreeMap;

typedef struct TreePair
{
    unsigned short * key;
    void * value;
} TreePair;

TreeMap * createTreeMap(int (*lower_than_int) (unsigned short * key1, unsigned short * key2));

void insertTreeMap(TreeMap * tree, unsigned short * key, void * value);

void eraseTreeMap(TreeMap * tree, unsigned short * key);

TreePair * searchTreeMap(TreeMap * tree, unsigned short * key);

TreePair * upperBound(TreeMap * tree, unsigned short * key);

TreePair * firstTreeMap(TreeMap * tree);

TreePair * nextTreeMap(TreeMap * tree);

#endif /* TREEMAP_h */