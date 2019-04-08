
#include <Encoder.h>
Encoder myEnc(5, 6);
int g=1;
int say = 0; 
int abut,ibut,dbut;
int ln3 =2;
int ln4 =3;
bool durb = false;
const int b1 =8;
const int b2 =10;
const int b3 =12;
int vites=1;
int butonsayac=0;
int gvites=0;

void setup() {

pinMode (ln3 ,OUTPUT);
pinMode (ln4 ,OUTPUT);
pinMode (b1,INPUT);
pinMode (b2,INPUT);
pinMode (b3,INPUT);
Serial.begin(9600);
}

long oldPosition  = -999;

void loop() {
bul();

 }
int sayi(){
oldPosition=0;
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
  //Serial.println(newPosition);
    say=newPosition;
    return(say);
  }

  Serial.println("ssss");
}

void bul(){
  abut = digitalRead(b1);
  ibut = digitalRead(b2);
  dbut = digitalRead(b3);  
  abut = digitalRead(b1);
  if (abut == HIGH) {
     delay(20);
  while(abut == HIGH){ 
   abut= digitalRead(b1); 
   delay(10);
   }
     if(vites!=1 and vites>1){
        vites --;
      }
     else {
    vites=1;
    } 
 butonsayac++;
   gvites=vites;
   sol(gvites);
 }
  if (ibut == HIGH) {
 ibut = digitalRead(b2);
     delay(20);
while(ibut == HIGH){ 
     ibut= digitalRead(b2); 
     delay(10);
     }
    if(vites!=5 and vites<5){
       vites ++;
       }
 else {
  vites=5;
 }
    butonsayac++;
    butonsayac++;
     gvites=vites;
     sag(gvites); 
 }
   }


int hesaplar(){
int vites1=1;
int vites2=186;
int vites3=370;
int vites4=550;
int vites5=670;

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

void sag(int gvites){
  g=hesaplar();  
  while (g>=sayi())
  {
    digitalWrite (ln3, HIGH);
    digitalWrite (ln4, LOW);
  }
dur();
}
void sol(int gvites){
  g=hesaplar();
  while(g<=sayi())
   {
    digitalWrite (ln4, HIGH);
    digitalWrite (ln3, LOW);  
    }
dur();
}

void dur()
{
  digitalWrite (ln4, LOW);
  digitalWrite (ln3, LOW);
bul();
}
