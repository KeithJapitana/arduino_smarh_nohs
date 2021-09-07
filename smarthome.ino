
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// DHT SECTION
#include <dht.h>
#define dataPin 12 
dht DHT; 
int TempI = 0; // Index

// Counter of pages
int page_counter=1 ;       //To move beetwen pages
int subpage_counter=0;    //To move submenu 1 
int subpage2_counter=0;   //To move submenu 2 
int subpage3_counter=0;
int subpage4_counter=0;



// Buttons
int up = 3;               //Up button
int down = 2;           //Down button
int sel = 4; 
int fan1 = 8;
int fan2 = 9;
int fan3 = 10;
int fan4 = 11;
int fan5 = 7;

//Debounce
boolean current_up = LOW;          
boolean last_up=LOW;            
boolean current_sel = LOW;
boolean last_sel = LOW;
boolean last_down = LOW;
boolean current_down = LOW;

// Fan State
boolean turbo_state = LOW;
boolean fan1_state = LOW;
boolean fan2_state = LOW;
boolean fan3_state = LOW;
boolean fan4_state = LOW;

// Sensors ---------------//
int fan1_temp = 32;
int fan2_temp = 32;
int fan3_temp = 32;
int fan4_temp = 32;
int gas_level = 20;




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




        

void setup() {
  
  pinMode(fan1, OUTPUT);
  pinMode(fan2, OUTPUT);
  pinMode(fan3, OUTPUT);
  pinMode(fan4, OUTPUT);
  pinMode(fan5, OUTPUT);
  digitalWrite(fan1,HIGH);
  digitalWrite(fan2,HIGH);
  digitalWrite(fan3,HIGH);
  digitalWrite(fan4,HIGH);
  digitalWrite(fan5,HIGH);
  lcd.begin(16,2); 

  lcd.createChar(1, back);
  lcd.createChar(2, arrow);
  lcd.createChar(3, cell);
  lcd.setCursor(0,0);
  lcd.print("Gas Leakage &");
  lcd.setCursor(0,1);
  lcd.print("Smoke Sensor");
  lcd.clear();
  

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


void loop() {

current_up = debounce(last_up, up);         //Debounce for Up button
current_down = debounce(last_down, down);   //Debounce for Down button
current_sel = debounce(last_sel, sel);




//----Page counter function to move pages----//
if(subpage_counter==0 && subpage2_counter==0){ 
//Page Up
    if (last_up== LOW && current_up == HIGH){  //When up button is pressed
      lcd.clear();                     //When page is changed, lcd clear to print new page  
      if(page_counter <7){              //Page counter never higher than 3(total of pages)
      page_counter= page_counter +1;   //Page up
      
      }
      else{
      page_counter= 7;  
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
  switch (page_counter) {
   
    case 1:{     //Design of home page 1
   int readData = DHT.read22(dataPin); 
   float t = DHT.temperature;
   float h = DHT.humidity;
   lcd.setCursor(0,0);
   lcd.print("TEMP.    HUMID.");
   lcd.setCursor(TempI, 1);
   lcd.print(t);
   lcd.write(byte(3));
   lcd.print("C");
   lcd.setCursor(9,1);
   lcd.print(h);
   lcd.print("%");
   delay(100);;
    }
    break;

    case 2: { //TEMP OF FAN 1
    //Design of page 2 RGB control

   //Static objects 
     lcd.setCursor(0,0); 
     lcd.print("TURBO MODE");
     lcd.setCursor(2,1);
     lcd.print("=");   
     lcd.setCursor(6,1);
     lcd.write(byte(1));     //Return custom char
      
     //Functions
     
     // Sub counter control
     if (last_sel== LOW && current_sel == HIGH){ //select button pressed
      if(subpage_counter <2){                    // subpage counter never higher than 4 (total of items)
     subpage_counter ++;                         //subcounter to move beetwen submenu
     }
     else{                                       //If subpage higher than 4 (total of items) return to first item
      subpage_counter=1;
     }
     }
     last_sel=current_sel;                      //Save last state of select button
     
     //First item control(subpage_counter =1) red led
     if(subpage_counter==1){
     lcd.setCursor(5,1);
     lcd.print(" ");                           //Delete last arrow position              
     lcd.setCursor(0,1);
     lcd.write(byte(2));
     if (last_up== LOW && current_up == HIGH){  //Up, red led on
     turbo_state=HIGH;
     }
     last_up=current_up;
     
     if(last_down== LOW && current_down == HIGH){//Down, red led off
     turbo_state=LOW; 
     }
     last_down=current_down;
     }

     
     if(turbo_state==LOW){ 
     lcd.setCursor(3,1);
     lcd.print("N"); 
     digitalWrite(fan5, HIGH);
    
   
     }

    
     
 
     else{      
     lcd.setCursor(3,1);
     lcd.print("Y");   
     digitalWrite(fan5, LOW);           
     
     }
     

    

     //Fourth item control (subpage_counter=4) back
     if(subpage_counter==2){
     lcd.setCursor(0,1);
     lcd.print(" ");                           //Delete last arrow position                    
     lcd.setCursor(5,1);                      //Place the arrow
     lcd.write(byte(2));                      
     if (last_up== LOW && current_up == HIGH){ //Yes button pressed
     lcd.setCursor(5,1);
     lcd.print(" ");                            //Delete arrow 
     subpage_counter=0;                         //if sub page 0, exit sub menu, up/down pages enabled
     }
     last_up=current_up;
     
     if(last_down== LOW && current_down == HIGH){//No button pressed
     subpage_counter=1;                          //Stay on sub menu, return to R
     }
     last_down=current_down;
     }
    }//case2 end
  
    break;

    case 3: {   //TEMP OF FAN 1
     lcd.setCursor(3,0);
     lcd.print("Fan 1 Temp");
     lcd.setCursor(8,1);
     lcd.write(byte(3));
     lcd.print("C");
     lcd.setCursor(6,1);
     if(fan1_temp <10){    //To avoid "0" of number 10
     lcd.setCursor(6,1);
     lcd.print("0");
     }
     lcd.print(fan1_temp);
     
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
     if(fan1_temp < 90){                         //led_lum never higher than 10 (max value)
     fan1_temp ++ ;
     }
     else{
     fan1_temp = 0;                              //If led_lum higher than 10, return to 0
     }
     }
     last_up=current_up;
     
     
     if(last_down== LOW && current_down == HIGH){  //Down bright -
     if(fan1_temp >0){                              //led_lum never lower than 0
     fan1_temp --;                                   
     }
     else{
     fan1_temp = 0;                                 
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

    case 4: {   //TEMP OF FAN 2
     lcd.setCursor(3,0);
     lcd.print("Fan 2 Temp");
     lcd.setCursor(8,1);
     lcd.write(byte(3));
     lcd.print("C");
     lcd.setCursor(6,1);
     if(fan2_temp <10){    //To avoid "0" of number 10
     lcd.setCursor(6,1);
     lcd.print("0");
     }
     lcd.print(fan2_temp);
     
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
     if(fan2_temp < 90){                         //led_lum never higher than 10 (max value)
     fan2_temp ++ ;
     }
     else{
     fan2_temp = 0;                              //If led_lum higher than 10, return to 0
     }
     }
     last_up=current_up;
     
     
     if(last_down== LOW && current_down == HIGH){  //Down bright -
     if(fan2_temp >0){                              //led_lum never lower than 0
     fan2_temp --;                                   
     }
     else{
     fan2_temp = 0;                                 
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

    case 5: {   //TEMP OF FAN 3
     lcd.setCursor(3,0);
     lcd.print("Fan 3 Temp");
     lcd.setCursor(8,1);
     lcd.write(byte(3));
     lcd.print("C");
     lcd.setCursor(6,1);
     if(fan3_temp <10){    //To avoid "0" of number 10
     lcd.setCursor(6,1);
     lcd.print("0");
     }
     lcd.print(fan3_temp);
     
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
     if(fan3_temp < 90){                         //led_lum never higher than 10 (max value)
     fan3_temp ++ ;
     }
     else{
     fan3_temp = 0;                              //If led_lum higher than 10, return to 0
     }
     }
     last_up=current_up;
     
     
     if(last_down== LOW && current_down == HIGH){  //Down bright -
     if(fan3_temp >0){                              //led_lum never lower than 0
     fan3_temp --;                                   
     }
     else{
     fan3_temp = 0;                                 
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

    case 6: {   //TEMP OF FAN 4
     lcd.setCursor(3,0);
     lcd.print("Fan 4 Temp");
     lcd.setCursor(8,1);
     lcd.write(byte(3));
     lcd.print("C");
     lcd.setCursor(6,1);
     if(fan4_temp <10){    //To avoid "0" of number 10
     lcd.setCursor(6,1);
     lcd.print("0");
     }
     lcd.print(fan4_temp);
     
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
     if(fan4_temp < 90){                         //led_lum never higher than 10 (max value)
     fan4_temp ++ ;
     }
     else{
     fan4_temp = 0;                              //If led_lum higher than 10, return to 0
     }
     }
     last_up=current_up;
     
     
     if(last_down== LOW && current_down == HIGH){  //Down bright -
     if(fan4_temp >0){                              //led_lum never lower than 0
     fan4_temp --;                                   
     }
     else{
     fan4_temp = 0;                                 
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

    case 7: {   //TEMP OF FAN 1
     lcd.setCursor(3,0);
     lcd.print("Gas Level=");
     lcd.setCursor(8,1);    
     lcd.print("%");
     lcd.setCursor(6,1);
     if(gas_level <10){    //To avoid "0" of number 10
     lcd.setCursor(6,1);
     lcd.print("0");
     }
     lcd.print(gas_level);
     
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
     if(gas_level < 90){                         //led_lum never higher than 10 (max value)
     gas_level ++ ;
     }
     else{
     gas_level = 0;                              //If led_lum higher than 10, return to 0
     }
     }
     last_up=current_up;
     
     
     if(last_down== LOW && current_down == HIGH){  //Down bright -
     if(gas_level >0){                              //led_lum never lower than 0
     gas_level --;                                   
     }
     else{
     gas_level = 0;                                 
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
    
  } //switch end
    //FAN 1 CONTROL
     if(DHT.temperature >= fan1_temp ){
     digitalWrite(fan1, LOW);
     }
      if(DHT.temperature < fan1_temp ){
     digitalWrite(fan1, HIGH);
     }
    //FAN 2 CONTROL
     if(DHT.temperature >= fan2_temp ){
     digitalWrite(fan2, LOW);
     }
      if(DHT.temperature < fan2_temp ){
     digitalWrite(fan2, HIGH);
     }
    //-------------FAN 3 CONTROL-----//
     if(DHT.temperature >= fan3_temp ){
     digitalWrite(fan3, LOW);
     }
      if(DHT.temperature < fan3_temp ){
     digitalWrite(fan3, HIGH);
     }

     //---------FAN 4 CONTROL------------//
     if(DHT.temperature >= fan4_temp ){
     digitalWrite(fan4, LOW);
     }
      if(DHT.temperature < fan4_temp ){
     digitalWrite(fan4, HIGH);
     }
   
  
 
     


  
} //loop end
