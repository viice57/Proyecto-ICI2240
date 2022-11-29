#ifndef GAME_H
#define GAME_H

typedef struct recipe recipe;
typedef struct ingredient ingredient;
typedef struct user user;
typedef struct item item;

int insertOnList(char * sType, HashMap * map, char * key, item * newItem);

void importDatabases(TreeMap * rByPopularity, HashMap * mapUsers, HashMap * mapIngredients, HashMap * mapRecipes);

int userLogin(HashMap * mapUsers, char * userID);

int addIngredient(HashMap * mapIngredients, char * userID);

int addRecipe(HashMap * mapRecipes, char * userID);

int addFavs(HashMap * mapUsers, HashMap * mapIngredients, HashMap * mapRecipes, char * userID);

int showRecipes(HashMap * mapRecipes, TreeMap * rByPopularity);

#endif