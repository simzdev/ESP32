#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include"esp_gap_bt_api.h"
#include "esp_err.h"

#include <SPI.h>

#define PAIR_MAX_DEVICES 20
uint8_t pairedDeviceBtAddr[PAIR_MAX_DEVICES][6];
char bda_str[18];

void disable_enabled() {
    esp_bluedroid_disable();
    esp_bluedroid_deinit();
    btStop();
}

String bt_remove_paired_devices() {
    if (!btStart()) {
        return "Failed to initialize controller!";
    }

    if (esp_bluedroid_init() != ESP_OK) {
        return "Failed to initialize bluedroid!";
    }

    if (esp_bluedroid_enable() != ESP_OK) {
        return "Failed to enable bluedroid!";
    }

    int count = esp_bt_gap_get_bond_device_num();
    if (!count) {
        disable_enabled();
        return "No bonded device found.";
    }
    else {
        if (PAIR_MAX_DEVICES < count) {
            count = PAIR_MAX_DEVICES;
        }
        esp_err_t tError = esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
        if (ESP_OK == tError) {
            char cnt_i[2];
            for (int i = 0; i < count; i++) {
                sprintf(cnt_i, "%c", i);
                const uint8_t* bda_ = pairedDeviceBtAddr[i];
                esp_err_t tError = esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);
                if (ESP_OK != tError) {
                    disable_enabled();
                    return "Failed to remove bonded device/s.";
                }
            }
            disable_enabled();
            return "Removed bonded device/s.";
        }
    }
}