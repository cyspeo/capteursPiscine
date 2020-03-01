#include "config.h"
#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN D4     // Pin sur lequel est branché le DHT
#define DHTTYPE DHT22 // Type de DHT
DHT dht(DHTPIN, DHTTYPE);

#include <OneWire.h>
#include <DallasTemperature.h>

#define SMTP_PORT 587
#define ONE_WIRE_PIN 5
#define SLEEP_TIME (30 * 60) // 30 minutes

enum rst_reason
{
  REASON_DEFAULT_RST = 0,      /* normal startup by power on */
  REASON_WDT_RST = 1,          /* hardware watch dog reset */
  REASON_EXCEPTION_RST = 2,    /* exception reset, GPIO status won’t change */
  REASON_SOFT_WDT_RST = 3,     /* software watch dog reset, GPIO status won’t change */
  REASON_SOFT_RESTART = 4,     /* software restart ,system_restart */
  REASON_DEEP_SLEEP_AWAKE = 5, /* wake up from deep-sleep */
  REASON_EXT_SYS_RST = 6       /* external system reset */
};

struct rst_info
{
  uint32 reason;
  uint32 exccause;
  uint32 epc1;
  uint32 epc2;
  uint32 epc3;
  uint32 excvaddr;
  uint32 depc;
};



WiFiClient client;
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature DS18B20(&oneWire);
DeviceAdress = {0x28,  0x95,  0x31,  0x79,  0xA2,  0x19,  0x3,  0x33}

#define NB_TRYWIFI 10    // nbr d'essai connexion WiFi, number of try to connect WiFi
bool debug = false;      //Affiche sur la console si True

void setup_DHT22()
{
  dht.begin();
}

void setup_DS18B20()
{
}

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connexion a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connexion WiFi etablie ");
  Serial.print("=> Addresse IP : ");
  Serial.print(WiFi.localIP());
}

getDHTTemperature()
{
  // Lecture de la température en Celcius
  float t = dht.readTemperature();
  //Inutile d'aller plus loin si le capteur ne renvoi rien
  if (isnan(t))
  {
    Serial.println("Echec de lecture ! Verifiez votre capteur DHT");
    return;
  }
  if (debug)
  {
    Serial.print("Temperature : ");
    Serial.print(t);
  }
}

getDHTHumidite()
{
  //Lecture de l'humidité ambiante
  float h = dht.readHumidity();
  //Inutile d'aller plus loin si le capteur ne renvoi rien
  if (isnan(h))
  {
    Serial.println("Echec de lecture ! Verifiez votre capteur DHT");
    return;
  }
  if (debug)
  {
    Serial.print(" | Humidite : ");
    Serial.println(h);
  }
}

float getTemperatureDS18B20()
{
  float tempC;
  do
  {
    DS18B20.requestTemperatures();
    tempC = DS18B20.getTempCByIndex(0);
    delay(100);
  } while (tempC == 85.0 || tempC == (-127.0));
  return tempC;
}

void setup()
{
  rst_info *resetInfo;
  byte ret;
  Serial.begin(115200);
  resetInfo = ESP.getResetInfoPtr();
  Serial.print("Wakeup reason: ");
  Serial.println((*resetInfo).reason);

  setup_wifi()

      // Analyse reason wake up
      switch (resetInfo->reason)
  {
  case REASON_DEFAULT_RST:
    ret = sendEmail("**** Just Started ****");
    break;
  case REASON_WDT_RST:
    ret = sendEmail("**** Watchdog Reset ****");
    break;
  case REASON_EXCEPTION_RST:
    ret = sendEmail("**** Exception Reset ****");
    break;
  case REASON_SOFT_WDT_RST:
    ret = sendEmail("**** Software Watchdog Reset ****");
    break;
  case REASON_SOFT_RESTART:
    ret = sendEmail("**** Software Reset ****");
    break;
  case REASON_DEEP_SLEEP_AWAKE:
    if (motionPin1)
    {
      ret = sendEmail("**** Motion1 Detected ****");
    }
    if (motionPin2)
    {
      ret = sendEmail("**** Motion2 Detected ****");
    }
    if (!motionPin1 && !motionPin2)
    {
      char strValue[6];
      char s[32];
      dtostrf(getDHTTemperature(), 5, 2, strValue);
      sprintf(s, "**** temperture is %s ****", strValue);
      dtostrf(getDHTHumidite(), 5, 2, strValue);
      sprintf(s, "**** Humidite is %s ****", strValue);
      dtostrf(getTemperatureDS18B20(), 5, 2, strValue);
      sprintf(s, "**** Humidite is %s ****", strValue);
      ret = sendEmail(s);
    }
    break;
  case REASON_EXT_SYS_RST:
    ret = sendEmail("**** System Reset ****");
    break;
  }

  Serial.print("Going into deep sleep for ");
  Serial.print(SLEEP_TIME);
  Serial.println(" seconds");
  ESP.deepSleep(1000000L * SLEEP_TIME);
}

void loop()
{
}

byte sendEmail(const char *data)
{
  byte thisByte = 0;
  byte respCode;

  if (client.connect(server, SMTP_PORT) == 1)
  {
    Serial.println(F("connected"));
  }
  else
  {
    Serial.println(F("connection failed"));
    return 0;
  }
  if (!recv())
    return 0;

  Serial.println(F("Sending HELLO"));
  client.println("EHLO www.example.com");
  if (!recv())
    return 0;
  Serial.println(F("Sending auth login"));
  client.println("auth login");
  if (!recv())
    return 0;
  Serial.println(F("Sending User"));
  client.println(userID);
  if (!recv())
    return 0;
  Serial.println(F("Sending Password"));
  client.println(userPWD);
  if (!recv())
    return 0;
  Serial.print(F("Sending From "));
  Serial.println(sender);
  client.print(F("MAIL From: "));
  client.println(sender);
  if (!recv())
    return 0;
  Serial.print(F("Sending To "));
  Serial.println(recipient);
  client.print(F("RCPT To: "));
  client.println(recipient);
  if (!recv())
    return 0;
  Serial.println(F("Sending DATA"));
  client.println(F("DATA"));
  if (!recv())
    return 0;
  Serial.println(F("Sending email"));
  client.print(F("To: "));
  client.println(recipient);
  client.print(F("From: "));
  client.println(sender);
  client.println(F("Subject: My first Email from ESp8266\r\n"));
  client.print(F("From ESP8266 N° "));
  client.println(ESP.getChipId(), HEX);
  Serial.println(data);
  client.println(data);
  client.println(F("."));
  if (!recv())
    return 0;
  Serial.println(F("Sending QUIT"));
  client.println(F("QUIT"));
  if (!recv())
    return 0;
  client.stop();
  Serial.println(F("disconnected"));
  return 1;
}

byte recv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;
  while (!client.available())
  {
    delay(1);
    loopCount++;
    if (loopCount > 10000)
    {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }
  respCode = client.peek();
  while (client.available())
  {
    thisByte = client.read();
    Serial.write(thisByte);
  }
  if (respCode >= '4')
  {
    return 0;
  }
  return 1;
}