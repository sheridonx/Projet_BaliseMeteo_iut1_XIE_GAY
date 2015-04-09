////////////////////////////////////////////////////////////////////////////////////////
//
//	Fichier RS232.h
//
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// Déclaration des E/S
#define		TX		PORTBbits.TX2
#define		RX		PORTBbits.RX2
// A configurer en entrées à l'initialisation

///////////////////////////////////////////////////////////////////////////////////////
// Prototypes des fonctions pour gestion série RS232
void InitRS232(void);
char RS232_Recu(void);
void RS232_Envoie_Car(char data);
void RS232_Recu_Buffer(char *pTab, char size);
unsigned long ValeurBufferDec(char *pTab,char size);
unsigned long ValeurBufferHexa(char *pTab,char size);
void RS232_Envoie_Text(const char *ptext);
void RS232_Envoie_Tab1(const char *pTab, char size);
void RS232_Envoie_Tab2(char *pTab, char size);
char RS232_Envoie_Tab2_Sum(char *pTab, char size);
void RS232TabHexa(char *pTab, char size);
void RS232TabDec(const char *pTab, char size);
void RS232OctetDec(char data);
void RS232OctetBin(char data);
void RS232OctetHexa(char data);
void RS232MotDec(unsigned long data);
void RS232MotBin(unsigned long data);
void RS232MotHexa(unsigned long data);
//void RS232FloatDec(float24 data);

// Commande AT
void EnvoieAT(const char *ptext);


///////////////////////////////////////////////////////////////////////////////////////
// Commandes pour Terminal
#define		LF				0x0A	    	// Saut de Ligne
#define		CR				0x0D	    	// Retour à la ligne
#define		TAB				0x09			// Tabulation horizontale
//#define		VT				0x0B	    	// Tabulation verticale

///////////////////////////////////////////////////////////////////////////////////////
// Macros pour Terminal
#define SL RS232_Envoie_Car(10); //RS232_Envoie_Car(13)


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//Fonction pour la Configuration Module EUASRT
void InitRS232(void)
{

//	9600 Bauds, 8 bits, pas de parité (Pas de parité hardware), 1 bit de stop
	// Pour Fosc=4MHz
    
        TXSTA2=0b00000100;
        RCSTA2=0b10010000;
        BAUDCON2=0b01010000;
        
	SPBRGH2=0;
	SPBRG2=25;		// Voir datasheet pour calcul (Fosc=4Mhz : H interne par défaut)
	TXSTA2bits.BRGH=1;			// High speed
	BAUDCON2bits.BRG16=0;			// 8bit baud rate
	TXSTA2bits.SYNC=0;			// Mode Asynchrone
	


	// Configuration commune
	TXSTA2bits.TXEN=0;				// Emission désactivée
	RCSTA2bits.CREN=1;				// Réception activée
	RCSTA2bits.SPEN=1;				// Module activé
	RC2IF=0;					// Drapeau interruption: Arrivé d'un octet
	RC2IE=1;					// Autorisation interruption depuis réception UART
	PEIE=1;						// Autorisation interruption depuis périphériques
}



///////////////////////////////////////////////////////////////////////////////////////
//Fonction pour Reception série RS232 (Utilisation Module UART)
// Bloquante tant que TXSTA2BITS.TRMT=0
char RS232_Recu(void)
{
	char	temp;
	temp=0;
	TXSTA2bits.TXEN=0;						// Emission désactivée
	RCSTA2bits.CREN=1;						// Réception activée
	RC2IF=0;
	while(RCIF==0);
	temp=RCREG2;
	RC2IF=0;
	return(temp);
}

///////////////////////////////////////////////////////////////////////////////////////
// Fonction pour Emission série RS232 (Utilisation Module UART)
void RS232_Envoie_Car(char data)
{
	RCSTA2bits.CREN=0;						// Réception désactivée
	TXSTA2bits.TXEN=1;						// Emission activée
	TXREG2=data;
	TXSTA2bits.TRMT=0;
	while(TXSTA2bits.TRMT==0);
	TXSTA2bits.TXEN=0;						// Emission désactivée
	RCSTA2bits.CREN=1;						// Réception activée
}

///////////////////////////////////////////////////////////////////////////////////////
// Fonction pour Reception d'une trame série RS232 (Utilisation Module UART)
// Fin de la trame avec le caractere LF (Termite), stocké en fin de chaîne
// Bloquante tant que RCIF=0
void RS232_Recu_Buffer(char *pTab,char size)
{
	char	i,fin;
	fin=0;
	i=0;
	TXSTA2bits.TXEN=0;						// Emission désactivée
	RCSTA2bits.CREN=1;						// Réception activée
	do
	{
		RCIF=0;
		while(RCIF==0);
		*pTab=RCREG2;
		if (*pTab==LF)
			fin=1;
		pTab=pTab+1;
		i=i+1;
	} while ((i<size) && (fin==0));
	if ((i>=size) && (fin==0))
		*pTab=LF;
//	RCSTABITS.CREN=0;						// Réception désactivée
}

///////////////////////////////////////////////////////////////////////////////////////
// Fonction la valeur d'une trame reçue (Si caracteres de la trame sont des chiffres
// S'utilise pour un buffer composé de char '0' à '9' (non vérifié par la fonction)
// Fin de la trame avec le caractere LF (Termite), stocké en fin de chaîne
unsigned long ValeurBufferDec(char *pTab,char size)
{
	char	i;
	unsigned long	temp;
	temp=0;
	i=1;
	temp=(unsigned long)*pTab-48;
	pTab=pTab+1;
	while ((*pTab!=LF) && (i<5))
	{
		temp=temp*10;
		temp=temp+(*pTab-48);
		pTab=pTab+1;
		i=i+1;
	}
	return(temp);
}

///////////////////////////////////////////////////////////////////////////////////////
// Fonction la valeur d'une trame reçue (Si caracteres de la trame sont des chiffres
// S'utilise pour un buffer composé de char '0' à 'F' (non vérifié par la fonction)
// Fin de la trame avec le caractere LF (Termite), stocké en fin de chaîne
unsigned long ValeurBufferHexa(char *pTab,char size)
{
	char	i;
	unsigned long	temp1,temp2;
	temp1=temp2=0;
	i=1;

		while ((*pTab!=LF) && (i<5))
		{
			temp1=temp1*16;
			if ((*pTab>='0') && (*pTab<='9'))
				temp2=*pTab-48;
			if ((*pTab>='A') && (*pTab<='F'))
				temp2=*pTab-'A'+10;
			temp1=temp1+temp2;
			pTab=pTab+1;
			i=i+1;
		}
	return(temp1);
}


///////////////////////////////////////////////////////////////////////////////////////////
// Fonction pour affichage d'un texte
// La chaîne de texte se termine par '0'
void RS232_Envoie_Text(const char *ptext)
{
	RCSTA2bits.CREN=0;						// Réception désactivée
	TXSTA2bits.TXEN=1;						// Emission activée

	do
	{
		TXREG2=*ptext;
		TXSTA2bits.TRMT=0;
		while(TXSTA2bits.TRMT==0);
		ptext=ptext+1;
	}while (*ptext!=0);
	TXSTA2bits.TXEN=0;						// Emission désactivée
	RCSTA2bits.CREN=1;						// Réception activée
}

///////////////////////////////////////////////////////////////////////////////////////////
// Fonction pour affichage d'un tableau de chiffres ou symbole
void RS232_Envoie_Tab1(const char *pTab, char size)
{
	char	i;
	for (i=0;i<size;++i)
	{
		if (*pTab<=9)
			RS232_Envoie_Car(48+*pTab);
		else
			RS232_Envoie_Car(*pTab);
		RS232_Envoie_Car(';');
		pTab=pTab+1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// Fonction pour affichage d'un tableau de caracteres ASCII
// Fin du tableau avec LF
void RS232_Envoie_Tab2(char *pTab, char size)
{
	char	i;
	i=0;
	while ((*pTab!=LF) && (i<size))
	{
		RS232_Envoie_Car(*pTab);
//		RS232_Envoie_Car(';');
		pTab=pTab+1;
		i=i+1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// Fonction pour affichage d'un tableau de caracteres ASCII
// Fin du tableau avec LF + Calcul du checksum
char RS232_Envoie_Tab2_Sum(char *pTab, char size)
{
	char	i,temp;
	i=0;
	temp=0;
	while ((*pTab!=LF) && (i<size))
	{
		RS232_Envoie_Car(*pTab);
		temp=temp^(*pTab);
//		RS232_Envoie_Car(';');
		pTab=pTab+1;
		i=i+1;
	}
	return(temp);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Fonction pour affichage des octets (en hexa) d'un tableau
void RS232TabHexa(char *pTab, char size)
{
	char	i,temp;
	for(i=0;i<size;i++)
	{
		temp=i%4;
		if (temp==0)
		{
			RS232_Envoie_Car(LF);
		}
		RS232OctetHexa(*pTab);
		RS232_Envoie_Car(' ');
		pTab=pTab+1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// Fonction pour affichage d'un tableau d'octets
void RS232TabDec(const char *pTab, char size)
{
	char	i;
	for (i=0;i<size;++i)
	{
		RS232OctetDec(*pTab);
		RS232_Envoie_Car(';');
		pTab=pTab+1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un octet en décimal
void RS232OctetDec(char data)
{
	char cent, diz, unit;
	cent=data/100;
	diz=data%100;
	unit=diz%10;
	diz=diz/10;
	RS232_Envoie_Car(48+cent);
	RS232_Envoie_Car(48+diz);
	RS232_Envoie_Car(48+unit);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un octet en binaire
void RS232OctetBin(char data)
{
	char	i;
	for (i=0; i<8; ++i)					// Envoie des 8 bits à la suite (poids fort en 1er)
	{
		RS232_Envoie_Car(48+(data&0x80));
		data=data<<1;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un octet en hexadécimal
void RS232OctetHexa(char data)
{
	char	i, temp;
	temp=data;							// Pour 1er symbole hexa
	temp=temp&0b11110000;				// Poids forts
	for (i=0; i<4; ++i)					// 4 décalage à droite
	{
		temp=temp>>1;
	}
	temp=temp&0b00001111;
	if (temp<10)
		RS232_Envoie_Car(48+temp);
	else
	{
		temp=temp-9;
		RS232_Envoie_Car(64+temp);
	}
	temp=data;							// Pour 2eme symbole hexa
	temp=temp&0b00001111;				// Poids faibles
	if (temp<10)
		RS232_Envoie_Car(48+temp);
	else
	{
		temp=temp-9;
		RS232_Envoie_Car(64+temp);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un mot en décimal
void RS232MotDec(unsigned long data)
{
	char dixmille, mille, cent, diz, unit;
	unsigned long	temp;
	dixmille=data/10000;
	temp=data%10000;
	mille=temp/1000;
	temp=temp%1000;
	cent=temp/100;
	temp=temp%100;
	diz=temp/10;
	unit=temp%10;
	if (dixmille!=0)
		RS232_Envoie_Car(48+dixmille);
	if ((dixmille!=0) || ((dixmille==0) && (mille!=0)))
		RS232_Envoie_Car(48+mille);
	RS232_Envoie_Car(48+cent);
	RS232_Envoie_Car(48+diz);
	RS232_Envoie_Car(48+unit);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un mot en binaire
void RS232MotBin(unsigned long data)
{
	char	i;
	for (i=0; i<16; ++i)					// Envoie des 8 bits à la suite (poids fort en 1er)
	{
		RS232_Envoie_Car(48+(data&0x8000));
		data=data<<1;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un mot en hexadécimal
void RS232MotHexa(unsigned long data)
{
	char	temp1,i,j,k;
	unsigned long	temp2;
	for (i=4; i>0; --i)				// 12 décalages à droite
	{
		temp2=data;					// Poids forts
		j=4*(i-1);
		for (k=0; k<j; ++k)
		{
			temp2=temp2>>1;
		}
		temp1=temp2&0x0F;
		if (temp1<10)
		RS232_Envoie_Car(48+temp1);
		else
		{
			temp1=temp1-9;
			RS232_Envoie_Car(64+temp1);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un flottant en décimal
//		nécessite MATH24F.H
/*
void RS232FloatDec(float24 data)
{
	float24		temp;
	unsigned long		val1,val2;
	if (data>0)
		RS232_Envoie_Car('+');
	else
	{	data=-data;
		RS232_Envoie_Car('-');
	}
	val1=data;						// Partie entière de data MAIS avec arrondi!!
	temp=(float24)val1;				// Partie entiere dans un float24
	data=_fsub24(data,temp);		// Soustraction nb réel - partie entiere
	if (data<0)						// Résultat négatif: on compense l'arrondi
	{	data=1+data;
		val1=val1-1;
	}
	val2=data*1000;					// Partie décimale en 1000eme
	RS232MotDec(val1);
	RS232_Envoie_Car('.');
	RS232MotDec(val2);
}
*/

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// Commande AT
////////////////////////////////////////////////////////////////////////////////////////////
// Fonction d'envoie Commande AT
void EnvoieAT(const char *ptext)
{
	RCSTA2bits.CREN=0;						// Réception désactivée
	TXSTA2bits.TXEN=1;						// Emission activée
	do							// Envoie CommandeAT
	{
		TXREG2=*ptext;
		TXSTA2bits.TRMT=0;
		while(TXSTA2bits.TRMT==0);
		ptext=ptext+1;
	}while (*ptext!=0);
	TXREG2=CR;					// Envoie CR
	TXSTA2bits.TRMT=0;
	while(TXSTA2bits.TRMT==0);
	TXSTA2bits.TXEN=0;						// Emission désactivée
	RCSTA2bits.CREN=1;						// Réception activée
}


// Fin fichier
