# Module

Objectif, faire un module avec le minimum nécessaire pour faire un capteur de temperature de l'eau de la piscine ainsi que la temperature et humidite de l'exterieur le tout sur batterie.

## Specification

Le microcontroleur est en sommeil profond pendant 10 minutes.
A chaque reveil : 
- Il initialise la communication wifi
- Il récupère les vaeurs des capteurs 
- Il envoie une requette HTTP à un serveur maison (ma box) surement équipé d'un module rednode.
- Puis il s'endort à nouveau 

## Liens
http://riton-duino.blogspot.com/2019/02/esp8266-sur-batterie.html

https://community.home-assistant.io/t/outdoor-temperature-humidity-sensor-with-an-esp32-and-esphomeyaml/58147

# BOM
- Wemos D1 mini
- pile CR123A
- capteur de temperateur water proof : DS18B20
- capteur de temperature et humidité : DHT22
- PCB a pastille
- un support de pile (récupéré)

# Alimentation
Batterie ou pile CR123A
voir article : https://faire-ca-soi-meme.fr/domotique/2017/01/20/capteur-temperature-humidite-wifi-sur-pile-a-moins-de-10-euros-hardware/


# DEEP SLEEP
Pour utiliser le deep-sleep, il faut relier la broche RST à D0.
Attention, lorsqu'elle sont relier elle empêche tout téléversement de programme. Donc prévoir un jumper.

Doc : https://projetsdiy.fr/esp8266-test-mode-deep-sleep-reveil-wakeup-detecteur-mouvement-pir/#Differents_modes_de_veille_et_consommation_d8217un_module_ESP8266EX

# CAPTEUR DE TEMPERATURE
voir https://projetsdiy.fr/esp8266-dht22-mqtt-projet-objet-connecte/

- librairie DHT.h pour le capteur DHT22.
```
#include "DHT.h"          // Librairie des capteurs DHT
#define DHTPIN D4    // Pin sur lequel est branché le DHT
#define DHTTYPE DHT22         // DHT 22  (AM2302)

//Création des objets
DHT dht(DHTPIN, DHTTYPE);

void setup() {
   dht.begin();
}
void loop() {
   //Lecture de l'humidité ambiante
    float h = dht.readHumidity();
    // Lecture de la température en Celcius
    float t = dht.readTemperature();
    //Inutile d'aller plus loin si le capteur ne renvoi rien
    if ( isnan(t) || isnan(h)) {
      Serial.println("Echec de lecture ! Verifiez votre capteur DHT");
      return;
    }
}
```
# Capteur DS18B20 (temperature water proof)
voir : https://projetsdiy.fr/mesure-temperature-dallas-ds18b20-code-arduino-compatible-esp8266-esp32-domoticz-http/

