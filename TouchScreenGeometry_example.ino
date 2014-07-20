/* 
  Title: TouchScreenGeometry Tutorial
  About: Example/tutorial file for how to create geometric shapes with the TouchScreenGeometry library
  Author: Richard Kirkpatrick
  Date: 20 July 2014
*/

// Libraries
#include <stdint.h>  
#include <TouchScreen.h>  // Library for the Seeed Studio TFT Touch Shield 
#include <TFT.h>      // Library for the Seeed Studio TFT Touch Shield 
#include <Password.h> // http://www.arduino.cc/playground/uploads/Code/Password.zip
#include <TouchScreenGeometry.h>  // Library for drawing shapes for the keypad
#include <TouchScreenStrings.h> // Library for drawing strings for the keypad

#ifdef SEEEDUINO
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 14   // can be a digital pin, this is A0
  #define XP 17   // can be a digital pin, this is A3 
#endif

#ifdef MEGA
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 54   // can be a digital pin, this is A0
  #define XP 57   // can be a digital pin, this is A3 
#endif 

//Measured ADC values for (0,0) and (210-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1
#define TS_MINX 140
#define TS_MAXX 900
#define TS_MINY 120
#define TS_MAXY 940

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Function prototypes
void eraseScreen();

void setup() 
{
  Tft.init();             // Initializes the TFT library
  Serial.begin(9600);     // Used for debugging
}

void loop() 
{
  polygonExamples();
  rectangleExamples();
  circleExamples();
  triangleExamples();
  moveExamples();
}

void rectangleExamples()
{
  int timeInterval = 1000;
  // Test Case 1 - Tests the Rectangle class
  Rectangle r1; // Default constructor
  Rectangle r2(10, 20, 40, 50, WHITE, BLUE); // Parameter constructor
  Rectangle r3 = r2; // Copy constructor
  r2.draw(); // Draws rectangle r2
  r2.fill(); // Fill rectangle r2
  delay(timeInterval);
  delay(timeInterval);
  r1.setValues(100, 150, 45, 35); // Sets r1 xstart, ystart, width, height
  r1.draw(); // Draws the rectangle
  r1.fill(); // Fills the rectangle
  delay(timeInterval);
  r1.setFillColor(GREEN); // Sets the fill color to Green
  r1.fill();  // Re-fills the rectangle
  delay(timeInterval);
  r2.setBorderColor(BLACK); // Sets border color to black
  r2.setFillColor(BLACK); // Sets the fill color to black
  r2.draw(); // Erases the rectangle
  r2.fill(); // Erases the rectangle
  delay(timeInterval);
  r2.setSize(20, 25); // Resizes the rectangle
  r2.setUpperLeft(45, 90); // Sets the x and y coordinates
  r2.setBorderColor(RED);  // Sets the border color to red
  r2.setFillColor(YELLOW);  // Set the fill color to yellow
  r2.draw();
  r2.fill(); 
  eraseScreen(); 
  r2.setFillColor(RED);
  
  // Rescale 
  for(int i = 0; i < 20; i++) {
    r2.scale(1.1);
    delay(50);
  }
  delay(timeInterval);
  for(int i = 0; i < 20; i++) {
    r2.scale(0.9);
    delay(50);
  }
  // Getter methods
  Serial.println("Test");
  Serial.println(r1.getXStart()); // Should be 100
  Serial.println(r1.getYStart()); // Should be 150
  Serial.println(r1.getXEnd()); // Should be 145
  Serial.println(r1.getYEnd()); // Should be 185
  Serial.println(r1.getBorderColor()); // Should be WHITE
  Serial.println(r1.getFillColor()); // Should be GREEN
  delay(3000);
  eraseScreen();  
}

void polygonExamples()
{
  int timeInterval = 1000;
  // Create the points that the define the Polygon vertices
  Point2D p1(50, 10);
  Point2D p2(90, 40);
  Point2D p3(90, 80);
  Point2D p4(50, 110);
  Point2D p5(10, 80);
  Point2D p6(10, 40);
  Point2D points[] = {p1, p2, p3, p4, p5, p6};
  Point2DArray vertices(points, 6);
  Polygon poly1(vertices);
  poly1.draw();
  delay(timeInterval);
  poly1.setBorderColor(YELLOW);
  poly1.draw();
  delay(timeInterval);
  eraseScreen(); 
}


void circleExamples() 
{
  int timeInterval =3000;
  // Test Case 2 - Tests the Circle class
  Circle c1(10, 20, 10, WHITE, BLUE); // Parameter constructor
  Circle c2 = c1; // Copy constructor
  c1.draw(); // Draws Circle c1
  c1.fill(); // Fill Circle c1
  delay(timeInterval);
  c1.setFillColor(GREEN); // Sets the fill color to Green
  c1.fill();  // Re-fills the Circle
  delay(timeInterval);
  c1.setBorderColor(BLACK); // Sets border color to black
  c1.setFillColor(BLACK); // Sets the fill color to black
  c1.draw(); // Erases the Circle
  c1.fill(); // Erases the Circle
  delay(timeInterval);
  c1.setRadius(55); // Resizes the Circle
  c1.setCentroid(100, 100); // Moves the circle's centroid to (100, 100)
  c1.setBorderColor(RED); 
  c1.setFillColor(YELLOW); 
  c1.draw();
  c1.fill();
  delay(timeInterval);
  eraseScreen();
  // Scale method
  for (int i = 0; i < 5; i++) {
    c1.scale(1.1);
    delay(50);
  }
  
  for (int i = 0; i < 5; i++) {
    c1.scale(0.8);
    delay(50);
  }
  
  // Getter methods
  Serial.println("Test");
  Serial.println(c1.getXCoord()); // Should be 100
  Serial.println(c1.getYCoord()); // Should be 150
  Serial.println(c1.getRadius()); // Should be 35
  Serial.println(c1.getBorderColor()); // Should be WHITE
  Serial.println(c1.getFillColor()); // Should be GREEN
  delay(3000);
  eraseScreen();
}

void triangleExamples()
{
  // Define the vertices for the triangle
  Point2D p1(20, 20);
  Point2D p2(30, 30);
  Point2D p3(10, 30);
  // Create the triangle
  Triangle t1(p1, p2, p3, YELLOW);
  t1.draw(); 
  delay(1000);
  // Create another triangle and set its border color to red
  Triangle t2(Point2D(50, 50), Point2D(100, 100), Point2D(25, 100), RED);
  // Draw the 2nd triangle
  t2.draw();
  delay(1000);
  eraseScreen();
}


void moveExamples() 
{
  // Test Case 3 - Test the movements of shapes
  Rectangle r1(20, 100, 40, 50, WHITE, BLUE); // Parameter constructor 
  Circle c1(150, 100, 30, GREEN, GREEN); // Parameter constructor 
  r1.draw();
  c1.draw();
  // Move the circle and rectangle down 2 pixels every 10 msec
  for(int y = 0; y < 20; y++) {
    r1.move(0, -1);
    c1.move(0, -1);
    delay(10);
  }
  delay(1000);
  eraseScreen();
  
  // Move the circle and rectangle
  for(int y = 0; y < 20; y++) {
      r1.move(0, 1);
      c1.move(0, 1);
      delay(10);
  }
 delay(250);
 
 for(int y = 0; y < 40; y++) {
    r1.move(1, -1);
    c1.move(-1, -1); 
    delay(10);
 }
 
 for(int y = 0; y < 40; y++) {
   r1.move(-1, 1);
   c1.move(1, 1);
   delay(10);
 }
 
 eraseScreen();
}


void eraseScreen() 
{
  // Erases entire screen
  Tft.fillRectangle(0, 0, 240, 320, BLACK);
}

