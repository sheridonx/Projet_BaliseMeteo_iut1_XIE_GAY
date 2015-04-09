/* 
 * File:   TX20.h
 * Author: vince
 *
 * Created on 27 février 2015, 11:12
 */
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// Déclaration des E/S pour communication I2C (Entrées au départ)
#define Vent	    	PORTBbits.RB5
#define REG		PORTBbits.RB4		 //à l'état bas si on veux lire vent
#define inv 1
#define ninv 0
// A configurer en entrées à l'initialisation

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// Prototypes des fonctions pour gestion I2C
void DetectVent(char *tab);
char traitementVent(char x,char y,char *tab, char Inv);
void tempo_1ms200us(void);
void tempo_200us(void);
void tempo_Xms(long int time);
char DirVent(char *tab);
unsigned long VitVentkmh(char *tab);
unsigned long VitVentms(char *tab);

//// Variable Globale /////
char Start, Dir,V1,V2,V3,V11,V22,V33,Chk,Dir,Dirverif;
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// Utilisation Module interne pour Communication I2C  //////////////////////////////////////////////////
// Utilisation Horloge interne (4 MHz) ATTENTION si différente: Actualiser SSPADD/////////
///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void DetectVent(char *tab)
{
    int i;

    REG=0; //on active la reception de donnée du vent
    while(Vent == 0);
    tempo_200us();
    tempo_200us();
    tempo_200us(); // on attend 600 us (une demi periode de bits car on va lire au milieu des bits)

    for(i=0;i<42;i++)
	{
		//Test=1;
		tab[i]=Vent; //on attribu dans le tableau
		//Test=0;
		tempo_1ms200us(); //on attend le milieu du prochain bit
	}
    REG=1; //On arrete la capture du vent
    
        //Chk=traitementVent(20,24,tab,inv);

}

char traitementVent(char x,char y,char *tab, char Inv)
{
    unsigned long i;
	char data;
	for(i=y;i>x;i--)   //Comme on recois le poid faible en premier on remet dans lordre
	{
                data=data<<1;   //Decalage a gauche
		data=data|tab[i];
	}

        if(Inv==1)
            data=~data;

	data=data&0x0F; // On s'assure d'avoir la valeur sur 4bits
	return(data);
}

char DirVent(char *tab)
{
      Dir=traitementVent(4,8,tab,inv);
      Dirverif=traitementVent(24,28,tab,ninv);
      if(Dir==Dirverif)
      return(Dir);
      else
      return(200);
}
unsigned long VitVentkmh(char *tab)
{
    unsigned long Vit;
        V1=traitementVent(8,12,tab,inv); //4bits
        V2=traitementVent(12,16,tab,inv);
        V3=traitementVent(16,20,tab,inv);
        V11=traitementVent(28,32,tab,ninv);
        V22=traitementVent(32,36,tab,ninv);
        V33=traitementVent(36,40,tab,ninv);
        if((V1==V11)&(V2==V22)&(V3==V33)) // on vérifie si nous avons bien les bonnes valeurs
        {  Vit=(V3<<8)+(V2<<4)+(V1);
            Vit=(Vit)*36;
        }
        else
            Vit=666;
      return(Vit);
}
unsigned long VitVentms(char *tab)
{
    unsigned long Vit;
        V1=traitementVent(8,12,tab,inv); //4bits
        V2=traitementVent(12,16,tab,inv);
        V3=traitementVent(16,20,tab,inv);
        V11=traitementVent(28,32,tab,ninv);
        V22=traitementVent(32,36,tab,ninv);
        V33=traitementVent(36,40,tab,ninv);
        if((V1==V11)&(V2==V22)&(V3==V33)) // on vérifie si nous avons bien les bonnes valeurs
          Vit=(V3<<8)+(V2<<4)+(V1);
        else
            Vit=666;
      return(Vit);
}