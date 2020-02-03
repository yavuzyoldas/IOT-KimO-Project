

#include <ESP8266WiFi.h>

/********* WiFi Access Point ***********/
#define WLAN_SSID "Yavuz"
#define WLAN_PASS "xvgt1894"

#define SERVER "api.pushetta.com"
String API("0fba37cfabcd6ea013c339720246c4c5763c6584");
String CHANNEL("KimO");


int PIR = D8;//2 nolu pini PIR olarak atadık
int durum=0;

const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum


void wifiSetup()
{
 
 // Connect to WiFi access point.
 Serial.println(); Serial.println();
 Serial.print("Connecting to ");
 Serial.println(WLAN_SSID);
 WiFi.begin(WLAN_SSID,WLAN_PASS);
 while (WiFi.status()!= WL_CONNECTED)
 {
 delay(500);
 Serial.print(".");
 }
 Serial.println();
 Serial.println("WiFi connected");
 Serial.println("IP address: "); 
 Serial.println(WiFi.localIP());
  
  
}


void setup()
{
  Serial.begin(115200);
  delay(10);
  wifiSetup();
  Serial.println("AT");
  delay(5000);

  if(Serial.find("OK")){         //esp modülü ile bağlantıyı kurabilmişsek modül "AT" komutuna "OK" komutu ile geri dönüş yapıyor.
     Serial.println("AT+CWMODE=1"); //esp modülümüzün WiFi modunu STA şekline getiriyoruz. Bu mod ile modülümüz başka ağlara bağlanabilecek.
     delay(2000);
     String baglantiKomutu=String("AT+CWJAP=\"")+WLAN_SSID+"\",\""+WLAN_PASS+"\"";
    Serial.println(baglantiKomutu);
    
     delay(5000);
 }
 }
  
 void loop()
 {
  durum = digitalRead(PIR);//PIR içindeki durumu "durum" değikenine atadık.
  Serial.println(durum);//"durum" değişkenin değerini seri porttan görebilmemiz için
  delay(1000);

  int sensorOkuma = analogRead(A0);
   int deger= map(sensorOkuma, sensorMin, sensorMax, 0, 2);
 switch (deger) {
 case 0:    
    Serial.println("ışık");
    break;
 case 1:    
    Serial.println("karanlik");
    break;
  }

   if (deger == 0 && durum == 1){  
    veriPushetta(CHANNEL,"HIRSIZ VAR!");
                         
  }

 }

 void veriPushetta(String CHANNEL,String text) {

  Serial.println(String("AT+CIPSTART=\"TCP\",\"") + SERVER + "\",80"); //server'a bağlanma
  
  delay(3000);
 String   deneme =("POST /api/pushes/");
    deneme+= (CHANNEL);
    deneme+= ("/ HTTP/1.1");
    deneme+= ("\r\n");
    deneme+= ("Host: ");
    deneme+= (SERVER);
    deneme+= ("\r\n");
    deneme+= ("Authorization: Token ");
    deneme+= (API);
    deneme+= ("\r\n");
    deneme+= ("Content-Type: application/json");
    deneme+= ("\r\n");
    deneme+= ("Content-Length: ");
    deneme+= (text.length() + 46);
    deneme+= ("\r\n\r\n");
    deneme+= ("{ \"body\" : \"");
    deneme+= (text);
    deneme+= ("\", \"message_type\" : \"text/plain\" }");
    deneme+= ("\r\n\r\n");
    
    Serial.print("AT+CIPSEND=");
 delay(100);
 Serial.println(deneme.length());
 // Sorun oluşmadıysa veriyi gönder.
 if(Serial.find(">")){
 Serial.println(deneme);
 Serial.println("AT+CIPCLOSE=0");
 delay(100);

 }
 else{
 Serial.println("AT+CIPCLOSE=0");
 }
    
  }
