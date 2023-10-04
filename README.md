# m5-tough-touchmenu-for-esphome  
Dieses Projekt bietet ein frei konfigurierbares Touch-Menü für das M5Stack Tough.  
![image](https://github.com/SmartHome-yourself/m5-tough-touchmenu-for-esphome/assets/705724/436d5e05-b337-465f-96b5-c69d2b321631)

  
Über die ESPHome Konfiguration wird ein Text-Sensor vom Typ `shys_m5_tough` angelegt und über das Attribut `buttons` die gewünschten Schaltflächen angegeben.  
Der Textsensor liefert alle Informationen zum Status und eventuellen Touch-Events.  

Der Touchscreen sperrt sich zur Sicherheit standardmäßig nach einigen Sekunden und muss anschließend zuerst über eine PIN wieder freigegeben werden. Die Standard PIN ist: 12345  
Diese PIN kann über einen Dienst jeder Zeit neu gesetzt werden.
----

## Minimale Konfiguration
Dieses Beispiel zeigt das absolute Minimum der ESPHome Konfiguration für zwei Buttons.  
In dieser Variante steht für den Tastendruck nur der Wert des Textsensor zur Verfügung.  
Automatisierungen lassen sich damit natürlich auch realisieren.  

Die Standard Binär-Sensoren für "Angemeldet", "Display an" und der Sensor für die Anzahl fehlgeschlagener Logins seit letzter erfolgreicher Anmeldung stehen immer zur Verfügung.  
  
```
substitutions:
  name: shys-m5-tough-123456
  friendly_name: "M5 Tough"
  m5_textsensor_id: "m5_textsensor_id"
  wifi_ssid: !secret wifi_ssid
  wifi_password: !secret wifi_password

packages:
  smarthomeyourself.m5_tough: 
    url: https://github.com/SmartHome-yourself/m5-tough-touchmenu-for-esphome
    file: shys-m5-tough-touchmenu.yaml

esphome:
  name: ${name}
  name_add_mac_suffix: false

api:

text_sensor:  
  - platform: shys_m5_tough
    name: "M5 Tough Status"
    id: m5_textsensor_id
    init_sound: off
    login_enabled: on

    buttons:
      - x: 10
        y: 10
        width: 220
        height: 150
        text: "Carport"

      - x: 10
        y: 160
        width: 220
        height: 150
        text: "Garage"
```
  
  
  
# Konfiguration
In den Substitutions sollten `name`, `friendly_name` und `m5_textsensor_id` angegeben werden.  
  
## Alle zur Verfügung stehenden Parameter (Substitutions):

```
substitutions:
  name: "m5-tough"
  friendly_name: "M5 Tough"
  m5_textsensor_id: "m5_textsensor_id"
```
  
### name
*default: m5-tough*  
Der Hostname des Geräts.  
  
### friendly_name
*default: M5 Tough*  
Der Name, der im Frontend angezeigt wird.  
  
### m5_textsensor_id
*default: m5_textsensor_id*  
Gibt die ID des Textsensors vom Platform-Typ `shys_m5_tough` an. 
  
### wifi_ssid
*default: !secret wifi_ssid*  
Gibt den Namen des WLAN an, in das verbunden werden soll
  
### wifi_password
*default: !secret wifi_password*  
Gibt das WLAN-Passwort an

  
&nbsp;  
  
## Text-Sensor
**Beispiel:**
```
text_sensor:  
  - platform: shys_m5_tough
    name: "Mein M5-Tough"
    id: m5_textsensor_id
    init_sound: off
    login_enabled: on

    buttons:
      - x: 1
        y: 1
        width: 100
        height: 100
        text: "Mein Touchfeld"
        fontsize: 4
        color: "TFT_BLUE"
        textcolor: "TFT_WHITE"
        bgcolor: "TFT_DARKGREY"
```
### platform
Die Platform des Textsensors muss `shys_m5_tough` sein.
### name
Der Name des Textsensor kann frei gewählt werden.
### id
Die ID sollte `m5_textsensor_id` sein. (Ansonsten sollte diese mit der Substitution "m5_textsensor_id" übereinstimmen)
### init_sound
Gibt an, ob beim Einschalten eine kurze Melodie abgespielt werden soll.  
*Gültige Werte: on/off*
### login_enabled
Hier lässt sich der Login an/abschalten.  
Bei aktiviertem Login wird das Display automatisch nach einigen Sekunden inaktivität wieder gesperrt.
*Gültige Werte: on/off*
### buttons:
Das Attribut `buttons` ist eine Liste, in der die einzelnen Schaltflächen für das Touchmenü hinterlegt werden können.  
Es können beliebig viele Schaltflächen angelegt werden.  
  
**x**  
Gibt die X-Koordinate der Schaltfläche an  
**y**  
Gibt die Y-Koordinate der Schaltfläche an  
**width**  
Gibt die Breite der Schaltfläche an  
**height**  
Gibt die Höhe der Schaltfläche an  
**text**  
Gibt die Beschriftung der Schaltfläche an.  
Dieser Text wird auch im Textsensor beim drücken der Schaltfläche mit ausgegeben, um auswerten zu können, welches Element gedrückt wurde.  
**fontsize**  
Legt die Schriftgröße des Textes auf dem Button fest.  
**color**  
Legt die Rahmen-Farbe des Button fest.  
**textcolor**  
Legt die Schriftfarbe des Textes auf dem Button fest.  
**bgcolor**  
Legt die Hintergrundfarbe des Button fest.  




  
## Farben
Hier eine Liste der Möglichen Farbwerte:
TFT_BLACK      /*   0,   0,   0 */  
TFT_NAVY      /*   0,   0, 128 */  
TFT_DARKGREEN      /*   0, 128,   0 */  
TFT_DARKCYAN      /*   0, 128, 128 */  
TFT_MAROON      /* 128,   0,   0 */  
TFT_PURPLE      /* 128,   0, 128 */  
TFT_OLIVE      /* 128, 128,   0 */  
TFT_LIGHTGREY      /* 211, 211, 211 */  
TFT_DARKGREY      /* 128, 128, 128 */  
TFT_BLUE      /*   0,   0, 255 */  
TFT_GREEN      /*   0, 255,   0 */  
TFT_CYAN      /*   0, 255, 255 */  
TFT_RED      /* 255,   0,   0 */  
TFT_MAGENTA      /* 255,   0, 255 */  
TFT_YELLOW      /* 255, 255,   0 */  
TFT_WHITE      /* 255, 255, 255 */  
TFT_ORANGE      /* 255, 180,   0 */  
TFT_GREENYELLOW      /* 180, 255,   0 */  
TFT_PINK      /* 255, 192, 203 */  
TFT_BROWN      /* 150,  75,   0 */  
TFT_GOLD      /* 255, 215,   0 */  
TFT_SILVER      /* 192, 192, 192 */  
TFT_SKYBLUE      /* 135, 206, 235 */  
TFT_VIOLET      /* 180,  46, 226 */  
  
-----    
    
# Dienste
##  esphome.\<name\>_setnewpassword
Ein Dienst, über den aus Home Assistant heraus das Passwort zum Entsperren des Displays festgelegt werden kann.  

### Attribute
**new_password** 

  
##  esphome.\<name\>_showcurrentpassword
Gibt das aktuelle Passwort im Log über die Serielle Konsole aus  

  
&nbsp;    
  
-----
  
&nbsp;  
  
# Zusätzlichen Binary-Sensoren für jede Touchfläche
In diesem Beispiel wird für jeden Button zusätzlich je ein eigener Binär-Sensor angelegt.  
Diese Sensoren werden dann über die "on_value"-Aktion am Text-Sensor aktualisiert.  
  
Die Standard Binär-Sensoren für "Angemeldet", "Display an" und der Sensor für die Anzahl fehlgeschlagener Logins seit letzter erfolgreicher Anmeldung stehen auch hierbei zur Verfügung.  

```
substitutions:
  name: shys-m5-tough-123456
  friendly_name: "M5 Tough"
  m5_textsensor_id: "m5_textsensor_id"

packages:
  smarthomeyourself.m5_tough: 
    url: https://github.com/SmartHome-yourself/m5-tough-touchmenu-for-esphome
    file: shys-m5-tough-touchmenu.yaml

esphome:
  name: ${name}
  name_add_mac_suffix: false

api:

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password


binary_sensor:
  - platform: template
    name: "Motorrad-Garagentor Taster"
    id: motorrad_tor_taster

  - platform: template
    name: "Carport-Tor Taster"
    id: carport_tor_taster

text_sensor:  
  - platform: shys_m5_tough
    name: "M5 Tough Status"
    id: m5_textsensor_id
    init_sound: off
    login_enabled: on

    buttons:
      - x: 10
        y: 10
        width: 220
        height: 150
        text: "Carport"
        fontsize: 4
        textcolor: "TFT_WHITE"

      - x: 10
        y: 160
        width: 220
        height: 150
        text: "Garage"
        fontsize: 4
        color: "TFT_BLUE"
        textcolor: "TFT_WHITE"
        bgcolor: "TFT_DARKGREY"

    on_value:
      then:
        - if:
            condition:
              - text_sensor.state:
                  id: m5_textsensor_id
                  state: "Button: Carport"
            then:
              - binary_sensor.template.publish:
                  id: carport_tor_taster
                  state: ON
              - delay: 2s
              - binary_sensor.template.publish:
                  id: carport_tor_taster
                  state: OFF

        - if:
            condition:
              - text_sensor.state:
                  id: m5_textsensor_id
                  state: "Button: Garage"
            then:
              - binary_sensor.template.publish:
                  id: motorrad_tor_taster
                  state: ON
              - delay: 2s
              - binary_sensor.template.publish:
                  id: motorrad_tor_taster
                  state: OFF
```
