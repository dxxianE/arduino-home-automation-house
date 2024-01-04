// Load Wi-Fi library
#include <WiFi.h>
#include <Servo.h>

//global vars para el servo
const int servo = 10;
//constant "kE" de la libreria orignal
Servo myservo = Servo();

// Constantes para la conec WiFi
const char* ssid = "CasquetVolador2G";
const char* password = "ZLe04M4Y41997D1nA";

//const char* ssid = "acm1pt"; 
//const char* password = "Rocaroc10"; 

//Iniciar server con port 80
WiFiServer server(80);

// Variable para HTTP request
String header;

// Auxiliar variables to store the current output state
String led1State = "off";
String led2State = "off";
String led3State = "off";
String led4State = "off";
String led5State = "off";

// Assign output variables to GPIO pins
const int led1 = 1;
const int led2 = 2;
const int led3 = 3;
const int led4 = 41;
const int led5 = 42;

//Variables globales para el sensor de ultrasonido
const int Trigger = 35;
const int Echo = 36;
long t, d;

// Current time para los clientes del server
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout 
const long timeoutTime = 2000;

void setup() {
  //puerto serie ESP32_S2
  Serial.begin(115200);
  // Inicializar the output variables as outputs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  // Leds en t_0 en LOW (para que este off)
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);

  //echo trigger sensor
  pinMode(Trigger, OUTPUT); 
  pinMode(Echo, INPUT);
  myservo.attach(10);

  //Wifi net and server start
  connectWiFi();
  server.begin();
}

void loop(){

  digitalWrite(Trigger, HIGH);
  //con esto enviamos los pulsos
  delayMicroseconds(10);          
  digitalWrite(Trigger, LOW);    
  
  //obtenemos el ancho del pulso
  t = pulseIn(Echo, HIGH);
  //escalamos el tiempo a una distancia en cm
  d = t/59;             
  
  Serial.print("Distancia: ");
  Serial.print(d);      
  Serial.print("cm");
  Serial.println();

  //si la distancia es menor a 80 enciende el servo que acciona la puerta del parking
  if (d < 80 ){
    Serial.println("Enciende el motor");
    myservo.write(servo, 80);       
    delay(3000);
    
  }
  else {
    Serial.println("Apagamos motor ");
    myservo.write(servo, 175);        
    delay(1500);
    
  }
  delay(100);     
  
  //Escuchamos a los clientes 
  WiFiClient client = server.available(); 

  //--------------------

  //Verificamos si tenemos cliente.
  if (client) {                             

    //variables para el cliente
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";     

    // Bucle que se ejecuta mientras el cliente este conectado y no haya superado el 
    //timeout impuesto anteriormente
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  
      currentTime = millis();

      //Miramos si hay bytes disponibles para leer desde el cliente (request by client)
      //si devuelve true entonces estamos teniendo una request. 
      if (client.available()) {    
        //leemos el byte         
        char c = client.read();            
        Serial.write(c);    
        //agregamos el byte leido al header que usaremos para almacenar la soliciutd HTTP.               
        header += c;
        // Comprueba si el byte leído es un carácter de nueva línea. Esto indica el final de una línea en la solicitud HTTP.
        if (c == '\n') {                   
          //Verifica si la línea actual está vacía. Si es así, significa que ha habido dos caracteres 
          //de nueva línea consecutivos, lo que indica el final de la solicitud HTTP del cliente.
          if (currentLine.length() == 0) {
            //Envía una respuesta HTTP al cliente indicando que la solicitud se ha procesado correctamente.
            client.println("HTTP/1.1 200 OK");
            //Envía una cabecera HTTP al cliente indicando el tipo de contenido que se enviará, en este caso, texto HTML que esta abajo.
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Control de los leds
            if (header.indexOf("GET /LED_1/on") >= 0) {
              Serial.println("GPIO LED_1 on");
              led1State = "on";
              digitalWrite(led1, HIGH);
            } else if (header.indexOf("GET /LED_1/off") >= 0) {
              Serial.println("GPIO LED_1 off");
              led1State = "off";
              digitalWrite(led1, LOW);

            } else if (header.indexOf("GET /LED_2/on") >= 0) {
              Serial.println("GPIO LED_2 on");
              led2State = "on";
              digitalWrite(led2, HIGH);
            } else if (header.indexOf("GET /LED_2/off") >= 0) {
              Serial.println("GPIO LED_2 off");
              led2State = "off";
              digitalWrite(led2, LOW);

            } else if (header.indexOf("GET /LED_3/on") >= 0) {
              Serial.println("GPIO LED_3 on");
              led3State = "on";
              digitalWrite(led3, HIGH);
            } else if (header.indexOf("GET /LED_3/off") >= 0) {
              Serial.println("GPIO LED_3 off");
              led3State = "off";
              digitalWrite(led3, LOW);
            }

             else if (header.indexOf("GET /LED_4/on") >= 0) {
              Serial.println("GPIO LED_4 on");
              led4State = "on";
              digitalWrite(led4, HIGH);
            } else if (header.indexOf("GET /LED_4/off") >= 0) {
              Serial.println("GPIO LED_4 off");
              led4State = "off";
              digitalWrite(led4, LOW);
            }

              else if (header.indexOf("GET /LED_5/on") >= 0) {
              Serial.println("GPIO LED_5 on");
              led5State = "on";
              digitalWrite(led5, HIGH);
            } else if (header.indexOf("GET /LED_5/off") >= 0) {
              Serial.println("GPIO LED_5 off");
              led5State = "off";
              digitalWrite(led5, LOW);
            }
            
            // ***** CODIGO HTML ****
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS de los botones
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Cabecera de la pagina
            client.println("<body><h1>ESP32S2 Web Server</h1>");
            
            //Mostramos el estado actual del boton, en este caso del bano
            client.println("<p>Led Bano - State " + led1State + "</p>");      
            if (led1State=="off") {
              client.println("<p><a href=\"/LED_1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/LED_1/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            client.println("<p>Led Garaje - State " + led2State + "</p>");
            if (led2State=="off") {
              client.println("<p><a href=\"/LED_2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/LED_2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>Led Dormitorio - State " + led3State + "</p>");
            if (led3State=="off") {
              client.println("<p><a href=\"/LED_3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/LED_3/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>Led Recibidor - State " + led4State + "</p>");
            if (led4State=="off") {
              client.println("<p><a href=\"/LED_4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/LED_4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>Led Sala de estar / Cocina - State " + led5State + "</p>");
            if (led5State=="off") {
              client.println("<p><a href=\"/LED_5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/LED_5/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("</body></html>");
            
         
            client.println();
            // final del bucle While
            break;
          } else { //Si hay nueva linea borramos la actual. 
            currentLine = "";
          }
        } else if (c != '\r') {  //retorno de carro
          currentLine += c;     
        }
      }
    }
    // Limpiamos el header
    header = "";
    // Finalizamos conec
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  //Esta línea oculta la visualización de ESP como punto de acceso wifi
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  Serial.print("connected to : "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}