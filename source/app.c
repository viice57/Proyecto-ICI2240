#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../header/app.h"

/* Estructura receta */
struct recipe
{
	char name[30], type[10];
	unsigned short popularity;
	List * ingredients;
};

/* Estructura ingrediente */
struct ingredient
{
	char name[20], owner[15];
	unsigned short quantity, calories;
};

/* Estructura usuario */
struct user
{
    char name[15];
	float imc;
	List * favIngredients, favRecipes, mealPlan;
};

int insertOnList(HashMap * map, char * key, char newIngredient)
{
  	HashPair * searchMyMap = searchMap(map, key);

  	/* Si el key buscado existe */
  	if(searchMyMap)
  	{
    	/* Obtención de la lista desde tabla hash. Insertamos un ingrediente a la lista */
    	List * listAux = ((user *) searchMyMap->value)->favIngredients;
   		pushBack(listAux, newIngredient); 
		//comprobar si no existe un favorito con ese nombre
  	}
}

void importDatabases(HashMap * mapUsers, HashMap * mapIngredients, HashMap * mapRecipes)
{
	/* Variable exclusiva para líneas de usuario */
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
		
		char * readIMC = strtok(NULL, "\n");
     	importUser->imc = (float) atof(readIMC);

		/* Inserción a mapa usuarios */ 
    	insertMap(mapUsers, importUser->name, importUser);
	}

	/*************************************************************/

    /* Variable exclusiva para líneas de receta */
  	char readLineRecipes[100];

	/* Abrimos el archivo de recetas en modo lectura */
  	FILE * recipesDB = fopen("./file/recetas.csv", "r");
	if(!recipesDB)
	{
		printf("Archivo de recetas no existe.\n");
		exit(EXIT_FAILURE);
	}

	while(fgets(readLineRecipes, 100, recipesDB))
	{
    	/* Creación de nodo tipo receta */
    	recipe * importRecipe = (recipe *) malloc(sizeof(recipe));
		
    	/* Asignación de los valores a cada elemento de la estructura receta */
    	char * readRecipeName = strtok(readLineRecipes, ",");
    	strncpy(importRecipe->name, readRecipeName, sizeof(importRecipe->name));
    
    	char * readRecipePopularity = strtok(NULL, ",");
    	importRecipe->popularity  = (unsigned short) atoi(readRecipePopularity);
      	//strncpy(importRecipe->popularity, readRecipePopularity, sizeof(importRecipe->popularity));

    	char * readRecipeType = strtok(NULL, "\n");
    	strncpy(importRecipe->type, readRecipeType, sizeof(importRecipe->type));

		//leer ingredientes
    
    	/* Insertamos nodo en tabla hash */
    	insertMap(mapRecipes, importRecipe->name, importRecipe);
	}


	/*************************************************************/

    /* Variable exclusiva para líneas de ingrediente */
  	char readLineIngredients[100];

	/* Abrimos el archivo despensa en modo lectura */
  	FILE * ingredientsDB = fopen("./file/despensa.csv", "r");
	if(!ingredientsDB)
	{
		printf("Archivo despensa no existe.\n");
		exit(EXIT_FAILURE);
	}

	while(fgets(readLineIngredients, 100, ingredientsDB))
	{
    	/* Creación de nodo tipo ingrediente */
    	ingredient * importIngredient = (ingredient *) malloc(sizeof(ingredient));
		
    	/* Asignación de los valores a cada elemento de la estructura ingrediente */
    	char * readIngredientName = strtok(readLineIngredients, ",");
    	strncpy(importIngredient->name, readIngredientName, sizeof(importIngredient->name));
    
    	char * readIngredientQty = strtok(NULL, ",");
    	importIngredient->quantity  = (unsigned short) atoi(readIngredientQty);

    	char * readIngredientCal = strtok(NULL, ",");
    	importIngredient->calories = (unsigned short) atoi(readIngredientCal);

    	char * readIngredientOwner = strtok(NULL, "\n");
    	strncpy(importIngredient->owner, readIngredientOwner, sizeof(importIngredient->owner));
    
    	/* Insertamos nodo en tabla hash */
		//importar solo los del usuario que inició sesión
    	insertMap(mapIngredients, importIngredient->name, importIngredient);
	}
}

int userLogin(HashMap * mapUsers, char * userID)
{
	unsigned short whatToDo = 0;
	char username[15];
	HashPair * userSearch;
	float weight, height, imc;
	
	fflush(stdin);
	printf("\nBienvenido al programa. Puede iniciar sesión (1) o crear un nuevo usuario (2).\n");

	/* El usuario podrá ingresar solo una de las dos opciones */
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

			/* Ya que el usuario existe, obtenemos sus datos y se guarda el nombre del usuario */
			user * currentUser = (user *) userSearch->value;
			strncpy(userID, currentUser->name, sizeof(userID));

			/* Se ingresó correctamente */
			return 2;
		}
		case 2:
		{
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
			
			printf("Por último, ingrese su altura (Ejemplo: 1.60): ");
			scanf("%f", &height);
			//revisar si se ingresa una coma,convertirla a punto

			/* Calculamos el imc a partir de los datos ingresados */
			imc = weight / pow(height, 2);

			/* Estructuramos los datos del usuario  */
			user * newUser = (user *) malloc(sizeof(user));
			strncpy(newUser->name, username, sizeof(newUser->name));
			newUser->imc = imc;

			/* Puntero que guarda el nombre de usuario */
			strncpy(userID, newUser->name, sizeof(userID));

			/* Se inserta el usuario en la tabla hash */
			insertMap(mapUsers, newUser->name, newUser);
			
			/* Se ingresó correctamente */
			return 3;
		}
		default:
		{
			exit(EXIT_FAILURE);
			break;
		}
	}
}

/*int addIngredient(HashMap * mapRecipes, HashMap * mapIngredients)
{
	printf("A continuación, usted podrá ingresar ingredientes que posee:")
}*/

int addFavs(HashMap * mapUsers, HashMap * mapIngredients, HashMap * mapRecipes, char * userID)
{
	char typeOfFav[15];
	unsigned short qty;
  	char nombreIngrediente[30];
	//HashPair * ingredientSearch;

	printf("\nIngrese qué tipo de favorito quiere ingresar (ingrediente o receta): ");
	scanf("%15s", typeOfFav);
	getchar();

	if(strcmp(typeOfFav, "ingrediente") == 0)
	{
		printf("\n¿Cuántos ingredientes desea ingresar?: ");
		scanf("%hu", &qty);

    	for(unsigned short i = 1; i <= qty; i++)
		{
			unsigned short status;
      		/* Obtención del ingrediente. Si no existe, error. */
			//ingredientSearch = searchMap(mapIngredients, nombreIngrediente);
			//buscar si no se repite

			printf("Nombre del ingrediente %hu: ", i);
			scanf("%s", &nombreIngrediente);
			getchar();
			
		  	status = insertOnList(mapUsers, userID, nombreIngrediente);

			if(!status)
			{
				printf("El ingrediente ya había sido ingresado.");
				break;
				//implementar goto
			}
		}
		return 0;
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
}

int showRecipes(HashMap * mapRecipes, TreeMap * rByPopularity)
{
  	unsigned short opcion = 0;

	printf("\nA continuación, usted podrá visualizar las recetas disponibles: \n");
  	printf("(1) Mostrar todos los nombres\n");
  	printf("(2) Mostrar por popularidad\n");
  	printf("(3) Mostrar por tipo\n");
  
  	do
	{
		printf("\nSeleccione una de las anteriores: ");
		scanf("%hu", &opcion);
	} while(opcion < 1 || opcion > 3);
	
  	/* Mostrar por nombre */
	printf("\nLista de recetas disponibles:\n");
  	switch(opcion)
	{
      	case 1:
		{
          	for(HashPair * receta = firstMap(mapRecipes); receta != NULL; receta = nextMap(mapRecipes))
	        {
		    	//recipe * recipeValues = (recipe *) receta->value;
              	printf("• %s\n",(char*) receta->key);
            }
            break;
      	}
     	case 2:
        {
        	for(HashPair * receta = firstMap(mapRecipes); receta != NULL; receta = nextMap(mapRecipes))
			{
		      	recipe * recipeValues = ((recipe *) receta->value);
          		insertTreeMap(rByPopularity, (char *)recipeValues->popularity, recipeValues);
        	}

        	for(TreePair * newSearch = firstTreeMap(rByPopularity); newSearch != NULL; newSearch =         nextTreeMap(rByPopularity))
			{
          		printf("%s\n", (char *) newSearch->key);
          
        	}
          	break;
      	}
 	}
}
  
