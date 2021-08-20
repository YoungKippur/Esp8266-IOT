// https://arduino.esp8266.com/stable/package_esp8266com_index.json
// Para configurar mirar la foto
// Recordar cambiar la WIFI
// Sacar las partes del puerto serial

#include <ESP8266WiFi.h>

#define LED_PING 12
#define LED_PINB 13
#define LED_PINR 14

#define REL_PINA 15
#define REL_PINB 3

#define SENS_PIN A0

const char* ssid = "CABLEVISION-eec1"; // Cambiar por red personal 
const char* password = "1805BMISBQHA";

int value1 = 1;
int value2 = 1;
int value3 = 1;

int aux;
int sensAux;
int sensValue;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(LED_PINR, OUTPUT);
  pinMode(LED_PING, OUTPUT);
  pinMode(LED_PINB, OUTPUT);
  digitalWrite(LED_PINR, LOW);
  digitalWrite(LED_PING, LOW);
  digitalWrite(LED_PINB, LOW);

  pinMode(REL_PINA, OUTPUT);
  digitalWrite(REL_PINA, LOW);

  pinMode(REL_PINB, OUTPUT);
  digitalWrite(REL_PINB, LOW);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("/");
  }

  Serial.println("");

  server.begin();

  Serial.print("URL: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  while (!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/LED1=GREEN") != -1)  {
    digitalWrite(LED_PINR, LOW);
    digitalWrite(LED_PING, HIGH);
    digitalWrite(LED_PINB, LOW);
    value1 = 1;
    aux = 0;
  }
  if (request.indexOf("/LED1=BLUE") != -1)  {
    digitalWrite(LED_PINR, LOW);
    digitalWrite(LED_PING, LOW);
    digitalWrite(LED_PINB, HIGH);
    value1 = 2;
    aux = 0;
  }
  if (request.indexOf("/LED1=RED") != -1)  {
    digitalWrite(LED_PINR, HIGH);
    digitalWrite(LED_PING, LOW);
    digitalWrite(LED_PINB, LOW);
    value1 = 3;
    aux = 0;
  }
  if (request.indexOf("/LED1=PATRON1") != -1)  {
    aux = 1;
  }
  if (request.indexOf("/LED1=CANCEL") != -1)  {
    digitalWrite(LED_PINR, LOW);
    digitalWrite(LED_PING, LOW);
    digitalWrite(LED_PINB, LOW);
    aux = 0;
  }
  if (request.indexOf("/RELE1=OFF") != -1)  {
    digitalWrite(REL_PINA, LOW);
    value2 = 1;
    aux = 0;
  }
  if (request.indexOf("/RELE1=ON") != -1)  {
    digitalWrite(REL_PINA, HIGH);
    value2 = 2;
    aux = 0;
  }
  if (request.indexOf("/RELE1=PULSO") != -1)  {
    digitalWrite(REL_PINA, HIGH);
    delay(200);
    digitalWrite(REL_PINA, LOW);
    value2 = 1;
    aux = 0;
  }
  if (request.indexOf("/RELE2=OFF") != -1)  {
    digitalWrite(REL_PINB, LOW);
    value3 = 1;
    aux = 0;
  }
  if (request.indexOf("/RELE2=ON") != -1)  {
    digitalWrite(REL_PINB, HIGH);
    value3 = 2;
    aux = 0;
  }
  if (request.indexOf("/RELE2=PULSO") != -1)  {
    digitalWrite(REL_PINB, HIGH);
    delay(200);
    digitalWrite(REL_PINB, LOW);
    value3 = 1;
    aux = 0;
  }
  if (request.indexOf("/SENS=START") != -1)  {
    aux = 2;
  }
  if (request.indexOf("/SENS=CANCEL") != -1)  {
    sensValue = 0;
    aux = 0;
  }

  // faltan las condiciones del rgb

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("By Marcos Strauss");
  //client.println(" && Mateo Ziffer");
  client.println("<br><br>");
  client.println("Device: 01");
  client.println("<br><br>");
  client.println("<div style='display: flex; flex-direction: row;justify-content: center;'>");
  client.print("Led 1: ");

  if (value1 == 1) {
    client.print("Green");
  }
  else if (value1 == 2) {
    client.print("Blue");
  }
  else {
    client.print("Red");
  }
  client.println("</div>");
  client.println("<br><br>");
  client.println("<div style='display: flex; flex-direction: row;justify-content: center;gap: 20px;'>");
  client.println("<a href=\"/LED1=RED\"\"><button style='color: red'>Red </button></a>");
  client.println("<a href=\"/LED1=GREEN\"\"><button style='color: green'>Green </button></a>");
  client.println("<a href=\"/LED1=BLUE\"\"><button style='color: blue'>Blue </button></a>");
  client.println("<a href=\"/LED1=PATRON1\"\"><button >Patron 1 </button></a>");
  client.println("<a href=\"/LED1=CANCEL\"\"><button >Cancel </button></a>");

  if (aux == 1)  {
    asturbandaOpa();
    client.print("<meta http-equiv='refresh' content='0'>");
  }

  client.println("</div>");
  client.println("<br><br>");
  client.println("<div style='display: flex; flex-direction: list;justify-content: center;'>");
  client.println("RGB Type Element:");
  client.println("</div>");
  client.println("<br><br>");
  client.print("<head></head><body><form onsubmit='showRgb(event)' style='margin: 0 auto;width: 20%;display: flex;flex-direction: column;gap: 10px;text-align: center;'><label for='Red'>Red</label><input type='number' placeholder='enter the ammount of red...' /><label for='Green'>Green</label><input type='number' placeholder='enter the ammount of green...' /><label for='Blue'>Blue</label><input type='number' placeholder='enter the ammount of blue...' /><button type='submit'>Submit</button><p>Result</p><p id='result'></p></form></body><script>const showRgb = event => {const getTarget = i => event.target[i].value;event.preventDefault();let result = document.getElementById('result');str = getTarget(0) + ',' + getTarget(1) + ',' + getTarget(2);result.innerHTML = `<a href='rgb${str}'>${str}</a>`;}</script>");
  client.println("<div style='display: flex; flex-direction: row;justify-content: center;'>");
  client.print("Rele 1: ");

  if (value2 == 1) {
    client.print("Off");
  }
  else if (value2 == 2) {
    client.print("On");
  }
  client.println("</div>");
  client.println("<br><br>");
  client.println("<div style='display: flex; flex-direction: row;justify-content: center;gap: 20px;'>");
  client.println("<a href=\"/RELE1=PULSO\"\"><button>Pulso </button></a>");
  client.println("<a href=\"/RELE1=ON\"\"><button>On </button></a>");
  client.println("<a href=\"/RELE1=OFF\"\"><button>Off </button></a>");
  client.println("</div>");
  client.println("<br><br>");
  client.println("<div style='display: flex; flex-direction: row;justify-content: center;'>");
  client.print("Rele 2: ");

  if (value3 == 1) {
    client.print("Off");
  }
  else if (value3 == 2) {
    client.print("On");
  }
  client.println("</div>");
  client.println("<br><br>");
  client.println("<div style='display: flex; flex-direction: row;justify-content: center;gap: 20px;'>");
  client.println("<a href=\"/RELE2=PULSO\"\"><button>Pulso </button></a>");
  client.println("<a href=\"/RELE2=ON\"\"><button>On </button></a>");
  client.println("<a href=\"/RELE2=OFF\"\"><button>Off </button></a>");
  client.println("</div>");
  client.println("<br><br>");
  client.println("<div style='display: flex; flex-direction: row;justify-content: center;'>");
  client.println("Sensor 1");
  client.println("<br><br>");
  client.println("</div>");
  client.println("<div style='display: flex; flex-direction: row;justify-content: center;gap: 20px;'>");
  client.println("<a href=\"/SENS=START\"\"><button >Start </button></a>");
  client.println("<a href=\"/SENS=CANCEL\"\"><button >Cancel </button></a>");
  client.println("<br><br>");
  client.println("</div>");

  client.println("<div style='font-size:25px; display: flex; flex-direction: row;justify-content: center; gap: 20px;'>");
  if (aux == 2) {
    sensValue = analogRead(SENS_PIN);
    sensValue = map(sensValue, 0, 1023, 0, 100);

    // aca se pone el analog y tamb se iguala a el sensValue
    // no se porque mierda se traba tanto
    client.print("<div style='color:#FF0000;'>");
    client.print(sensValue);
    client.print("</div>");

    sensValue = map(sensValue, 0, 100, 100, 0);

    client.print("<div style='color:#0000FF;'>");
    client.print(sensValue);
    client.print("</div>");
    
    client.println("</div>");
    client.println("%");
    client.print("<meta http-equiv='refresh' content='0'>");
    delay(2000); // para que no este todo eltiempo rompiendo las bolas
  }

  client.println("</html>");

  delay(1);
}

void asturbandaOpa() {
  digitalWrite(LED_PINR, HIGH);
  digitalWrite(LED_PING, LOW);
  digitalWrite(LED_PINB, LOW);
  delay(1000);
  digitalWrite(LED_PINR, LOW);
  digitalWrite(LED_PING, HIGH);
  digitalWrite(LED_PINB, LOW);
  delay(1000);
  digitalWrite(LED_PINR, LOW);
  digitalWrite(LED_PING, LOW);
  digitalWrite(LED_PINB, HIGH);
  delay(1000);
}

// Para sensores y eso
// client.print("<meta http-equiv='refresh' content='0.3; url=http://");
// client.print(WiFi.localIP());
// client.println("/LED1=PATRON1'>");

// Actualizacion
// este es mejor:
// client.print("<meta http-equiv='refresh' content='0'>");
// y desp le metes un delay
