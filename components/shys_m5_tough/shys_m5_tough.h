#pragma once
#include "esphome.h"
#include "esp_log.h"

#undef millis
#undef micros
#undef delayMicroseconds
#undef delay

#include "M5Unified.h"

#include "EEPROM.h"

#define debug_output false

#define ss 18
#define rst 14
#define dio0 26

#define SYNC_WORD 0xA3

#define EEPROM_SIZE 32

namespace esphome
{
  namespace shys_m5_tough
  {

    class ShysM5Tough : public text_sensor::TextSensor, public Component, public esphome::api::CustomAPIDevice
    {
    protected:
      bool init_sound_enabled = false;
      bool login_enabled = true;

      String tmpPwInfo = "";

      String myPasswd = "12345";
      int timeToLock = 20000;
      int timeToScreenOff = 30000;
      int maxVolume = 30;

      enum Pages
      {
        PASSWORD_PAGE,
        BUTTON_PAGE,
        SCREEN_OFF,
        OK,
        FAILURE
      };

      Pages currentPage = PASSWORD_PAGE;
      boolean loggedIn = false;
      String pwd_input = "";
      unsigned long lastTouchEvent = 0;

      struct button_t
      {
        const std::uint8_t LINE_THICKNESS = 4;

        std::uint16_t color = TFT_BLUE;
        std::uint16_t bg_color = TFT_DARKGREY;
        std::uint16_t text_color = TFT_WHITE;

        int x = -1;
        int y = -1;
        int w = -1;
        int h = -1;
        int fontsize = -1;
        String text;

        void clear(void)
        {
          M5.Display.setColor(M5.Display.getBaseColor());
          for (int i = 0; i < LINE_THICKNESS; ++i)
          {
            M5.Display.drawRect(x + i, y + i, w - i * 2, h - i * 2);
          }
        }

        void draw(void)
        {
          M5.Display.setColor(color);
          M5.Display.setTextColor(text_color, bg_color);
          M5.Display.setTextSize(fontsize);

          for (int i = 0; i < LINE_THICKNESS; ++i)
          {
            M5.Display.drawRect(x + i, y + i, w - i * 2, h - i * 2);
          }

          M5.Display.fillRect(x + LINE_THICKNESS, y + LINE_THICKNESS, w - LINE_THICKNESS * 2, h - LINE_THICKNESS * 2, bg_color);
          int textX = x + (w / 2) - M5.Display.textWidth(text) / 2;
          int textY = (y + (h / 2)) - (M5.Display.fontHeight() / 2);

          if (debug_output)
          {
            ESP_LOGI("txt", "%i / %i", textX, textY);
          }

          M5.Display.setCursor(textX, textY);
          M5.Display.print(text);
        }

        bool contain(int x, int y)
        {
          return this->x <= x && x < (this->x + this->w) && this->y <= y && y < (this->y + this->h);
        }
      };

      /**
       * Button-Objekt Array
       */
      button_t buttons[5][20];
      int buttonAnzahl[5] = {0, 0, 0, 0, 0};

    public:
      void dump_config() override;
      void setup() override;
      void loop() override;

      ShysM5Tough() : Component() {}

      /**
       * @brief
       *
       */
      void handleTouch()
      {
        vTaskDelay(1);

        M5.update();
        validateTouchTimeout();

        auto count = M5.Touch.getCount();
        if (!count)
        {
          return;
        }

        checkForTouch(count);
        M5.Display.display();
      }

      /**
       * @brief Dienste registrieren
       *
       */
      void registerServices()
      {
        register_service(&ShysM5Tough::showCurrentPassword, "showCurrentPassword");
        register_service(&ShysM5Tough::savePassword, "setNewPassword", {"new_password"});
      }

      /**
       * @brief
       *
       */
      void set_init_sound_enabled(bool initSound)
      {
        init_sound_enabled = initSound;
      }

      /**
       * @brief Set the login enabled object
       *
       * @param secured
       */
      void set_login_enabled(bool secured)
      {
        login_enabled = secured;
      }

      /**
       * @brief Create a Button object
       *
       * @param x
       * @param y
       * @param width
       * @param height
       * @param text
       * @param fontsize
       */
      void createButton(int x, int y, int width, int height, String text, int fontsize, String color, String textcolor, String bgcolor)
      {
        updateButton(buttonAnzahl[BUTTON_PAGE], x, y, width, height, text, fontsize, color, textcolor, bgcolor);
        buttonAnzahl[BUTTON_PAGE] = buttonAnzahl[BUTTON_PAGE] + 1;
      }

      /**
       * @brief
       *
       * @param index
       * @param x
       * @param y
       * @param width
       * @param height
       * @param text
       * @param fontsize
       */
      void updateButton(int index, int x, int y, int width, int height, String text, int fontsize, String color, String textcolor, String bgcolor)
      {
        buttons[BUTTON_PAGE][index].x = x;
        buttons[BUTTON_PAGE][index].y = y;
        buttons[BUTTON_PAGE][index].w = width;
        buttons[BUTTON_PAGE][index].h = height;
        buttons[BUTTON_PAGE][index].text = text;
        buttons[BUTTON_PAGE][index].fontsize = fontsize;
        buttons[BUTTON_PAGE][buttonAnzahl[BUTTON_PAGE]].color = getColor(color.c_str());
        buttons[BUTTON_PAGE][buttonAnzahl[BUTTON_PAGE]].text_color = getColor(textcolor.c_str());
        buttons[BUTTON_PAGE][buttonAnzahl[BUTTON_PAGE]].bg_color = getColor(bgcolor.c_str());
      }

      int getColor(const char *colorname)
      {
        if (strcmp(colorname, "TFT_BLACK") == 0)
        {
          return TFT_BLACK;
        }
        else if (strcmp(colorname, "TFT_NAVY") == 0)
        {
          return TFT_NAVY;
        }
        else if (strcmp(colorname, "TFT_DARKGREEN") == 0)
        {
          return TFT_DARKGREEN;
        }
        else if (strcmp(colorname, "TFT_MAROON") == 0)
        {
          return TFT_MAROON;
        }
        else if (strcmp(colorname, "TFT_PURPLE") == 0)
        {
          return TFT_PURPLE;
        }
        else if (strcmp(colorname, "TFT_OLIVE") == 0)
        {
          return TFT_OLIVE;
        }
        else if (strcmp(colorname, "TFT_LIGHTGREY") == 0)
        {
          return TFT_LIGHTGREY;
        }
        else if (strcmp(colorname, "TFT_DARKGREY") == 0)
        {
          return TFT_DARKGREY;
        }
        else if (strcmp(colorname, "TFT_BLUE") == 0)
        {
          return TFT_BLUE;
        }
        else if (strcmp(colorname, "TFT_GREENYELLOW") == 0)
        {
          return TFT_GREENYELLOW;
        }
        else if (strcmp(colorname, "TFT_GREEN") == 0)
        {
          return TFT_GREEN;
        }
        else if (strcmp(colorname, "TFT_YELLOW") == 0)
        {
          return TFT_YELLOW;
        }
        else if (strcmp(colorname, "TFT_ORANGE") == 0)
        {
          return TFT_ORANGE;
        }
        else if (strcmp(colorname, "TFT_PINK") == 0)
        {
          return TFT_PINK;
        }
        else if (strcmp(colorname, "TFT_CYAN") == 0)
        {
          return TFT_CYAN;
        }
        else if (strcmp(colorname, "TFT_DARKCYAN") == 0)
        {
          return TFT_DARKCYAN;
        }
        else if (strcmp(colorname, "TFT_RED") == 0)
        {
          return TFT_RED;
        }
        else if (strcmp(colorname, "TFT_MAGENTA") == 0)
        {
          return TFT_MAGENTA;
        }
        else if (strcmp(colorname, "TFT_WHITE") == 0)
        {
          return TFT_WHITE;
        }

        return TFT_WHITE;
      }

      /**
       * @brief
       *
       */
      void initializeShysM5Tough()
      {
        initM5Display();
        loadPassword();

        registerServices();

        initSpeaker();

        createKeypad();
        createOkBtn();
        createFailureBtn();

        esphome::delay(50);

        if (init_sound_enabled)
        {
          playInitSound();
        }

        setCurrentPage(login_enabled ? PASSWORD_PAGE : BUTTON_PAGE);
        refreshScreen();
      }

      /**
       * @brief
       *
       */
      void initM5Display()
      {
        M5.begin();
        M5.Display.setEpdMode(epd_mode_t::epd_fastest);

        if (M5.Display.width() > M5.Display.height())
        {
          M5.Display.setRotation(M5.Display.getRotation() ^ 3);
        }
      }

      /**
       * zeigt anhand von "currentPage" die entsprechende Seite an
       */
      void showCurrentPage()
      {
        switch (currentPage)
        {
        case PASSWORD_PAGE:
          showPwdInput();
          showKeypad();
          break;
        case BUTTON_PAGE:
          showButtonMenu();
          break;

        case OK:
          showOk();
          break;

        case FAILURE:
          showFailure();
          break;

        case SCREEN_OFF:
          M5.Display.fillRect(0, 0, 240, 320, TFT_BLACK);
        default:
          showPwdInput();
        }
      }

      /**
       * @brief
       *
       */
      void showButtonMenu()
      {
        M5.Display.setTextSize(3);
        drawButtons(buttons[BUTTON_PAGE], buttonAnzahl[BUTTON_PAGE]);
      }

      /**
       * Passwort Eingabe über Keypad anzeigen
       */
      void showPwdInput()
      {
        M5.Display.setTextSize(3);
        M5.Display.startWrite();
        M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
        M5.Display.setCursor(40, 5);
        M5.Display.print(pwd_input);
        M5.Display.endWrite();
      }

      /**
       * Numpad anzeigen
       */
      void showKeypad()
      {
        M5.Display.setTextSize(3);
        drawButtons(buttons[PASSWORD_PAGE], 12);
      }

      /**
       * @brief
       *
       */
      void showOk()
      {
        M5.Display.setTextSize(10);
        drawButtons(buttons[OK], 1);
      }

      /**
       * @brief
       *
       */
      void showFailure()
      {
        M5.Display.setTextSize(10);
        drawButtons(buttons[FAILURE], 1);
      }

      /**
       * @brief Alle Buttons aus Array anzeigen
       *
       * @param _btn
       */
      void drawButtons(button_t _btn[], int btnCount)
      {
        M5.Display.startWrite();

        for (int i = 0; i < btnCount; ++i)
        {
          _btn[i].draw();
        }

        M5.Display.endWrite();
      }

      /**
       * Prüfen ob Touch Eingabe vorhganden sind
       * und ggf. auswerten
       */
      void checkForTouch(int count)
      {
        lastTouchEvent = esphome::millis();
        boolean repaint = false;

        static m5::touch_state_t prev_state;
        for (int i = 0; i < count; ++i)
        {
          auto touchEvent = M5.Touch.getDetail(i);
          if (touchEvent.wasPressed())
          {
            handleTouchInput(touchEvent.x, touchEvent.y);

            showCurrentPage();
          }
        }
      }

      /**
       * Touch-Eingabe beim Numpad verarbeiten
       */
      void handleTouchInput(int x, int y)
      {
        if (currentPage == SCREEN_OFF)
        {
          publish_state("Display on");
          setCurrentPage(login_enabled ? PASSWORD_PAGE : BUTTON_PAGE);
          pwd_input = "";
          esphome::delay(500);
          return;
        }

        for (int i = 0; i < buttonAnzahl[currentPage]; ++i)
        {
          button_t currentButton = buttons[currentPage][i];
          if (currentButton.contain(x, y))
          {
            ESP_LOGI("log", "Button %s pressed", currentButton.text.c_str());
            playTouchBeep();

            switch (currentPage)
            {
            case PASSWORD_PAGE:
              handlePasswordTouchInput(currentButton);
              break;
            case BUTTON_PAGE:
              handleButtonTouchInput(currentButton);
              break;

            case OK:
              break;
            case FAILURE:
              break;

            case SCREEN_OFF:
              break;
            }
          }
        }
      }

      /**
       * @brief Touch-Eingabe bei der Passworteingabe
       *
       */
      void handlePasswordTouchInput(button_t button)
      {
        if (isDigit(button.text.charAt(0)) && pwd_input.length() < 8)
        {
          pwd_input = pwd_input + button.text;
        }

        if (button.text == "X")
        {
          pwd_input = "";
          M5.Display.fillRect(0, 0, 240, 39, TFT_BLACK);
        }

        if (button.text == "OK")
        {
          validatePwd();
        }
      }

      /**
       * @brief  Touch-Eingabe im Tor-Menü
       *
       */
      void handleButtonTouchInput(button_t button)
      {
        char message[1000];
        strcpy(message, "Button: ");
        strcat(message, button.text.c_str());
        publish_state(message);
        setCurrentPage(OK);
        esphome::delay(1000);
        setCurrentPage(BUTTON_PAGE);
      }

      /**
       * Display nach "timeToLock" sperren
       */
      void validateTouchTimeout()
      {
        if (!loggedIn || !login_enabled)
        {
          if (currentPage != SCREEN_OFF && esphome::millis() - lastTouchEvent > timeToScreenOff)
          {
            displayOff();
            ESP_LOGI("sleep", "Sleep after %d ms", timeToScreenOff);
            publish_state("Display off");
          }
        }
        else if (esphome::millis() - lastTouchEvent > timeToLock && login_enabled)
        {
          logout();
          ESP_LOGI("autolock", "No action for %d ms", timeToLock);
          publish_state("Screen locked");
        }
      }

      /**
       * @brief Display abschalten
       *
       */
      void displayOff()
      {
        setCurrentPage(SCREEN_OFF);
      }

      /**
       * @brief Abmelden
       *
       */
      void logout()
      {
        loggedIn = false;
        setCurrentPage(PASSWORD_PAGE);
      }

      /**
       * @brief Im Parameter übergebene Seite laden
       *
       * @param newPage
       */
      void setCurrentPage(Pages newPage)
      {
        currentPage = newPage;

        if (currentPage == SCREEN_OFF)
        {
          M5.Lcd.setBrightness(0);
        }
        else
        {
          M5.Lcd.setBrightness(100);
        }

        M5.Display.fillRect(0, 0, 240, 320, TFT_BLACK);
        showCurrentPage();
      }

      /**
       * Lautsprecher initialisieren
       */
      void initSpeaker()
      {
        M5.Speaker.begin();
        // M5.Speaker.setVolume(30);
        M5.Speaker.setAllChannelVolume(maxVolume);
      }

      /**
       * @brief Ton für Touch-Berührung abspielen
       *
       */
      void playTouchBeep()
      {
        M5.Speaker.tone(1500, 200);
        esphome::delay(10);
        M5.Speaker.tone(1000, 50);
        esphome::delay(100);
        M5.Speaker.stop();
      }

      /**
       * @brief Initial-Sound über Lautsprecher abspielen
       *
       */
      void playInitSound()
      {
        // A6
        M5.Speaker.tone(1760, 100);
        esphome::delay(150);
        // A6
        M5.Speaker.tone(1760, 100);
        esphome::delay(240);
        // A6
        M5.Speaker.tone(1760, 100);
        esphome::delay(300);
        // F6
        M5.Speaker.tone(1397, 80);
        esphome::delay(120);
        // A6
        M5.Speaker.tone(1760, 100);
        esphome::delay(300);
        // C7
        M5.Speaker.tone(2093, 100);
        esphome::delay(600);
        // C6
        M5.Speaker.tone(1047, 300);
      }

      /**
       * Tasten-Objekte für Numpad erzeugen
       */
      void createKeypad()
      {
        buttonAnzahl[PASSWORD_PAGE] = 12;

        buttons[PASSWORD_PAGE][0].x = 20;
        buttons[PASSWORD_PAGE][0].y = 40;
        buttons[PASSWORD_PAGE][0].w = 60;
        buttons[PASSWORD_PAGE][0].h = 60;
        buttons[PASSWORD_PAGE][0].text = "1";
        buttons[PASSWORD_PAGE][0].fontsize = 3;

        buttons[PASSWORD_PAGE][1].x = 90;
        buttons[PASSWORD_PAGE][1].y = 40;
        buttons[PASSWORD_PAGE][1].w = 60;
        buttons[PASSWORD_PAGE][1].h = 60;
        buttons[PASSWORD_PAGE][1].text = "2";
        buttons[PASSWORD_PAGE][1].fontsize = 3;

        buttons[PASSWORD_PAGE][2].x = 160;
        buttons[PASSWORD_PAGE][2].y = 40;
        buttons[PASSWORD_PAGE][2].w = 60;
        buttons[PASSWORD_PAGE][2].h = 60;
        buttons[PASSWORD_PAGE][2].text = "3";
        buttons[PASSWORD_PAGE][2].fontsize = 3;
        // ---------------------
        buttons[PASSWORD_PAGE][3].x = 20;
        buttons[PASSWORD_PAGE][3].y = 110;
        buttons[PASSWORD_PAGE][3].w = 60;
        buttons[PASSWORD_PAGE][3].h = 60;
        buttons[PASSWORD_PAGE][3].text = "4";
        buttons[PASSWORD_PAGE][3].fontsize = 3;

        buttons[PASSWORD_PAGE][4].x = 90;
        buttons[PASSWORD_PAGE][4].y = 110;
        buttons[PASSWORD_PAGE][4].w = 60;
        buttons[PASSWORD_PAGE][4].h = 60;
        buttons[PASSWORD_PAGE][4].text = "5";
        buttons[PASSWORD_PAGE][4].fontsize = 3;

        buttons[PASSWORD_PAGE][5].x = 160;
        buttons[PASSWORD_PAGE][5].y = 110;
        buttons[PASSWORD_PAGE][5].w = 60;
        buttons[PASSWORD_PAGE][5].h = 60;
        buttons[PASSWORD_PAGE][5].text = "6";
        buttons[PASSWORD_PAGE][5].fontsize = 3;
        // ---------------------
        buttons[PASSWORD_PAGE][6].x = 20;
        buttons[PASSWORD_PAGE][6].y = 180;
        buttons[PASSWORD_PAGE][6].w = 60;
        buttons[PASSWORD_PAGE][6].h = 60;
        buttons[PASSWORD_PAGE][6].text = "7";
        buttons[PASSWORD_PAGE][6].fontsize = 3;

        buttons[PASSWORD_PAGE][7].x = 90;
        buttons[PASSWORD_PAGE][7].y = 180;
        buttons[PASSWORD_PAGE][7].w = 60;
        buttons[PASSWORD_PAGE][7].h = 60;
        buttons[PASSWORD_PAGE][7].text = "8";
        buttons[PASSWORD_PAGE][7].fontsize = 3;

        buttons[PASSWORD_PAGE][8].x = 160;
        buttons[PASSWORD_PAGE][8].y = 180;
        buttons[PASSWORD_PAGE][8].w = 60;
        buttons[PASSWORD_PAGE][8].h = 60;
        buttons[PASSWORD_PAGE][8].text = "9";
        buttons[PASSWORD_PAGE][8].fontsize = 3;
        // ---------------------
        buttons[PASSWORD_PAGE][9].x = 20;
        buttons[PASSWORD_PAGE][9].y = 260;
        buttons[PASSWORD_PAGE][9].w = 60;
        buttons[PASSWORD_PAGE][9].h = 60;
        buttons[PASSWORD_PAGE][9].text = "X";
        buttons[PASSWORD_PAGE][9].bg_color = TFT_RED;
        buttons[PASSWORD_PAGE][9].fontsize = 3;

        buttons[PASSWORD_PAGE][10].x = 90;
        buttons[PASSWORD_PAGE][10].y = 260;
        buttons[PASSWORD_PAGE][10].w = 60;
        buttons[PASSWORD_PAGE][10].h = 60;
        buttons[PASSWORD_PAGE][10].text = "0";
        buttons[PASSWORD_PAGE][10].fontsize = 3;

        buttons[PASSWORD_PAGE][11].x = 160;
        buttons[PASSWORD_PAGE][11].y = 260;
        buttons[PASSWORD_PAGE][11].w = 60;
        buttons[PASSWORD_PAGE][11].h = 60;
        buttons[PASSWORD_PAGE][11].text = "OK";
        buttons[PASSWORD_PAGE][11].bg_color = TFT_GREEN;
        buttons[PASSWORD_PAGE][11].fontsize = 3;
      }

      /**
       * @brief Create a Ok Btn object
       *
       */
      void createOkBtn()
      {
        buttonAnzahl[OK] = 1;

        buttons[OK][0].x = 0;
        buttons[OK][0].y = 0;
        buttons[OK][0].w = 240;
        buttons[OK][0].h = 320;
        buttons[OK][0].bg_color = TFT_LIGHTGREY;
        buttons[OK][0].text_color = TFT_GREEN;
        buttons[OK][0].color = TFT_GREEN;
        buttons[OK][0].text = "OK";
        buttons[OK][0].fontsize = 6;
      }

      /**
       * @brief Create a Failure Btn object
       *
       */
      void createFailureBtn()
      {
        buttonAnzahl[FAILURE] = 1;

        buttons[FAILURE][0].x = 0;
        buttons[FAILURE][0].y = 0;
        buttons[FAILURE][0].w = 240;
        buttons[FAILURE][0].h = 320;
        buttons[FAILURE][0].bg_color = TFT_YELLOW;
        buttons[FAILURE][0].text_color = TFT_RED;
        buttons[FAILURE][0].color = TFT_RED;
        buttons[FAILURE][0].text = "X";
        buttons[FAILURE][0].fontsize = 8;
      }

      /**
       * @brief Password aus EEPROM lesen
       *
       * @return String
       */
      String getPassword()
      {
        return readStringFromEEPROM(0);
      }

      /**
       * @brief Password in EEPROM schreiben
       *
       * @param newPw
       */
      void savePassword(std::string newPw)
      {
        myPasswd = String(newPw.c_str());

        writeStringToEEPROM(0, myPasswd);

        ESP_LOGI("current_password", "Neues Passwort wurde gespeichert: '%s'", newPw.c_str());
      }

      /**
       * @brief Aktuelles Passwort im Log ausgeben
       *
       */
      void showCurrentPassword()
      {
        loadPassword();
        ESP_LOGI("current_password", "Aktuelles Passwort lautet: '%s'", myPasswd.c_str());
      }

      /**
       * @brief Passwort aus EEPROM laden falls vorhanden
       *
       */
      void loadPassword()
      {
        String tmpPw = getPassword();
        if (tmpPw != NULL && !tmpPw.isEmpty())
        {
          myPasswd = tmpPw;
          ESP_LOGI("current_password", "Aktuelles Passwort aus EEPROM: '%s'", myPasswd.c_str());
        } else {
          ESP_LOGI("current_password", "Aktuelles Passwort (kein gespeichertes Passwd gefunden): '%s'", myPasswd.c_str());
        }
      }

      /**
       * Passworteingabe prüfen
       */
      void validatePwd()
      {
        M5.Display.fillRect(0, 0, 240, 39, TFT_BLACK);

        if (pwd_input.equals(myPasswd))
        {
          loggedIn = true;
          publish_state("Login OK");
          pwd_input = "";
          setCurrentPage(OK);
          esphome::delay(1000);
          setCurrentPage(BUTTON_PAGE);
        }
        else
        {
          publish_state("Login failed");
          pwd_input = "";
          setCurrentPage(FAILURE);
          esphome::delay(1000);
          setCurrentPage(PASSWORD_PAGE);
        }
      }

      /**
       * Text in EEProm ablegen
       */
      void writeStringToEEPROM(int addrOffset, const String &strToWrite)
      {
        EEPROM.begin(EEPROM_SIZE);

        byte len = strToWrite.length();
        EEPROM.write(addrOffset, len);
        for (int i = 0; i < len; i++)
        {
          EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
        }
        EEPROM.commit();
        EEPROM.end();
      }

      /**
       * Text aus EEProm auslesen
       */
      String readStringFromEEPROM(int addrOffset)
      {
        String retVal = "";

        EEPROM.begin(EEPROM_SIZE);

        int newStrLen = EEPROM.read(addrOffset);
        if(!isnan(newStrLen)){
          char data[newStrLen + 1];
          for (int i = 0; i < newStrLen; i++)
          {
            data[i] = EEPROM.read(addrOffset + 1 + i);
          }
          data[newStrLen] = '\0';
          
          EEPROM.end();
          retVal = String(data);
        }
        
        return retVal;
      }

      void refreshScreen()
      {
        M5.Display.startWrite();
        M5.Display.fillRect(0, 0, 240, 320, TFT_BLACK);
        M5.Display.endWrite();
        M5.Display.setTextSize(3);

        showCurrentPage();
      }

      /**
       Display / Farben
      ----------------
      The screen pixel is 320x240, with the top left corner of the screen as the origin (0,0)

      Definition	Value	R	G	B
      TFT_BLACK	      0x0000	0	0	0
      TFT_NAVY	      0x000F	0	0	128
      TFT_DARKGREEN	  0x03E0	0	128	0
      TFT_MAROON  	  0x7800	128	0	0
      TFT_PURPLE  	  0x780F	128	0	128
      TFT_OLIVE	      0x7BE0	128	128	0
      TFT_LIGHTGREY	  0xC618	192	192	192
      TFT_DARKGREY	  0x7BEF	128	128	128
      TFT_BLUE	      0x001F	0	0	255
      TFT_GREENYELLOW	0xB7E0	180	255	0
      TFT_GREEN	      0x07E0	0	255	0
      TFT_YELLOW	    0xFFE0	255	255	0
      TFT_ORANGE	    0xFDA0	255	180	0
      TFT_PINK	      0xFC9F	255	255	16
      TFT_CYAN	      0x07FF	0	255	255
      TFT_DARKCYAN	  0x03EF	0	128	128
      TFT_RED	        0xF800	255	0	0
      TFT_MAGENTA	    0xF81F	255	0	255
      TFT_WHITE	      0xFFFF	255	255	255

      */
    };

  }
}
