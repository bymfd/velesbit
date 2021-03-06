
/*
kodlar içinde bulunan Serial.println(xxxxxxxxxxxxxxx) ve Serial.begin(9600) 
test aşamasında sistem kontrolü kolaylığı sağlaması için kullanılmış ve daha sonra 
lazım olma ihtimlaine karşın yorum satırına alınmıştır.
kullanılan kütüphaneler :
Rotary :https://github.com/brianlow/Rotary
LCD :https://github.com/johnrickman/LiquidCrystal_I2C
*/
#include <Wire.h>  // I2C için gerekli kütüphane içe aktarma
#include <LiquidCrystal_I2C.h> // lcd ekran için gerekli kütüphane içe aktarma
LiquidCrystal_I2C lcd(0x3F, 16, 2); // lcd ekran adreslenmesi ve boyutlarının belirlenmesi

#include <Rotary.h> // Rotary encoder ile pedal yönü kontrolü için gerekli kütüphane içe aktarılıyor
Rotary r = Rotary(3,2 ); // rotary nesnesi oluşturulup 2 ve 3. pinlerin kontrol edileceği belirtiliyor
int oto=12; // otomatik manuel mod değişimi sağlayan buton pini
int pedal=0;
int ln3 =5; // l298n sürücüsü için kullanılan pinlerin belirlenmesi
int ln4 =6; //l298n sürücüsü için kullanılan pinlerin belirlenmesi
int p=0;
const int butondur =11; // vites sınır butonunun pini
const int butonpin =7; // tur sayma için kullanılan butonun pini
const int vazb =10; // vites azaltma için kullanılan pini
const int vartb =4; // vites artırma için kullanılan butonun pini
int tur=0;  // tur değişkeni
int vites=0; //vites değişekni
int buton=0; // tur sayan butonun ilk halinini LOW kabul edilmesi için değiken ataması
int vaz=0; // vites azaltma butonunun ilk halininin LOW kabul edilmesi için değişken ataması
int vart=0; //vites artırma butonunun ilk halinin LOW kabul edilmesi için değişken ataması
int buta=0; // vites sınır butonunun ilk halinin LOW kabul edilmesi için değiken ataması
int vtur[5] = {0,5,11,17,23};  // vites tur sınırları 
int first=0; //vitesin bir kez başa çakilmesi için while kırma değişkeni
#define reed 8//hall effect kontrol pini 

float radius = 20.0;// teker inch'i  hız ölçülecek teker boyutu 20.00"
float limit[8] = {0.00,8.00,8.01,15.00,15.01,22.00,22.01,30.00};  // vites hız sınırları 
int reedVal; // hız hesabı için hall effect sensor durumu tutucu değişkeni
float hiz=0.00; //hız değikeni
long timer = 0;// bir tam tur değişkeni
float circumference; //teker çevre hesabı tutucusu
int maxReedCounter = 100;// bir dönüşün minimum süresi
int reedCounter; // hall efect sayacı


void setup() {
Serial.begin(9600); // seri habeleşmenin hazırlanması
Serial.println("----------********DURDUR********---------------");
  r.begin(true); // rotary kütüphanesinin hazırlanması
  lcd.init();   //lcd ekranın hazırlanması
  lcd.backlight();
  lcd.clear();
lcd.print("Pedallayin"); // bilgilendirme mesajı 
/*
bu kısımda kullanılan tüm
pinmode (x,INPUT)---> giriş pini
pinmode (x,OUTPUT) -------> çıkış pini 
pinleri giriş ve çıkış pini olarak belirlemek için kullanılmıştır.
*/

pinMode(1,OUTPUT);
pinMode (ln3 ,OUTPUT); 
pinMode (ln4 ,OUTPUT);
pinMode (butonpin,INPUT);
pinMode (vazb,INPUT);
pinMode (vartb,INPUT);
pinMode (butondur,INPUT);
pinMode (oto,INPUT);
/*
sistem başlar başlamaz vitesi birinci vitese çekmek için kullanılan while döngüsü
butondur pini HIGH -1 olana dek motoru birinci vitese doğru çekmek için sol fonksiyonunu kullanır 
döngüden çıkmak için first değişkeni 0 dan farklı bir değer alır ve void setup devam eder.
*/
buta = digitalRead(butondur);
delay(1000);

/* 
kullanılan tüm
lcd.clear();---> lcd ekranda yazan herşeyi silmek 
lcd.print("");---> lcd ekrana istenileni yazmak 
lcd.setCursor(x,y);--->lcd ekranda imleç yerini belirtmek (x satır y sütun)
için kullanılmıştır.
*/ 
lcd.clear();
lcd.print("Pedallayin"); // bilgilendirme mesajı 


  //  zamanlayıcı kesmesi reed anahtarının önceden belirlenmiş zamanlamalı ölçümlerine izin verilmesi
  //daha fazla bilgi için  http://arduino.cc/playground/Code/Timer1
  cli();//kesmeleri durdur
  //timer1 i 1khz e ayarlama
  TCCR1A = 0;//  TCCR1A register ' ı 0 sıfıra eşitleniyor
  TCCR1B = 0;// TCCR1b register ' ı 0 sıfıra eşitleniyor
  TCNT1  = 0;
  // 1 khz artışlar için zamanlayıcı sayısını ayarlama
  OCR1A = 1999;  // = (1/1000) / ((1/(16*10^6))*8) - 1
  //  CTC mod etkin 
  TCCR1B |= (1 << WGM12);
  // 8 ön ölçekleyici için CS11 bitini ayarlama
  TCCR1B |= (1 << CS11);   
  // zamanlayıcıyı etkinleştir ve kesmeyi karşılaştır
  TIMSK1 |= (1 << OCIE1A);
  sei();//kesmelere izin ver 

  //zamanlayıcı ayarlama sonu
  reedCounter = maxReedCounter; // hall effect sayacının minimum dönüş süresi ile eşitlenmesi
  circumference = 2*3.14*radius; // teker çevresinin hesaplanması

while (first==0){
 Serial.println("pedal : ");
  //Serial.println(pedal);
  if(pedal==1){
  p++;
  sol();
if(p>=4){
p=0;
pedal=0;
dur();
}
}
 
 buta = digitalRead(butondur);
if(buta==HIGH){
  dur();
    vites=1;
  sag();
  delay(200);
  dur();
  tur=0;
  first=1;
}
}

lcd.clear();
}


ISR(TIMER1_COMPA_vect) { 
//  hızı ölçmek için 1kHz frekansında kesinti
  reedVal = digitalRead(reed);// 8. pin değeri okunuyor
  if (reedVal){//hall effect sensor 1 ise
    if (reedCounter == 0){//dönüş için maksimum süre tamamalnmış ise 
      hiz = (56.8*float(circumference))/float(timer);//hızı hesaplanıyor

      timer = 0;//timer değiikeni sıfırlanıyor
      reedCounter = maxReedCounter;//reedCount değeri sıfırlanıyor
    }
    else{
      if (reedCounter > 0){//reedCounter değişkeninin  değeri negatif olmaması için 
        reedCounter -= 1;//reedCount değerini 1 azalt
      }
    }
  }
  else{// hall  effect değeri 0 ise
  if (reedCounter > 0){// reedCounter değişkeninin değeri negatif olmaması için 
      reedCounter -= 1;//reedCounter değerini 1 azalt
    }
  }
  if (timer > 2000){
    hiz = 0.00;//Hall effect sensor hiç timer değeri 2000 olana dek hiç tetiklenmemişse hiz 0 a eşitle
   // pedal=0;
  }
  else{
    timer += 1;//timer değerini azalt
  }
     
  unsigned char result = r.process();
  if (result) {
    pedal=(result == DIR_CW ? 1 : 0);
}



}


void loop() {

  Serial.print("VITES: ");
  Serial.println(vites);
  Serial.print("HIZ: ");
  Serial.println(hiz);

          lcd.setCursor(0, 0);
        lcd.print("HIZ:");
        lcd.setCursor(4,0);
        lcd.print(hiz);
        Serial.print("*************pedal :");
if(pedal==1){
  p++;
if(p>=4){
p=0;
pedal=0;
}}
Serial.println(pedal);
//vites sınır kontrol bloğu başı 
 buta = digitalRead(butondur);// buta buta değişkeni vites sınır butonu ile ilişkilendiriliyor
if(buta==HIGH){
  dur();//motorun daha fazla hareket etmemesi için motor durduruluyor
  sag();// 200 ms vites artırma yönünde hareket ederek butona basılı kalmaması sağlanıyor
  delay(200);
  vites=1; //vites bir hata olmasına karşın 1 olarak atanıyor
  tur=0; //tur hatası olmaması için tur sıfırlanıyor
}
//vites sınır kontrol bloğu sonu

   

//otomatik manuel mod değiştirme buton kontrolü için 12. pin değeri kontrol ediliyor
if (digitalRead(oto)==1){ //eğer buton 1 ise otomatik mod etkin oluyor
  Serial.println("OTO");
     lcd.setCursor(13, 0); // lcd ekran imlecini sağa taşı
  lcd.print("MOD");
  lcd.setCursor(15, 1); // lcd ekran imlecini sağa taşı
  lcd.print("O"); //lcd ekrana modun otomatik olduğunu belirten "O" harfinin yazılması 
  hizal(); // hıza bağlı vites değişimi için hizal fonksiyonu çalışıtırılıyot
  
  }
  else{// 12. pin 0 ise vites azaltma ve artırma tuşları kontrol ediliyor
  Serial.println("MANUEL");
   lcd.setCursor(13, 0); // lcd ekran imlecini sağa taşı
  lcd.print("MOD");
 lcd.setCursor(15, 1); // lcd ekran imlecini sağa taşı
  lcd.print("M"); //lcd ekrana modun manuel olduğunu belirten "M" harfinin yazılması 
 vart = digitalRead(vartb);

if(vart==1 and vites < 5){ // vites artırma tuşuna basılı ve vites 5 ten küçük ise 
 //  Serial.println("vart");
  delay(100); //ark için bekleme
  vites ++; //vites değişkeni artılıyor
 // Serial.println(vites);
   vitesbul(); //vites değiştirmek için vitesbul fonksiyonu çalıştırılıyor
//butona basılı tutma ve ark önlemi başı
  while(vart == HIGH){
     vart = digitalRead(vartb);
  }//butona basılı tutma ve ark önlemi sonu 
  delay(100);//buton kontrolü için küçük bekkletme
 
}else {vitesbul();}
//-------------------------------------------


 vaz = digitalRead(vazb); 
if(vaz==1 and vites>1){  //vites azaltma butonuna basılı ve vites 1 den büyük ise 
   //Serial.println("vazb");
  delay(100); //ark için bekleme
  vites --; //vites değişkeni bir azaltılıyor
  vitesbul(); // vites değiştirmek için vitesbul fonksiyonu çalıştırılıyor
  // Serial.println(vites);
//butona basılı tutma ve ark önlemi başı
  while(vaz == HIGH){
     vaz = digitalRead(vazb);
  }//butona basılı tutma ve ark önlemi sonu 
  delay(100);//buton kontrolü için küçük bekkletme
}
else {vitesbul();}

}
//--------------------------------------------


}

void sag(){
 
    //Serial.println("sag");
  //motor hareket yönü belirlemek için dijital pin çıkışlarının tetiklenmesi başı   
  digitalWrite (ln3, HIGH);
  digitalWrite (ln4, LOW);
  //motor hareket yönü belirlemek için dijital pin çıkışlarının tetiklenmesi sonu
  buton = digitalRead(butonpin); // tur sayma butonunun pin ile ilşkilendirilmesi
if(buton==1){ // tur sayma butonuna basılı ise 
 delay(10); //ark önlemi 
 tur ++;// tur 1 arttırılıyor
     Serial.print("TUR: ");
  Serial.println(tur);
//butona basılı tutma ve ark önlemi başı
  while(buton == HIGH){
     buton = digitalRead(butonpin);
  }//butona basılı tutma ve ark önlemi sonu 
  delay(10);//buton kontrolü için küçük bekkletme
}
}

void sol(){
 
    //Serial.println("sol");
  //motor hareket yönü belirlemek için dijital pin çıkışlarının tetiklenmesi başı   
  digitalWrite (ln4, HIGH);
  digitalWrite (ln3, LOW);
 //motor hareket yönü belirlemek için dijital pin çıkışlarının tetiklenmesi sonu
  buton = digitalRead(butonpin);  // tur sayma butonunun pin ile ilşkilendirilmesi
if(buton==1){ //tur sayma butonuna basılı ise 
 delay(10);//ark önlemi
 tur --; // tur  1 azaltılıyor
     Serial.print("TUR: ////////////////*777777777777777777777777777777777777777777777777777777777777");
  Serial.println(tur);
   //butona basılı tutma ve ark önlemi başı
  while(buton == HIGH){
     buton = digitalRead(butonpin);
  }//butona basılı tutma ve ark önlemi sonu
 delay(10);//buton kontrolü için küçük bekkletme
}
}

void dur(){
   // Serial.println("dur");
   //motor hareketlerini durdurmak için dijital pin çıkışlrının kapatılması başı 
  digitalWrite (ln4, LOW);
  digitalWrite (ln3, LOW);
//motor hareketlerini durdurmak için dijital pin çıkışlrının kapatılması sonu
 lcd.setCursor(0, 1);
 lcd.print("VITES:");
 lcd.setCursor(6,1);
 lcd.print(vites  );
}

void hizal(){  
hiz=hiz;
//delay(500);
//hiz=hiz;
//   Serial.println("HIZ =");
//  Serial.println(hiz);
//     Serial.print("vites =");
//  Serial.println(vites);

/*
bu kısımda 
algılanan hızın hız limiti listesi aralıklarına göre karşılaştırılarak
vites değişkenine olması gereken vites değeri atanıp 
vites değiştirme işlemi için vitesbul fonksiyonu çalıştırılmıştır.
*/
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
  
    else {vites=vites;  //runtime error yememek için hız değişkeni hiçbir hız limtine girmiyor ise vites vites olarak kalmalıdır 
   
 //Serial.println("gvites else");
    }
  


}

void vitesbul(){
  /*
  bu kısımda 
  vites değişeni kontrol edilerek motorun hangi vites için hareket edeceği belirlenmektedir
  pedal fonksiyonundan dönen true değeri pedalın saat yönünde döndürüldüğünü doğrular "and" ile kullanılarak pedal doğru hareket etmiyorsa vites aktarımı yapılmamalı
  ve ikinci kontrolde tur değişkeni vtur listesi içinde bulunan değerden küçük ise vites artmalı yani sag fonksiyonu çalıştırılmalı
  eğer tur değişkeni vtur listesi içindeki değerden büyük ise vites azaltılmalı yani sol fonksiyonu çalıştırılmalıdır
  eğer tur vtur içindeki hiçbir değere uymuyorsa yada pedal doğru yönde hareket etmiyorsa dur fonksiyonu ile işlem kesilmiş olur  
  */
  if(vites==0 and pedal==1){ 

     sol(); 
}
  else{
    dur();

  }
  
  
if(vites==1){ 
  if(tur<vtur[0] and pedal==1){
    sag();
  }
  else if(tur>vtur[0] and pedal==1){
    sol(); 
  }
  else{
    dur();

  }
}
//-----------------------
if(vites==2){
  if(tur<vtur[1] and pedal==1){
    sag();
  }
  else if(tur>vtur[1] and pedal==1){
    sol(); 
  }
  else{
    dur();

  }
}
//-----------------------
if(vites==3){
  if(tur<vtur[2] and pedal==1){
    sag();
  }
  else if(tur>vtur[2] and pedal==1){
    sol(); 
  }
  else{
    dur();
   
  }
}
//------------------------
if(vites==4){
  if(tur<vtur[3] and pedal==1){
    sag();
  }
  else if(tur>vtur[3] and pedal==1){
    sol(); 
  }
  else{
    dur();
    
  }
}
//------------------------
if(vites==5){
    if(tur<vtur[4] and pedal==1){
    sag();
  }
  else if(tur>vtur[4] and pedal==1){
    sol(); 
  }
  else{
    dur();
    
  }
}
  

  }
  
