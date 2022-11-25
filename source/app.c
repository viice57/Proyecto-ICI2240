#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../header/app.h"

/* Estructura receta */
struct recipe
{
	char name[30];
	char type[10];
	List * ingredients;
	unsigned short popularity;
};

/* Estructura ingrediente */
struct ingredient
{
	char name[20];
	unsigned short quantity;
	unsigned short calories;
};

/* Estructura usuario */
struct user
{
    char name[15];
	float imc;
	unsigned short active;
	List * favThings;
	//List * mealPlan;
};

void getListFromMap(HashMap * map, char * key, char newIngredient) {
  HashPair * searchMyMap = searchMap(map, key);

  /* Si el key buscado existe */
  if(searchMyMap) {
    /* Obtención de la lista desde tabla hash. Insertamos un ingrediente a la lista */
    List * listAux = ((user *) searchMyMap->value)->favThings;
    pushBack(listAux, newIngredient);
  }
}

void importDatabases(HashMap * mapUsers, HashMap * mapIngredients, HashMap * mapRecipes)
{
    char readLineUsers[30];

    /* Abrimos el archivo de usuarios en modo lectura. Si no existe, se acaba el programa. */
    FILE * usersDB = fopen("./file/usuarios.csv", "r");
	if(!usersDB)
	{
		printf("Archivo de usuarios no existe.\n");
		exit(EXIT_FAILURE);
	}

 	/* Ciclo para leer las líneas del archivo y asignar cada usuario a una tabla hash */
	while(fgets(readLineUsers, 30, usersDB))
	{
    	user * importUser = (user *) malloc(sizeof(user));
		
		/* Separamos los siguientes datos: nombre e IMC. Inactivo por defecto */
		char * readUsername = strtok(readLineUsers, ",");
		strncpy(importUser->name, readUsername, sizeof(importUser->name));
		
		char * readIMC = strtok(readLineUsers, "\n");
     	importUser->imc = (float) atoi(readIMC);
		
		importUser->active = 0;

		/* Inserción a mapa usuarios */ 
    	insertMap(mapUsers, importUser->name, importUser);
	}

	// parte recetas
  	char readLineRecipes[100];
  	FILE * recipesDB = fopen("./file/recetas.csv", "r");
	
	if(!recipesDB)
	{
		printf("Archivo de recetas no existe.\n");
		exit(EXIT_FAILURE);
	}

	while(fgets(readLineRecipes, 100, recipesDB)){
    	// crea nodo tipo receta
    	recipe * importRecipe = (recipe *) malloc(sizeof(recipe));
		
    	//  asigna los valores a cada elemento de la estructura
    	char * readRecipe = strtok(readLineRecipes, ",");
    	strncpy(importRecipe->name, readRecipe, sizeof(importRecipe->name));
    
    	char * readPopularity = strtok(NULL, ",");
    	importRecipe->popularity  = (int) atoi(readPopularity);

    	char * readType = strtok(NULL, "\n");
    	strncpy(importRecipe->type, readType, sizeof(importRecipe->type));

    	////////////////////falta la lista con ingredientes////////////////////////////////////
      
    /*
    void insertListOnTree(TreeMap * map, game * gameFromHash, char * key) {
  TreePair * searchTree = searchTreeMap(map, key);

  if(searchTree) {
   
    List * listAux = (List *) searchTree->value;
    pushBack(listAux, gameFromHash);
  } else {
   
    List * newList = createList();
    pushBack(newList, gameFromHash);
    insertTreeMap(map, key, newList);
  }
}

*/
    
    	// inserta nodo en el mapa 
    	insertMap(mapRecipes, importRecipe->name, importRecipe);
	}
}

int userLogin(HashMap * mapUsers, char * userID)
{
	unsigned short whatToDo = 0;
	char username[15];
	HashPair * userSearch;
	
	fflush(stdin);
	printf("\nBienvenido al programa. Puede iniciar sesión (1) o crear un nuevo usuario (2).\n");
	
	do
	{
		printf("\nSeleccione una de las anteriores: ");
		scanf("%hu", &whatToDo);
	} while(whatToDo < 1 || whatToDo > 2);
	
	/* Switch para elegir si se inicia o se crea */
	switch(whatToDo)
	{
		case 1:
		{
			/* Mostramos los usuarios que previamente se importaron */
			printf("\nLista de usuarios disponibles:\n");
    		for(HashPair * availableUsers = firstMap(mapUsers); availableUsers != NULL; availableUsers = nextMap(mapUsers))
			{
				printf("• %s\n", availableUsers->key);
			}
			
			/* Pedimos el nombre de usuario */
			printf("\nHa seleccionado iniciar sesión.\nPor favor, ingrese su nombre: ");
    		scanf("%15s", username);
    		getchar();

			/* Obtención del usuario. Si no existe, error. */
			userSearch = searchMap(mapUsers, username);
			if(!userSearch) return 1;

			/* Ya que el usuario existe, obtenemos sus datos y se cambia su estado a activo */
			user * currentUser = (user *) userSearch->value;
			currentUser->active = 1;

			/* Puntero que guarda el nombre de usuario */
			*userID = currentUser->name;

			/* Se ingresó correctamente */
			return 2;
			//break;
		}
		case 2:
		{
			float weight;
			float height;

			/* Pedimos el nombre del posible usuario */
			printf("\nHa seleccionado crear un nuevo usuario.\nPor favor, ingrese su nombre: ");
			scanf("%15s", username);
			getchar();

			/* Verificación del usuario. Si existe el nombre, error. */
			userSearch = searchMap(mapUsers, username);
			if(userSearch) return 1;

			/* Pedimos al nuevo usuario información adicional */
			printf("Ingrese su peso (Ej: 87): ");
			scanf("%f", &weight);
			getchar();
			
			printf("Por último, ingrese su altura (Ejemplo: 1.60): ");
			scanf("%f", &height);
			getchar();

			/* Calculamos el imc a partir de los datos ingresados */
			float imc = weight / pow(height, 2);

			/* Insertamos el usuario */
			user * newUser = (user *) malloc(sizeof(user));
			strncpy(newUser->name, username, sizeof(newUser->name));
			newUser->imc = imc;
			newUser->active = 1;

			/* Puntero que guarda el nombre de usuario */
			*userID = newUser->name;

			insertMap(mapUsers, newUser->name, newUser);
			
			/* Se ingresó correctamente */
			return 3;
			//break;
		}
	}
}

/*int addIngredient(HashMap * mapRecipes, HashMap * mapIngredients)
{
	printf("A continuación, usted podrá ingresar ingredientes que posee:")
}

int addFavs(HashMap * mapUsers, HashMap * mapIngredients, HashMap * mapRecipes)
{
	char typeOfFav[15];
	unsigned short qty;

	printf("Ahora usted puede ingresar qué tipo de favorito quiere ingresar (ingrediente o receta): ")
	scanf("%15s", typeOfFav);
	getchar();

	if(strcmp(typeOfFav, "ingrediente") == 0)
	{
		printf("¿Cuántos ingredientes desea ingresar?");
		scanf("%hu", &qty);

		//añadir a hashmap
	}
	else if(strcmp(typeOfFav, "receta") == 0)
	{
		printf("¿Cuántas recetas desea ingresar?");
		scanf("%hu", &qty);

		//añadir a hashmap
	}
 	else
	{
    	printf("Opción inválida. Pruebe con alguna de las anteriores.");
    	return 1; 
  	}
}*/

int showRecipes(HashMap * mapRecipes, TreeMap * rByPopularity)
{
  	unsigned short opcion = 0;

	printf("\nA continuación, usted podrá visualizar las recetas disponibles: \n");
  	printf("(1) Mostrar por nombre\n");
  	printf("(2) Mostrar por popularidad\n");
  	printf("(3) Mostrar por tipo\n");
  
  	do
	{
		printf("\nSeleccione una de las anteriores: ");
		scanf("%hu", &opcion);
	} while(opcion < 1 || opcion > 3);
	
  	/* Mostrar por nombre */
	printf("\nLista de recetas disponibles:\n");
    for(HashPair * receta = firstMap(mapRecipes); receta != NULL; receta = nextMap(mapRecipes))
	{
		recipe * recipeValues = (recipe *) receta->value;
		
		switch(opcion)
        {
			case 1:
			{
				printf("• %s\n", receta->key);
				break;
			}
            case 2:
			{
          		printf("• %hu\n", recipeValues->popularity);
				break;
          	}
    	}
  	}
}
  
