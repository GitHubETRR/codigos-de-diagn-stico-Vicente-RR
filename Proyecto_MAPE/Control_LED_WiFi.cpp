/*Este programa permite conectar un ESP a WiFi con SmartConfig (con aplicaciones móviles como ESP Config) y guarda las credenciales de la red
y enviarle señales de /on o /off desde un celular conectado a la misma red
Usa una aplicación sencilla hecha con App Inventor
*/

#include <WiFi.h>
#include <ESPmDNS.h> // Permite conectar con la aplicación sin necesidad de la IP local
#include <Preferences.h> // Permite guardar las credenciales de la red

WiFiServer server(80);
Preferences prefs;

const int ledPin = 4;
const int resetPin = 12;
const int NOT_FOUND = -1;
const int TIMEOUT_RED = 10000;

void guardarCredenciales(const char* ssid, const char* pass) {
  prefs.begin("wifi", false);
  prefs.putString("ssid", ssid);
  prefs.putString("pass", pass);
  prefs.end();
}

bool obtenerCredenciales(String& ssid, String& pass) {
  prefs.begin("wifi", true);
  ssid = prefs.getString("ssid", "");
  pass = prefs.getString("pass", "");
  prefs.end();
  return (ssid.length() > 0 && pass.length() > 0);
}

void iniciarSmartConfig() {
  Serial.println("Iniciando SmartConfig...");
  WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();

  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nSmartConfig recibido. Conectando a red...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }

  Serial.println("\n¡Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  guardarCredenciales(WiFi.SSID().c_str(), WiFi.psk().c_str());
}

void iniciarMDNS() {
  if (MDNS.begin("mi-esp")) {
    Serial.println("mDNS iniciado: http://mi-esp.local");
  } else {
    Serial.println("Error al iniciar mDNS");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(resetPin, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  String ssid, pass;
  if (obtenerCredenciales(ssid, pass))
  {
    Serial.println("Red guardada encontrada: " + ssid);
    WiFi.begin(ssid.c_str(), pass.c_str());

    Serial.print("Conectando a red guardada...");
    unsigned long inicio = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - inicio < TIMEOUT_RED)
    {
      delay(500);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\nConectado a red guardada");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("\nNo se pudo conectar. Usando SmartConfig");
      iniciarSmartConfig();
    }
  } else {
    Serial.println("No hay red guardada");
    iniciarSmartConfig();
  }

  iniciarMDNS();
  server.begin();
}

void loop() {
  if (digitalRead(resetPin) == LOW) {
    Serial.println("Botón de vinculación presionado. Borrando configuración y reiniciando SmartConfig.");
    delay(1000);
    prefs.begin("wifi", false);
    prefs.clear();
    prefs.end();
    WiFi.disconnect(true);
    delay(1000);
    iniciarSmartConfig();
    iniciarMDNS();
  }

  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();
  Serial.println(request);

  if (request.indexOf("/on") != NOT_FOUND) {
    digitalWrite(ledPin, HIGH);
  }

  if (request.indexOf("/off") != NOT_FOUND) {
    digitalWrite(ledPin, LOW);
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<h1>OK</h1>");
  client.stop();
}
