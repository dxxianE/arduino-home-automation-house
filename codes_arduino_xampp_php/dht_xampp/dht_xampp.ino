#include <WiFi.h>
#include <HTTPClient.h>

#include <DHT.h> 
#define DHTPIN 19
#define DHTTYPE DHT11 
DHT dht11(DHTPIN, DHTTYPE); 

String URL = "http://192.168.0.28/dht11_project/test_data.php";
//String URL = "http://192.168.228.251/dht11_project/test_data.php";

const char* ssid = "CasquetVolador2G"; 
const char* password = "ZLe04M4Y41997D1nA"; 

//const char* ssid = "acm1pt"; 
//const char* password = "Rocaroc10"; 

float temperature = 0;
float humidity = 0;

void setup() {
  Serial.begin(115200);
  dht11.begin(); 
  connectWiFi();
}

void loop() {
  //verificamos si tenemos conexion.
  if(WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  //obtenemos los datos del sensor
  Load_DHT11_Data();
  //cadena de texto postData que usaremos para el codigo .php, o sea, en el formato necesario para enviarlo al servidor
  String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity);
  
  //establecemos la conexion HTTP con el server
  HTTPClient http;
  http.begin(URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  //enviamos los datos al server mediante el POST().
  int httpCode = http.POST(postData);
  //payload sera la respuesta del server
  String payload = http.getString();

  Serial.print("URL : "); Serial.println(URL); 
  Serial.print("Data: "); Serial.println(postData);
  Serial.print("httpCode: "); Serial.println(httpCode);
  Serial.print("payload : "); Serial.println(payload);
  Serial.println("--------------------------------------------------");
  delay(5000);
}

//Funcion para obtener los datos del sensor. 
void Load_DHT11_Data() {
  //-----------------------------------------------------------
  temperature = dht11.readTemperature(); //C
  humidity = dht11.readHumidity();
  //-----------------------------------------------------------
  // Check si ha fallado
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    temperature = 0.0;
    humidity = 0.0;
  }
  //-----------------------------------------------------------
  Serial.printf("Temperature: %f °C\n", temperature);
  Serial.printf("Humidity: %f %%\n", humidity);
}

void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);

  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  Serial.print("connected to : "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}