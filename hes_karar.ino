bool pyon=true;//pedal yönü kontrol değişkeni

int g=1;       //vites aktarımı için gidilmesi gerekn vites değişken tutucusu
int tur = 0;  //motorun kaç tur döndüğünü tutan değişken
int abut,ibut,dbut,mothiz,tbut,mbut; //butonlar ve diğer sensorler için kullanılan geçici değişkenler

int vites=1;  //global vites tutucusu





#define reed 2//pin connected to read switch

float radius = 20.0;// tire radius (in inches)- CHANGE THIS FOR YOUR OWN BIKE
float limit[8] = {0.00,12.00,13.00,25.00,26.00,38.00,39.00,49.00};  // vites hız sınırları 
int reedVal,say;
float mphh=0.00;
float hiz=0.00;
int gvites=1;
long timer = 0;// time between one full rotation (in ms)
float mph = 0.00;
float circumference;

int maxReedCounter = 100;//min time (in ms) of one rotation (for debouncing)
int reedCounter;


void setup(){
  
  reedCounter = maxReedCounter;
  circumference = 2*3.14*radius;
  pinMode(1,OUTPUT);//tx
  pinMode(2,OUTPUT);//backlight switch
  pinMode(reed, INPUT);

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
  
  Serial.begin(9600);
}


ISR(TIMER1_COMPA_vect) {//Interrupt at freq of 1kHz to measure reed switch
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
}


void loop(){

  delay(1000);
  
   Serial.print("looop a döndümm");
  hizal();
}
void hizal(){
  Serial.print("merkez urasi hizal");
if(say==0 and mph<=69){
  mphh=mph;
  say++;
  }
  else{
    mph=mphh;
    say=0;
    }

   Serial.print("Speed =");
  Serial.println(mph);
     Serial.print("vites =");
  Serial.println(vites);
  
 //   Serial.println(limit[1]);
hiz=mph;
  Serial.print("gvites BAS");
  Serial.println(gvites);
    if (hiz>=limit[0] and hiz<=limit[1])
    {
   // Serial.print("gvites1");
     gvites=1; Serial.print(gvites);
     otovites(gvites);
    
    }
    else if (hiz>=limit[2] and hiz<=limit[3])
    {

     gvites=2;
     otovites(gvites);  
     
    }
    else if (hiz>=limit[4] and hiz<=limit[5])
    {

     gvites=3;
     otovites(gvites);  
    }
    else if (hiz>=limit[6] and hiz<=limit[7])
    {
   
     gvites=4;
     otovites(gvites);  
    }
    else if (hiz>=limit[8])
    {
       
     gvites=5;
     otovites(gvites);  
    }
   
    else {gvites=vites;
    otovites(gvites); 
    Serial.print("gvites else");}
    
} 


void otovites(int gvites){
  
  if (vites==gvites)
  { 
    loop();
  }
  else if (vites<gvites)
  {
    sol(gvites);
  }
  else if (vites>gvites)
  {
    sag(gvites);
  } 
    
} 


//vites artırmak için motor hareket kodları
void sag(int gvites){
   Serial.print("SAGA GELDI ");
    Serial.print(gvites);
  g=hesaplar();  
  if(pedal()){//pedal saat yönünde dönüyorsa devam et
      Serial.print("PEDAL OK");
  while (g>sayi())             //tur tamamlanana kadar motora güç ver
  {
     Serial.print("Psayi");
     Serial.println(g);
  
      
    digitalWrite (13, HIGH);
    delay(1000);
    digitalWrite (13, LOW);  
  }}
  vites=gvites;
  Serial.print("Sag bitti ");
    delay(100);
dur();
}

//vitesdüşürmek için motor hareket kodları
void sol(int gvites){
  g=hesaplar();
  if(pedal()){//pedal saat yönünde dönüyorrsa devam et
  while(g<sayi2())                 //tur tamamlanana kadar motora güç ver
   {
    digitalWrite (13, HIGH);
    delay(1000);
    digitalWrite (13, LOW);  
    }}
      vites=gvites;
    Serial.print("Sol bitti ");
    delay(100);
dur();
}
//motor harekeetini durdurmak için
void dur()
{
  digitalWrite (13, LOW);
 delay(50);
//moda göre geldiği yere dönme 
Serial.print("dur bitti ");

loop();
}


int hesaplar(){
int vites1=0;
int vites2=5;
int vites3=11;
int vites4=17;
int vites5=23;

if(gvites==1){
  g=vites1;
  }
 else if(gvites==2){
  g=vites2;
  }
 else if(gvites==3){
  g=vites3;
  }
 else if(gvites==4){
  g=vites4;
  }
 else if(gvites==5){
  g=vites5;
  }
return g;
}

bool pedal(){
  
  return(true);
  }


int sayi()
{
  return(g);
  }
  
 //vites düşürmek için tur hesabı 
int sayi2(){

return(g);
}
