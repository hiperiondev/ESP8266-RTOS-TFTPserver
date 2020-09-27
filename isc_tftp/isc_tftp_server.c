/*
 * This file is part of the esp-iot-secure-core distribution (https://github.com/hiperiondev/esp-iot-secure-core).
 * Copyright (c) 2019 Emiliano Augusto Gonzalez (comercial@hiperion.com.ar)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "isc_common.h"
#include "isc_spiffs.h"
#include "tftp_server.h"
static const char *TAG = "isc_tftp_server";

void* tftp_open(const char *fname, const char *mode, u8_t write) {
    ESP_LOGI(TAG, "tftp_open: %s", fname);
    isc_fs_delete(fname);
    return fopen(fname, write ? "w" : "r");
}

void tftp_close(void *handle){
    ESP_LOGI(TAG, "tftp_close");
    fclose(handle);
}

int tftp_read(void *handle, void *buf, int bytes) {
    char *err = fgets(buf, bytes, handle);
    return (err == NULL) ? -1 : 1;
}

int tftp_write(void *handle, struct pbuf *p) {
    return fwrite(p->payload, p->len, sizeof(uint8_t), handle);
}

static void isc_tftp_task(void *pvParameters) {
    struct tftp_context tftp_ctx;
       tftp_ctx.close = tftp_close;
        tftp_ctx.open = tftp_open;
        tftp_ctx.read = tftp_read;
       tftp_ctx.write = tftp_write;

       tftp_init(&tftp_ctx);

       while(1)
           vTaskDelay(5000 / portTICK_PERIOD_MS);
}

void isc_tftp_start(void) {
    xTaskCreate(
            isc_tftp_task,
                "isc_tftp_task",
                8192,
                NULL,
                12,
                NULL);
}
