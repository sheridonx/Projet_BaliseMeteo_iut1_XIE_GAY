/////Fonctions ////////////
void DS1621_init(char data, char TH, char TL);
void DS1621_ecrire(char adresse, char data);
void DS1621_conv(void);
char DS1621_lire2octets(char adresse);
char DS1621_lire1octets(char adresse);


void DS1621_init(char data, char TH, char TL)
{
	//Config
        i2c_start();				// Start I2C
	i2c_ecrire(ADD_W_DS1621);		// Envoie de l'adresse DS1621 pour écriture
	i2c_ecrire(Acces_Config);		// Envoie de l'adresse du registre à écrire
	i2c_ecrire(data);			// Envoie de la donnée
        //TH
        i2c_restart();
        i2c_ecrire(ADD_W_DS1621);		// Envoie de l'adresse DS1621 pour écriture
	i2c_ecrire(Acces_TH);		// Envoie de l'adresse du registre à écrire
	i2c_ecrire(TH);			// Envoie de la donnée
        //TL
        i2c_restart();
        i2c_ecrire(ADD_W_DS1621);		// Envoie de l'adresse DS1621 pour écriture
	i2c_ecrire(Acces_TL);		// Envoie de l'adresse du registre à écrire
	i2c_ecrire(TL);			// Envoie de la donnée

        i2c_stop();

        DS1621_conv();              //Lancement conversion 

}

void DS1621_ecrire(char adresse, char data)
{
	i2c_start();				// Start I2C
	i2c_ecrire(ADD_W_DS1621);		// Envoie de l'adresse DS1621 pour écriture
	i2c_ecrire(adresse);		// Envoie de l'adresse du registre à écrire
	i2c_ecrire(data);			// Envoie de la donnée
	i2c_stop();					// Stop I2C
}

//////////////////////////////////////////////////////////////////////////////////////
//Fonction pour lancer la conversion de température du DS1621
void DS1621_conv(void)
{
	i2c_start();				// Start I2C
	i2c_ecrire(ADD_W_DS1621);		// Envoie de l'adresse RTC pour écriture
	i2c_ecrire(Start_Convert_T);	// Envoie ordre de conversion de la température
	i2c_stop();					// Stop I2C
}
// Fonctions lecture pour DS1621
char DS1621_lire2octets(char adresse)
{
	char tempH,tempL;
	i2c_start();				// Start I2C
	i2c_ecrire(ADD_W_DS1621);		// Envoie de l'adresse RTC pour écriture
	i2c_ecrire(adresse);		// Envoie de l'adresse du registre à lire
	i2c_restart();			// Repeat Start I2C
	i2c_ecrire(ADD_R_DS1621);		// Envoie de l'adresse RTC pour lecture
             // Lecture de l'octet haut
        tempH=i2c_lire(0);
            // Lecture de l'octet bas, 1 pour envoyer un acknoledge (donc fin de la trame)
        tempL=i2c_lire(1);
        i2c_stop();
        return(tempH);

}

char DS1621_lire1octets(char adresse)
{
	char	data;
	i2c_start();				// Start I2C
	i2c_ecrire(ADD_W_DS1621);		// Envoie de l'adresse RTC pour écriture
	i2c_ecrire(adresse);		// Envoie de l'adresse du registre à lire
	i2c_restart();			// Repeat Start I2C
	i2c_ecrire(ADD_R_DS1621);		// Envoie de l'adresse RTC pour lecture
	data=i2c_lire(1);			// Lecture du registre souhaité + NOack (fin de communication)
	i2c_stop();					// Stop I2C
	return(data);
}