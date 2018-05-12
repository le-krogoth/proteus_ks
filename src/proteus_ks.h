/*-----------------------------------------------------------------------------
 **
 ** - Proteus Kickstart -
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

#ifndef PROTEUS_KS_PROTEUS_KS_H
#define PROTEUS_KS_PROTEUS_KS_H

#include <Arduino.h>

#include <U8g2lib.h>

#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>

#include <Esp.h> // deep sleep and system info

static const std::string VERSION = "v1";

// needed to connect to a wifi
static const std::string SSID = "proteus_ks";
static const std::string PSK = "proteus.abcd";

// make sure to end with a /
static const std::string OTA_SERVER = "http://192.168.4.4:8081/";

bool bUpdatedSPIFFS = false;
bool bUpdatedFW = false;


void printScreen(std::string message);
void update();

#endif //PROTEUS_KS_PROTEUS_KS_H
