
#include <Rotary.h>
Rotary r = Rotary(2, 3);
int oto=1;
int ln3 =6;
int ln4 =5;
unsigned char result;
const int butondur =11;
const int butonpin =7;
const int vazb =9;
const int vartb =4;
int tur=0;
int vites=1;
int buton=0;
int vaz=0;
int vart=0;
int buta=0;
int vtur[5] = {0,5,11,17,23};  // vites tur sınırları 
//int vites2=5;
//int vites3=11;
//int vites4=17;
//int vites5=23;

int first=0;
#define reed 8//pin connected to read switch

float radius = 20.0;// tire radius (in inches)- CHANGE THIS FOR YOUR OWN BIKE
float limit[8] = {0.00,8.00,8.01,15.00,15.01,22.00,22.01,30.00};  // vites hız sınırları 
int reedVal,say;
float mphh=0.00;
float hiz=0.00;
long timer = 0;// time between one full rotation (in ms)
float mph = 0.00;
float circumference;
int maxReedCounter = 100;//min time (in ms) of one rotation (for debouncing)
int reedCounter;



void setup() {
 r.begin(true);
Serial.begin(9600);
  Serial.println("-----------------------------------------------------------------*************----------------------------------");
  // TIMER SETUP- the timer interrupt allows preceise timed measurements of the reed switch
  //for mor info about configuration of arduino timers see http://arduino.cc/playground/Code/Timer1
  cli();//stop interrupts

  //set timer1 interrupt at 1kHz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;
  // set timer count for 1khz increments
  OCR1A = 1999;// = (1/1000) / ((1/(16*10^6))*8) - 1
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for 8 prescaler
  TCCR1B |= (1 << CS11);   
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  sei();//allow interrupts

  //END TIMER SETUP


  
  reedCounter = maxReedCounter;
  circumference = 2*3.14*radius;
  pinMode(1,OUTPUT);//tx
pinMode (ln3 ,OUTPUT);
pinMode (ln4 ,OUTPUT);
pinMode (butonpin,INPUT);
pinMode (vazb,INPUT);
pinMode (vartb,INPUT);
pinMode (butondur,INPUT);
//Serial.begin(9600);
buta = digitalRead(butondur);

//while(digitalRead(buta) ==HIGH){//sınır butonu 1 olana kadar çalışacağı için başa gelmek zorunda
//  Serial.println("stop butonuna basana kadar");
//digitalWrite (ln4, HIGH);
//digitalWrite (ln3, LOW);
// buta = digitalRead(butondur);
//  }//motoru durdur 
//digitalWrite (ln4, LOW);
//digitalWrite (ln3, LOW); 
//delay(100);
//  Serial.println("stop durdu");
////lcd ile bilgi ver 
// // lcd.clear();
//  //lcd.print("otomatik mod etkin"); 
//// lcd.setCursor(0,4);
//  //   lcd.println("kaskinizi takin");
// digitalWrite (ln4, LOW);
//digitalWrite (ln3, HIGH);
//delay(100);
// digitalWrite (ln4, LOW);
//digitalWrite (ln3, LOW); 
//delay(1);
//  Serial.println("bitti");

pinMode (ln3 ,OUTPUT);
pinMode (ln4 ,OUTPUT);

delay(2000);
sol();
while (first==0){
 buta = digitalRead(butondur);
if(buta==HIGH){
  dur();
//  lcd.clear();
  //lcd.print("BASMAA"); 
    vites=0;
  sag();
  delay(200);
  dur();
  tur=0;
  first=1;
}
}

 
}


ISR(TIMER1_COMPA_vect) {
   // Serial.println("hiz alma zimbirtisi");
  //Interrupt at freq of 1kHz to measure reed switch
  reedVal = digitalRead(reed);//get val of A0
  if (reedVal){//if reed switch is closed
    if (reedCounter == 0){//min time between pulses has passed
      mph = (56.8*float(circumference))/float(timer);//calculate miles per hour
      timer = 0;//reset timer
      reedCounter = maxReedCounter;//reset reedCounter
    }
    else{
      if (reedCounter > 0){//don't let reedCounter go negative
        reedCounter -= 1;//decrement reedCounter
      }
    }
  }
  else{//if reed switch is open
    if (reedCounter > 0){//don't let reedCounter go negative
      reedCounter -= 1;//decrement reedCounter
    }
  }
  if (timer > 2000){
    mph = 0;//if no new pulses from reed switch- tire is still, set mph to 0
  }
  else{
    timer += 1;//increment timer
  }
  //  Serial.println("hizal zimbirtisi sonu"); 
}








void loop() {
//-----------------------------------------
 buta = digitalRead(butondur);
if(buta==HIGH){
  dur();
  sag();// result = r.process();
//  if (result) {
//    Serial.println("true");
//    return true;
//  }
// else {
// Serial.println("false");
// return false;
// }
  delay(200);
  vites=1;
  tur=0;
}
if (oto==0){
  //Serial.println("hizal");
  hizal();
  
  }
  else{

 vart = digitalRead(vartb);

if(vart==1 and vites < 5){
   Serial.println("vart");
  delay(100);
  vites ++;
  Serial.println(vites);
   vitesbul();

  while(vart == HIGH){
     vart = digitalRead(vartb);
  }
  delay(100);
 
}
//-------------------------------------------
 vaz = digitalRead(vazb); 
if(vaz==1 and vites>1){
   Serial.println("vazb");
  delay(100);
  vites --;
  vitesbul();
    
 Serial.println(vites);
  while(vaz == HIGH){
     vaz = digitalRead(vazb);
  }
  delay(100);

}

}
//--------------------------------------------


}

void sag(){
    Serial.println("sag");
  digitalWrite (ln3, HIGH);
  digitalWrite (ln4, LOW);
  buton = digitalRead(butonpin); 
if(buton==1){
 delay(10);
 tur ++;
   Serial.println(tur);
  while(buton == HIGH){
     buton = digitalRead(butonpin);
  }
 delay(10);
}
}

void sol(){
    Serial.println("sol");
  digitalWrite (ln4, HIGH);
  digitalWrite (ln3, LOW);
  buton = digitalRead(butonpin); 
if(buton==1){
 delay(10);
 tur --;
   Serial.println(tur);
  while(buton == HIGH){
     buton = digitalRead(butonpin);
  }
 delay(10);
}
}

void dur(){
    Serial.println("dur");
  digitalWrite (ln4, LOW);
  digitalWrite (ln3, LOW);

}

void hizal(){



  
 //   Serial.println(limit[1]);
hiz=mph;


   Serial.println("Speed =");
  Serial.println(mph);
     Serial.print("vites =");
  Serial.println(vites);
  
    if (hiz>=limit[0] and hiz<=limit[1])
    {

     vites=1; 
    vitesbul(); 
    
    
    }
    else if (hiz>=limit[2] and hiz<=limit[3])
    {

     vites=2; 
     vitesbul(); 
     
    }
    else if (hiz>=limit[4] and hiz<=limit[5])
    {

     vites=3;
     vitesbul(); 
  
    }
    else if (hiz>=limit[6] and hiz<=limit[7])
    {
   
     vites=4;
     vitesbul(); 

    }
    else if (hiz>=limit[8])
    {
       
     vites=5;
     vitesbul(); 

    }
   
    else {vites=vites;

    Serial.println("gvites else");
    }
  


}

void vitesbul(){
  
  
if(vites==1){
  if(tur<vtur[0] and pedal()==true){
    sag();
  }
  else if(tur>vtur[0] and pedal()==true){
    sol(); 
  }
  else{
    dur();

  }
}
//-----------------------
if(vites==2){
  if(tur<vtur[1] and pedal()==true){
    sag();
  }
  else if(tur>vtur[1] and pedal()==true){
    sol(); 
  }
  else{
    dur();

  }
}
//-----------------------
if(vites==3){
  if(tur<vtur[2] and pedal()==true){
    sag();
  }
  else if(tur>vtur[2] and pedal()==true){
    sol(); 
  }
  else{
    dur();
   
  }
}
//------------------------
if(vites==4){
  if(tur<vtur[3] and pedal()==true){
    sag();
  }
  else if(tur>vtur[3] and pedal()==true){
    sol(); 
  }
  else{
    dur();
    
  }
}
//------------------------
if(vites==5){
  if(tur<vtur[4] and pedal()==true){
    sag();
  }
  else if(tur>vtur[4] and pedal()==true){
    sol(); 
  }
  else{
    dur();
    
  }
}
  

  }
  
boolean pedal(){

// result = r.process();
//  if (result) {
//    Serial.println("true");
//    return true;
//  }
// else {
// Serial.println("false");
return true;
// }

}


