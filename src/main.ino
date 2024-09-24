#include <SPI.h>
#include <Ethernet.h>
#include <SHT1x.h>
#include <math.h>
#include <LiquidCrystal.h>

#define dataPin  7
#define clockPin 6
SHT1x sht1x(dataPin, clockPin);
//                RS, En, D5, D4, D3 ,D2
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);
// This is the variable we will send, but it will be formated as String first.
String Data = "";
String Data1 = "";
String Data2 = "";
String Data3 = "";
String Data4 = "";

IPAddress server(192, 168, 1, 2);

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
float suhuC;
float hum;
int number = 0;


char inString[32]; // string for incoming serial data
int stringPos = 0; // string index counter
boolean startRead = false;

int RelayKipas = 5;
int RelayAC = 4;

float temp_c;
float temp_f;
float humidity;

void setup() {
  // Open serial communications and wait for port to open:
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  lcd.begin(16, 2);

  pinMode(RelayKipas, OUTPUT);
  pinMode(RelayAC, OUTPUT);
  String txData2 = "STATUSK=OFF";
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
}

void loop(){
  tempHum();
  datakipastxt();
  dataACtxt();
  datalog();
  delay(5000);
}

void tempHum(){
  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();
  lcd.setCursor(0, 0);
  lcd.print("Tmp : ");
  lcd.print(temp_c);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Hum : ");
  lcd.print(humidity);
  lcd.print("%");
}

///////////////////////////////////////////////////////////////////
String connectAndRead(String location) {
  //connect to the server
  //port 80 is typical of a www page
  if (client.connect(server, 8080)) {
    client.print("GET ");
    client.println(location);
    client.print( "Host: " );
    client.println(server);
    client.println();
    //Connected - Read the page
    return readPage(); //go and read the output
  }
  else {
    return "connection failed to" + location;
  }
}
//////////////////////////////////////////////////////////////////
String readPage() {
  //read the page, and capture & return everything between '<' and '>'
  stringPos = 0;
  memset( &inString, 0, 32 ); //clear inString memory

  while (true) {

    if (client.available()) {
      char c = client.read();
      if (c == '<' ) { //'<' is our begining character
        startRead = true; //Ready to start reading the part
      }

      else if (startRead) {
        if (c != '>') { //'>' is our ending character
          inString[stringPos] = c;
          stringPos ++;
        }

        else {
          //got what we need here! We can disconnect now
          startRead = false;
          client.flush();
          client.stop();
          return inString;
        }
      } //end StartRead
    } //end ClientAvailable
  }//end while
}//endReadPage

void datakipastxt(){
  temp_c = sht1x.readTemperatureC();
  humidity = sht1x.readHumidity();
  Serial.print("suhu: ");
  Serial.println(temp_c );
  Serial.print("humadity: ");
  Serial.println (humidity);
  String location = "/incubator/datakipas.txt HTTP/1.0";
  String pageValue = connectAndRead(location); //connect to the server and read the output
  Serial.println(pageValue);
  Serial.println();

  if (pageValue == "ON")    // these are the snippets the Arduino is watching for.
  {
    Data2 = "STATUSK=M/ON";
    Serial.println("KIPAS M/ON");
    digitalWrite(RelayKipas, HIGH);
  }

  else if (pageValue == "OFF")
  {
    Data2 = "STATUSK=M/OFF";
    Serial.println("KIPAS M/OFF");
    digitalWrite(RelayKipas, LOW);
  }

  else if (pageValue == "MANUAL")
  {
    Data2 = "STATUSK=M/OFF";
    Serial.println("KIPAS M/OFF");
    digitalWrite(RelayKipas, LOW);
  }

  else if (pageValue == "AUTO")
  {
    Serial.println("AUTO");
    ////////////////////////ARTIFICIAL INTELEGENCE//////////////////////
    if (temp_c > 37 || humidity > 80)
    {
      Data2 = "STATUSK=A/ON";
      Data4 = "STATUSAC=A/OFF";
      digitalWrite(RelayKipas, HIGH);
      digitalWrite(RelayAC, LOW);
      Serial.println("KIPAS A/ON");
      Serial.println("AC A/OFF");
    }

    else if ( temp_c < 35 || humidity < 60)
    {
      Data2 = "STATUSK=A/OFF";
      Data4 = "STATUSAC=A/ON";
      digitalWrite(RelayKipas, LOW);
      digitalWrite(RelayAC, HIGH);
      Serial.println("KIPAS A/OFF");
      Serial.println("AC A/ON");
    }

    else
    {
      Data2 = "STATUSK=A/OFF";
      Data4 = "STATUSAC=A/OFF";
      digitalWrite(RelayKipas, LOW);
      digitalWrite(RelayAC, LOW);
      Serial.println("KIPAS A/OFF");
      Serial.println("AC A/OFF");
    }
  }
}
////////////////////////////end DATA.txt////////////////////////////


/////////////////////////////Data2.txt/////////////////////////////
void dataACtxt(){
  String location = "/incubator/dataAC.txt HTTP/1.0";
  String pageValue = connectAndRead(location); //connect to the server and read the output
  Serial.println(pageValue);
  Serial.println();

  // these are the snippets the Arduino is watching for.
  if (pageValue == "ON")
  {
    Data4 = "STATUSAC=M/ON";
    Serial.println("AC M/ON");
    digitalWrite(RelayAC, HIGH);
  }

  else if (pageValue == "OFF")
  {
    Data4 = "STATUSAC=M/OFF";
    Serial.println("AC M/OFF");
    digitalWrite(RelayAC, LOW);
  }

  else if (pageValue == "MANUAL")
  {
    Data4 = "STATUSAC=M/OFF";
    Serial.println("AC M/OFF");
    digitalWrite(RelayAC, LOW);
  }
}
/////////////////////////END DATA2txt///////////////////////////////

//////////////////////////START DataLOG/////////////////////////
void datalog(){
  number++;
  suhuC = sht1x.readTemperatureC();
  hum = sht1x.readHumidity();
  char buffer[10];

  String tem = dtostrf(suhuC, 5, 2, buffer);
  String temper = (tem);

  String hmdty = dtostrf(hum, 5, 2, buffer);
  String humadity = (hmdty);

  String alamat = "GET /incubator/updated2.php?";

  Data = "SUHU=" + (String (temper));
  Data1 = "nomer=" + (String (number));
  Data3 = "hmdty=" + (String (humadity));
  //Data2 STATUSK, Data4 STATUSAC
  koneksi(alamat, Data, Data1, Data2, Data3, Data4);
}
/////////////////////////////////////end DataLOG//////////////////////////


//////////////////////////////koneksi/////////////////////////////////////////////
void koneksi(String alamat, String txData, String txData1, String txData2, String txData3, String txData4){
  if (client.connect(server, 8080)){
    client.print(alamat);
    client.print(txData);
    client.print("&&");
    client.print(txData1);
    client.print("&&");
    client.print(txData2);
    client.print("&&");
    client.print(txData3);
    client.print("&&");
    client.print(txData4);
    client.println( " HTTP/1.0");
    client.print( "Host: " );
    client.println(server);
    client.print("Connection: close");
    client.println();
    client.println();
    client.stop();
  }
  else
  {
    Serial.println("Connection Failed to updated2.php"); // Again the Serial is for feedback.
    Serial.println();
  }
  /////////////////////////////////////////////////////////////////////////////
  // 'BUTTON_STATE' is the POST variable that the PHP page is looking for.
  // The '+' puts the string parts together.
  //  '(String (bstate))' formats 'bstate' from a numerical value to a string.
  // Multiples would look like
  //txData = "VARIABLE1="+ (String (var1))+"&VARIABLE2=" (String(var2));

  //("yourwebsite.com",80) could also be, I think an IP, (xxx.xxx.xxx.xxx,80) or
  // you could define a variable 'server' like we did with 'ip' and 'mac'
  // like this, 'byte server[]  = { xxx, xxx, xxx, xxx }'
  // then this line would look like  'if (client.connect(server,80))'
}
