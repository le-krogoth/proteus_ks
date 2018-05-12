/*-----------------------------------------------------------------------------
 **
 ** -  Proteus Kickstart -
 **
 ** Copyright 2018 by Krogoth and the contributing authors
 **
 ** This program is free software; you can redistribute it and/or modify it
 ** under the terms of the GNU Affero General Public License as published by the
 ** Free Software Foundation, either version 3 of the License, or (at your option)
 ** any later version.
 **
 ** This program is distributed in the hope that it will be useful, but WITHOUT
 ** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 ** FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License
 ** for more details.
 **
 ** You should have received a copy of the GNU Affero General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **
 ** -----------------------------------------------------------------------------*/

#include "proteus_ks.h"

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ 16);
//U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 2, /* reset=*/ 4);

// ------------------------------------------------------------------
// ------------------------------------------------------------------
void setup()
{
    // start serial port
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(10);

    u8g2.begin();
    u8g2.clearDisplay();
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.setFontMode(0);    // enable transparent mode, which is faster

    printScreen("welcome");

    Serial.print("Proteus KS ");
    Serial.print(VERSION.c_str());
    Serial.println(" booting up");

    Serial.println("Connecting WiFi SSID proteus_ks with PSK proteus.abcd");

    WiFi.mode(WIFI_STA);

    WiFi.begin(SSID.c_str(), PSK.c_str());

    printScreen("Connecting to WiFi");

    delay(10);
}

// ------------------------------------------------------------------
void loop()
{
    printScreen(".");

    update();

    delay(1000);
}

void printScreen(std::string message)
{
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.firstPage();
    do {

        u8g2.drawUTF8(0,10,"Proteus KS");
        u8g2.drawUTF8(80,10,VERSION.c_str());

        u8g2.drawUTF8(0,30,message.c_str());

    } while ( u8g2.nextPage() );
}

void update()
{
    // wait for WiFi connection
    if((WiFi.status() == WL_CONNECTED))
    {
        if(!bUpdatedSPIFFS)
        {
            printScreen("Checking SPIFFS");

            Serial.println("Update SPIFFS...");

            String sSpiffsFile = OTA_SERVER.c_str();
            sSpiffsFile += "spiffs/file";

            t_httpUpdate_return retS = ESPhttpUpdate.updateSpiffs(sSpiffsFile);
            switch(retS) {
                case HTTP_UPDATE_FAILED:
                    printScreen("SPIFFS update failed");
                    Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                    break;

                case HTTP_UPDATE_NO_UPDATES:
                    printScreen("SPIFFS, no update");
                    Serial.println("HTTP_UPDATE_NO_UPDATES");
                    break;

                case HTTP_UPDATE_OK:
                    printScreen("SPIFFS updated");
                    Serial.println("HTTP_UPDATE_OK");
                    bUpdatedSPIFFS = true;
                    break;
            }

            // give the user time to read
            delay(3000);
        }

        if(!bUpdatedFW)
        {
            Serial.println("Update sketch...");
            printScreen("Checking FW");

            String sUpdateFile = OTA_SERVER.c_str();
            sUpdateFile += "fw/v2";

            t_httpUpdate_return retFW = ESPhttpUpdate.update(sUpdateFile);
            switch (retFW) {
                case HTTP_UPDATE_FAILED:
                    printScreen("FW update failed");
                    Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s", ESPhttpUpdate.getLastError(),
                                  ESPhttpUpdate.getLastErrorString().c_str());
                    break;

                case HTTP_UPDATE_NO_UPDATES:
                    printScreen("FW, no update");
                    Serial.println("HTTP_UPDATE_NO_UPDATES");
                    break;

                case HTTP_UPDATE_OK:
                    printScreen("FW updated, reboot me now");
                    Serial.println("HTTP_UPDATE_OK");
                    bUpdatedFW = true;
                    break;
            }

            // give the user time to read
            delay(3000);
        }
    }
}