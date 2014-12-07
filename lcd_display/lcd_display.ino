#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

// any pins can be used
#define SCK 10
#define MOSI 9
#define SS 8

Adafruit_SharpMem display(SCK, MOSI, SS);

#define BLACK 0
#define WHITE 1

char alz_name[] = "Dorothy";
char alz_add_1[] = "1000 Olin Way";
char alz_add_2[] = "Needham, MA";
int care_alz_dir = 300;
int care_dir_face = 200;

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Hello!");
  
  // start & clear the display
  display.begin();
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(2,2);
  display.println(alz_name);
  display.setCursor(2,12);
  display.println("has wandered to");
  display.setCursor(2,22);
  display.println(alz_add_1);
  display.setCursor(2,32);
  display.println(alz_add_2);
  display.println();
  display.refresh();
  delay(10000);
  display.clearDisplay();

}

void loop(void) 
{
  // Screen must be refreshed at least once per second
  display.refresh();
  delay(500);
}


