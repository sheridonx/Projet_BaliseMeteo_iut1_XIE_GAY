/* 
 * File:   I2C.h
 * Author: vince
 *
 * Created on 27 février 2015, 11:12
 */
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// Déclaration des E/S pour communication I2C (Entrées au départ)
#define SDA1	    	PORTCbits.4		 	// Signal SDA
#define SCL1		PORTCbits.3		 	// Signal SCL
// A configurer en entrées à l'initialisation

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// Prototypes des fonctions pour gestion I2C
void i2c_init(void);
void i2c_start(void);
void i2c_restart(void);
void i2c_stop(void);
void i2c_ack(void);
void i2c_error(void);
void i2c_ecrire(char i2c_octet);
char i2c_lire(int ack);

void i2c_1trame(char add, char i2c_octet);

void i2c_1trame(char i2c_add, char i2c_octet)
{
    i2c_start();
    i2c_ecrire(i2c_add);
    i2c_ecrire(i2c_octet);
    i2c_stop();
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// Utilisation Module interne pour Communication I2C  //////////////////////////////////////////////////
// Utilisation Horloge interne (4 MHz) ATTENTION si différente: Actualiser SSPADD/////////
///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void i2c_init(void)
{
    SSP1STAT=0b10000000;//Lecture des flag(259) communicaton standard (100khz/1Mhz)
   
    SSP1CON1=0b00101000; //Pins SDA et SCL configurer comme des inputs (mais mode série validé)(260)
    SSP1CON2=0b00000000; //Automatic in hardware (not idle) (262)
    SSP1CON3=0b00001000; //Enable interrupt (263)
    SSP1ADD=0x27; //Clock PIC 16Mhz, et Fclock I²C 100Khz Baud generator(258)
    
    SSP1IF=0; //Flag interrupt a zéro
}

//////////////////////////////////////////////////////////////////////////
void i2c_start(void)
{
        SSP1CON2bits.SEN=1; //SEN=1					// Start I2C
        while (SSP1CON2bits.SEN==1);				// Test: fin de transmission
}

//////////////////////////////////////////////////////////////////////////
void i2c_restart(void)
{
	SSP1CON2bits.RSEN=1;						// Repeat-Start I2C
	while (SSP1CON2bits.RSEN==1);			// Test: fin de transmission
}

//////////////////////////////////////////////////////////////////////////
void i2c_stop(void)
{
	SSP1CON2bits.PEN=1;
	while (SSP1CON2bits.PEN==1);				// Test: fin de transmission
}

//////////////////////////////////////////////////////////////////////////
void i2c_error(void)
{
}

//////////////////////////////////////////////////////////////////////////
void i2c_ack(void)
{
	if (SSP1CON2bits.ACKSTAT==1)				// Pas de réception acknowledge de l'esclave
		i2c_error();
}

//////////////////////////////////////////////////////////////////////////
void i2c_ecrire(char i2c_octet)
{
	                
        SSP1BUF=i2c_octet;
        SSP1IF=0;
        while (SSP1IF==0);
        
	while(SSP1CON2bits.ACKSTAT==1);				// Attente acknowledge
        SSP1IF=0;
        //while (SSP1IF==1);
       // SSP1IF=0;

}

//////////////////////////////////////////////////////////////////////////
char i2c_lire(int ack)			// ack=0: Acknowledge; ack=1: No Acknowledge
{
	char	octetlu;
	
        SSP1CON2bits.RCEN=1;				// Réception de l'octet émis par esclave
	SSP1IF=0;
        while (SSPIF==0);
        

        while (SSP1CON2bits.RCEN==1);			// Test fin de transmission
	if (ack==0)
		SSP1CON2bits.ACKDT=0;
	else
		SSP1CON2bits.ACKDT=1;
	octetlu=SSPBUF;
        SSP1CON2bits.ACKEN=1;					// Envoie acknowledge avec valeur de ACKDT
        
        //while (SSPIF==1);
           
        while (SSP1CON2bits.ACKEN==1);			// Test fin de transmission
	
	return(octetlu);
}


