# Arduino-BrickBreakingGame

The aim of this project is to develop a microcontroller-based brick-breaking game using various tools.

The game machine has been created on Wokwi. 

You can use the following link to test the game machine: [https://wokwi.com/projects/395809465315796993](https://wokwi.com/projects/405017537183784961) 

**Projeyi Geliştirenler**

- [Ceylin ATLI](https://github.com/ceylinatli)
- [Zehra KARABEKTAŞ](https://github.com/zehrakarabektas)
  
**Oyun Hakkında Bilgi:**

Bu projenin amacı, çeşitli araçlar kullanarak mikrodenetleyici tabanlı bir oyun makinesi  geliştirmektir.

Oyun makinesi C dili ile yazılmış olup Wokwi'de geliştirilmiştir. 

Oyun makinesini denemek için bağlantıyı kullanabilirsiniz:[ https://wokwi.com/projects/395809465315796993](https://wokwi.com/projects/405017537183784961)

**Koddaki değişkenler:**

- **'kopuk' (Tuğla):** Oyuncunun kırması gereken tuğlayı ifade eder.

- **'civciv' (Top):** Etrafında hareket eden ve tuğlalara çarpan topu ifade eder.

- **'tahta'(Palet):** Topu yönlendirmek ve hareketini kontrol etmek için kullanılan bir paleti ifade eder.

- **'kalp':** Oyuncunun canlarını temsil eden simgeler veya nesneler. Bu genellikle oyuncunun oyun içindeki kalan can sayısını ifade eder.

### A. Devre Tasarımında Kullanılan Araçlar (Simülasyon)

Bu projede kullanılan devrede:
- Arduino Mega 2560
- 128x64 OLED ekran (oyun yansıtması için)
- Sliding variable resistor (lineer potansiyometre) (palet kontrol cihazı olarak)
- 7 Segment Display (skor göstergesi için)
- Üç buton (başlat, çıkış ve seçim kontrolleri için)
- Üç LED (oyuncunun can göstergesi için)
- Işık sensörü (objelerin rengini değiştirmek için)
- Dirençler ve bağlantı kabloları kullanılmıştır.

### B. Devre Tasarımında Oluşturulan Bağlantılar

- **OLED Ekran Bağlantıları:**
  - VCC (Güç) pini Arduino Mega 2560'ın 5V pinine
  - GND (Toprak) pini Arduino Mega 2560'ın GND pinine
  - SCL (Serbest Clock Hattı) pini Arduino Mega 2560'ın SCL pinine
  - SDA (Serbest Dağıtım Hattı) pini Arduino Mega 2560'ın SDA pinine

  Bu bağlantılar, I2C protokolü üzerinden iletişim sağlamak amacıyla yapılmıştır, böylece Arduino Mega OLED ekranı kontrol edebilir.

- **Işık Sensörü (Fotoresistor) Bağlantıları:**
  - VCC pini Arduino Mega 2560'ın 5V pinine
  - GND pini Arduino Mega 2560'ın GND pinine
  - Analog pin (A0) Arduino Mega 2560'ın A1 pinine

- **Potansiyometre Bağlantıları:**
  - VCC pini Arduino Mega 2560'ın 5V pinine
  - GND pini Arduino Mega 2560'ın GND pinine
  - SIG pini Arduino Mega 2560'ın A0 pinine

- **LED Bağlantıları:**
  - Anot pinleri Arduino Mega 2560'ın dijital pinleri 31, 32, 33'e
  - Katot pinleri dirençlere ve Arduino Mega 2560'ın GND pinine

- **Buton Bağlantıları:**
  - İlk kontak sol pinleri Arduino Mega 2560'ın GND pinine
  - İkinci kontak sol pinleri Arduino Mega 2560'ın dijital pinleri 42, 43, 44'e
### C. Kullanılan Kütüphaneler

- `Adafruit_GFX.h`: Grafik nesnelerini çizmek için kullanılmıştır. Metin, çizgi, daire, dikdörtgen gibi çeşitli grafik nesnelerini çizmek için fonksiyonlar sağlar.
- `Adafruit_SSD1306.h`: Çeşitli ekran denetleyicileriyle uyumlu ekranlarda kullanılabilir ve OLED ekranlarda metin, şekiller ve görüntüler için fonksiyonlar sağlar.
- `TM1637Display.h`: Seven Segment Display LED ekranlarının kontrolü için kullanılmıştır.

### D. Simülasyonun Çalışma Prensibi

Proje ilk çalıştırıldığında, 'Başlat' ve 'Çıkış' seçeneklerini içeren bir menü ekranı gösterilir. Oyuncu butonlarla aşağı yukarı hareket edebilir ve seçim butonuyla seçenekler arasında geçiş yapabilir.

- **Başlat Butonu**: Oyunu başlatır ve ekranda 'Oyun Başlatılıyor...' yazısını gösterir.
- **Çıkış Butonu**: Oyunu bitirir ve ekranda 'Oyunumuza gösterdiğiniz ilgi için teşekkürler.' yazısını gösterir.

Oyun başladığında, top ve palet ortada yer alır. Top hareketi, potansiyometreden okunan değer değiştiğinde başlar. Top her tuğlaya çarptığında skor değişkeni bir artar ve Seven Segment Display'de gösterilir.

Oyuncunun canları üç adet LED ile gösterilir. Başlangıçta tüm LED'ler 'HIGH' durumundadır. Eğer top paleti geçerse, LED'ler 'LOW' durumuna getirilir. Üç can bitince, Seven Segment Display'de yazılan skor 3 saniye boyunca ekranda gösterilir ve oyun başlat-çıkış ekranına dönülür.

Oyun ekranı temizlenir ve 'Oyun Başlatılıyor...' yazısı bir süre ekranda gösterildikten sonra oyun ekranı OLED ekranda görüntülenir. Oyuncunun maksimum 3 canı vardır ve bu canlar LED'lerle gösterilir. Oyuncu can kaybettiğinde LED'lerin durumu güncellenir. Oyuncu %10 olasılıkla düşen objeleri yakalarsa can sayısı artırılabilir (maksimum 3).

Her oyun yeniden başlatıldığında veya seviye geçildiğinde top ve palet ortada ayarlanır ve topun hareketi potansiyometreden okunan değerle başlatılır. Top paletin soluna çarptığında sol çapraza, sağa çarptığında sağ çapraza, ortasına çarptığında ise düz ilerleyecek şekilde ayarlanmıştır. Topun hareketi oyuncunun yönlendirmesine bırakılmıştır ve bu kontroller ‘oyunGuncelle()’ fonksiyonunda sağlanmıştır. Oyuncu can kaybettiğinde seviye kaldığı yerden devam eder. Oyundaki tüm tuğlalar bittiğinde 5 saniyelik bir ara ekran gösterilir, ardından diğer seviye başlar. Her seviyedeki tuğla dizaynı farklıdır ve seviye geçildiğinde topun hızı %20 arttırılır. LDR'nin analog değerleri ile ekranın renkleri kontrol edilir: değer 250'nin altındaysa arka plan beyaz, tuğlalar siyah yapılır; değer 250'nin üstündeyse arka plan siyah, tuğlalar beyaz yapılmıştır.
