#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../header/app.h"

/* Estructura receta */
struct recipe
{
	char name[30], type[20];
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
  	char name[15], imc[6];
	List * favIngredients;
	List * favRecipes;
	List * mealPlan;
};

/* Estructura para favoritos */
struct item
{
  	char name[30];
};

/* Función que permite ingresar datos a una lista dentro de una tabla hash */
void insertIntoList(char * sType, HashMap * map, char * key, item * newItem)
{
	/* Obtención del nodo desde tabla hash (puede ser recetas o usuarios) */
  	HashPair * searchInMap = searchMap(map, key);

  	/* Si el nodo buscado existe */
  	if(searchInMap)
  	{
		List * listAux;
		
    	/* Obtención de la lista desde nodo->value. Pero primero verificar con qué elemento se trabaja */
		if(strcmp(sType, "ingredient") == 0)
		{
			/* Lista ingredientes favoritos (existe en mapa usuarios) */
			listAux = ((user *) searchInMap->value)->favIngredients;
		}
		else if(strcmp(sType, "recipe") == 0)
		{
			/* Lista recetas favoritas (existe en mapa usuarios) */
			listAux = ((user *) searchInMap->value)->favRecipes;
		}
		else if(strcmp(sType, "recipe_ingredient") == 0)
		{
			/* Lista ingredientes (existe en mapa recetas) */
			listAux = ((recipe *) searchInMap->value)->ingredients;
		}

		/* Comprobamos de que existan elementos en la lista */
		if(firstList(listAux))
		{
			/* Recorremos los nombres que hay en la lista */
			for(item * checkDuplicate = (item *) firstList(listAux); checkDuplicate != NULL; checkDuplicate = (item *) nextList(listAux))
			{
				/* Si ya existe el nombre que se quiere ingresar, error */
				if(strcmp(checkDuplicate->name, newItem->name) == 0)
				{
					printf("\nYa había sido agregado.\n");
					exit(EXIT_FAILURE);
				}
			}
		}

		/* Se inserta el nombre en la lista */
   		pushBack(listAux, newItem);
  	}
}

/* Función válida únicamente para árbol de popularidades. Crea u obtiene la lista dentro de nodo->value */
void insertListIntoTree(TreeMap * rByPopularity, unsigned short * popularity, char * recipeName)
{
	/* Creación de un nuevo item (nombre de receta) para la lista que está en el árbol de popularidades */
	item * addRecipeName = (item *) malloc(sizeof(item));
	strncpy(addRecipeName->name, recipeName, sizeof(addRecipeName->name));

	/* Buscar si la popularidad está en el árbol. Si no existe, se ingresa. */
	TreePair * searchTree = searchTreeMap(rByPopularity, popularity);
	if(!searchTree)
	{
		/* Creación de lista, agregar nombre e insertar al árbol la nueva popularidad (la lista es el value) */
		List * recipeNames = createList();
		pushBack(recipeNames, addRecipeName);
		insertTreeMap(rByPopularity, popularity, recipeNames);
	}
	else
	{	
		/* Como ya existe, obtenemos la lista de nombres y agregamos un nombre más */
    	List * listAux = (List *) searchTree->value;
    	pushBack(listAux, addRecipeName);
	}
}

/* Función que permite importar todos los archivos de la carpeta /file a tablas y listas correspondientes */
void importDatabases(TreeMap * rByPopularity, HashMap * mapUsers, HashMap * mapIngredients, HashMap * mapRecipes)
{
	/*************************************************************/
	/*                Importación de usuarios                    */
	/*************************************************************/
	
	/* Variable exclusiva para líneas de usuarios */
    char readLineUsers[30];

    /* Abrimos el archivo de usuarios en modo lectura. Si no existe, se acaba el programa. */
    FILE * usersDB = fopen("./file/usuarios.csv", "r");
	if(!usersDB)
	{
		printf("Archivo de usuarios no existe.\n");
		exit(EXIT_FAILURE);
	}

 	/* Leemos cada línea hasta que no quede alguna */
	while(fgets(readLineUsers, 30, usersDB))
	{
		/* Reservamos memoria para un nuevo nodo usuario */
    	user * importUser = (user *) malloc(sizeof(user));
		
		/* Separamos los siguientes datos: nombre e IMC */
		strncpy(importUser->name, strtok(readLineUsers, ","), sizeof(importUser->name));
		strncpy(importUser->imc, strtok(NULL, "\n"), sizeof(importUser->imc));
		importUser->favIngredients = createList();
		importUser->favRecipes = createList();
		importUser->mealPlan = createList();

		/* Inserción a mapa usuarios */ 
    	insertMap(mapUsers, importUser->name, importUser);
	}

	/*************************************************************/
	/*                Importación de favoritos                   */
	/*************************************************************/

	/* Variable exclusiva para líneas de favoritos */
    char readLineFavs[50];

    /* Abrimos el archivo de usuarios en modo lectura. Si no existe, se acaba el programa. */
    FILE * favsDB = fopen("./file/favoritos.csv", "r");
	if(!favsDB)
	{
		printf("Archivo de favoritos no existe.\n");
		exit(EXIT_FAILURE);
	}

 	/* Ciclo para leer cada favorito e insertarlo a una lista */
	while(fgets(readLineFavs, 50, favsDB))
	{
		/* Reservamos memoria para un nuevo favorito */
    	item * newFav = (item *) malloc(sizeof(item));
		
		/* Separamos los siguientes datos: nombre, usuario al que pertenece y tipo de favorito */
		strncpy(newFav->name, strtok(readLineFavs, ","), sizeof(newFav->name));
		char * readFavOwner = strtok(NULL, ",");
		char * readFavType = strtok(NULL, "\n");

    	/* Inserción en lista de ingredientes o recetas favorit@s */
		insertIntoList(readFavType, mapUsers, readFavOwner, newFav);
	}
	
	/*************************************************************/
	/*                 Importación de recetas                    */
	/*************************************************************/

    /* Variable exclusiva para líneas de recetas */
  	char readLineRecipes[100];

	/* Abrimos el archivo de recetas en modo lectura */
  	FILE * recipesDB = fopen("./file/recetas.csv", "r");
	if(!recipesDB)
	{
		printf("Archivo de recetas no existe.\n");
		exit(EXIT_FAILURE);
	}

	/* Leemos cada línea hasta que no quede alguna */
	while(fgets(readLineRecipes, 100, recipesDB))
	{
    	/* Creación de nodo tipo receta */
    	recipe * importRecipe = (recipe *) malloc(sizeof(recipe));
		
    	/* Asignación de los valores a cada elemento de la estructura receta */
    	strncpy(importRecipe->name, strtok(readLineRecipes, ","), sizeof(importRecipe->name));
    	importRecipe->popularity  = (unsigned short) atoi(strtok(NULL, ","));
    	strncpy(importRecipe->type, strtok(NULL, "\n"), sizeof(importRecipe->type));
		importRecipe->ingredients = createList();

		insertListIntoTree(rByPopularity, &importRecipe->popularity, importRecipe->name);

    	/* Insertamos nodo en tabla hash */
    	insertMap(mapRecipes, importRecipe->name, importRecipe);
	}

	/*************************************************************/
	/*          Importación de ingredientes de recetas           */
	/*************************************************************/

	/* Variable exclusiva para líneas de favoritos */
    char readRecipeIngredients[50];

    /* Abrimos el archivo de usuarios en modo lectura. Si no existe, se acaba el programa. */
    FILE * recipeIngredientsDB = fopen("./file/ingredientes_recetas.csv", "r");
	if(!recipeIngredientsDB)
	{
		printf("Archivo de ingredientes no existe.\n");
		exit(EXIT_FAILURE);
	}

 	/* Ciclo para leer cada ingrediente e insertarlo a una lista */
	while(fgets(readRecipeIngredients, 50, recipeIngredientsDB))
	{
		/* Reservamos memoria para un nuevo ingrediente*/
    	item * newIngredient = (item *) malloc(sizeof(item));
		
		/* Separamos los siguientes datos: nombre y receta al que pertenece */
		strncpy(newIngredient->name, strtok(readRecipeIngredients, ","), sizeof(newIngredient->name));
		char * readRecipeName = strtok(NULL, "\n");
		
    	/* Inserción en lista de ingredientes o recetas favorit@s */
		insertIntoList("recipe_ingredient", mapRecipes, readRecipeName, newIngredient);
	}

	/*************************************************************/
	/*               Importación de ingredientes                 */
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

	/* Leemos cada línea hasta que no quede alguna */
	while(fgets(readLineIngredients, 100, ingredientsDB))
	{
    	/* Creación de nodo tipo ingrediente */
    	ingredient * importIngredient = (ingredient *) malloc(sizeof(ingredient));
		
    	/* Asignación de los valores a cada elemento de la estructura ingrediente */
    	strncpy(importIngredient->name, strtok(readLineIngredients, ","), sizeof(importIngredient->name));
    	importIngredient->quantity  = (unsigned short) atoi(strtok(NULL, ","));
    	importIngredient->calories = (unsigned short) atoi(strtok(NULL, ","));
    	strncpy(importIngredient->owner, strtok(NULL, "\n"), sizeof(importIngredient->owner));
    
    	/* Insertamos nodo en tabla hash */
		//importar solo los del usuario que inició sesión
    	insertMap(mapIngredients, importIngredient->name, importIngredient);
	}

	/* Cierre de todos los archivos usados */
	fclose(usersDB);
	fclose(favsDB);
	fclose(recipesDB);
	fclose(recipeIngredientsDB);
	fclose(ingredientsDB);
}

/* Función que permite al usuario ingresar o crear un usuario */
int userLogin(HashMap * mapUsers, char * userID)
{
	unsigned short whatToDo = 0, count;
	char username[15];
	float weight, height, imc;
	HashPair * userSearch;
	
	fflush(stdin);
	printf("\nBienvenido al programa. Puede iniciar sesión (1) o crear un nuevo usuario (2).\n");

	/* El usuario podrá ingresar solo una de las dos opciones a la vez */
	do
	{
		printf("\nSeleccione una de las anteriores: ");
		count = scanf("%hu", &whatToDo);
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
    		count = scanf("%15s", username);
    		getchar();

			/* Búsqueda del usuario ingresado. Si no existe, error. */
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
			/* Pedimos el nombre del nuevo usuario */
			printf("\nHa seleccionado crear un nuevo usuario.\nPor favor, ingrese su nombre: ");
			count = scanf("%15s", username);
			getchar();

			/* Verificación del usuario. Si existe el nombre, error. */
			userSearch = searchMap(mapUsers, username);
			if(userSearch) return 1;

			/* Pedimos al nuevo usuario información adicional */
			printf("Ingrese su peso (Ej: 87): ");
			count = scanf("%f", &weight);
			
			printf("Por último, ingrese su altura (Ejemplo: 1.60): ");
			count = scanf("%f", &height);

			/* Calculamos el imc a partir de los datos ingresados */
			imc = weight / pow(height, 2);

			/* Reservamos memoria para un nuevo usuario */
			user * newUser = (user *) malloc(sizeof(user));

			/* Estructuramos los datos del usuario  */
			strncpy(newUser->name, username, sizeof(newUser->name));
			gcvt(imc, 3, newUser->imc);
			newUser->favIngredients = createList();
			newUser->favRecipes = createList();
			newUser->mealPlan = createList();

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

/* Función para agregar un nuevo ingrediente aparte de los que ya existen */
int addIngredient(HashMap * mapIngredients, char * userID)
{
	unsigned short i, ingredientsQty, count;
	HashPair * ingredientSearch;
	
	/* Pedimos la cantidad de ingredientes */
	fflush(stdin);
	printf("\n¿Cuántos ingredientes desea ingresar?: ");
	count = scanf("%hu", &ingredientsQty);

	for(i = 1; i <= ingredientsQty; i++)
	{
		printf("\n");

		/* Reservamos memoria para un nuevo ingrediente */
		ingredient * newIngredient = (ingredient *) malloc(sizeof(ingredient));
		
		/* Pedimos al usuario información sobre el ingrediente */
		printf("Nombre del ingrediente %hu: ", i);
		count = scanf("%20s", newIngredient->name);
		getchar();

		/* Comprobamos si es que ya existe el ingrediente con el mismo dueño */
		ingredientSearch = searchMap(mapIngredients, newIngredient->name);
		if(ingredientSearch)
		{
			/* Comparamos el dueño del ingrediente con el nombre del usuario actual */
			if(strcmp(((ingredient *) ingredientSearch->value)->owner, userID) == 0)
			{
				/* Error */
				return 1;
			}
		}

		strncpy(newIngredient->owner, userID, sizeof(newIngredient->owner));

		printf("Cantidad del ingrediente %hu: ", i);
		count = scanf("%hu", &newIngredient->quantity);

		printf("Calorías del ingrediente %hu por porción: ", i);
		count = scanf("%hu", &newIngredient->calories);

		/* Se inserta el ingrediente en la tabla hash */
		insertMap(mapIngredients, newIngredient->name, newIngredient);
	}
	/* Se ingresó correctamente */
	return 0;
}

/* Función para agregar una nueva receta aparte de las que ya existen */
int addRecipe(HashMap * mapRecipes, TreeMap * rByPopularity, char * userID)
{
	char temp, ingredientName[20];
	unsigned short i, j, recipesQty, ingredientsQty, count;
	
	/* Pedimos la cantidad de ingredientes */
	fflush(stdin);
	printf("\n¿Cuántas recetas desea ingresar?: ");
	count = scanf("%hu", &recipesQty);

	for(i = 1; i <= recipesQty; i++)
	{
		printf("\n");
		
		/* Reservamos memoria para una nueva receta */
		recipe * newRecipe = (recipe *) malloc(sizeof(recipe));
		
		/* Pedimos al usuario información sobre el ingrediente */
		printf("Nombre de la receta %hu: ", i);
		count = scanf("%c", &temp);
  		fgets(newRecipe->name, sizeof(newRecipe->name), stdin);
  		newRecipe->name[strcspn(newRecipe->name, "\n")] = 0;

		/* Si ya existe alguna receta con el mismo nombre, retorna error */
		if(searchMap(mapRecipes, newRecipe->name)) return 1;

		printf("Popularidad de la receta %hu: ", i);
		count = scanf("%hu", &newRecipe->popularity);

		insertListIntoTree(rByPopularity, &newRecipe->popularity, newRecipe->name);

		printf("Tipo de receta %hu: ", i);
		count = scanf("%10s", newRecipe->type);
		getchar();

		newRecipe->ingredients = createList();

		/* Se inserta la receta en la tabla hash */
		insertMap(mapRecipes, newRecipe->name, newRecipe);

		/* Ya que es necesario que exista la receta para ingresar ingredientes, este apartado queda acá */
		printf("Cantidad de ingredientes de la receta %hu: ", i);
		count = scanf("%hu", &ingredientsQty);

		/* Ciclo para ingresar ingredientes */
		for(j = 1; j <= ingredientsQty; j++)
		{
			/* Memoria para un nuevo ingrediente */
			item * newIngredient = (item *) malloc(sizeof(item));
			
			printf("Ingrese nombre del ingrediente %hu: ", j);
			count = scanf("%20s", newIngredient->name);
			getchar();

			/* Se inserta el ingrediente en la lista */
			insertIntoList("recipe_ingredient", mapRecipes, newRecipe->name, newIngredient);
		}
	}
	/* Se ingresó correctamente */
	return 0;
}

/* Función para añadir favoritos */
int addFavs(HashMap * mapUsers, HashMap * mapIngredients, HashMap * mapRecipes, char * userID)
{
	char typeOfFav[15], name[30], temp;
	unsigned short i, qty, count;

	/* Pedimos al usuario el tipo de favorito */
	fflush(stdin);
	printf("\nIngrese qué tipo de favorito quiere ingresar (ingrediente o receta): ");
	count = scanf("%15s", typeOfFav);
	getchar();

	/* Clasificación según tipo de favorito */
	if(strcmp(typeOfFav, "ingrediente") == 0)
	{
		/* Cantidad máxima de i. favoritos */
		printf("\n¿Cuántos ingredientes desea ingresar?: ");
		count = scanf("%hu", &qty);

    	for(i = 1; i <= qty; i++)
		{
			printf("\n");
			
			/* Pedimos el nombre del nuevo ingrediente favorito */
			printf("Nombre del ingrediente %hu: ", i);
			count = scanf("%20s", name);
			getchar();

			/* Búsqueda del ingrediente. Si no existe, error. */
			if(!searchMap(mapIngredients, name)) return 1;

			/* Reservamos memoria para un nuevo ingrediente favorito */
			item * newFavIngredient = (item *) malloc(sizeof(item));
			strncpy(newFavIngredient->name, name, sizeof(newFavIngredient->name));

			/* Inserción en lista de ingredientes favoritos */
		  	insertIntoList("ingredient", mapUsers, userID, newFavIngredient);
		}
		return 0;
	}
	else if(strcmp(typeOfFav, "receta") == 0)
	{
		/* Cantidad máxima de r. favoritas */
		printf("\n¿Cuántas recetas desea ingresar?: ");
		count = scanf("%hu", &qty);

    	for(i = 1; i <= qty; i++)
		{
			printf("\n");
			
			/* Pedimos el nombre de la nueva receta favorita */
			printf("Nombre de la receta %hu: ", i);
			count = scanf("%c", &temp);
  			fgets(name, sizeof(name), stdin);
  			name[strcspn(name, "\n")] = 0;

			/* Búsqueda de la receta. Si no existe, error. */
			if(!searchMap(mapRecipes, name)) return 1;

			/* Reservamos memoria para una nueva receta favorita */
			item * newFavRecipe = (item *) malloc(sizeof(item));
			strncpy(newFavRecipe->name, name, sizeof(newFavRecipe->name));

			/* Inserción en lista de recetas favoritas */
		  	insertIntoList("recipe", mapUsers, userID, newFavRecipe);
		}
		return 0;
	}
 	else
	{
    	printf("Opción inválida. Pruebe con alguna de las anteriores.");
    	return 1; 
  	}
}

/* Función para mostrar recetas con dos tipos de filtros */
int showRecipes(HashMap * mapRecipes, TreeMap * rByPopularity)
{
  	unsigned short whatToDo = 0, count;

	/* Desplegamos submenú con las opciones de búsqueda */
	fflush(stdin);
	printf("\nA continuación, usted podrá elegir: \n");
  	printf("(1) Mostrar todos los nombres\n");
  	printf("(2) Mostrar por popularidad\n");

	/* Solo una opción a la vez, 1 o 2 */
  	do
	{
		printf("\nSeleccione una de las anteriores: ");
		count = scanf("%hu", &whatToDo);
	} while(whatToDo < 1 || whatToDo > 2);
	
	printf("\nLista de recetas disponibles:\n");
  	switch(whatToDo)
	{
		/* Mostrar por nombre */
      	case 1:
		{
			/* Recorrido por el mapa de recetas */
          	for(HashPair * receta = firstMap(mapRecipes); receta != NULL; receta = nextMap(mapRecipes))
	        {
				/* Para obtener nodo->value */
				recipe * recipeValues = (recipe *) receta->value;
				
				/* Nombre de la receta */
              	printf("\n• %s\n", (char*) receta->key);
				printf("• Tipo de receta: %s\n", (char*) recipeValues->type);

				/* Obtenemos la lista de ingredientes desde nodo->value */
				printf("• Ingredientes: ");
				for(item * listIngredients = (item *) firstList(recipeValues->ingredients); listIngredients != NULL; listIngredients = (item *) nextList(recipeValues->ingredients))
				{
					/* Se imprime cada nombre de ingrediente */
					printf("%s - ", listIngredients->name);
				}
				printf("\n");
            }
            break;
      	}
		/* Mostrar por popularidad */
     	case 2:
        {
			/* Recorremos el árbol de forma creciente, en búsqueda de popularidades */
        	for(TreePair * newSearch = firstTreeMap(rByPopularity); newSearch != NULL; newSearch = nextTreeMap(rByPopularity))
			{
				/* Número de popularidad */
          		printf("%hu: ", *newSearch->key);

				/* Lista de recetas que tienen la misma popularidad */
				for(item * listNames = (item *) firstList(newSearch->value); listNames != NULL; listNames = (item *) nextList(newSearch->value))
				{
					/* Nombre de la receta */
					printf("%s - ", listNames->name);
				}
				printf("\n");
        	}
          	break;
      	}
 	}
}

/*int desingMinutas(HashMap * mapRecipes, HashMap * mapUsers, char * userID){


  // creacion de listas segun el horario de comida
  List * recetasDesayuno = createList();
  List * recetasAlmuerzo = createList();
  List * recetasCena = createList();

  // Se recorre el mapa de recetas y se van almacenando las recetas segun el tipo
  for(HashPair * receta = firstMap(mapRecipes); receta != NULL; receta = nextMap(mapRecipes)){
    recipe * recipeValues = (recipe *) receta->value;

    if(recipeValues->type == "almuerzo") pushBack(recetasAlmuerzo,recipeValues->name);

    elif(recipeValues->type == "desayuno") pushBack(recetasDesayuno, recipeValues->name);

    elif(recipeValues->type == "cena") pushBack(recetasCena, recipeValues->name);
  }

  // buscamos la primera receta de las favoritas del usuario
  HashPair * searchUser = searchMap(mapUsers, userID);

  // Si el usuario existe, listAux se define como la lista de recetas favoritas
  if(searchUser){
    List * listAux;  
    listAux = &((user *) searchUser->value)->favIngredients;
  }
  // posible funcion
  // Recorremos la lista de recetas favoritas
  for(recipe * rFav = firstList(listAux); rFav != NULL; rFav = nextList(listAux)){

    // ingredientes de la receta favorita
    List * ingRecFav = rFav->ingredients;

    // recorremos lista buscando ingredientes con x% de similitud

    for(recipe * rDesayuno = firstList(recetaDesayuno); rDesayuno != NULL; rDesayuno = nextList(recetaDesayuno)){

      // ingredientes receta de desayuno
      List* ingRecDes = rDesayuno->ingredients;
      for(List * ingComun = firstList(ingRecFav); ingComun!= NULL; ingComun = nextList(ingRecFav)){

        

      }
      
    

      
    }
  }
}*/