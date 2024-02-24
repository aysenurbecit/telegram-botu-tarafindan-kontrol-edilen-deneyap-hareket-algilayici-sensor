#include <deneyap.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Deneyap_HareketAlgilama.h>              

const char* ssid = "baglanilan WiFi adi";
const char* password = "baglanilan WiFi sifresi";

//int hareket;

#define BOTtoken "Buraya Token Yaz"  // botfather ile olusturulan botun ID'si " " icine girilir.
#define CHAT_ID "Buraya Chat Id yaz" // ogrenilen chat ID " " icine girilir

const unsigned long BOT_MTBS = 1000; 
unsigned long bot_lasttime; 

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

Gesture Hareket;       


int kontrol(){
  bool gestureDurum = Hareket.readGesture();
  if (gestureDurum)
    return 0;
  else
    return 1;
}

void handleNewMessages(int numNewMessages) {//burada komutlar yazildiginda botun yapmasini istedigimiz eylemleri acikliyoruz

  for (int i = 0; i < numNewMessages; i++){
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

  if (text == "/aktif") {
    if(kontrol()){
      bot.sendMessage(chat_id, "Aktif mod aktiflestirildi. Hareket algilandiginda mesaj gonderilecek.");
     // bot.sendMessage(CHAT_ID,"Hareket Algılandı.");
    }
  }
  if (text == "/pasif") {
    if(kontrol()){
      bot.sendMessage(chat_id, "Pasif mod aktiflestirildi. Artik hareket algilandiginda mesaj gonderilmeyecek.");
    }
  }
  if (text == "/start") {
    if(kontrol()){
    String giris = "Hosgeldin\n";
    giris += "/aktif : Algilamayi yazma acik\n";
    giris += "/pasif : Algilamayi yazma kapali\n";
    bot.sendMessage(chat_id, giris + "Tamamlandi.");
    }
  }
  }
}

void setup() {
    Serial.begin(115200);                              
    if (!Hareket.begin(0x32)) {                      
        delay(3000);
        Serial.println("I2C bağlantısı başarısız ");    
        while (1);
    }
  
  client.setInsecure();
  Serial.print("İnternete bağlanıyor: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi bağlandı");
  Serial.print("IP adresi: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(CHAT_ID, "Cihaz aktif.");
}

void loop(){
 
  if (millis() - bot_lasttime > BOT_MTBS){ //son gelen mesaji kontrol eder
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages){
      handleNewMessages(numNewMessages); //yeni mesaj varsa fonksiyon cagrilir
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_lasttime = millis();
  }


  if (kontrol()) {
    String text = bot.messages[0].text;
    if (text != "/pasif") {
      bot.sendMessage(CHAT_ID, "Hareket Algılandı.");
    }
  }
