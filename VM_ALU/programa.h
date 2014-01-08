/**
 * Simulador de arquitectura hipotética especificada en el PDF del
 * proyecto final, interpretamos bitecode del compilado generado con el "compiler"
 * proporcionado, y ejecutamos el programa.
 *
 * Registros: 14
 * Memoria primaria
 * Unidad Aritmético-Lógica y Unidad de Control
 * Syscalls
 * Manejo de errores. 
 *
 * Además la máquina virtual consta de 16 opcodes: 4 de aritmética entera, 4 operaciones de bits
 * 4 operaciones de memoria, 3 operaciones de salto de instrucción y de llamada al sistema.
 * 14 registros: 8 de propósito general, dos de argumentos para llamadas al sistema, uno de registro
 * de retorno de datos de llamada a sistema, un registro contador de programa, un registro de apuntador
 * a la pila de memoria y un registro para apuntar a direcciones de regreso
 * 8 syscalls: 4 para leer de la consola y 4 para escribir.  
 *
 * @author: Martinez Rodriguez Rodrigo Ivan
 * @author: Valdivia Carbonell Mariana
 * @version: 3.14
 */

/* 
 * Cabeceras para cargar lo necesario
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Creamos una funcion para poder asegurar que trabajaremos con valores enteros o flotantes
 * con esto damos opcion a que con lo que trabajemos sea un tipo conocido
 */
union ENT_FLOTANTE
{
  int i;
  float f;
};

//Creamos un arreglo con 14 registros, para operar con ENT_FLOTANTE
union ENT_FLOTANTE reg[14]; 
//Contador para medir el tamaño de nuestra memoria
int TAM_MEMORIA;
//Apuntador al Final de pila la Memoria.
int FINAL_MEMORIA_PUNT;

//Esto yo no queria ponerlo :<(
//Indicador de la linea actual en ejecucion 
int lineaEnEjecucion;

//COntador del reloj
int relojCRS;


//Aqui viene lo divertido!! Ajaaaaa,!!!
/*
 * Comenzamos a programar todas las posibles operaciones que hara el programa
 * basandonos en las que vienen definidas en el PDF!!! 
 */

/*
 * Antes que nada creamos una funcion que verifica si la direccion de memoria es valida
 */
void checkMem(union ENT_FLOTANTE * mem, int x, int dirMemo)
{
  if(!x)
  {
    if(FINAL_MEMORIA_PUNT == TAM_MEMORIA)
    {
      fputs("Error, estas fuera de la memoria\n", stderr);
      exit(3);  
	} 
  }else
  {
    if(dirMemo < FINAL_MEMORIA_PUNT || dirMemo >= TAM_MEMORIA)
    {
	  fputs("Error, la direccion de la memoria es invalida\n", stderr);
	  exit(2);
	}
  }
}

/*
 * Funcion para las llamadas al sistema
 * Lecturas y Escrituras
 */
void syscall(union ENT_FLOTANTE * mem){
	//Se definen operadores
	char ayuda;
	char *string;
	int s; //es para los for de abajo
	
	switch(reg[8].i){
		case 0x0: //leer un entero
			scanf("%i",&(reg[10].i));
			break;
		case 0x1: //leer un caracter
			scanf("%c",(char*)&(reg[10].i));
			break;
		case 0x2: //leer un flotante
			scanf("%f",&(reg[10].f));
			break;
		case 0x3: //leer una cadena
			scanf("%s",string);
			reg[10].i = FINAL_MEMORIA_PUNT;
			for (s = 0 ; s < strlen(string) ; s++){
				mem[FINAL_MEMORIA_PUNT].i = string[s];    
				checkMem(mem,0,0);
				reg[13].i = ++FINAL_MEMORIA_PUNT;
				checkMem(mem,0,0);
			}
			break;
		case 0x4: //escribir un entero
			printf("%d\n",reg[9].i);
			break;
		case 0x5: //escribir un caracter
			printf("%c\n",reg[9].i);
			break;
		case 0x6: //escribir un flotante
			printf("%f\n",reg[9].f);
			break;
		case 0x7: //escribir una cadena
			for(s = reg[9].i,ayuda = mem[s].i ; ayuda != '\0' ; s++){
				ayuda = mem[s].i;
				printf("%c", ayuda);
			}
			printf("\n");
			break;
		case 0x8: //salir
			printf("\n%i\n", relojCRS);
			exit(0); //salimos de forma genial! exit success
		default:
			fputs("Error en la llamada al Sitema, revisa tus parametros, el syscall no jala \n", stderr);
			exit(6); //Error Codigo de llamada al sistema invalido
    }  
}

/*
 * Lee archivo y lo pasa a memoria
 */
void leerArchivo(char* name_File, union ENT_FLOTANTE * mem)
{
	int byteNumber;
	FILE *f;
	f = fopen(name_File ,"r");
	do
	{
		byteNumber =  fread(&mem[FINAL_MEMORIA_PUNT], 1, 1, f);
		checkMem(mem,0,0);
		reg[13].i = ++FINAL_MEMORIA_PUNT;
		checkMem(mem,0,0);    
	}while(byteNumber);
	fclose(f);   
}

/*
 * Suma entera (con signo)
 * @param memoria del programa
 * Duración en ciclos de reloj: 3
 */
void add(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > registro1 ||registro1 > 13 || 0 > registro2 ||registro2 > 13 || 0 > temporalS ||temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:1 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }  
  int *Result = &reg[temporalS].i; // guardamos en Result
  int o1 = reg[registro1].i; // guardamos int en registro1
  int o2 = reg[registro2].i; // guardamos int en registro2

  *Result = o1 + o2;  // hacemos operacion y guardamos en Result
  relojCRS += 3;      // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Diferencia entera (con signo)
 * @param memoria del programa
 * Duración en ciclos de reloj: 4
 */
void sub(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > registro1 ||registro1 > 13 || 0 > registro2 ||registro2 > 13 || 0 > temporalS ||temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:2 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }      
  int *Result = &reg[temporalS].i; // guardamos en Result
  int o1 = reg[registro1].i; // guardamos int en registro1
  int o2 = reg[registro2].i; // guardamos int en registro2
	
  *Result = o1 - o2; // hacemos operacion y guardamos en Result
  relojCRS += 4; // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Producto entero (con signo)
 * @param memoria del programa
 * duración en ciclos de reloj: 10
 */
void mul(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > registro1 ||registro1 > 13 || 0 > registro2 ||registro2 > 13 || 0 > temporalS ||temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:3 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }
  int * Result = &reg[temporalS].i; // guardamos en Result
  int o1 = reg[registro1].i; // guardamos int en registro1
  int o2 = reg[registro2].i; // guardamos int en registro2
	
  *Result = o1 * o2; // hacemos operacion y guardamos en Result
  relojCRS += 10; // aumentamos a la duración del reloj
  relojCRS++; //aumentamos uno mas para que nos de el resutado 
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Cociente entero (con signo)
 * Se considera la division entre cero
 * @param memoria del programa
 * duración en ciclos de reloj : 11
 */
void divi(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > registro1 ||registro1 > 13 || 0 > registro2 ||registro2 > 13 || 0 > temporalS ||temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:4 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }

  //Esto es para que validar que no estemos dividiendo entre cero
  if (reg[registro2].i == 0)
  {
    fputs("Estas dividiendo entre cero, y nadie puede hacer eso\n", stderr);
    exit(1); // Error Division entre cero
  }
  int *Result = &reg[temporalS].i; // guardamos en Result
  int o1 = reg[registro1].i; // guardamos int en registro1
  int o2 = reg[registro2].i; // guardamos int en registro2
	
  *Result = o1 / o2; // hacemos operacion y guardamos en Result
  relojCRS += 11; // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Suma flotante
 * @param memoria del programa
 * duración en ciclos de reloj: 4
 */
void fadd(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > registro1 ||registro1 > 13 || 0 > registro2 ||registro2 > 13 || 0 > temporalS ||temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:5 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }      
  float *Result = &reg[temporalS].f; // guardamos en Result
  float o1 = reg[registro1].f; // guardamos int en registro1
  float o2 = reg[registro2].f; // guardamos int en registro2
	
  *Result = o1 + o2; // hacemos operacion y guardamos en Result
  relojCRS += 4; // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Diferencia flotante
 * @param memoria del programa
 * duración en ciclos de reloj: 5
 */
void fsub(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > registro1 ||registro1 > 13 || 0 > registro2 ||registro2 > 13 || 0 > temporalS ||temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:6 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }      
  float * Result = &reg[temporalS].f; // guardamos en Result
  float o1 = reg[registro1].f; // guardamos int en registro1
  float o2 = reg[registro2].f; // guardamos int en registro2
  
  *Result = o1 - o2; // hacemos operacion y guardamos en Result
  relojCRS += 5; // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Producto flotante
 * @param memoria del programa
 * duración en ciclos de reloj: 9
 */
void fmul(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > registro1 ||registro1 > 13 || 0 > registro2 ||registro2 > 13 || 0 > temporalS ||temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:7 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }      
  float * Result = &reg[temporalS].f; // guardamos en Result
  float o1 = reg[registro1].f; // guardamos int en registro1
  float o2 = reg[registro2].f; // guardamos int en registro2
  
  *Result = o1 * o2; // hacemos operacion y guardamos en Result
  relojCRS += 9; // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Cociente flotante
 * @param memoria del programa
 * duración en ciclos de reloj: 10
 */
void fdiv(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > registro1 ||registro1 > 13 || 0 > registro2 ||registro2 > 13 || 0 > temporalS ||temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:8 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }

  //Esto es para que validar que no estemos dividiendo entre cero
  if (reg[registro2].f == 0)
  {
    fputs("[ERROR] DIVIDED BY ZERO\n", stderr);
    exit(1); // Error Division entre cero
  }
  float * Result = &reg[temporalS].f; // guardamos en Result
  float o1 = reg[registro1].f; // guardamos int en registro1
  float o2 = reg[registro2].f; // guardamos int en registro2
  
  *Result = o1 / o2; // hacemos operacion y guardamos en Result
  relojCRS += 10; // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Operador de bits AND
 * @param memoria del programa
 * duración en ciclos de reloj: 1
 */
void and(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > registro1 ||registro1 > 13 || 0 > registro2 ||registro2 > 13 || 0 > temporalS ||temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:9 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }

  reg[temporalS].i = reg[registro1].i & reg[registro2].i; // hacemos operacion y la guardamos en el registro temportal
  relojCRS ++;  // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Operador de bits OR
 * @param memoria del programa
 * duración en ciclos de reloj: 1
 */
void or(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > registro1 ||registro1 > 13 || 0 > registro2 ||registro2 > 13 || 0 > temporalS ||temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:10 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }      
  reg[temporalS].i = reg[registro1].i | reg[registro2].i; // hacemos operacion y la guardamos en el registro temportal
  relojCRS ++; // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Operador de bits XOR
 * @param memoria del programa
 * duración en ciclos de reloj: 1
 */
void xor(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > registro1 ||registro1 > 13 || 0 > registro2 ||registro2 > 13 || 0 > temporalS ||temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:11 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }
  reg[temporalS].i = reg[registro1].i ^ reg[registro2].i; // hacemos operacion y la guardamos en el registro temportal
  relojCRS ++; // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

void not(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > registro1 ||registro1 > 13 || 0 > registro2 ||registro2 > 13 || 0 > temporalS ||temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:12 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }      
  reg[temporalS].i = ~reg[registro2].i; // hacemos operacion y la guardamos en el registro temportal
  relojCRS ++; // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Cargar byte
 * @param memoria del programa
 * duración en ciclos de reloj: 500
 */
void lb(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  //este no se usa ahora
  //int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if( 0 > temporalS ||temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:13 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }      
  reg[temporalS].i = registro1; // hacemos operacion y la guardamos en el registro temportal
  relojCRS += 500; // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Cargar palabra (4 bytes)
 * @param memoria del programa
 * duración en ciclos de reloj: 1500
 */
void lw(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  //este no se usa ahora
  //int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > temporalS || temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:14 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }      
  reg[temporalS].i = registro1; // hacemos operacion y la guardamos en el registro temportal
  relojCRS += 1500; // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Guardar byte
 * @param memoria del programa
 * duración en ciclos de reloj: 700
 */
void sb(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  //este no se usa ahora
  //int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > temporalS || temporalS > 13)
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:15 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }      
  reg[temporalS].i = registro1; // hacemos operacion y la guardamos en el registro temportal
  relojCRS += 700; // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Guardar palabra
 * @param memoria del programa
 * duración en ciclos de reloj: 2100
 */
void sw(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  //este no se usa ahora
  //int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > temporalS || temporalS > 13 )
  {
    fputs("Registro Invalido, Revisa los valores ErrorNum:16 \n", stderr);
    exit(4); // Error Número de registro ínvalido
  }      
  reg[temporalS].i = registro1; // hacemos operacion y la guardamos en el registro temportal
  relojCRS += 2100; // aumentamos a la duración del reloj
  lineaEnEjecucion++; // incrementamos el contador de la liena de ejecucion del programa
    
}

/*
 * Cargar valor constante
 * @param memoria del programa
 * duración en ciclos de reloj: 1500
 */
void li(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int temporalS = mem[++lineaEnEjecucion].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  //validacion para tamaños de registros checamos índices del arreglo para no salirnos
  if(0 > temporalS || temporalS > 13 )
  {
	fputs("Registro Invalido, Revisa los valores ErrorNum:17 \n", stderr);
	exit(4); // Error Número de registro ínvalido
  }
  int entAuxiliar = registro1; //Entero auxiliar
  entAuxiliar << 8;
  entAuxiliar = entAuxiliar | registro2;
  entAuxiliar << 8;
  registro1 = mem[++lineaEnEjecucion].i; //operaciones de intercambio
  registro2 = mem[++lineaEnEjecucion].i;
  entAuxiliar = entAuxiliar | registro1;
  entAuxiliar << 8;
  entAuxiliar = entAuxiliar | registro2;
  reg[temporalS].i = entAuxiliar;  
  relojCRS += 1500;// aumentamos a la duración del reloj
  lineaEnEjecucion++;  // incrementamos el contador de la liena de ejecucion del programa
}

/*
 * Salto incondicional
 * @param memoria del programa
 * duración en ciclos de reloj: 1
 */
void b(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int registro1 = mem[++lineaEnEjecucion].i;
  int rSalto = reg[registro1].i;

  //validacion para saber si aun estamos dentro de la memoria permitida
  if(rSalto >= FINAL_MEMORIA_PUNT)
  {
	fputs("Direccion de Memoria invalida, Revisa los registros ErrorNum:18\n", stderr);
	exit(2); // Error Direccion de memoria invalida
  }
  relojCRS++; // aumentamos a la duración del reloj
  lineaEnEjecucion = rSalto; //La linea de ejecucion sera ahora el valor del reg
}

/*
 * Salto si es igual a 0 
 * @param memoria del programa
 * duración en ciclos de reloj: 4
 */
void beqz(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int lineaEjec = mem[++lineaEnEjecucion].i;
  int rSalto = reg[lineaEjec].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;

  int o2 = reg[registro2].i;

  // Vaidamos que el salto se dara solo si el reg es igual a cero
  if(o2 == 0)
  {    
	if(rSalto >= FINAL_MEMORIA_PUNT)
	{
      fputs("Direccion de Memoria invalida, Revisa los registros ErrorNum:19\n", stderr);
	  exit(2); // Error Direccion de memoria invalida
	}
	lineaEnEjecucion = rSalto; 
  }else
  {
    lineaEnEjecucion++;
  }
  relojCRS += 4; // aumentamos a la duración del reloj
}

/*
 * Salto si es menor que 0
 * @param memoria del programa
 * duración en ciclos de reloj: 5
 */
void bltz(union ENT_FLOTANTE * mem)
{
  //Se definen operadores para poder realizar la operacion
  int lineaEjec = mem[++lineaEnEjecucion].i;
  int rSalto = reg[lineaEjec].i;
  int registro1 = mem[++lineaEnEjecucion].i;
  int registro2 = mem[++lineaEnEjecucion].i;
  int o2 = reg[registro2].i;

  // Vaidamos que el salto se dara solo si el reg es menor a cero
  if(o2 <= 0)
  {
    if(rSalto >= FINAL_MEMORIA_PUNT)
    {
      fputs("Direccion de Memoria invalida, Revisa los registros ErrorNum:20\n", stderr);
      exit(2); // Error Direccion de memoria invalida
    }
    lineaEnEjecucion = rSalto;
  }else
  {
    lineaEnEjecucion ++;
  }
  relojCRS += 5; // aumentamos a la duración del reloj
}

/*
 * Pues selecciona la opcion mas acorde al opcode a ejecutar
 *
 * @param mem: La memoria en la que esta el programa
 */
void opCodeL(union ENT_FLOTANTE * mem){
  int opCode = mem[lineaEnEjecucion].i;
  switch(opCode){
    case 0x0: add(mem);
      break;
	case 0x1: sub(mem);
	  break;
	case 0x2: mul(mem);
	  break;
	case 0x3: divi(mem);
	  break;
	case 0x4: fadd(mem);
	  break;
	case 0x5: fsub(mem);
	  break;
	case 0x6: fmul(mem);
	  break;
	case 0x7: fdiv(mem);
	  break;
	case 0x8: and(mem);
	  break;
	case 0x9: or(mem);
	  break;
	case 0xA: xor(mem);
	  break;
	case 0xB: not(mem);
	  break;
	case 0xC: lb(mem);
	  break;
	case 0xD: lw(mem);
	  break;
	case 0xE: sb(mem);
	  break;
	case 0xF: sw(mem);
	  break;
	case 0x10: li(mem);
	  break;
	case 0x11: b(mem);
	  break;
	case 0x12: beqz(mem);
	  break;
	case 0x13: beqz(mem);
	  break;
	case 0x14:
		  lineaEnEjecucion +=3;
		  relojCRS += 50;
		  syscall(mem);
		  lineaEnEjecucion++;
	  break;
	default:
		  fputs("[ERROR] INVALID OPCOADE1\n", stderr);
		  exit(5);
	  break; 
  }
}
