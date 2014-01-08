MaquinaVirtualSPIM
==================

Maquina virtual de SPIM en lenguaje C, Se incluye compiler para SPIM y varios files de TEST, así como un PDF de especificaciones 


Maquina Virtual de SPIM en C




--Sobre el programa

La finalidad de este proyecto es que implementen un simulador de una arquitectura de computadora hipotetica especificada en este documento en lenguage C.
Para poder implementar este simulador es necesario que conozcan las diferentes partes que lo componen y posteriormente lean la especificación dada de la máquina para implementarla fidedignamente. En este caso, van a implementar un int´ erprete del código máquina de la máquina virtual especificada. Para facilitar la programación de dicha máquina se les proporcionará un compilador de un lenguaje ensamblador similar a MIPS al código máquina del simulador que deben implementar.


--Requerimientos del simulador

El simulador que implementen, además de poder ejecutar programas escritos en el lenguaje de máquina especificado en la sección anterior, deberá implementar las siguientes caracteristicas:

1. Argumentos de linea de comando
La invocación de su máquina virtual deberá ser de la forma

$ ./myvm -m 65536 helloworld.bin

Donde el argumento -m representa el tama˜ño de la memoria principal (medida en bytes), y el archivo helloworld.bin representa un programa escrito en el lenguaje máquina que interpreta su simulador.


--Ejecucion
En caso de que la ejecución del programa sea satisfactoria, su simulador debe imprimir al final un número entero positivo que representa el tiempo de ejecución del programa medido en ciclos de reloj. Ejemplo:

$ ./myvm -m 1048576 helloworld.bin

Hello World!
6950

Para ello deberán llevar la cuenta en ciclos de reloj de cada instrucción que ejecuta el programa e imprimir la suma al final de la ejecución. La duración en ciclos de reloj de cada instrucción está especificada en la tabla de operaciones que deben implementar.
Si el programa produce un error fatal en su máquina virtual, la ejecución se deberá finalizar inmediatamente y deberán guardar un volcado de la memoria en el archivo dumpfile.bin. No deben imprimir el número de ciclos de reloj en este caso, deben salir inmediatamente.


--Compilador
Para facilitar la programación de su simulador, se les proporcionará un compilador de lenguaje ensamblador que podrán usar para producir código máquina de la máquina virtual que deben implementar. La semántica del compilador es exactamente igual que los códigos de operación de su máquina virtual, y la sintaxis es muy similar a la del interprete de SPIM.


--Instalación
Para poder instalar el simulador, deberán desempacar el tarball donde viene el código fuente y compilarlo ingresando en el subdirectorio src y tecleando lo siguiente:

$ make

Al final de este proceso deberán obtener un archivo ejecutable llamado sasm. La sintaxis del ensamblador es:

$ sasm <archivo fuente> <binario destino>

El compilador está totalmente contenido en dicho ejecutable y lo pueden mover a donde necesiten para poder compilar sus programas.


--Paquete de pruebas
El paquete de pruebas estará contenido en el subdirectorio test. Para probar su proyecto, compilen su máquina virtual y llamen al ejecutable del simulador svm. Posteriormente colóquenlo en test y estando en dicho subdirectorio ejecuten:

$ ./test


Este fue un proyecto para la materia de Arquitectura de Computadoras, Obligatoria para la carrera Ciencias de la Computacion impartida en la Facultad de Ciencias de la UNAM.


--Se distribuye bajo la licencia MIT--
