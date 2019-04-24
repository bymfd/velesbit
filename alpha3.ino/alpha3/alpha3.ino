#include <LiquidCrystal.h>
const int rs = 12, en = 10, d4 = A3, d5 = A2, d6 = A1, d7 = A0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool pyon=false; //pedal yönü değişkeni
int hiz=0;         //rpm 
float sabit=0.33;//step motordann alınan veriyi km/s e çevirmek için sabit çarpan 
int hizpin=A0;
int oto=0;      // otomatik mod kontrol değişkeni oto mod =0
int limit[9] = {1,15,16,30,31,45,46,50,51};  // vites hız sınırları 
			//       | 1 ||  2 ||  3 || 4 || 5|
int g=1;
int tur = 0;    //tel asılma sistemi üzerindeki butonun dönüşünü tutan sayaç
int abut,ibut,dbut,mothiz,tbut,mbut;
int ln3 =2;//motor 1.pim
int ln4 =3;//mootor2.pin

const int b4=7;//tur butonu tbut
const int b1 =8;//manuel vites -
const int b2 =10;//manuel vites +
const int b0 =11;//durdurma butonu dbut
const int b3 =12;//mod butonu mbut
int vites=1;
int gvites=0;

void setup() {
pinMode (hizpin,INPUT);
pinMode (ln3 ,OUTPUT);
pinMode (ln4 ,OUTPUT);
pinMode (b0,INPUT);
pinMode (b1,INPUT);
pinMode (b2,INPUT);
pinMode (b3,INPUT);
pinMode (b4,INPUT);
Serial.begin(9600);
lcd.begin(16, 4);
//motoru birinci vitese çekme 
 dbut = digitalRead(b0);
 while(dbut == LOW){//sınır butonu 1 olana kadar çalışacağı için başa gelmek zorunda
digitalWrite (ln4, HIGH);
digitalWrite (ln3, LOW);
 dbut = digitalRead(b0);
  }//motoru durdur 
 digitalWrite (ln4, LOW);
digitalWrite (ln3, LOW); 
//lcd ile bilgi ver 
  lcd.clear();
  lcd.print("otomatik mod etkin"); 
 lcd.setCursor(0,4);
     lcd.println("kaskinizi takin");
}


void loop() {

if(oto==0)
    {
		hizal();
	}
	else
	{	
	    bul();
	}
//mod değişim butonu 
	 mbut = digitalRead(b3);
if(mbut==HIGH){
  if(oto!=0)
  {oto=0;
    lcd.clear();
  lcd.print("otomatik mod etkin"); }
  else {oto=1;
    lcd.clear();
  lcd.print("manuel mod etkin"); }
  while(mbut == HIGH){
  dbut = digitalRead(b3);
  }
 }

}
bool pedal(){
  
  return(true);
  }
//vitesin artılıp azaltılacağını belirleme
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
 //otomatik vites değiştirmek için hız alma ve hız aralıklarına bağlı vites kararı
void hizal(){		
mothiz = analogRead(hizpin);
delay(10);
hiz=mothiz*sabit;
		if (hiz>=limit[0] or hiz<=limit[1])
		{
		 vites=1;
		 gvites=vites;
		 otovites(gvites);
		}
		else if (hiz>=limit[2] or hiz<=limit[3])
		{
		vites=2;
		 gvites=vites;
		 otovites(gvites);	
		}
		else if (hiz>=limit[4] or hiz<=limit[5])
		{
		vites=3;
		 gvites=vites;
		 otovites(gvites);	
		}
		else if (hiz>=limit[6] or hiz<=limit[7])
		{
		vites=4;
		 gvites=vites;
		 otovites(gvites);	
		}
		else if (hiz>=limit[8] or hiz<=limit[9])
		{
		vites=5;
		 gvites=vites;
		 otovites(gvites);	
		}
	
} 


 
 //vites artırmak için tur hesabı
int sayi()
{
tbut = digitalRead(b4);
 tbut = digitalRead(b4);
if(tbut==1){
 delay(10);
 tur ++;
 lcd.setCursor(0,4);
     lcd.print(tur);
     lcd.println(". tur");
  while(tbut == HIGH){
     tbut = digitalRead(b4);
  }
 delay(10);
}
    return(tur);
  }

  
 //vites düşürmek için tur hesabı 
int sayi2(){
   tbut = digitalRead(b4);
if(tbut==1){
 delay(10);
 tur --;
 lcd.setCursor(0,4);
     lcd.print(tur);
     lcd.println(". tur");
  while(tbut == HIGH){
     tbut = digitalRead(b4);
  }
 delay(10);
}
    return(tur);
}

//manuel vites değiştirme kararı
void bul(){
  abut = digitalRead(b1);
  ibut = digitalRead(b2);
//artırma butonuna basma denetim başı
//---------------------------------
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
   gvites=vites;
   sol(gvites);
 }
//artırma butonuna basma denetim sonu
//-------------------------------
//düşürme butonuna basma denetim başı
//----------------------------------
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
     gvites=vites;
     sag(gvites); 
 }
//düşürme denetim sonu
//------------------------
   }

//vitesler arasındaki tur sayıları ve sisteme bildirmek için değiken ataması
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
//vites artırmak için motor hareket kodları
void sag(int gvites){
  g=hesaplar();  
  if(pedal()){//pedal saat yönünde dönüyorsa devam et
  while (g>=sayi())             //tur tamamlanana kadar motora güç ver
  {
    digitalWrite (ln3, HIGH);
    digitalWrite (ln4, LOW);
  }}
dur();
}

//vitesdüşürmek için motor hareket kodları
void sol(int gvites){
  g=hesaplar();
  if(pedal()){//pedal saat yönünde dönüyorrsa devam et
  while(g<=sayi2())                 //tur tamamlanana kadar motora güç ver
   {
    digitalWrite (ln4, HIGH);
    digitalWrite (ln3, LOW);  
    }}
dur();
}
//motor harekeetini durdurmak için
void dur()
{
  digitalWrite (ln4, LOW);
  digitalWrite (ln3, LOW);
//moda göre geldiği yere dönme 
if (oto!=0){bul();}
else{hizal();}
}
