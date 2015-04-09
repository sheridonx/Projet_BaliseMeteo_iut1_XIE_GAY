////////// Commande du capteur de température ////////////////

#define Read_Temperature 0xAA
#define Acces_TH 0xA1
#define Acces_TL 0xA2
#define Acces_Config 0xAC
#define Read_Counter 0xA8
#define Read_Slope 0xA9
#define Start_Convert_T 0xEE
#define Stop_Convert_T 0x22

#define ADD_W_DS1621 0x98
#define ADD_R_DS1621 0x99

////////// Commande de l'afficheur ////////////////

#define ADD_W_GDM1602A 0x7C
#define ADD_R_GDM1602A 0x7D

#define Aff_Clear 0x01
#define Aff_Return 0x02
#define Aff_A 0x71