/// Programme VinYu meteo programme 1 essai ////////////

// On inclus la librairie du PIC
#include <p18f26k22.h>

// Raccourci des commandes sur la liaison I2C (afficheur et capteur température)
#include "Commands.h"

//On inclue la librairie I2C
#include "I2C.h"

//On inclue l'affichage
#include "Fonctions_Affich.h"

//On inclue l'affichage
#include "Mode_Affich.h"

//On inclue le debug par la liaison RS232
#include "RS232.h"

//On inclue la gestion TX20
#include "TX20.h"

// PIC18F26K22 Configuration Bit Settings
#include "Config_Bits.c"

// DS1621 Configuration Bit Settings
#include "DS1621_Commands.c"

//Pour interruption
#include "xc.h"



void Init(void);
char rafraich(void);
void tempo_1ms(void);
void tempo_Xms(long int time);
void tempo_200us(void);
void tempo_1ms200us(void);



/////////Variable Globale ///////////

    char tab[50]; //Tableau pour traiter les données du vent
    //char chgmt=0; //Variable pour le rafraichissement de l'écran
    char bouton=0; //Valeur initiale du bouton à 0
    char inter; // situation de vérification d'interruption
    unsigned long curs; //situation du curseur
/////////////////////////////////////

//////// Interrupts/ ///////////
    void interrupt tc_int(void)             // High priority interrupt
{
    if (INT0IF)
    {
          bouton=bouton+1;
          //chgmt=1;
          inter=1;
          curs=1;
          tempo_Xms(500); // On attend pour éviter que le compteur s'affole lorsque l'on reste appuyé
          INT0IF=0; // On baisse le flag pour relancer l'IT
    }
}


void main(void) {

    char temp;
    char TH=26;
    char TL=24;
 

    Init();
    i2c_init();
    InitLCD();
    DS1621_init(0x0A,TH,TL);
    InitRS232();						// 9600 Bauds
    
    temp=DS1621_lire2octets(Read_Temperature); //on lance une première conversion de température
    DetectVent(tab); //et de vent

    while(1){


          inter=0;
          switch (bouton){

              
              case 1 :  AffichTemp(curs, temp);
                        break;       
              case 2 : Affich_V_Vent_kmh(curs, tab);
                        break;
              case 3 : Affich_V_Vent_ms(curs, tab);
                        break;
              case 4 : Affich_D_Vent(curs, tab);
                        break;
              case 5 :  LCD_CLEAR;
                        afftext("Retour Debut");
                        bouton=0;
                        break;  
              }

          if(inter==0)
          curs++;
          //chgmt=0;
          
      //}


     temp=rafraich();
             
      /* RS232_Envoie_Car(0xAA);
      tempo_Xms(2000);
      RS232(CR);
      RS232MotBin(tempH);
      RS232_Envoie_Car(LF);
      RS232MotBin(tempL);*/
    }
    
}


void Init(void) 
{    
    //Initialisation des I/O
	ANSELA=0;						// Pas d'entrée analogique portA
	ANSELB=0;						// Pas d'entrée analogique portB
        ANSELC=0;						// Pas d'entrée analogique portA
	TRISA=0x00;						//PORTA en sortie
	TRISB=0xA3;				 		//PORTB en 10100011
        PORTC=0x18;                                             //On met le port b en initialisation
	TRISC=0x98;                                             //PORTC SDA SCL en entrées, txgsm entrée


///************Activation des UARTs **************///////////////
        PORTCbits.RX1=1;                        //PORTC activation RX
        PORTCbits.TX1=1;                        //PORTC activation TX
        PORTBbits.RX2=1;                        //PORTB activation RX
        PORTBbits.TX2=1;                        //PORTB activation TX

        REG=1; // PORTB4=1 On desactive pour l'instant les receptions de données du vent en mettant a 1 reg

///// Interrupt /////
        GIE=1; //Activation interruptions generale
        INT0IE=1; //activation interrpution externe int0 PORTB0


//Timer 0
    OSCCON = 0b01110000; // intosc 16MHz, primary clock ;
    T0CON = 0b11000100; // timer0 enable, prescaller 32, internal clock;
      
}
///////////////////////////////FIN INIT ////////////////////////
///////////////////////////////////////////////////////////////

//////Rafraichissement écran //////////////////

char rafraich(void)
{
      char temp;
      if((bouton==2)||(bouton==3)) //Si je suis dnas les menus tritant le vent
        {DetectVent(tab);//chgmt=1;
        }

      if(bouton==1)
        {temp=DS1621_lire2octets(Read_Temperature);
           // chgmt=1;
        }
      
        return(temp);
    
}


////////////////Temps/////////////////////////
void tempo_Xms(long int time)
{
    int i;
    for(i=0;i<time;i++){
        tempo_1ms();
    }
}
void tempo_1ms(void)
{
//Timer0
     TMR0IF=0; //RAZ flag
     TMR0L=131;
     while(TMR0IF==0);

}
void tempo_1ms200us(void)
{
    //Timer0
     tempo_1ms();

     TMR0IF=0; //RAZ flag
     TMR0L=231;
     while(TMR0IF==0);

}
void tempo_200us(void)
{
    //Timer0

     TMR0IF=0; //RAZ flag
     TMR0L=231;
     while(TMR0IF==0);

}