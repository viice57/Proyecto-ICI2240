#ifndef GAME_H
#define GAME_H

typedef struct recipe recipe;
typedef struct ingredient ingredient;
typedef struct user user;

int insertOnList(HashMap * map, char * key, char newIngredient);

void importDatabases(HashMap * mapUsers, HashMap * mapIngredients, HashMap * mapRecipes);

int userLogin(HashMap * mapUsers, char * userID);

int addFavs(HashMap * mapUsers, HashMap * mapIngredients, HashMap * mapRecipes, char * userID);

int showRecipes(HashMap * mapRecipes, TreeMap * rByPopularity);

#endif