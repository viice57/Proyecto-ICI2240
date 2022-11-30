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

	/* Creación de mapas y árbol de popularidades */
	HashMap * mapRecipes = createMap(50);
	HashMap * mapIngredients = createMap(150);
	HashMap * mapUsers = createMap(20);
	TreeMap * rByPopularity = createTreeMap(lower_than_string);

	/* Importamos todos los datos que están en archivos .csv */
	importDatabases(rByPopularity, mapUsers, mapIngredients, mapRecipes);

	/* Mientras la opción no sea 0 */
	do {
		/* Menú de opciones */
    	showMenu();

		/* Escaneamos opción */
    	fflush(stdin);
		count = scanf("%hi", &choice);

		/* Verificación de que el usuario haya pasado por la opción 1 */
		if(choice > 1 && choice < 7) {
      		if(!searchMap(mapUsers, userID)) {
				printf("\nNo se ha iniciado sesión. Cerrando...");
				break;
      		}
		}

		/* Se muestra la función elegida */
		showChoice(mapRecipes, mapIngredients, mapUsers, rByPopularity, choice, userID);

		/* Se muestra el usuario que inició sesión */
		if(searchMap(mapUsers, userID) && choice != 0) printf("%sInició sesión como: %s%s", "\x1B[1m", userID, "\x1B[0m");
	} while(choice != 0);
  
  	return 0;
}