#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <math.h>

// any pins can be used
#define SCK 10
#define MOSI 9
#define SS 8

Adafruit_SharpMem display(SCK, MOSI, SS);

#define BLACK 0
#define WHITE 1

float pi = 3.14159265358979;
float dx;
float dy;
int angle = 0;
int quadrant = 0; // quadrant the arrow will lie in
int care_dir_face = 50; // direction caregiver is facing, in degrees
int care_alz_dir = 350; // NSEW direction caregiver needs to travel to reach patient
int adj_care_alz_dir = 0; // angle from caregiver to patient, where 0 == direction caregiver is facing


void setup(void)
{
  Serial.begin(9600);
  Serial.println("Hello!");

  // start & clear the display
  display.begin();
  display.clearDisplay();
  
  // if direction to patient is less than the direction caregiver is facing, adjust caregiver's facing direction to zero, leaving other positive
  if (care_alz_dir < care_dir_face) {
    adj_care_alz_dir = care_alz_dir + 360 - care_dir_face;
  }
  // if direction to patient is greater than direction caregiver is facing, adjust
  else if (care_alz_dir >= care_dir_face) {
    adj_care_alz_dir = care_alz_dir - care_dir_face;
  }
  
  
  //calculate change in x and y
  if (adj_care_alz_dir == 0) {
    dx = 0;
    dy = 1;
  }
  else if (0 < adj_care_alz_dir < 90) {
    dx = sin(adj_care_alz_dir*pi/180);
    dy = cos(adj_care_alz_dir*pi/180);
    display.drawLine(30,50,30+dx*10,50-dy*10,BLACK);
  }
  else if (adj_care_alz_dir == 90) {
    dx = 1;
    dy = 0;
  }
  else if (90 < adj_care_alz_dir < 180) {
    dx = cos(adj_care_alz_dir*pi/180);
    dy = sin(adj_care_alz_dir*pi/180);
    display.drawLine(30,50,30+dx*10,50+dy*10,BLACK);
  }
  else if (adj_care_alz_dir == 180) {
    dx = 0;
    dy = -1;
  }
  else if (180 < adj_care_alz_dir < 270) {
    dx = sin(adj_care_alz_dir*pi/180);
    dy = cos(adj_care_alz_dir*pi/180);
    display.drawLine(30,50,30+dx*10,50+dy*10,BLACK);
  }
  else if (adj_care_alz_dir == 270) {
    dx = -1;
    dy = 0;
  }
  else if (270 < adj_care_alz_dir < 360) {
    dx = cos(adj_care_alz_dir*pi/180);
    dy = sin(adj_care_alz_dir*pi/180);
    display.drawLine(30,50,30+dx*10,50-dy*10,BLACK);
  }
  
  
  
  Serial.println(dx);
  Serial.println(dy);
  
  
  //display.drawLine(30,30,30+dx*10,30-dy*10,BLACK);
  //display.drawLine(30,30,dx*2, dy*(10)+30, BLACK);
  
  
//  if (angle == 0) {
//    display.drawLine(7,7,7,17,BLACK);
//    display.drawLine(7,7,4,10,BLACK);
//    display.drawLine(7,7,10,10,BLACK);
//    delay(1000);
//  }
}


void loop(void) 
{
  // Screen must be refreshed at least once per second
  display.refresh();
  delay(500);
}
