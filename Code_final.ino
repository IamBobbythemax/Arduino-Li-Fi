int A0ValueNormal;
int A0Value;
//int A0moy;

int delai = 1;

void setup() {
  Serial.begin(9600);
  pinMode(12,OUTPUT);
  delay(500);//delais pour éviter des erreurs dû au système pour la prise de A0ValueNormal

  A0ValueNormal = analogRead(A0);//Luminosité ambiante
  A0Value = analogRead(A0);
  //A0moy = (A0Value - A0ValueNormal)/2 + 1;
  Serial.println(A0ValueNormal);

}

boolean eetdr = 0; //eetdr(est en train de recevoir, si non = 0, si oui = 1.
boolean eetde = 0; //eetde(est en train d'envoié, si non = 0, si oui = 1.

int k = 0;
void loop() {
  A0Value = analogRead(A0);
  delay(delai*2);
    if(A0Value > A0ValueNormal + 3 && eetde == 0){//Cette condition vérifie si un message est entrain d'être recu et que aucun message est entrain d'être envoié.
      k++;
      if (k>5){
        eetdr = 1;
        c_recu();   
      }
    } else {
      k = 0;
    }
    if (Serial.available() > 0 && eetdr == 0){ //Cette condition vérifie si un message est envoié et que aucun message est en train d'être recu.
     
      
      char cr = (char)Serial.read(); //Lit le message entré dans la console charachtère par charchtère et l'attribut à la variable cr;
      
      Serial.print(cr);
      Serial.print(" :");

      eetde = 1;
      c_envoi(cr); //appele de la fonction envoi
      
  }


  
}

void c_recu() {
  
  delay(delai*3);
  
  char octetRecu;
  
  for(int j = 0; j<8; j++){
       delay(delai*2);
       byte bitRecu;
       
       A0Value = analogRead(A0);

       
       if (A0Value > A0ValueNormal + 5){
         bitRecu = 1;
          
       } else {
          bitRecu = 0;
          
       } 
      octetRecu = (octetRecu << 1) | bitRecu;

      //Serial.print(bitRecu);

  }
   Serial.println(octetRecu); //Affiche le message envoyé 
   eetde = 0;
}

void c_envoi(char cr){
    
    
     char c = (cr);

     String octet = String(c, BIN);//Binéarisation du charachtère 
     
     digitalWrite(12,"HIGH");// signal lumineux pour annoncer le début de l'envoi d'un charachtère
     delay(delai*7);
     digitalWrite(12,"LOW");
     delay(delai*3);
     

     digitalWrite(12,LOW); //Les charachtères s'écrivent sur 7bits mon système est sur 8bits j'ajoute donc un 0 qui ne change pas le charachtère.
     delay(delai*2);//délai entre envoi de chaque bit
     Serial.print(0);

     if( c == ' '){//Le charachtère "espace" s'écrit 00100001 en 8bits,il a 2 bits de poids faible(0). j'en ajoute donc un de +
      Serial.print("0");
      digitalWrite(12,LOW);
      delay(delai*2);
      
     }
     
     for(int j = 0; j<8; j++){

       if (octet[j] == '1'){
        digitalWrite(12, HIGH);
        Serial.print(octet[j]);
       } else {
        digitalWrite(12, LOW);
        Serial.print(octet[j]);
       }      
      
      delay(delai*2);   
     }

  Serial.println();
  eetdr = 0;
}
