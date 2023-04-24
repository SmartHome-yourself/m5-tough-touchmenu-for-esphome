# m5-tough-touchmenu-for-esphome  
  
## Minimale Konfiguration
Dieses Beispiel zeigt das absolute Minimum der ESPHome Konfiguration für zwei Buttons.  
In dieser Variante steht für den Tastendruck nur der Wert des Textsensor zur Verfügung.  
Automatisierungen lassen sich damit natürlich auch realisieren.  

Die Standard Binär-Sensoren für "Angemeldet", "Display an" und der Sensor für die Anzahl fehlgeschlagener Logins seit letzter erfolgreicher Anmeldung stehen auch hierbei zur Verfügung.  
```
substitutions:
  name: shys-m5-tough-123456
  friendly_name: "M5 Tough"
  m5_textsensor_id: "m5_textsensor_id"

packages:
  smarthomeyourself.barcodescanner: github://SmartHome-yourself/m5-tough-touchmenu-for-esphome/shys-m5-tough-touchmenu.yaml@main

esphome:
  name: ${name}
  name_add_mac_suffix: false

api:

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

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

  
## Vollständige Konfiguration mit Binary-Sensoren für die einzelnen Taster des Touchscreen-Menüs
In diesem Beispiel wird für jeden Button zusätzlich noch ein eigener Binär-Sensor angelegt.  
Diese Sensoren werden über "on_value" am Text-Sensor aktualisiert.  
  
Die Standard Binär-Sensoren für "Angemeldet", "Display an" und der Sensor für die Anzahl fehlgeschlagener Logins seit letzter erfolgreicher Anmeldung stehen auch hierbei zur Verfügung.  

```
substitutions:
  name: shys-m5-tough-123456
  friendly_name: "M5 Tough"
  m5_textsensor_id: "m5_textsensor_id"

packages:
  smarthomeyourself.barcodescanner: github://SmartHome-yourself/m5-tough-touchmenu-for-esphome/shys-m5-tough-touchmenu.yaml@main

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
    init_sound: on
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
