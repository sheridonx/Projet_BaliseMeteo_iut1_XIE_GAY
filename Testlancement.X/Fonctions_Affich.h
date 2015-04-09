/* 
 * File:   Affich.h
 * Author: vince
 */
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
#define		E		LATC0			// bit E
#define		RW		LATC1 			// bit RS
#define		RS		LATC2 			// bit RS

////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// Commandes pour afficheur LCD (Voir datasheet LCD)
#define		LCD_CLEAR		valinstr(0x01);tempo_Xms(2)    	// Efface Module LCD
#define		CURS_DEB		valinstr(0x02)   	// Curseur au début de ligne
#define		LIGNE2			valinstr(0xC0)		// Curseur début ligne2
#define		LCD_CONF		valinstr(0x06)    	// Configuration LCD
#define		AFF_OFF			0b00001000    	// Affichage OFF
#define		AFF_ON			0b00001100    	// Affichage ON et curseur invisible
#define		CURS_ON			0b00001110 	  	// Curseur Visible
#define		CURS_CLIG		valinstr(0x0F)		// Curseur Clignotant
#define		CURS_DEC		0b00011100		// Curseur et affichage décalé de 1 caractère à droite.
#define		LCD4BITS		0b00101100		// Mode 4 bits, 2 lignes, 10*5 points.
#define		LCD8BITS		valinstr(0x38);tempo_Xms(5)		// Mode 8 bits, 2 lignes, 10*5 points.
#define		LCD_DDRAM		0b10000000		// Adresse DDRAM
#define		LCD_CGRAM		0b01000000		// Adresse CGRAM
////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// Prototypes des fonctions pour gestion LCD 8 bits série
void InitLCD(void);
void valinstr(char data);
void valcaract(char data);
//void serie(char data);
void afftab(const char *ptext, char size);
void afftext(const char *ptext);
void affoctetdec(char data);
void affoctetbin(char data);
void affoctethexa(char data);
void affmotdec(unsigned long data, char coma);
void affmotdecsign(long data);
void affmotbin(unsigned long data);
void affmothexa(unsigned long data);
void creationcar(char adresse);
void effacecar(char adresse);

/////////tempo//////////
void tempo_Xms(long int time);
void tempo_1ms(void);
void tempo_200us(void);
void MessageAccueil(void);



////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// Utilisation Afficheur LCD8bits série  //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour initialiser LCD
void InitLCD()
{

	LCD8BITS;
        LCD8BITS;
        LCD8BITS;
        LCD8BITS;
        CURS_CLIG;
        LCD_CONF;
        LCD_CLEAR;
        MessageAccueil();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour valider instruction
void valinstr(char data)
{
	E=0;
	RS=0;
        NOP(); NOP(); NOP(); NOP();
	i2c_1trame(ADD_W_GDM1602A,data);
        NOP(); NOP(); NOP(); NOP();
	E=1;
	tempo_1ms();
	E=0;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour valider caractère
void valcaract(char data)
{
	E=1;
	RS=1;
	NOP(); NOP(); NOP(); NOP();
	i2c_1trame(ADD_W_GDM1602A,data);
	NOP(); NOP(); NOP(); NOP();
	E=1;
	tempo_1ms();
	E=0;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour mise en série
/*void serie(char data)
{
	char	i;
	CLK=0;
	for (i=0; i<8; i++)
	{
		D=data.7;						// Envoie bit poids fort
		tempo_Xms(1);						// Tempo
		CLK=1;
		data=rl(data);					// Décalage à gauche
		tempo_Xms(1);						// Tempo
		CLK=0;
	}
}
*/
///////////////////////////////////////////////////////////////////////////////////////////
// Utilisation Afficheur LCD  Fonctions communes //////////////////////////////////////////////////
// Utilisation tempo_Xms (timer0, prescaler=8) //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// Fonction pour affichage d'un tableau
void afftab(const char *ptext, char size)
{
	char	i;
	for (i=0; i<size; ++i)					// Envoie des caractères à afficher
	{	if (*ptext<=9)
			valcaract(48+*ptext);
		else
			valcaract(*ptext);
		ptext=ptext+1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// Fonction pour affichage d'un texte
void afftext(const char *ptext)
{
	//char	i;
	//i=0;
	do
	{
		valcaract(*ptext);
		ptext=ptext+1;
	//	i=i+1;
	}while (*ptext!=0);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un octet en décimal
void affoctetdec(char data)
{
	char cent, diz, unit;
	cent=data/100;
	diz=data%100;
	unit=diz%10;
	diz=diz/10;
	if(cent!=0)
	{valcaract(48+cent);}
	if((diz!=0)||(cent!=0))
	{valcaract(48+diz);}
	valcaract(48+unit);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un octet en binaire
void affoctetbin(char data)
{
	char	i;
	for (i=0; i<8; ++i)					// Envoie des 8 bits à la suite (poids fort en 1er)
	{
            if((data&0x80)>0)
                valcaract(49);
            else
                valcaract(48);
		data=data<<1;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un octet en hexadécimal
void affoctethexa(char data)
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
		valcaract(48+temp);
	else
	{
		temp=temp-9;
		valcaract(64+temp);
	}
	temp=data;							// Pour 2eme symbole hexa
	temp=temp&0b00001111;				// Poids faibles
	if (temp<10)
		valcaract(48+temp);
	else
	{
		temp=temp-9;
		valcaract(64+temp);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un mot en décimal
void affmotdec(unsigned long data, char coma)
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
		valcaract(48+dixmille);
	if ((mille!=0) || (dixmille!=0))
		valcaract(48+mille);
   if ((coma==3)&&(data!=0))
   valcaract(',');
        if ((cent!=0) || (mille!=0) || (dixmille!=0))
                valcaract(48+cent);
   if ((coma==2)&&(data!=0))
   valcaract(',');
        if ((diz!=0) || (cent!=0) || (mille!=0) || (dixmille!=0))
                valcaract(48+diz);
   if ((coma==1)&&(data!=0))
       valcaract(',');
        valcaract(48+unit);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un mot SIGNE en décimal
void affmotdecsign(long data)
{
	char dixmille, mille, cent, diz, unit;
	unsigned long	temp1,temp2;
	if ((data&0x100)>127)				// Nb négatif
	{
		valcaract('-');
		data= ~data;
		data=data+1;
	}
	else
		valcaract('+');
	temp1=data;						// Tranformation de type
	dixmille=temp1/10000;
	temp2=temp1%10000;
	mille=temp2/1000;
	temp2=temp2%1000;
	cent=temp2/100;
	temp2=temp2%100;
	diz=temp2/10;
	unit=temp2%10;
	if (dixmille!=0)
		valcaract(48+dixmille);
	if ((dixmille!=0) || ((dixmille==0) && (mille!=0)))
		valcaract(48+mille);
	valcaract(48+cent);
	valcaract(48+diz);
	valcaract(48+unit);
}


///////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un mot en binaire
void affmotbin(unsigned long data)
{
	char	i;
	for (i=0; i<16; ++i)					// Envoie des 8 bits à la suite (poids fort en 1er)
	{
		valcaract(48+(data&0x8000));
		data=data<<1;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un mot en hexadécimal
void affmothexa(unsigned long data)
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
		valcaract(48+temp1);
		else
		{
			temp1=temp1-9;
			valcaract(64+temp1);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour affichage d'un nb flottant en décimal
//		nécessite MATH24F.H
/*
void afffloatdec(float24 data)
{
	float24		temp;
	uns16		val1,val2;
	if (data>0)
		valcaract('+');
	else
	{	data=-data;
		valcaract('-');
	}
	val1=data;						// Partie entière de data MAIS avec arrondi!!
	temp=(float24)val1;				// Partie entiere dans un float24
	data=_fsub24(data,temp);		// Soustraction nb réel - partie entiere
	if (data<0)						// Résultat négatif: on compense l'arrondi
	{	data=1+data;
		val1=val1-1;
	}
	val2=data*1000;					// Partie décimale en 1000eme
	affmotdec(val1);
	valcaract('.');
	affmotdec(val2);
}
/*
////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour effacer un caractère créé en CGRAM
void effacecar(char adresse)
{
	char	i;
	adresse=LCD_CGRAM+adresse*8;		// Adresse de début d'effacement
	for (i=0; i<8; ++i)
	{
		valinstr(adresse+i);
		valcaract(0b00000000);
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour création d'un caractère en CGRAM
void creationcar(char adresse)
{
	adresse=LCD_CGRAM+adresse*8;		// Adresse de début de stockage
	valinstr(adresse);					// Stockage caractère de 8 lignes dans CGRAM
	valcaract(0b00000100);
	valinstr(adresse+1);
	valcaract(0b00011010);
	valinstr(adresse+2);
	valcaract(0b00011001);
	valinstr(adresse+3);
	valcaract(0b00000001);
	valinstr(adresse+4);
	valcaract(0b00000001);
	valinstr(adresse+5);
	valcaract(0b00011001);
	valinstr(adresse+6);
	valcaract(0b00011010);
	valinstr(adresse+7);
	valcaract(0b00000100);
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

*/