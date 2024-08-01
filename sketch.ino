#include <Wire.h> 
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h>
#include <TM1637Display.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define yukariButonu 44
#define asagiButonu 43
#define secimButonu 42

#define tahtapot A0

#define kalp1 31
#define kalp2 32
#define kalp3 33

#define DIO 2
#define CLK 3

#define pin1 A2
#define SEGMENT_A 24
#define SEGMENT_B 25
#define SEGMENT_C 26
#define SEGMENT_D 27
#define SEGMENT_E 28
#define SEGMENT_F 29
#define SEGMENT_G 30

const byte rakamlar[10][7]={
 {0,0,0,0,0,0,1},{1,0,0,1,1,1,1},{0,0,1,0,0,1,0},{0,0,0,0,1,1,0},{1,0,0,1,1,0,0}, {0,1,0,0,1,0,0},{0,1,0,0,0,0,0},{0,0,0,1,1,1,1},{0,0,0,0,0,0,0},{0,0,0,0,1,0,0}
 };

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); 
TM1637Display scorboard(CLK,DIO);

int secilenSecenek = 1;
bool yukariButonaBasma = false;
bool asagiButonaBasma = false;
bool oyunubaslatma=false;
bool anaekranadon=false;
bool yeni_level=false;

int tahtaX = 20; 
int tahtaY = 5; 
int tahtahiz = 5; 
int tahtakonum = SCREEN_WIDTH / 2 - tahtaX / 2; 
int oncekipotdegeri;  
int isiksensordegeri; 
bool civcivhareketbaslat=false; 
int civcivboyut = 2; 
float civcivX = SCREEN_WIDTH / 2.0; 
float civcivY = SCREEN_HEIGHT - tahtaY - civcivboyut - 1; 
int civcivyonX = 1; 
int civcivyonY = -1; 
float civcivhiz = 1; 

int kopuksatir = 2; 
int kopuksutun = 3; 
int kopukX = 10; 
int kopukY = 5; 
int kopukmesafe = 2; 
int kalpsayisi=3;
int maxkalpsayisi=3;
int levelkopuksayisi=0;
int skorkopuksayisi=0;
bool kopukdurum[20][20];
int skor = 0; 
int level=1;
uint8_t digitPinsIn[] = {24,25,26,27,28,29,30};
uint8_t kalp[] = {kalp1,kalp2,kalp3};
uint16_t siyah= SSD1306_BLACK;
uint16_t beyaz= SSD1306_WHITE;
uint16_t kopukcivcivrengi=beyaz;
uint16_t arkaplanrengi=siyah;
bool obje=false;
int objeX;
int objeY;
int objeboyut=5;
int objehizi=1;
void setup() {

  pinMode(yukariButonu, INPUT_PULLUP);
  pinMode(asagiButonu, INPUT_PULLUP);
  pinMode(secimButonu, INPUT_PULLUP);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 ekranı başlatılamadı"));
    while (1);
  }
  display.clearDisplay();
  display.display(); 
  //display.clearDisplay(); // Ekranı temizleme
  scorboard.setBrightness(0x0f); 
  scorboard.showNumberDec(skor, false); 
  kalpledleriniYak();
  anaEkranGuncelleme(secilenSecenek);
  pinMode(pin1,OUTPUT);
  pinMode(SEGMENT_A, OUTPUT);
  pinMode(SEGMENT_B, OUTPUT);
  pinMode(SEGMENT_C, OUTPUT);
  pinMode(SEGMENT_D, OUTPUT);
  pinMode(SEGMENT_E, OUTPUT);
  pinMode(SEGMENT_F, OUTPUT);
  pinMode(SEGMENT_G, OUTPUT);
  pinMode(kalp1, OUTPUT);
  pinMode(kalp2, OUTPUT);
  pinMode(kalp3, OUTPUT);
  pinMode(tahtapot, INPUT);
  
  kopuksifirla(); 
}

void loop() {
  SEVENSEGMENT(skor);
  if(anaekran()==false){
    return;
  }
  else if(anaekranadon==true){
    display.display(); 
    display.clearDisplay(); 
    yukariButonaBasma = false;
    asagiButonaBasma = false;
    oyunubaslatma=false;
    anaekranadon=false;
    kalpsayisi=3;
    civcivhiz=1;
    level=1;
    skorkopuksayisi=0;
    kopuksifirla();
    kalpledleriniYak();
    anaEkranGuncelleme(secilenSecenek);
  }else if(yeni_level==true){
    display.display(); 
    display.clearDisplay();
    yeni_level=false;
    obje=false;
    kopuksifirla();
  }
  if(oyunubaslatma==true){
    oyuncizim(); 
    oncekipotdegeri=analogRead(A0);
    kopukcizim(); 
    objedusurme();
    civcivcizim(); 
    display.display(); 
    oyunguncelle(); 
    SEVENSEGMENT(skor);

  }
  if(level>5){
    display.clearDisplay(); 
    display.setTextSize(1); 
    display.setTextColor(WHITE); 
    display.setCursor(0, SCREEN_HEIGHT / 2 - 5); 
    display.println("OYUN BITTI"); 
    display.println("TEBRİKLER"); 
    display.setCursor(0, SCREEN_HEIGHT / 2 + 5); 
    display.print("Score: "); 
    display.println(skor); 
    display.display();  
    delay(3000);
    anaekranadon=true;
  }
  
}
bool anaekran(){
   
  if (digitalRead(yukariButonu) == HIGH) { 
    yukariButonaBasma= false;
  }else if (digitalRead(yukariButonu) == LOW && !yukariButonaBasma) { 
    secilenSecenek=1;
    anaEkranGuncelleme(secilenSecenek);
    delay(100);
  }

  if (digitalRead(asagiButonu) == HIGH) { 
    asagiButonaBasma = false;
  }else if (digitalRead(asagiButonu) == LOW && !asagiButonaBasma) { 
    secilenSecenek=2;
    anaEkranGuncelleme(secilenSecenek);
    delay(100);
  }

  if (digitalRead(secimButonu) == LOW) {
    
    if (secilenSecenek == 1) {
      display.clearDisplay(); 
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE); 
      display.setCursor(5, 30);
      display.println("Oyun baslatiliyor..."); 
      display.display(); 
      delay(1000); 
      oyunubaslatma=true;
    } 
    if(secilenSecenek==2) {
      display.clearDisplay();
      display.setTextSize(1); 
      display.setTextColor(SSD1306_WHITE); 
      display.setCursor(0, 5);
      display.println("     Oyunumuza\n\n   gosterdiginiz\n\n     ilgi icin \n\n    tesekkurler.");
      display.display();
      delay(1000); 
      oyunubaslatma=false;
    }
    delay(1000);
  }
  return oyunubaslatma;
}
void anaEkranGuncelleme(int secilen) {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30, 20);
  display.println("BASLAT");
  display.setCursor(30, 40); 
  display.println("CIKIS");
  display.setCursor(10, secilen * 20);
  display.write('>');
  display.display();
}

void oyuncizim() {
  display.clearDisplay(); 
  sensorilerenkdegis();
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, kopukcivcivrengi); 
  if(civcivhareketbaslat==true){
      tahtakonum = map(analogRead(A0), 0, 1023, 0, SCREEN_WIDTH - tahtaX);
  }
  else{
    tahtakonum = SCREEN_WIDTH/2-tahtaX/2; 
  }
  display.fillRect(tahtakonum, SCREEN_HEIGHT - tahtaY, tahtaX, tahtaY, kopukcivcivrengi); 
  kopukcizim(); 
  civcivcizim(); 
  scorboard.showNumberDec(skor, false); 
}

void kopukcizim() {
  for (int i = 0; i < kopuksatir; i++) {
    for (int j = 0; j < kopuksutun; j++) {
      if (kopukdurum[i][j]) {
        int kopukyonX = j * (kopukX + kopukmesafe);
        int kopukyonY = i * (kopukY + kopukmesafe);
        display.fillRect(kopukyonX, kopukyonY, kopukX, kopukY,kopukcivcivrengi); 
      }
    }
  }
}

void civcivcizim() {
  display.fillCircle(civcivX, civcivY, civcivboyut,kopukcivcivrengi); 
}

void oyunguncelle() { 
  int potdegeri = analogRead(tahtapot);
  
  if (oncekipotdegeri != potdegeri) {
    oncekipotdegeri = potdegeri;
    civcivhareketbaslat = true;
  }
  
  if (civcivhareketbaslat==true) {
    tahtakonum = map(potdegeri, 0, 1023, 0, SCREEN_WIDTH - tahtaX); 
    civcivX += civcivyonX * civcivhiz; 
    civcivY += civcivyonY * civcivhiz; 
    if (civcivX <= 0 || civcivX >= SCREEN_WIDTH - civcivboyut) {
      civcivyonX *= -1;
    }
    if (civcivY <= 0) {
      civcivyonY *= -1; 
    }
    if (civcivY >= SCREEN_HEIGHT - civcivboyut) {
      kalpsayisi--;
      kalpGuncelleSondurme(kalpsayisi);
      civcivX = SCREEN_WIDTH / 2; 
      civcivY = SCREEN_HEIGHT - tahtaY - civcivboyut - 1; 
      civcivhareketbaslat=false;
      civcivyonY = -1;
      if(kalpsayisi<=0){
          oyunbitti(); 
      }
     
    }
    if (civcivX >= tahtakonum && civcivX <= tahtakonum + tahtaX && civcivY >= SCREEN_HEIGHT - tahtaY - civcivboyut) {
      civcivyonY *= -1; 
      int tahtaOrta = tahtakonum + tahtaX / 2;
      int civcivOrta = civcivX + civcivboyut / 2;
      if (civcivOrta < tahtaOrta) {
          civcivyonX = -1; 
       }

      else if (civcivOrta > tahtaOrta) {
         civcivyonX = 1; 
      }

      else {
         civcivyonX = 0; 
      }
    }
    for (int i = 0; i < kopuksatir; i++) {
      for (int j = 0; j < kopuksutun; j++) {
        if (kopukdurum[i][j]) {
          int kopukyonX = j * (kopukX + kopukmesafe);
          int kopukyonY = i * (kopukY + kopukmesafe);
          if (civcivX + civcivboyut >= kopukyonX && civcivX <= kopukyonX + kopukX && civcivY + civcivboyut >= kopukyonY && civcivY <= kopukyonY + kopukY) {
            bool carptiSoldan = civcivX + civcivboyut <= kopukyonX;
            bool carptiSagdan = civcivX + civcivboyut >= kopukyonX + kopukX;
            bool carptiYukardan = civcivY + civcivboyut <= kopukyonY;
            bool carptiAsagidan = civcivY + civcivboyut >= kopukyonY + kopukY;
            
            if (carptiSoldan && civcivyonX > 0) {
              civcivyonX *= -1;
            }
            if (carptiSagdan && civcivyonX < 0) {
              civcivyonX *= -1;
            }
            if (carptiYukardan && civcivyonY > 0) {
              civcivyonY *= -1;
            }
            if (carptiAsagidan && civcivyonY < 0) {
              civcivyonY *= -1;
            }

            kopukdurum[i][j] = false; 
            skor++; 
            if (!obje && random(0, 10) == 0) {
               objeX = kopukyonX + kopukX / 2;
               objeY = kopukyonY + kopukY / 2;
               obje = true;
            }
            if (skor == skorkopuksayisi) {
              levelatla(); 
            }
          }
        }
      }
    }
  }
}

void oyunbitti() {
  display.clearDisplay(); 
  display.setTextSize(1); 
  display.setTextColor(WHITE); 
  display.setCursor(0, SCREEN_HEIGHT / 2 - 5); 
  display.println("Game Over!"); 
  display.setCursor(0, SCREEN_HEIGHT / 2 + 5); 
  display.print("Score: "); 
  display.println(skor); 
  display.display();  
  delay(3000);
  anaekranadon=true;
  skor=0;
  kopuksatir=2;
  kopuksutun=3;
}
void levelatla() {
  level+=1;
  civcivhiz=civcivhiz+civcivhiz*0.2;
  civcivX = SCREEN_WIDTH / 2; 
  civcivY = SCREEN_HEIGHT - tahtaY - civcivboyut - 1; 
  civcivhareketbaslat=false;
  civcivyonY = -1;
  display.clearDisplay(); 
  display.setTextSize(1); 
  display.setTextColor(WHITE); 
  display.setCursor(0, SCREEN_HEIGHT / 2 - 5);
  display.println("Level Basarili"); 
  display.setCursor(0, SCREEN_HEIGHT / 2 + 5); 
  display.print("Yeni Level: "); 
  display.println(level); 
  display.display();  
  delay(5000);
  yeni_level=true;
  kopuksatir+=3;
  kopuksutun+=3;
}

void kopuksifirla() {
  switch(level){
    case 1: {
      levelkopuksayisi=0;
      kopuksatir=4;
      kopuksutun=11;
      bool level1[4][11] = {
        {false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false},
        {false, false, true, true, true, false, true, true, true, false, false},
        {false, false, true, true, true, false, true, true, true, false, false}
      };
      for (int i = 0; i < kopuksatir; i++) {
        for (int j = 0; j < kopuksutun; j++) {
          kopukdurum[i][j] = level1[i][j];
          if(kopukdurum[i][j]==true){
              levelkopuksayisi++;
          }
        }
      } skorkopuksayisi+=levelkopuksayisi;
      break;
    }
    case 2: {
      levelkopuksayisi=0;
      kopuksatir=4;
      kopuksutun=11;
      bool level2[5][11] = {
        {false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false},
        {false, false, true, true, true, true, true, true, true, false, false},
        {false, false, true, true, true, true, true, true, true, false, false},
        {false, false, false, false, false, false, false, false, false, false}
      };
      for (int i = 0; i < kopuksatir; i++) {
        for (int j = 0; j < kopuksutun; j++) {
          kopukdurum[i][j] = level2[i][j];
          if(kopukdurum[i][j]==true){
              levelkopuksayisi++;
          }
        }
      } skorkopuksayisi+=levelkopuksayisi;
      break;
    }
    case 3:{
      kopuksatir=5;
      kopuksutun=11;
      levelkopuksayisi=0;
      bool level3[5][11] = {
        {false, false, false, false, false, false, false, false, false, false,false},
        {false, true, false, false, true, false, true, false, false, true, false},
        {false, false, true, true, false, true, false, true, true, false, false},
        {false, true, false, false, true, false, true, false, false, true, false},
        {false, false, true, true, false, true, false, true, true, false, false}
      };
      for (int i = 0; i < kopuksatir; i++) {
        for (int j = 0; j < kopuksutun; j++) {
          kopukdurum[i][j] = level3[i][j];
          if(kopukdurum[i][j]==true){
            levelkopuksayisi++;
          }
        }
      } skorkopuksayisi+=levelkopuksayisi;
      break;
    }
    case 4:{
      levelkopuksayisi=0;
      kopuksatir=7;
      kopuksutun=11;
      bool level4[7][11] = {
        
        {true, true, true, true, true, true, true,  true, false, true, true},
        {true, true, true, true, true, true, true, false, true, true, true},
        {true, true, true, true, true, true, false, true, true, true,true},
        {true, true, true , true, true, false, true, true, true, true,true},
        {true, true, true , true, false,true, true, true, true, true,true},
        {true, true, true , false, true, true, true, true, true, true,true},
        {true, true, false, true , true, true, true, true, true, true,true},
        
      };
      for (int i = 0; i < kopuksatir; i++) {
        for (int j = 0; j < kopuksutun; j++) {
          kopukdurum[i][j] = level4[i][j];
          if(kopukdurum[i][j]==true){
            levelkopuksayisi++;
          }
        }
      } skorkopuksayisi+=levelkopuksayisi;
      break;
    }
    case 5:{
      levelkopuksayisi=0;
      kopuksatir=8;
      kopuksutun=11;
      bool level5[8][11] = {
        
        {true, true, true, false, true, true, true,  false, true, true, true},
        {true, true, true, true, false, true, false, true, true, true, true},
        {true, true, true, true, true, false, true, true, true, true,true},
        {true, true, true, true, false, true, false, true, true, true,true},
        {true,  true, true,false, true, true, true, false,true, true, true},
        {true,  true,false, true, true, true, true, true, false,true, true},
        {true, false, true, true, true, true, true, true, true,false, true},
        {false, true, true, true, true, true, true, true, true, true,false},
        
      };
      for (int i = 0; i < kopuksatir; i++) {
        for (int j = 0; j < kopuksutun; j++) {
          kopukdurum[i][j] = level5[i][j];
          if(kopukdurum[i][j]==true){
            levelkopuksayisi++;
          }
        }
      } skorkopuksayisi+=levelkopuksayisi;
      break;
    }
  }
}
void SEVENSEGMENT(int skor){
  int birlerbasamagi=skor%10;
  digitalWrite(pin1, 1);
  skoruSegmentYaz(birlerbasamagi);
  
}
void skoruSegmentYaz(int deger){
   for (int i = 0; i < 7; i++) {
    digitalWrite(digitPinsIn[i], rakamlar[deger][i]); 
  }

}
void kalpledleriniYak(){
  digitalWrite(kalp1, HIGH);  
  digitalWrite(kalp2, HIGH); 
  digitalWrite(kalp3, HIGH); 
}
void kalpGuncelleSondurme(int kalpsayisi){
  for(int i=maxkalpsayisi - 1;i >= kalpsayisi; i--){
    digitalWrite(kalp[i],LOW);
  }
}
void kalpGuncelleYakma(int kalpsayisi){
  for(int i=0;i <kalpsayisi; i++){
    digitalWrite(kalp[i],HIGH);
  }
}
void objedusurme(){
  if(obje){
      display.fillRect(objeX, objeY, objeboyut,objeboyut, kopukcivcivrengi);
      objeY=objeY+objehizi;
      if(objeX>=tahtakonum&&objeX<=tahtakonum+tahtaX&&objeY >= SCREEN_HEIGHT - tahtaY - objeboyut) {
        obje=false;
        if(kalpsayisi<maxkalpsayisi){
           kalpsayisi++;
           kalpGuncelleYakma(kalpsayisi);
        }
        
      } 
      if(objeY >= SCREEN_HEIGHT - objeboyut){
         obje=false;
      }
    }
}
void sensorilerenkdegis(){
  isiksensordegeri=analogRead(A1);
  if(isiksensordegeri>250){
      arkaplanrengi=beyaz; 
      kopukcivcivrengi=siyah;
      display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, arkaplanrengi);
    } else {
      arkaplanrengi=siyah; 
      kopukcivcivrengi=beyaz;
      display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, arkaplanrengi);
    }
}