#include "esphome/core/log.h"
#include "shys_m5_tough.h"

namespace esphome
{
    namespace shys_m5_tough
    {

        static const char *TAG = "shys_m5_tough";

        /**
         * @brief SETUP
         *
         * Initialisierung
         */
        void ShysM5Tough::setup()
        {
            ShysM5Tough::initializeShysM5Tough();
            ESP_LOGI("log", "%s", "M5 is initialized");
        }

        /**
         * @brief LOOP
         *
         * Standard Loop
         */
        void ShysM5Tough::loop()
        {
            ShysM5Tough::handleTouch();
            esphome::delay(5);
        }

        /**
         * @brief dump_config
         *
         * Ausgabe der aktuellen Konfiguration im Log nach Initialisierung
         */
        void ShysM5Tough::dump_config()
        {
            ESP_LOGCONFIG(TAG, "-----------------------------------");
            ESP_LOGCONFIG(TAG, "Shys M5 Tough");
            ESP_LOGCONFIG(TAG, "Password Secure: %s", this->login_enabled ? "on" : "off");
            ESP_LOGCONFIG(TAG, "Initial Sound: %s", this->init_sound_enabled ? "on" : "off");
            ESP_LOGCONFIG(TAG, "-----------------------------------");
        }

    }
}