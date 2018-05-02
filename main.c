#include <18F4620.h>
#include <STDIO.H>
#include <stdlib.h>

#use delay(clock=16000000)
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG

#use RS232(BAUD=9600, XMIT= PIN_C6, rcv=PIN_C7, BITS=8,PARITY=N, STOP=1)
#use fast_io(a)
#use fast_io(b)
#use fast_io(c)
#use fast_io(d)
#use fast_io(e)

void mostrarDatos(char vect[],int tam);
int evaluarBackSpace(char vect[], int cont);
int validarCadena(char vect[], int cont);
void operandos(char vect[],int tam);
long operacion(char vect[], int cont);
void rutinaDeError();
int numero[3];
int num1, num2;

void main (void){
   setup_oscillator(OSC_16MHZ);
   set_tris_b(0x00);
   set_tris_d(0x00);
   char dato[15];
   signed long resultado = 0x00;
   int contCaracter = 0x00, flagValido = 0x00;

   printf("<xxx,xxx,operacion>");
   printf("\r");
   
   while(1){
      if(kbhit()){
         dato[contCaracter] = getch(); 
         printf("%c", dato[contCaracter]);
         contCaracter= evaluarBackSpace(dato,contCaracter);
         
         if(dato[contCaracter] == 13){
            if(dato[0] == '<' && dato[contCaracter-1] == '>'){
               flagValido = validarCadena(dato, contCaracter);
               if(flagValido == 0x01){
                  mostrarDatos(dato, contCaracter);
                  operandos(dato, contCaracter);
                  if(num1 < 0x80 && num2 < 0x80){
                     resultado = operacion(dato, contCaracter);
                     //resultado = (long)num1 * (long)num2;
                     output_b(resultado);
                     output_d(resultado>>8);
                     printf("El resultado es: %ld",resultado);
                     printf("\r");
                     resultado = 0x00;
                     contCaracter = 0x00;
                  }
                  else{
                    printf("Valores fuera de rango");
                    printf("\r"); 
                    rutinaDeError();
                    resultado = 0x00;
                    contCaracter = 0x00;
                  }

               }
               else{
                  printf("Valores no validos");
                  printf("\r");
                  rutinaDeError();
                  contCaracter = 0x00;
               }   
            }
            else{
               printf("Parametros no validos");
               printf("\r");
               rutinaDeError();
               contCaracter == 0;
            }
            contCaracter = 0x00;
         }
         else
            contCaracter++;                     
      }
   }
}

/*FUNCIONES*/

void mostrarDatos(char vect[],int tam){
   for(int i = 0; i < tam; i++){
      printf("%c",vect[i]);
      printf("\r");                       
   }
}

int validarCadena(char vect[], int cont){
   int contValido = 0x00;
      for(int i = 1; i < cont-1; i++){
         if(vect[i] > 41 && vect[i] < 58){
            contValido++;
            if (contValido == cont-2){
               return 0x01;
            }
         }
         else
            return 0x00;
      }
}

void operandos(char vect[],int cont ){
   int contNumero = 0x00, contDigitos = 0x00;
   for(int i = 1; i < cont-1; i++){
      switch(vect[i]){
      case ',':
         
         numero[0] = vect[i-3];
         numero[1] = vect[i-2];
         numero[2] = vect[i-1];  
         contNumero++;
         
         if (contNumero == 1){
            num1 = atoi(numero);
            numero[0] = NULL;
            numero[1] = NULL;
            numero[2] = NULL;
         }
         else if(contNumero == 2){
            num2 = atoi(numero);
            numero[0] = NULL;
            numero[1] = NULL;
            numero[2] = NULL;
         } 
         break;
      }
   }  
}

long operacion(char vect[], int cont){
   switch(vect[cont-2]){
   case '+':
      return (long)num1 + (long)num2;
      break;
   case '-':
      return (long)num1 - (long)num2;
      break;
   case '*':
      return (long)num1 * (long)num2;
      break;
   case '/':
      if(num2 == 0){
         printf("Operacion no valida");
         printf("\r");
         rutinaDeError();
      }
      else
         return (long)num1 / (long)num2;
      break;
   }
}

int evaluarBackSpace(char vect[], int cont){
   if(vect[cont] == 8){
      return cont-2;
   }
   else{
      return cont;
   }
}

void rutinaDeError(){
   for(int i = 0 ; i < 3 ; i++){
      output_b(0xFF);
      output_d(0xFF);
      delay_ms(50);
      output_b(0x00);
      output_d(0x00);
      delay_ms(50);
   }
}
