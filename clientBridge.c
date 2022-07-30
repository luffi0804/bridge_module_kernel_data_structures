#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "clientBridge.h"

void write_struct(int fd, unsigned long command, struct complex_struct * struct_address){
    if (ioctl(fd, command, struct_address) == -1){
        perror("Write message error at ioctl");
    }
}

void write_message(int fd, unsigned long command, char * message){
    if (ioctl(fd, command, message) == -1){
        perror("Write message error at ioctl");
    }
}

void read_message(int fd, unsigned long command, char * message){
    if(ioctl(fd, command, message) == -1){
	perror("Read message error at ioctl");
    }
}

void read_message_param(int fd, unsigned long command, int * value){
    if(ioctl(fd, command, value) == -1){
        perror("Read message param error at ioctl");
    }else{
        printf("Copy the messsage from the kernel\n");
    }
}

void write_int(int fd, unsigned long command, int * value){
    if (ioctl(fd, command, value) == -1){
        perror("Write int error at ioctl");
    }
}

void read_int(int fd, unsigned long command, int * value){
    if(ioctl(fd, command, value) == -1){
        perror("Read int error at ioctl");
    }else{
        printf("Copy the int from the kernel\n");
    }
}

int send_empty_command(int fd, unsigned long command){
    int returned_value = ioctl(fd, command);
    if(returned_value == -1){
       perror("Send command error at ioctl");
    }else{
	return returned_value;
        printf("Command OK to the kernel\n");
    }
}

void write_several_messages(int fd){
    write_message(fd, BRIDGE_W_S, "Message 1");
    write_message(fd, BRIDGE_W_S, "Message 2");
    write_message(fd, BRIDGE_W_S, "Message 3");
}

void read_all_messages(int fd){
	char message[100];
	while( send_empty_command(fd, BRIDGE_STATE_S) > 0){
	    read_message(fd, BRIDGE_R_S, message);
	    printf("Message: %s\n", message);
	}
}
// punto 1
void punto_1(int fd)
{
    send_empty_command(fd, BRIDGE_CREATE_S);
    FILE *entrada;
    char texto[100];
    char texto_salida[100];
    entrada = fopen("archivo.txt", "r");
    if (entrada == NULL)
    {
        printf("no se abrio el archivo");
    }
    while (fgets(texto, 100, entrada))
    {
        write_message(fd, BRIDGE_W_S, texto);
    }
    read_message(fd, BRIDGE_R_S, texto_salida);
    printf("valor %s/n", texto_salida);
}

void punto_2(int fd)
{
}

void punto_3(int fd)
{
    send_empty_command(fd, BRIDGE_CREATE_S);
    
   const char* nombreArchivo = "documentoPunto3.txt";

    FILE* archivo = fopen(nombreArchivo, "r");
    
    struct stat sb;

    if (stat(nombreArchivo, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    int cantidad = sb.st_size;
    char* contenidoArchivo = malloc(cantidad);

    fread(contenidoArchivo, cantidad, 1, archivo);

    int cantidadParentesisIzquierdo = 0;
    int cantidadParentesisDerecho = 0;
    int cantidadLlaveIzquierda = 0;
    int cantidadLlaveDerecha = 0;


    for (int i = 1; i <= cantidad; i++){
        char caracter = contenidoArchivo[i];     
        write_message(fd, BRIDGE_W_S, &caracter);  
    }

    char message[100];

    for (int i = 1; i <= cantidad; i++){
        
        read_message(fd, BRIDGE_R_S, message);
        printf("se leyo: %s\n ", message);
        
        if(*message == '(' ){
            cantidadParentesisIzquierdo = cantidadParentesisIzquierdo + 1;
            printf("se encontro un parentesis izquierdo\n");
        }   

        if( *message == ')' ){
            cantidadParentesisDerecho = cantidadParentesisDerecho + 1;
            printf("se encontro un parentesis derecho\n");
        }

        if( *message == '{' ){
            cantidadLlaveIzquierda = cantidadLlaveIzquierda + 1;
            printf("se encontro un llave izquierda\n");
        }

        if( *message == '}' ){
            cantidadLlaveDerecha = cantidadLlaveDerecha + 1;
            printf("se encontro un llave derecha\n");
        }
    }
    
    if(cantidadParentesisIzquierdo == cantidadParentesisDerecho && cantidadLlaveIzquierda == cantidadLlaveDerecha){
        printf("el codigo fuente esta balanceado\n");
    }else{
        printf("el codigo fuente no esta balanceado\n");
    }
    
}

void punto_4(int fd)
{
    send_empty_command(fd, BRIDGE_DESTROY_Q);

    int n, opcion;
    char texto[100];
    char textoSalida[100];
    do
    {
        printf("\n   1. Escribir en cola alta.");
        printf("\n   2. Escribir en cola media.");
        printf("\n   3. Escribir en cola baja");
        printf("\n   4. Leer en cola alta.");
        printf("\n   5. Leer en cola media.");
        printf("\n   6. Leer en cola baja");
        printf("\n   7. Salir\n");


        scanf("%d", &opcion);

        switch (opcion){
        case 1:
            printf("\n   Introduzca el mensaje a ingresar el cola alta ");
            scanf("%s", texto);
            write_message(fd, BRIDGE_W_HIGH_PRIOR_Q, texto);
            break;

        case 2:
            printf("\n   Introduzca el mensaje a ingresar el cola media ");
            scanf("%s", texto);
            write_message(fd, BRIDGE_W_MIDDLE_PRIOR_Q, texto);
            break;

        case 3:
            printf("\n   Introduzca el mensaje a ingresar el cola baja ");
            scanf("%s", texto);
            write_message(fd, BRIDGE_W_LOW_PRIOR_Q, texto);
            break;

         case 4:
            read_message(fd, BRIDGE_R_HIGH_PRIOR_Q, textoSalida);
            printf("valor obtenido: %s\n ", textoSalida);
            break;

        case 5:
            read_message(fd, BRIDGE_R_MIDDLE_PRIOR_Q, textoSalida);
            printf("valor obtenido: %s\n ", textoSalida);
            break;

        case 6:
            read_message(fd, BRIDGE_R_LOW_PRIOR_Q, textoSalida);
            printf("valor obtenido: %s\n ", textoSalida);
            break;

        } 
    } while (opcion != 7);
}
void punto_5(int fd)
{
    send_empty_command(fd, BRIDGE_DESTROY_L);
}

void punto_6(int fd)
{
}

void punto_7(int fd)
{
}

void punto_8(int fd)
{
}

void punto_9(int fd)
{
}

void punto_10(int fd){

    send_empty_command(fd, BRIDGE_CREATE_S);
    int opcion = 0;
    int cantidad = 0;
    char texto[2];
    char textoNuevo[2] = "0";
    char textoSalida[2];


    do
    {
        printf("\n   1. Ingresar un numero.");
        printf("\n   2. Retornar el mayor.");
        printf("\n   3. Salir.\n");
        scanf("%d", &opcion);

        if(opcion == 1){
            printf("\n   Introduzca un numero ");
            scanf("%s", texto);
            write_message(fd, BRIDGE_W_S, texto);
            cantidad = cantidad + 1;
         }
       
        if(opcion == 2){
            for (int i = 0; i <= cantidad; i++){
                read_message(fd, BRIDGE_R_S, textoSalida);

                if(*textoSalida > *textoNuevo){
                    *textoNuevo = *textoSalida;
                }
            }   
            printf("el numero mayor es: %s\n ", textoNuevo);
         }

    } while (opcion != 3);

}

int main(int argc, char *argv[]){

    const char *file_name = "/dev/bridge"; //used by ioctl
    int fd;

    fd = open(file_name, O_RDWR);
    if (fd == -1){
        perror("Bridge ioctl file open");
        return 2;
    }

    punto_1(fd);
    send_empty_command(fd, BRIDGE_DESTROY_S);
    punto_3(fd);
    send_empty_command(fd, BRIDGE_DESTROY_S);
    punto_4(fd);
    send_empty_command(fd, BRIDGE_DESTROY_Q);
    punto_5(fd);
    punto_10(fd);
    send_empty_command(fd, BRIDGE_DESTROY_S);

    close (fd);
    return 0;
}
