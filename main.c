#include <stdio.h>
#include <stdlib.h>

#include "list.c"
#include "hashmap.c"
#include "treemap.c"
#include "menu.c"
#include "app.c"

int lower_than_string(unsigned short * key1, unsigned short * key2){
    char * k1 = (char *) key1;
    char * k2 = (char *) key2;
    if(strcmp(k1, k2) < 0) return 1;
    return 0;
}

int main(void) {
	short choice = -1, count;
	char userID[15];

	List * rByAffinity = createList();
	
	HashMap * mapRecipes = createMap(50);
	HashMap * mapIngredients = createMap(150);
	HashMap * mapUsers = createMap(20);
	
	TreeMap * rByPopularity = createTreeMap(lower_than_string);
  
	do {
    	showMenu();

		if(choice == -1) importDatabases(rByPopularity, mapUsers, mapIngredients, mapRecipes);
    
    	fflush(stdin);
		count = scanf("%hi", &choice);

		if(choice > 1 && choice < 10) {
      		if(!searchMap(mapUsers, userID)) {
				printf("\nNo se ha iniciado sesión. Cerrando...");
				break;
      		}
		}
		
		showChoice(rByAffinity, mapRecipes, mapIngredients, mapUsers, rByPopularity, choice, userID);
		
		if(searchMap(mapUsers, userID)) printf("%sInició sesión como: %s%s", "\x1B[1m", userID, "\x1B[0m");
	} while(choice != 0);
  
  	return 0;
}