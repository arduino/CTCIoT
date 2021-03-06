
//Libraries for the Yúns connection
#include <Bridge.h>// communication between the ATmega32U4(the arduino) and the AR9331 (Linux side)
#include <HttpClient.h> //Creates a HTTP client on Linux.

HttpClient client; // the HTTP client


//Libraries for the servo
#include <Servo.h>

//clothesline servo
Servo clotheslineservo;  // create continuous servo object to control a servo
//shades servo
Servo shadesservo;  // create standard servo object to control a servo

// fanmotor pin
int motorPin = 6;

// watersolenoid pin
int watersolenoid = 9;
// vent solenoid pin
int ventsolenoidPin = 10;


// the group numbers
int groupOne = 1; //clothesline servo
int groupTwo = 2; // vent the air solenoid
int groupThree = 3; // shades servo
int groupFour = 4;  // water plant solenoid
int groupFive = 5; //fan dc motor

//  temperature
int temperatureCount = 0;
// saveing the old temperature value
int oldtemperatureCount = 0;


int deviceSwitcher = 0; // counter for switching between the motors/solenoid and servos

void setup() {
  // configure the 13 pin as a OUTPUT
  pinMode(13, OUTPUT);


  // Blink once with the led on pin 13 when the connection is ready
  digitalWrite(13, LOW);
  Bridge.begin();  Bridge.begin(); //starts the Bridge
  digitalWrite(13, HIGH);


  clotheslineservo.attach(3);  // attaches the servo on pin 2 to the servo object

  shadesservo.attach(4);  // attaches the servo on pin 3 to the servo object

  // configure the solenoidPin as a OUTPUT
  pinMode(watersolenoid, OUTPUT);

  // configure the solenoidPin as a OUTPUT
  pinMode(ventsolenoidPin, OUTPUT);

  // configure the motorPin as a OUTPUT
  pinMode(motorPin, OUTPUT);

  // configure the serial
  Serial.begin(9600);


  while (!Serial);// waits for the serial monitor to open
}

void loop() {

  if (deviceSwitcher == 0) {
    //--------------------------------------------
    //temperature sensor and updater
    //--------------------------------------------

    // Take in the value from the sensor
    int sensorRaw = analogRead(0);// from analog sensor 0
    int sensor = (5.0 * sensorRaw * 100.0) / 1024;  // calculate the celsus from the raw value of the sensor
    // set the sensor value to the temperatureCount
    temperatureCount = sensor;

    //if the temp is the same, do nothing
    if (oldtemperatureCount == temperatureCount) {
      //temp is the same, no need to update it
    } else {

      //Setup for the content of the URL/web address

      // send the temperature to the Teacher API
      String url =  "http://verkstad.cc/iot/mmx/";
      url += 6; //teacher api
      url += "/index.php?temperatureCount=";
      url += temperatureCount ;

      // Sends the temperature to the api
      client.get(url); // sends the url to the client
      Serial.flush();//Waits for the transmission of outgoing serial data to complete

      Serial.print("Temp: ");
      Serial.println(temperatureCount);
      Serial.println("");

    }
    deviceSwitcher++;

  } else if (deviceSwitcher == 1) {

    //--------------------------------------------
    //clothesline servo
    //--------------------------------------------

    //Setup for the content of the URL/web address
    String url =  "http://verkstad.cc/iot/mmx/";
    url += groupOne ;
    url += "/get_value.php";
    // get tempControl state
    client.get(url); // sends the url to the client
    //temperature control state variable
    int tempControl = 0;
    // get the temperature control state and set it to the temperature control state variable
    while (client.available()) {
      char c = client.read();
      tempControl = tempControl * 10 + c - 48;
    }
    Serial.flush();//Waits for the transmission of outgoing serial data to complete


    //checks if the temp control is off/0 or anything else is on
    if (tempControl == 0) {
      // servo off state
      clotheslineservo.write(90);//90 is no rotation
    } else {
      // servo on state
      clotheslineservo.write(180);

    }
    Serial.println("Clothesline control");
    Serial.print("Temp control state: ");
    Serial.println(tempControl);
    Serial.println("");

    deviceSwitcher++;

  } else if (deviceSwitcher == 2) {
    //--------------------------------------------
    //vent the air solenoid
    //--------------------------------------------


    //Setup for the content of the URL/web address
    String url2 =  "http://verkstad.cc/iot/mmx/";
    url2 += groupTwo ;
    url2 += "/get_value.php";
    // get tempControl state
    client.get(url2); // sends the url to the client
    //temperature control state variable
    int tempControl2 = 0;
    // get the temperature control state and set it to the temperature control state variable
    while (client.available()) {
      char c = client.read();
      tempControl2 = tempControl2 * 10 + c - 48;
    }
    Serial.flush();//Waits for the transmission of outgoing serial data to complete

    //checks if the temp control is on/1 or anything else is off
    if (tempControl2 == 0) {
      // solenoid off
      digitalWrite(ventsolenoidPin, LOW);
    } else {
      // solenoid  on
      digitalWrite(ventsolenoidPin, HIGH);
    }

    Serial.println("Vent control");
    Serial.print("Temp control 2 state: ");
    Serial.println(tempControl2);
    Serial.println("");

    deviceSwitcher++;
  }
  else if (deviceSwitcher == 3) {
    //--------------------------------------------
    //shades servo
    //--------------------------------------------


    //Setup for the content of the URL/web address
    String url3 =  "http://verkstad.cc/iot/mmx/";
    url3 += groupThree ;
    url3 += "/get_value.php";
    // get tempControl state
    client.get(url3); // sends the url to the client
    //temperature control state variable
    int tempControl3 = 0;
    // get the temperature control state and set it to the temperature control state variable
    while (client.available()) {
      char c = client.read();
      tempControl3 = tempControl3 * 10 + c - 48;
    }
    Serial.flush();//Waits for the transmission of outgoing serial data to complete


    //checks if the temp control is off/0 or anything else is on
    if (tempControl3 == 0) {

      // servo off state
      shadesservo.write(30);

    } else {
      // servo on state
      shadesservo.write(180);
    }


    deviceSwitcher++;
  }
  else if (deviceSwitcher == 4) {
    //--------------------------------------------
    //water plant solenoid
    //--------------------------------------------

    //Setup for the content of the URL/web address
    String url4 =  "http://verkstad.cc/iot/mmx/";
    url4 += groupFour ;
    url4 += "/get_value.php";
    // get tempControl state
    client.get(url4);// sends the url to the client
    //temperature control state variable
    int tempControl4 = 0;
    // get the temperature control state and set it to the temperature control state variable
    while (client.available()) {
      char c = client.read();
      tempControl4 = tempControl4 * 10 + c - 48;
    }
    Serial.flush();//Waits for the transmission of outgoing serial data to complete


    //checks if the temp control is on/1 or anything else is off
    if (tempControl4 == 0) {
      // solenoid off
      digitalWrite(watersolenoid, LOW);
    } else {
      // solenoid on
      digitalWrite(watersolenoid, HIGH);
    }


    Serial.println("Water control");
    Serial.print("Temp control 4 state: ");
    Serial.println(tempControl4);
    Serial.println("");

    deviceSwitcher++;
  }
  else if (deviceSwitcher == 5) {
    //--------------------------------------------
    //fan dc motor
    //--------------------------------------------


    //Setup for the content of the URL/web address
    String url5 =  "http://verkstad.cc/iot/mmx/";
    url5 += groupFive ;
    url5 += "/get_value.php";
    // get tempControl state
    client.get(url5); // sends the url to the client
    //temperature control state variable
    int tempControl5 = 0;
    // get the temperature control state and set it to the temperature control state variable
    while (client.available()) {
      char c = client.read();
      tempControl5 = tempControl5 * 10 + c - 48;
    }

    Serial.flush();//Waits for the transmission of outgoing serial data to complete

    //checks if the temp control is off/0 or anything else is on
    if (tempControl5 == 0) {
      // motor/fan on

      digitalWrite(motorPin, LOW);

    } else {

      analogWrite(motorPin, 255);

    }

    Serial.println("Fan control");
    Serial.print("Temp control 5 state: ");
    Serial.println(tempControl5);
    Serial.println("");
    deviceSwitcher = 0;
  }
  delay(2000); //wait 2 sec

  //reset all motors/servos
  digitalWrite(motorPin, LOW);
  digitalWrite(watersolenoid, LOW);
  shadesservo.write(30);
  digitalWrite(ventsolenoidPin, LOW);
  clotheslineservo.write(90);

  oldtemperatureCount = temperatureCount;// update the old temperature to the current temperature count

}



