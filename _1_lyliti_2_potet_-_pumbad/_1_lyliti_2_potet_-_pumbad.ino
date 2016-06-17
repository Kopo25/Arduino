#include <LiquidCrystal.h>

const int potPin1 = 0;
const int potPin2 = 1;
int data1[3]; /*masiiv kolme positsiooniga, mahutab kolme lüliti aja väärtust. Kuue positsiooniga masiiv mahutaks aja ja koguse.
              int data[] = {(ajaline väärtus 1), (ajaline väärtus 2), (ajaline väärtus 3), (koguseline väärtus 1), (koguseline väärtus 2), (koguseline väärtus 3)};*/
float data2[3]; /*masiiv kolme positsiooniga, mahutab kolme lüliti aja väärtust. Kuue positsiooniga masiiv mahutaks aja ja koguse.
              int data[] = {(ajaline väärtus 1), (ajaline väärtus 2), (ajaline väärtus 3), (koguseline väärtus 1), (koguseline väärtus 2), (koguseline väärtus 3)};*/
const int lyl1 = 7;

int PUMP1 = 13;  // Pumba1 ja LED asub jalas 13
int PUMP2 = 10;
int PUMP3 = 6;

int buttonState = 0;

int aeg1=0; //kounterite ajad
int aeg2=0;
int aeg3=0;

int potVal1;
int potVal2;

int mode = 0;

// int z1;     // counter allalaadimise riba ajaks

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);   // initialize the library with the numbers of the interface pins


void setup() {
  Serial.begin(9600);  // et saaks kasutada serial monitori, def com pordi kiiruse
  
  lcd.begin(16, 2);  //ekraani suurus
  
  pinMode(lyl1, INPUT);  //lülitite defineerimine
  
  pinMode(PUMP1, OUTPUT);  // algväärtustame Pump1 väljundiks
  pinMode(PUMP2, OUTPUT);  // algväärtustame Pump2 väljundiks
  pinMode(PUMP3, OUTPUT);  // algväärtustame Pump3 väljundiks
}

void loop() {
  
  delay (1000);
  
  buttonState = digitalRead(lyl1);
 
    /****************SAGEDUSE MÄÄRAMINE**************/ 
  potVal1 = analogRead(potPin1);    
                                                        //Pote väärtus saab olla 0-1023. 1023/7= 146,14...
                float f1 = potVal1/146.14;               //floatiga tuleb komakohaga arv.
                int i1 = f1;                             //kui int-il määrata foati väärtus siis ta kaotab komakoha ära(ei ümarda vaid kaotab lihtsalt ära). 
      
  /****************PIKKUSE MÄÄRAMINE**************/
  potVal2 = analogRead(potPin2);
            float s1 = potVal2/292.28;             //Pote väärtus saab olla 0-1023 jagades 292.28 saab s1 max väärtus olla 3,5 
  
  if (buttonState == HIGH) {
                mode++;
            delay (1000);
         } 
  if(mode == 1){
         lcd.setCursor(0, 0);    // Aktiivsuse märkimine
         lcd.print(">");
         lcd.setCursor(5, 0);
         lcd.print(" ");
         lcd.setCursor(10, 0);
         lcd.print(" ");
       data1[0] = i1;                                    // pote väärtust loetakse data1 esimesse (0) kohta
       data2[0] = s1;
               }
  
  if(mode == 2){
         lcd.setCursor(5, 0);
         lcd.print(">");
         lcd.setCursor(0, 0);
         lcd.print(" ");
         lcd.setCursor(10, 0);
         lcd.print(" ");     
        data1[1] = i1; 
        data2[1] = s1;
               }
  
  if(mode == 3){
         lcd.setCursor(10, 0);
         lcd.print(">");   
         lcd.setCursor(5, 0);
         lcd.print(" ");
         lcd.setCursor(0, 0);
         lcd.print(" ");
       data1[2] = i1; 
       data2[2] = s1;
               }
  
  if(mode > 3){
    mode=0;
         lcd.setCursor(10, 0);
         lcd.print(" ");   
         lcd.setCursor(5, 0);
         lcd.print(" ");
         lcd.setCursor(0, 0);
         lcd.print(" ");
         }
  
  Serial.println("mode ");
  Serial.println(mode);
  Serial.print("buttonState ");
  Serial.println(buttonState);

  
     /**************Ekraanile andmed*****************/  
      
      int size1 = sizeof(data1);
  for (int indx = 0; indx < size1 ; indx++) {
    lcd.setCursor(1, 0);
    lcd.print(data1[0]);
    lcd.print("p");
    lcd.setCursor(6, 0);
    lcd.print(data1[1]);
    lcd.print("p");
    lcd.setCursor(11, 0);
    lcd.print(data1[2]);
    lcd.print("p");
  }
  
  /****Alumine rida***/
    int size2 = sizeof(data2);
  for (int indx = 0; indx < size2 ; indx++) {
    lcd.setCursor(0, 16);
    lcd.print(data2[0]);
    lcd.setCursor(5, 16);
    lcd.print(data2[1]);
    lcd.setCursor(10, 16);
    lcd.print(data2[2]);
  }
   
  
  //Pump #1 sagedus - Kui õige aeg, lülitab Pumba sisse xxx sek
  int time1 = (data1[0] * 10);
  int time2 = ((data2[0] / 0.5)*10);

  Serial.print("time1 ");
  Serial.println(time1);
  Serial.print("aeg1 "); 
  Serial.println(aeg1);
  Serial.print("time2 ");
  Serial.println(time2);

 
  if (time1 == 0) {                 //kui päev määrata 0 (time1 = 0), siis ei hakkaks pump tööle, määratakse aeg1 kõrgem väärtus kui time1
      aeg1 = 1;
     }

  if (aeg1 == time1) {               //Kui sekundite (tsüklite) arv on sama, mis pote1 määratud, käivitub
  
  /*   Alumise täiteriba kuvamine
  { z1=0; }
  if (z1 == i1) {
    lcd.setCursor(0, 16);             //Millisest kohast hakkab näitama
    
    int var = 0;                  
  while(var < 16)                     //Trükitakse täituvat riba 16 korda sümbolit >
     { 
        lcd.print (">");
         delay (data2[0]*62);         // 1000/16 = 62, ehk kõik 16 sümbolit tuleks displayle Pikkuse ajaga
         var++;
        }
         z1 = 0;
        
 
  lcd.setCursor(0, 16); 
  lcd.print("                ");   //kustutatakse täitunud riba
  }
  */
        
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  Pump #1");
         digitalWrite(PUMP1, HIGH);  //Pump1 hakkab tööle
       delay (time2*100);            // Kui pump pumpab 0,5L/s siis tehakse vastavalt nii pikk viivis, hetkel mitu L *2 sek
     digitalWrite(PUMP1, LOW);       //Pump1 lõpetab töö
        lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(data1[0]);
    lcd.print("p");
    lcd.setCursor(6, 0);
    lcd.print(data1[1]);
    lcd.print("p");
    lcd.setCursor(11, 0);
    lcd.print(data1[2]);
    lcd.print("p");
    lcd.setCursor(0, 16);
    lcd.print(data2[0]);
    lcd.setCursor(5, 16);
    lcd.print(data2[1]);
    lcd.setCursor(10, 16);
    lcd.print(data2[2]);
    
     aeg1 = 0;                  // pump #1 kounter nullitakse
  }
  
    //Pump2 sagedus - Kui õige aeg, lülitab Pumba sisse xxx sek
  int time3 = (data1[1] * 10);
  int time4 = ((data2[1] / 0.5)*10);
 
  Serial.print("time3 ");
  Serial.println(time3);
  Serial.print("aeg2  ");
  Serial.println(aeg2);

 if (time3 == 0) {                 //kui päev määrata 0 (time1 = 0), siis ei hakkaks pump tööle, määratakse aeg1 kõrgem väärtus kui time1
      aeg2 = 1;
      }

    if (aeg2 == time3) {               //Kui sekundite (tsüklite) arv on sama, mis pote1 määratud, käivitub 
          lcd.clear();                // Tühjendame ekraani
      lcd.setCursor(0, 0);
      lcd.print("  Pump #2");
         digitalWrite(PUMP2, HIGH);  //Pump2 hakkab tööle
       delay (time4*100); 			 // Kui pump pumpab 0,5L/s siis tehakse vastavalt nii pikk viivis
     digitalWrite(PUMP2, LOW);       //Pump2 lõpetab töö
              
      lcd.clear();                    //Algne txt tuuakse tagasi
    lcd.setCursor(1, 0);
    lcd.print(data1[0]);
    lcd.print("p");
    lcd.setCursor(6, 0);
    lcd.print(data1[1]);
    lcd.print("p");
    lcd.setCursor(11, 0);
    lcd.print(data1[2]);
    lcd.print("p");
    lcd.setCursor(0, 16);
    lcd.print(data2[0]);
    lcd.setCursor(5, 16);
    lcd.print(data2[1]);
    lcd.setCursor(10, 16);
    lcd.print(data2[2]);
      
     aeg2 = 0;                         // pump #2 kounter nullitakse
  }
  
      //Pump3 sagedus - Kui õige aeg, lülitab Pumba sisse xxx sek
  int time5 = (data1[2] * 10);
  int time6 = ((data2[2] / 0.5)*10);
  
  Serial.print("time5 ");
  Serial.println(time5);
  
   if (time5 == 0) {                 //kui päev määrata 0 (time1 = 0), siis ei hakkaks pump tööle, määratakse aeg1 kõrgem väärtus kui time1
      aeg3 = 1;
      }
  
  if (aeg3 == time5) {               //Kui sekundite (tsüklite) arv on sama, mis pote1 määratud, käivitub 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  Pump #3");
      
     digitalWrite(PUMP3, HIGH);         //Pump3 hakkab tööle
       delay (time6*100);   		// Kui pump pumpab 0,5L/s siis tehakse vastavalt nii pikk viivis
     digitalWrite(PUMP3, LOW);         //Pump3 lõpetab töö
         
    lcd.clear();                      //Algne txt toob tagasi eelńevad ekraanid
    lcd.setCursor(1, 0);
    lcd.print(data1[0]);
    lcd.print("p");
    lcd.setCursor(6, 0);
    lcd.print(data1[1]);
    lcd.print("p");
    lcd.setCursor(11, 0);
    lcd.print(data1[2]);
    lcd.print("p");
    lcd.setCursor(0, 16);
    lcd.print(data2[0]);
    lcd.setCursor(5, 16);
    lcd.print(data2[1]);
    lcd.setCursor(10, 16);
    lcd.print(data2[2]); 
    
    aeg3 = 0;                         // pump #3 kounter nullitakse
  }

  lcd.setCursor(14, 0); 
  lcd.print(time1);

   //kounterid
  aeg1 == aeg1++;
  aeg2 == aeg2++;
  aeg3 == aeg3++;
   //   z1++;

}
