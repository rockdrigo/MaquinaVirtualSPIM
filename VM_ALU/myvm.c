//Porfin!

/*
 * Ejecuta el interprete a partir de una entrada de la forma:
 *-m TAM_MEMORIA NombreDelArchivo
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "programa.h"


int main (int arg_size, char** args){
	//Si tienen los argumentos mal ya sea por el numero de argumentos o por los argumentos
	if(arg_size != 4 && strcmp("-m", args[1]) != 0 ){
		printf("[ERROR] INVALID ARGUMENTS");
		exit(0);
	}
	//Si si estan correctos los datos de entrada
	
	int memory = atoi(args[2]);
	union ENT_FLOTANTE  mem [memory];	
	TAM_MEMORIA = memory;
	lineaEnEjecucion = 0;
	relojCRS = 0;
	FINAL_MEMORIA_PUNT = 0;
	int i; 
	for(i = 0; i < 14; i++){
		reg[i].i = 0; 
	}
	leerArchivo(args[3],mem);
  
	do{
		opCodeL(mem);
	}while (1) ;
}
