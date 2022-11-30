#ifndef GAME_H
#define GAME_H

typedef struct recipe recipe;
typedef struct ingredient ingredient;
typedef struct user user;
typedef struct item item;

void insertIntoList(char * sType, HashMap * map, char * key, item * newItem);

void insertListIntoTree(TreeMap * rByPopularity, unsigned short * popularity, char * recipeName);

void importDatabases(TreeMap * rByPopularity, HashMap * mapUsers, HashMap * mapIngredients, HashMap * mapRecipes);

int userLogin(HashMap * mapUsers, char * userID);

int addIngredient(HashMap * mapIngredients, char * userID);

int addRecipe(HashMap * mapRecipes, TreeMap * rByPopularity, char * userID);

int addFavs(HashMap * mapUsers, HashMap * mapIngredients, HashMap * mapRecipes, char * userID);

int showRecipes(HashMap * mapRecipes, TreeMap * rByPopularity);

void exportDatabases(HashMap * mapUsers, HashMap * mapIngredients, HashMap * mapRecipes);

#endif