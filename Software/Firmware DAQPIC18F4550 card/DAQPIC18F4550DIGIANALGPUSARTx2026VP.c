/*********************************************************/
/*      PROGRAMA GENÉRICO DE LA DAQPIC18F4550            */
/*      PROGRAMA PILOTO DE PRUEBA PARA PIC18F4550        */
/*      LEE UN COMANDO PROPORCIONADO POR LABVIEW         */
/*      A TRAVÉS DEDEL PUERTO USB                        */
/*              V3.0 07/01/2026                          */
//              V3.1 21/05/2026                          */
//              V3.2 29/05/2026                          */
//              V3.3 02/06/2026                          */
/*********************************************************/

#include <18F4550.h>
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN

#device adc=10
#use delay(clock=48000000)
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7)
#include <usb_cdc.h>
#include <usb_desc_cdc_alex.h>

volatile unsigned int8 ID_operacion, puerto, operacion, dato, resultado, fuente=0;
volatile unsigned int8 resultadoAlto, resultadoBajo, contador;
volatile unsigned int16 resultado16;


//*****************************************************************************
//                      enviarResultado1B
//               Para uso con puertos digitales
//*****************************************************************************
void enviarResultado1B(int fuente){
   if(fuente==1){
      putc(resultado);   // se manda el dato capturado en el puertoA
   }                    // mediante el USART hacia la PC
   else{ 
      usb_cdc_putc(resultado); // se manda el dato capturado en el puertoA
   }
}


//*****************************************************************************
//                      PUERTOS DIGITALES
//*****************************************************************************
void puertos_digitales(int puerto){
   // ESTA FUNCION ACTIVA LOS PUERTOS DIGITALES DE ENTRADA/SALIDA
   switch(puerto){
      case 0:   //SELECCION DEL PUERTOA
         if (operacion == 0){    //operacion ==0 puertoA como salidas
            set_tris_A(0x00);    // configura puertoA como salidas
            output_A(dato);      // manda el dato a las salidas del puertoA
            }
         else{
            set_tris_A(0xFF);    // configura puertoA como entradas
            resultado=input_A()&0x7F; // captura el dato a la entrada del puertoA 
            
            enviarResultado1B(fuente); //mediante el puerto USB hacia la PC
         
         }
         break;
               
      case 1:   //SELECCION DEL PUERTOB
         if (operacion == 0){    //operacion ==0 puertoB como salidas
            set_tris_B(0x00);    // configura puertoB como salidas
            output_B(dato);      // manda el dato a las salidas del puertoB
            }
         else{
            set_tris_B(0xFF);    // configura puertoB como entradas
            resultado=input_B(); // captura el dato a la entrada del puertoB 
            
            enviarResultado1B(fuente); //mediante el puerto USB hacia la PC
         
         }
         break;
               
      case 2:   //SELECCION DEL PUERTOC
         if (operacion == 0){    //operacion ==0 puertoC como salidas
             set_tris_C(0x00);    // configura puertoC como salidas
             output_C(dato);      // manda el dato a las salidas del puertoC
               }
         else{
             set_tris_C(0xFF);    // configura puertoC como entradas
             resultado=input_C(); // captura el dato a la entrada del puertoC 
            
            enviarResultado1B(fuente); //mediante el puerto USB hacia la PC
         
         }
         break;

      case 3:   //SELECCION DEL PUERTOD
         if (operacion == 0){    //operacion ==0 puertoD como salidas
            set_tris_D(0x00);    // configura puertoD como salidas
            output_D(dato);      // manda el dato a las salidas del puertoD
            }
         else{
            set_tris_D(0xFF);    // configura puertoD como entradas
            resultado=input_D(); // captura el dato a la entrada del puertoD 
            
            enviarResultado1B(fuente); //mediante el puerto USB hacia la PC
         
         } 
         break;
               
      case 4:   //SELECCION DEL PUERTOE
         if (operacion == 0){    //operacion ==0 puertoE como salidas
            set_tris_E(0x00);    // configura puertoE como salidas
            output_E(dato);      // manda el dato a las salidas del puertoE
            }
         else{
            set_tris_E(0xFF);    // configura puertoE como entradas
            resultado=input_E()&0x03; // captura el dato a la entrada del puertoE 
            
            enviarResultado1B(fuente); //mediante el puerto USB hacia la PC
         
         }
         break;
      }
}

//*****************************************************************************
//                         leerADC
//               Para uso con puertos analógicos
//*****************************************************************************
void leerADC(unsigned int8 canal){
   set_adc_channel(canal);
   delay_us(300); //delay_ms(1);
   resultado16 = read_adc(ADC_START_AND_READ);
   resultadoAlto = make8(resultado16,1);
   resultadoBajo = make8(resultado16,0);
   
   delay_us(300); //delay_ms(1);
   
   if(fuente==1){
      putc(resultadoAlto); // se manda parte alta del dato capturado en el puerto
      putc(resultadoBajo); // se manda parte baja del dato capturado en el puerto      
    }                      // mediante el USART hacia la PC               
    else{
      usb_cdc_putc(resultadoAlto); // se manda por USB la parte alta del dato capturado
      usb_cdc_putc(resultadoBajo); // se manda por USB la parte baja del dato capturado
    }
}


//*****************************************************************************
//                      PUERTOS ANALOGICOS
//*****************************************************************************
void puertos_analogicos(int puerto){

   setup_adc(ADC_CLOCK_INTERNAL);

   switch(puerto){
      case 0:   //SELECCION DEL PUERTOA COMO ANALÓGICO
                // TODO EL PUERTOA ANALOGICO REFERENCIAS VDD Y VSS
         if (dato ==0){setup_adc_ports(AN0_TO_AN4);}
                // TODO EL PUERTOA ANALOGICO REFERENCIA RA3 VREF+ Y VSS
         else if (dato == 1) {setup_adc_ports(AN0_TO_AN4|VSS_VREF);}
                // TODO EL PUERTOA ANALOGICO REFERENCIAS RA3 Y RA2 VREF+ Y VREF-
         else if (dato == 2) {setup_adc_ports(AN0_TO_AN4|VREF_VREF);}
         
         leerADC(operacion);
         break;
                  
      case 1:   //SELECCION DEL PUERTOA Y PUERTOE COMO ANALÓGICOS
                //LOS PUERTOA Y PUERTOE ANALOGICO REFERENCIAS VDD Y VSS
         //if (dato ==0){setup_adc_ports(ALL_ANALOG);}
         if (dato ==0){setup_adc_ports(AN0_TO_AN7|VSS_VDD);}
                // TODO EL PUERTOA ANALOGICO REFERENCIA RA3 VREF+ Y VSS
         else if (dato == 1) {setup_adc_ports(AN0_TO_AN7|VSS_VREF);}
                // TODO EL PUERTOA ANALOGICO REFERENCIAS RA3 Y RA2 VREF+ Y VREF-
         else if (dato == 2) {setup_adc_ports(AN0_TO_AN7|VREF_VREF);}
                  
         leerADC(operacion);
         break;       
                  
      case 2:   //SELECCION DEL PUERTOA, PUERTOE y PUERTO B COMO ANALÓGICOS
                //LOS PUERTOA, PUERTOE Y PUERTOB ANALOGICOS REFERENCIAS VDD Y VSS
         //if (dato ==0){setup_adc_ports(ALL_ANALOG);}
         if (dato ==0){setup_adc_ports(ALL_ANALOG|VSS_VDD);}
                // TODO EL PUERTOA ANALOGICO REFERENCIA RA3 VREF+ Y VSS
         else if (dato == 1) {setup_adc_ports(ALL_ANALOG|VSS_VREF);}
                // TODO EL PUERTOA ANALOGICO REFERENCIAS RA3 Y RA2 VREF+ Y VREF-
         else if (dato == 2) {setup_adc_ports(ALL_ANALOG|VREF_VREF);}
         
         leerADC(operacion);
         break;  
         
      default: 
         break;
   }      
               
}

//*****************************************************************************
//                           PUERTO USART
//*****************************************************************************
void puerto_usart(int puerto){
      set_tris_C(0xFF);
      if (operacion == 0){    //operacion ==0 puertoC como salidas
            set_tris_C(0x00);    // configura puertoC como salidas
            putc(dato);      // manda el dato a las salidas del TX
            }
      else{
            set_tris_C(0xFF);    // configura puertoC como entradas
            resultado=getc();    // captura el dato a la entrada RX
            usb_cdc_putc(resultado); //semanda el dato capturado por el usart
                                 // hacia el puerto usb
            }
}

//*****************************************************************************
//                    rutina_pruebatodo
//*****************************************************************************
void rutina_pruebatodo(int puerto){
            set_tris_A(0x00);    // configura puertoE como salidas
            set_tris_B(0x00);    // configura puertoE como salidas
            set_tris_C(0x00);    // configura puertoE como salidas
            set_tris_D(0x00);    // configura puertoE como salidas            
            set_tris_E(0x00);    // configura puertoE como salidas
            
            
            output_A(0x1F);      // manda el dato a las salidas del puertoA
            output_B(0xFF);      // manda el dato a las salidas del puertoB
            output_C(0xFF);      // manda el dato a las salidas del puertoC
            output_D(0xFF);      // manda el dato a las salidas del puertoD
            output_E(0x03);      // manda el dato a las salidas del puertoE
}

//*****************************************************************************
//                rutina_entradastodo
//*****************************************************************************
void rutina_entradastodo(int puerto){
            set_tris_A(0xFF);    // configura puertoA como entradas
            set_tris_B(0xFF);    // configura puertoB como entradas
            set_tris_C(0xFF);    // configura puertoC como entradas
            set_tris_D(0xFF);    // configura puertoD como entradas            
            set_tris_E(0xFF);    // configura puertoE como entradas

}

//*****************************************************************************
//                reconectar_usb
//      Reconectar USB en caso de desconexión
//*****************************************************************************

void reconectar_usb() {
   if(!usb_cdc_connected())
   {
      usb_cdc_init();
      usb_init();
   }

   usb_task();
}



/*****************************************************************************/
/*                        MAIN                                               */
/*****************************************************************************/
void main(){

// ESTA ES UNA SECCION DE CONFIGURACION INICIAL
   setup_adc_ports(NO_ANALOGS);
   delay_ms(10);
   
   // AQUI SE DEFINE LA INTERFAZ DE COMUNICACION
   // SI EL DISPOSIIVO SE ENUMERA SERÁ USB
   // SI NO SE ENUMERA SERÁ USART
   usb_cdc_init();
   usb_init();
   
   contador = 0;
   while ((!usb_cdc_connected()&&contador<100)) {
      usb_task();
      contador = contador + 1;
      delay_ms(15);
   }
   
   if (contador<100){
      usb_task();
      contador = 0;
   }
   else {
      contador = 100;
   }
      
   while ((!usb_enumerated()&&contador<100)) {
      usb_task();      
      contador = contador + 1;
      delay_ms(20);
   }
      
   if (contador<100){
      fuente = 0; // INTERFAZ USB
   }
   else {
      fuente = 1; // INTERFAZ USART
   }
      
      
 // ESTE ES UN LAZO QUE SE EJECUTA DE MANERA PERMANENTE
   while(true){
   
      if (fuente == 1){
         //ID_operacion=getchar()-0x30;       //ID de la operación a realizar
         while(!kbhit()){}                   //espera un caracter
         ID_operacion=getc();               //ID de la operación a realizar
         while(!kbhit()){}                 //espera un caracter
         puerto=getc();                   //captura el puerto o recurso a utilizar
         while(!kbhit()){}                 //espera un caracter
         operacion=getc();              //captura la operación a realizar
      }
      else {

         // INSTRUCCIÓN PARA REPONER PROCESOS DE DESCONEXIÓN
         reconectar_usb();
   
         //ID_operacion=getchar()-0x30;       //ID de la operación a realizar
         while(!usb_cdc_kbhit()){usb_task();}           //espera un caracter
         ID_operacion=usb_cdc_getc();       //ID de la operación a realizar
         while(!usb_cdc_kbhit()){usb_task();}         //espera un caracter
         puerto=usb_cdc_getc();           //captura el puerto o recurso a utilizar
         while(!usb_cdc_kbhit()){usb_task();}       //espera un caracter
         operacion=usb_cdc_getc();      //captura la operación a realizar
      }
      
   // Una vez que se ha recibido los comandos se procede a ejecutar la operación
   switch(ID_operacion){
      //********************************************************************************
      case  0:     // no se asignó tareas para este número de ID_operacion
         rutina_pruebatodo(puerto);
         set_tris_A(0x00);    // configura puertoA como salidas
         output_A(0x29);      // manda el dato a las salidas del puertoA
         break;
      //********************************************************************************
      case  1:     // USO DE PUERTOS DIGITALES como salidas o como entradas
         if(operacion == 0){
            if(fuente==1){
               while(!kbhit()){}         //espera un caracter
               dato=getc();              //captura el dato a mostrar en operaciones de salida            
            }
            else{
            while(!usb_cdc_kbhit()){usb_task();}         //espera un caracter
            dato=usb_cdc_getc(); //captura el dato a mostrar en operaciones de salida
            }
         }
         else {}
         puertos_digitales(puerto);
         break;
      //********************************************************************************        
      case  2:     //USO DE PUERTOS ANALÓGICOS PA Y PE 
         if(fuente==1){
            while(!kbhit()){}         //espera un caracter
            dato=getc(); //aquí dato captura la referencia para operaciones CAD
         }
         else{
            while(!usb_cdc_kbhit()){usb_task();}         //espera un caracter
            dato=usb_cdc_getc(); //aquí dato captura la referencia para operaciones CAD  
         }
         puertos_analogicos(puerto);
         break;

      //********************************************************************************        
      case  3:     //COMUNICACION SERIAL USART RS232 
         if(operacion == 0){
            if(fuente==1){
               while(!kbhit()){}         //espera un caracter
               dato=getc();              //captura el dato a mostrar en operaciones de salida            
            }
            else{
               while(!usb_cdc_kbhit()){usb_task();}         //espera un caracter
               dato=usb_cdc_getc(); //captura el dato a mostrar en operaciones de salida
            }
         }
         else {}
         puerto_usart(puerto);
         
         break;
            
       default:   // no se asignó tareas para este número de ID_operacion
         rutina_entradastodo(puerto);        
         break;
      
      }
   }

}



