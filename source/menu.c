#include <stdio.h>
#include <stdlib.h>

#include "../header/app.h"
#include "../header/menu.h"

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

void showMenu()
{
	printf("\n%sMenú de opciones%s\n\n", "\x1B[1m", "\x1B[0m");

	printf("(1) Iniciar sesión\n");
	printf("(2) Agregar ingredientes\n");
  printf("(3) Mostrar recetas\n");
	printf("(4) Agregar receta\n");
	printf("(5) Agregar receta(s) o ingrediente(s) favorit@(s)\n");
	printf("(6) Recetas recomendadas\n");
	printf("(7) Diseñar minuta\n");
	printf("(8) Ver minuta de otros usuarios\n");
	printf("(0) Terminar el programa\n");
  
	printf("\nIndique la accion deseada (solo números): ");
}

void showChoice(List * rByAffinity, HashMap * mapRecipes, HashMap * mapIngredients, HashMap * mapUsers, TreeMap * rByPopularity, short choice, char * userID)
{
	int msg;

	/* Si el msg = 1, entonces es ocurrió un error */
	/* Si el msg = 0, entonces se ejecutó correctamente */
  	switch(choice)
	{
    	case 0:
		{
      		printf(GREEN "\n*********************\n");
  			printf("* Fin del programa. *\n");
  			printf("*********************\n" RESET);
      		break;
		}
		case 1:
		{
      		msg = userLogin(mapUsers, &userID);

     		if(msg == 1)
			{
       			printf(RED "\nNo se puede crear el usuario o no existe." RESET);
      		}
			else if(msg == 2)
			{
        		printf(GREEN "\n************************\n");
        		printf("* ¡Ha iniciado sesión! *\n");
        		printf("************************\n" RESET);
      		}
			else if(msg == 3)
			{
        		printf(GREEN "\n********************\n");
        		printf("* ¡Usuario creado! *\n");
        		printf("********************\n" RESET);
      		}
  			break;
		}
  		case 2:
		{
      		//msg = addIngredient(mapRecipes, mapIngredients);

      		if(msg)
			{
				printf(RED "\nFallo al agregar." RESET);
			}
			else
			{
        		printf(GREEN "\n****************************\n");
        		printf("* ¡Ingrediente(s) agregado(s)! *\n");
        		printf("****************************\n" RESET);  
      		}
  			break;
		}
  		case 3:
		{
  			msg = showRecipes(mapRecipes, rByPopularity);
			//msg = 1;

      		if(msg)
			{
        		printf(RED "\nNo se pudo procesar la solicitud." RESET);
			}
			else
			{
        		printf(GREEN "\n*********************************\n");
        		printf("* ¡Se han mostrado las recetas! *\n");
        		printf("*********************************\n" RESET);  
      		}
  			break;
		}
  		case 4:
		{
  			//msg = addRecipe(mapRecipes);
			msg = 1;

      		if(msg)
			{
        		printf(RED "\nNo se pudo añadir." RESET);
      		}
			else
			{
        		printf(GREEN "\n*********************************\n");
        		printf("* ¡Se han agregado las recetas! *\n");
        		printf("*********************************\n" RESET);  
      		}
  			break;
		}
  		case 5:
		{
  			//msg = addFavs();
			msg = 1;

      		if(msg)
			{
        		printf(RED "\nNo se pudo agregar." RESET);
      		}
			else
			{
        		printf(GREEN "\n***************************\n");
        		printf("* ¡Se han agregado tod@s! *\n");
        		printf("***************************\n" RESET);  
      		}
  			break;
		}
		case 6:
		{
  			//msg = recoRecipes(rByAffinity);
			msg = 1;

      		if(msg)
			{
        		printf(RED "\nNo se han encontrado resultados." RESET);
      		}
			else
			{
        		printf(GREEN "\n******************************************\n");
        		printf("* ¡Se han mostrado todos los resultados! *\n");
        		printf("******************************************\n" RESET);  
      		}
  			break;
		}
		case 7:
		{
  			//msg = design();
			msg = 1;

      		if(msg)
			{
        		printf(RED "\nNo se han encontrado resultados." RESET);
      		}
			else
			{
        		printf(GREEN "\n****************************\n");
        		printf("* ¡Se ha creado la minuta! *\n");
        		printf("****************************\n" RESET);  
      		}
  			break;
		}
		case 8:
		{
  			//msg = seeOthers();
			msg = 1;

      		if(msg)
			{
        		printf(RED "\nNo se han encontrado resultados." RESET);
      		}
			else
			{
        		printf(GREEN "\n******************************\n");
        		printf("* ¡Se ha mostrado la minuta! *\n");
        		printf("******************************\n" RESET);  
      		}
  			break;
		}
  		default:
		{
  			printf(RED "\n**************************\n");
  			printf("* ¡No existe tal opción! *\n");
  			printf("**************************\n" RESET);
  			break;
		}
	}
  	printf("\n");
}