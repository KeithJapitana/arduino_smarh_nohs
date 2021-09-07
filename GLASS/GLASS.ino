//----- Libs ---//

#include <MQ2.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//-----Declarations-----//
int Analog_Input = A0;
int lpg ;
int co ;
int smoke;
MQ2 mq2(Analog_Input);
int relay1 = 12;
int relay2 = 11;
int buzzer = 13;
int smokeA0 = A0;



//--Page Counter--//
int page_counter=1 ;       //To move beetwen pages
int subpage_counter=0;    //To move submenu 1 
int subpage2_counter=0;   //To move submenu 2 


//--Buttons--//
int up = 2;               //Up button
int down = 3;           //Down button
int sel = 4; 



//--Debounce--//
boolean current_up = LOW;          
boolean last_up=LOW;            
boolean current_sel = LOW;
boolean last_sel = LOW;
boolean last_down = LOW;
boolean current_down = LOW;

//--Gas level--//
int gas1_level = 20;
int lpg_level = 10;


// Costum Chars


//Custom return char
byte back[8] = {
  0b00011,
  0b00111,
  0b01111,
  0b11111,
  0b11111,
  0b01111,
  0b00111,
  0b00011
};

byte arrow[8] = {
  0b01000,
  0b00100,
  0b00010,
  0b11111,
  0b00010,
  0b00100,
  0b01000,
  0b00000
};

byte cell[8] = {
  0b00000,
  0b00110,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup(){
  lcd.begin(16,2);
  lcd.createChar(1, back);
  lcd.createChar(2, arrow);
  lcd.createChar(3, cell);
  lcd.setCursor(0,0);
  lcd.print("  Gas Leak &");
  lcd.setCursor(0,1);
  lcd.print("  Smoke Sensor");
  delay(3000);
  lcd.clear();

  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(buzzer,OUTPUT);

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2,HIGH);

  
}


   //---- De-bouncing function for all buttons----//
  boolean debounce(boolean last, int pin)
  {
  boolean current = digitalRead(pin);
  if (last != current)
  {
  delay(5);
  current = digitalRead(pin);
  }
  return current;
  }


  void loop(){
   




    //--End noel codes--//

    
   current_up = debounce(last_up, up);         //Debounce for Up button
   current_down = debounce(last_down, down);   //Debounce for Down button
   current_sel = debounce(last_sel, sel);


    //----Page counter function to move pages----//
if(subpage_counter==0 && subpage2_counter==0){ 
//Page Up
    if (last_up== LOW && current_up == HIGH){  //When up button is pressed
      lcd.clear();                     //When page is changed, lcd clear to print new page  
      if(page_counter <3){              //Page counter never higher than 3(total of pages)
      page_counter= page_counter +1;   //Page up
      
      }
      else{
      page_counter= 3;  
      }
  }
  
    last_up = current_up;

//Page Down
    if (last_down== LOW && current_down == HIGH){ //When down button is pressed
      lcd.clear();                     //When page is changed, lcd clear to print new page    
      if(page_counter >1){              //Page counter never lower than 1 (total of pages)
      page_counter= page_counter -1;   //Page down
      
      }
      else{
      page_counter= 1;  
      }
      }
    
      last_down = current_down;
      }
//------- Switch function to write and show what you want---// 
     switch (page_counter){
      case 1: {
  float* values= mq2.read(true); //set it false if you don't want to print the values in the Serial
  //lpg = values[0];
  lpg = mq2.readLPG();
  //co = values[1];
  co = mq2.readCO();
  //smoke = values[2];
  smoke = mq2.readSmoke();
  lcd.setCursor(0,0);
  lcd.print("LPG:");
  lcd.print(lpg);
  lcd.print(" CO:");
  lcd.print(co);
  lcd.setCursor(0,1);
  lcd.print("SMOKE:");
  lcd.print(smoke);
  lcd.setCursor(11,1);
  lcd.print("PPM");
  delay(100);
  
     
        
      }
      break;

      
      case 2:{ //--Level of Gas 1//
     lcd.setCursor(3,0);
     lcd.print("Gas Level Set");
     lcd.setCursor(10,1);
     lcd.write(byte(3));
     lcd.print("PPM");
     lcd.setCursor(6,1);
     if(gas1_level <100){    //To avoid "0" of number 10
     lcd.setCursor(6,1);
     lcd.print("0");
     }
     lcd.print(gas1_level);
     
     lcd.setCursor(15,1);
     lcd.write(byte(1));     //Return custom char
  // Sub counter 2 control
     if (last_sel== LOW && current_sel == HIGH){ //select button pressed
     if(subpage2_counter <2){                    // subpage counter never higher than 2(total of items)
     subpage2_counter ++;                         //subcounter to move beetwen submenu
     }
     else{                                       //If subpage higher than 2 (total of items) return to first item
      subpage2_counter=1;
     }
     }
     last_sel=current_sel;                      //Save last state of select button

     //First item control(subpage2_counter =1) led brightness
     if(subpage2_counter==1){
     lcd.setCursor(14,1);
     lcd.print(" ");                           //Delete last arrow position              
     lcd.setCursor(5,1);
     lcd.write(byte(2));
     
     //Control led_lum variable
     if (last_up== LOW && current_up == HIGH){  //Up bright +
     if(gas1_level < 500){                         //led_lum never higher than 10 (max value)
     gas1_level ++ ;
     }
     else{
     gas1_level = 0;                              //If led_lum higher than 10, return to 0
     }
     }
     last_up=current_up;
     
     
     if(last_down== LOW && current_down == HIGH){  //Down bright -
     if(gas1_level >0){                              //led_lum never lower than 0
     gas1_level --;                                   
     }
     else{
     gas1_level = 0;                                 
     }
     }
     last_down=current_down;
     
     }
 

//Second item control (subpage2_counter==2) back
     if(subpage2_counter==2){
     lcd.setCursor(5,1);
     lcd.print(" ");                           //Delete last arrow position                    
     lcd.setCursor(14,1);                      //Place the arrow
     lcd.write(byte(2));                      
     if (last_up== LOW && current_up == HIGH){ 
     subpage2_counter=0;                       //Exit submenu, up/down pages enabled
     lcd.setCursor(14,1);
     lcd.print(" ");
     }
     last_up=current_up;
     
     if(last_down== LOW && current_down == HIGH){
     subpage2_counter=1;                        //Stay on sub menu, return to led_lum control
     }
     last_down=current_down;
     }
     
    }
    break;
    case 3: {

      //--Level of LPG 1//
     lcd.setCursor(3,0);
     lcd.print("LPG Level Set");
     lcd.setCursor(10,1);
     lcd.write(byte(3));
     lcd.print("PPM");
     lcd.setCursor(6,1);
     if(lpg_level <10){    //To avoid "0" of number 10
     lcd.setCursor(6,1);
     lcd.print("0");
     }
     lcd.print(lpg_level);
     
     lcd.setCursor(15,1);
     lcd.write(byte(1));     //Return custom char
  // Sub counter 2 control
     if (last_sel== LOW && current_sel == HIGH){ //select button pressed
     if(subpage2_counter <2){                    // subpage counter never higher than 2(total of items)
     subpage2_counter ++;                         //subcounter to move beetwen submenu
     }
     else{                                       //If subpage higher than 2 (total of items) return to first item
      subpage2_counter=1;
     }
     }
     last_sel=current_sel;                      //Save last state of select button

     //First item control(subpage2_counter =1) led brightness
     if(subpage2_counter==1){
     lcd.setCursor(14,1);
     lcd.print(" ");                           //Delete last arrow position              
     lcd.setCursor(5,1);
     lcd.write(byte(2));
     
     //Control led_lum variable
     if (last_up== LOW && current_up == HIGH){  //Up bright +
     if(lpg_level < 500){                         //led_lum never higher than 10 (max value)
     lpg_level ++ ;
     }
     else{
     lpg_level = 0;                              //If led_lum higher than 10, return to 0
     }
     }
     last_up=current_up;
     
     
     if(last_down== LOW && current_down == HIGH){  //Down bright -
     if(lpg_level >0){                              //led_lum never lower than 0
     lpg_level --;                                   
     }
     else{
     lpg_level = 0;                                 
     }
     }
     last_down=current_down;
     
     }
 

//Second item control (subpage2_counter==2) back
     if(subpage2_counter==2){
     lcd.setCursor(5,1);
     lcd.print(" ");                           //Delete last arrow position                    
     lcd.setCursor(14,1);                      //Place the arrow
     lcd.write(byte(2));                      
     if (last_up== LOW && current_up == HIGH){ 
     subpage2_counter=0;                       //Exit submenu, up/down pages enabled
     lcd.setCursor(14,1);
     lcd.print(" ");
     }
     last_up=current_up;
     
     if(last_down== LOW && current_down == HIGH){
     subpage2_counter=1;                        //Stay on sub menu, return to led_lum control
     }
     last_down=current_down;
     }


      


      
      
    }
    break;

    
      
     } //-- end of Switch statement --//
    int analogSensor = analogRead(smokeA0);

     if ( smoke >= gas1_level)
     {
      //-- 1st cycle on
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,1000);
      delay(500);
      //--2nd cycle off
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      delay(500);
      //--  repeat cycle on --//
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,1000);
      delay(500);
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      delay(500);
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,1000);
      delay(500);
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      delay(500);
      //--   1st cycle --//
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,1000);
      delay(500);
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      delay(500);
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,1000);
      delay(500);
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,1000);
      delay(500);
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      delay(500);
      //--   1st cycle --//
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,1000);
      delay(500);
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      delay(500);
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,1000);
      delay(500);
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      delay(500);
      //--   1st cycle --//
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,1000);
      delay(500);
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      delay(500);
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,1000);
      delay(500);
      
     }
     if ( smoke < gas1_level)
     {
      //--   1st cycle --//
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      
     }


      if ( lpg >= lpg_level)
     {
      //-- 1st cycle on
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,500);
      delay(500);
      //--2nd cycle off
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      delay(500);
      //-- 1st cycle on
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,500);
      delay(500);
      //--2nd cycle off
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      delay(500);
      //-- 1st cycle on
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,500);
      delay(500);
      //--2nd cycle off
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      delay(500);
      //-- 1st cycle on
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      tone(buzzer,500);
      delay(500);
      //--2nd cycle off
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      delay(500);
     }

      if ( lpg < lpg_level)
     {
      //--   1st cycle --//
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      noTone(buzzer);
      
     }
      

     
      else{
        digitalWrite(relay1,HIGH);
        digitalWrite(relay2,HIGH);
        noTone(buzzer);
      }
  



   
  } //-- end of Void Loop --//



