
///////////////////////////////////////////////////////////////////////////////////////
// Prototypes des fonctions pour gestion LCD 8 bits série
void AffichTemp(unsigned long curs, char temp);
void MessageAccueil(void);
void Affich_V_Vent_kmh(unsigned long curs, char *tab);
void Affich_V_Vent_ms(unsigned long curs, char *tab);
void Affich_D_Vent(unsigned long curs, char *tab);
char DirVent(char *tab);
unsigned long VitVentkmh(char *tab);
unsigned long VitVentms(char *tab);


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// Utilisation Afficheur LCD8bits série  //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour initialiser LCD
void AffichTemp(unsigned long curs, char temp)
{
    char temp1;
    if(curs==1)
    {
        LCD_CLEAR;
        CURS_DEB;
        afftext("Temperature :");

    }
    if ((temp!=temp1)||(curs==1))
    {
        LIGNE2;  
        affmotdec(temp,0);
        valcaract(0xDF); // caractere °
    }
    temp1=temp;
}

void Affich_V_Vent_kmh(unsigned long curs, char *tab)
{
    unsigned long vent, vent1;
    if(curs==1)
    {
        LCD_CLEAR;
        afftext("Vitesse du vent :");
    }
    vent = VitVentkmh(tab); //On prend la valeur de la vitesse
    if((vent!=vent1)||(curs==1))
    {
        LIGNE2; afftext("                                       "); //On efface la ligne2
        LIGNE2;
        affmotdec(vent,2);
        afftext(" Km/H");

    }
    vent1=vent;
}
void Affich_V_Vent_ms(unsigned long curs, char *tab)
{
    unsigned long vent, vent1;
    if(curs==1)
    {
        LCD_CLEAR;
        afftext("Vitesse du vent :");
    }
    vent = VitVentms(tab); //On prend la valeur de la vitesse
    if((vent!=vent1)||(curs==1))
    {
        LIGNE2; afftext("                                       "); //On efface la ligne2
        LIGNE2;
        affmotdec(vent,1);
        afftext(" m/s");

    }
    vent1=vent;
}


void Affich_D_Vent(unsigned long curs, char *tab)
{
    char vent, vent1;
    if(curs==1)
    {
        LCD_CLEAR;
        afftext("Direction vent: ");
    }
    vent = DirVent(tab); //On prend la valeur de la vitesse
    if((vent!=vent1)||(curs==1))
    {
        LIGNE2; afftext("                                       "); //On efface la ligne2
        LIGNE2;
        switch (vent)
        {
                  case 0 : afftext("Nord");
                            break;
                  case 1 : afftext("Nord/Nord-Est");
                            break;
                  case 2 : afftext("Nord-Est");
                            break;
                  case 3 : afftext("Est/Nord-Est");
                            break;
                  case 4 : afftext("Est");
                            break;
                  case 5 : afftext("Est/Sud-Est");
                            break;
                  case 6 : afftext("Sud-Est");
                            break;
                  case 7 : afftext("Sud/Sud-Est");
                            break;
                  case 8 : afftext("Sud");
                            break;
                  case 9 : afftext("Sud/Sud-Ouest");
                            break;
                  case 10 : afftext("Sud-Ouest");
                            break;
                  case 11 : afftext("Ouest/Sud-Ouest");
                            break;
                  case 12 : afftext("Ouest");
                            break;
                  case 13 : afftext("Ouest/Nord-Ouest");
                            break;
                  case 14 : afftext("Nord-Ouest");
                            break;
                  case 15 : afftext("Nord/Nord-Ouest");
                            break;
                  default : afftext("Error");
        }
    }
    vent1=vent;



    
}

void MessageAccueil(void)
{
    afftext("Bonjour,");
    LIGNE2;
    afftext("Welcome");
    tempo_Xms(2000);
    CURS_DEB;
    afftext("VINYU Meteo a");
    LIGNE2;
    afftext("votre service !! :)");
    CURS_DEB;
}


