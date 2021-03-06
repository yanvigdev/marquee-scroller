/** The MIT License (MIT)

Copyright (c) 2018 David Payne

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*******************************************************
 * This is designed for the Wemos D1 ESP8266
 * Wemos D1 Mini:  https://amzn.to/2qLyKJd
 * MAX7219 Dot Matrix Module 4-in-1 Display For Arduino
 * Matrix Display:  https://amzn.to/2HtnQlD
 *******************************************************/

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "FS.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include "OpenWeatherMapClient.h"
#include "GeoNamesClient.h"
#include "TimeClient.h" // Using updated lib by Qrome
#include "NewsApiClient.h" 
#include "OctoPrintClient.h"
#include "AdviceSlipClient.h"

//******************************
// Start Settings
//******************************

const String APIKEY = ""; // Your API Key from http://openweathermap.com/
// Default City Location (use http://openweathermap.com/find to find city ID)
int CityIDs[] = { 5304391 }; //Only USE ONE for weather marquee
String marqueeMessage = "THANK YOU";
boolean IS_METRIC = false; // false = Imperial and true = Metric
boolean IS_24HOUR = false; // 23:00 millitary 24 hour clock
const int WEBSERVER_PORT = 80; // The port you can access this device on over HTTP
const boolean WEBSERVER_ENABLED = true;  // Device will provide a web interface via http://[ip]:[port]/
int minutesBetweenDataRefresh = 15;  // Time in minutes between data refresh (default 15 minutes)

boolean NEWS_ENABLED = true;
String NEWS_API_KEY = "";
String NEWS_SOURCE = "reuters";  // https://newsapi.org/sources to get full list of news sources available

boolean ADVICE_ENABLED = false;

// Display Settings
// CLK -> D5 (SCK)  
// CS  -> D6 
// DIN -> D7 (MOSI)
const int pinCS = D6; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int displayIntensity = 1;  //(This can be set from 0 - 15)

String timeDisplayTurnsOn = "06:30";  // 24 Hour Format HH:MM -- Leave blank for always on. (ie 05:30)
String timeDisplayTurnsOff = "23:00"; // 24 Hour Format HH:MM -- Leave blank for always on. Both must be set to work.

const String GEONAMES_USER = "qrome"; // user account for  http://www.geonames.org/ -- this service is used to lookup TimeZone Offsets

// OctoPrint Monitoring -- Monitor your 3D printer OctoPrint Server
boolean OCTOPRINT_ENABLED = false;
String OctoPrintApiKey = "";  // ApiKey from your User Account on OctoPrint
String OctoPrintServer = ""; // IP or Address of your OctoPrint Server (DO NOT include http://)
int OctoPrintPort = 80; // the port you are running your OctoPrint server on (usually 80);

//******************************
// End Settings
//******************************
