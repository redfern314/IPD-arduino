#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

// any pins can be used
#define SCK 10
#define MOSI 9
#define SS 8
#define BLACK 0
#define WHITE 1

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

Adafruit_SharpMem display(SCK, MOSI, SS);

int newData = 1;

char alz_name[] = "George";
char alz_add_1[] = "1000 Olin Way";
char alz_add_2[] = "Needham, MA";

float pi = 3.14159265358979;
float dx;
float dy;
int angle = 0;
int care_dir_face = 50; // direction caregiver is facing, in degrees
int care_alz_dir = 350; // NSEW direction caregiver needs to travel to reach patient
int adj_care_alz_dir = 330; // angle from caregiver to patient, where 0 == direction caregiver is facing

void drawAndRefresh(int heading) {
  if (newData) {
    newData = 0;

    // draw text
    display.clearDisplay();
    // display.setTextSize(1);
    // display.setTextColor(BLACK);
    // display.setCursor(2,2);
    // display.println(alz_name);
    // display.setCursor(2,12);
    // display.println("has wandered to");
    // display.setCursor(2,22);
    // display.println(alz_add_1);
    // display.setCursor(2,32);
    // display.println(alz_add_2);

    // draw arrow
    drawArrow(heading);
  }
  display.refresh();
}

void drawArrow(int heading) {
  #define arrowlength     20
  #define arrowheadlength 10
  #define arrowstartX     30
  #define arrowstartY     60

  int adj_care_alz_dir = heading;
  dx = sin(adj_care_alz_dir*pi/180);
  dy = cos(adj_care_alz_dir*pi/180);
  float px1 = dx*arrowlength+arrowstartX;
  float py1 = dy*arrowlength+arrowstartY;
  display.drawLine(arrowstartX,arrowstartY,px1,py1,BLACK);

  dx = sin((adj_care_alz_dir+150)*pi/180);
  dy = cos((adj_care_alz_dir+150)*pi/180);
  float px2 = dx*arrowheadlength+px1;
  float py2 = dy*arrowheadlength+py1;
  display.drawLine(px1,py1,px2,py2,BLACK);

  dx = sin((adj_care_alz_dir+210)*pi/180);
  dy = cos((adj_care_alz_dir+210)*pi/180);
  px2 = dx*arrowheadlength+px1;
  py2 = dy*arrowheadlength+py1;
  display.drawLine(px1,py1,px2,py2,BLACK);
}

void setup(void) 
{
  Serial.begin(9600);
  Serial1.begin(9600);

  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
}

float getHeading() {
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.22;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI;
  
  return(headingDegrees);
}

void loop(void) 
{
  // clear GPS serial buffer to get new data
  // while(Serial1.available()) {
  //   Serial1.readln();
  // }
  // delay(10);

  // // get GPS location
  // char GPS[100];
  // while (Serial1.available()) {
  //   char incoming[100];
  //   Serial1.readBytesUntil('\n',incoming,100);
  //   if(strncmp(incoming,"$GPRMC",6)==0) {
  //     strcpy(GPS,incoming);
  //     break;
  //   }
  // }

  // get magnetic heading
  int heading = (int)getHeading();

  // send location and heading to server
  // Serial.print("Heading,")
  // Serial.println(heading);
  // Serial.print("GPS,");
  // Serial.println(GPS);

  // // wait for the server to send back data, then read it
  // while(!Serial.available()) {}
  // char address[100];
  // char reldir[4];

  // // read address
  // int bytes = Serial.readBytesUntil('|',address,100);
  // address[bytes] = '\0'; // remove the trailing | character

  // // read direction
  // bytes = Serial.readBytes(reldir,3); // TODO: make sure we're sending 3 bytes

  // update and refresh screen
  newData = 1;
  drawAndRefresh(heading);

  // wait 500ms and repeat
  delay(500);
}