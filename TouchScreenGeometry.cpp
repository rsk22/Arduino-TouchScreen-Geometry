/*
  TouchScreenGeometry.h - Library for creating geometries shapes for the Seeed Studio TFT touch screen.
  14 June 2014: Created by Richard Kirkpatrick
  Revisions:
    -@date 14 June 2014:
        - Richard Kirkpatrick: Added Triangle class.
    -@date 16 June 2014:
        - Richard Kirkpatrick: Added Point2D and Point2DArray classes.

  Copyright: This material is influenced by material from MIT OpenCourseWare which operates under a
  Creative Common License.  For more info, visit http://ocw.mit.edu/terms/

  A lot of the material (RectangleArray, CircleArray, ...) was created by myself.  Please feel free to use this work
  as you wish but PLEASE give credit to MIT OpenCourseWare and myself when using this work.
*/

#include "Arduino.h"
#include "TouchScreenGeometry.h"

#include <stdint.h>
#include <TouchScreen.h>
#include <TFT.h>
#include <math.h>

/*
----------------------------------------------------------------
                          Point2D Class
----------------------------------------------------------------
*/

Point2D::Point2D() : x(0), y(0)
{
}

Point2D::Point2D(const int myX, const int myY) : x(myX), y(myY)
{
}

Point2D::Point2D(Point2D &otherPoint)
{
    x = otherPoint.getX();
    y = otherPoint.getY();
}

const int Point2D::getX()
{
    return x;
}

const int Point2D::getY()
{
    return y;
}

void Point2D::setX(const int myX)
{
    x = myX;
}

void Point2D::setY(const int myY)
{
    y = myY;
}

/*
----------------------------------------------------------------
                         Point2DArray Class
----------------------------------------------------------------
*/

Point2DArray::Point2DArray()
{
    size = 0;
    points = new Point2D[0]; // Allows deleting later
}

Point2DArray::Point2DArray(const Point2D ptsToCopy[], const int toCopySize)
{
    size = toCopySize;
    points = new Point2D[size];
    for(int i = 0; i < toCopySize; i++) {
        points[i] = ptsToCopy[i];
    }
}

Point2DArray::Point2DArray(const Point2DArray &other)
{
    size = other.size;
    points = new Point2D[size];
    for (int i = 0; i < size; i++) {
        points[i] = other.points[i];
    }
}

Point2DArray::~Point2DArray()
{
    delete [] points;
}

int Point2DArray::getSize()
{
    return size;
}

void Point2DArray::resize(int newSize)
{
    Point2D *pts = new Point2D[newSize];
    int minSize = (newSize > size ? size : newSize);
    for (int i = 0; i < minSize; i++)
        pts[i] = points[i];
    delete [] points;
    size = newSize;
    points = pts;
}

void Point2DArray::clear()
{
    resize(0);
}

void Point2DArray::pushBack(const Point2D &p)
{
    insert(size, p);
}

void Point2DArray::setPoint(const int pos, const Point2D &point)
{
    points[pos] = point;
}

void Point2DArray::insert(const int pos, const Point2D &p)
{
    resize(size + 1);

    for (int i = size - 1; i > pos; i--)
        points[i] = points[i - 1];

    points[pos] = p;
}

void Point2DArray::remove(const int pos)
{
    if (pos >= 0 && pos < size) {
        for (int i = pos; i < size - 2; i++) {
            points[i] = points[i + 1];
        }
        resize(size - 1);
    }
}

Point2D *Point2DArray::get(const int pos)
{
    return pos >= 0 && pos < size ? points + pos : NULL;
}

const Point2D *Point2DArray::get(const int pos) const
{
    return pos >= 0 && pos < size ? points + pos : NULL;
}

/*
----------------------------------------------------------------
                    Polygon Base Class
----------------------------------------------------------------
*/

int Polygon::numPolygons = 0;

Polygon::Polygon(const Point2DArray &pa, const unsigned int myBorderColor, const unsigned int myFillColor )
    : points(pa), borderColor(myBorderColor), fillColor(myFillColor)
{
    ++numPolygons;
}

Polygon::Polygon(const Point2D points[], const int numPoints, const unsigned int myBorderColor, const unsigned int myFillColor)
    : points(points, numPoints), borderColor(myBorderColor), fillColor(myFillColor)
{
    ++numPolygons;
}

void Polygon::setBorderColor(const unsigned int myBorderColor)
{
    borderColor = myBorderColor;
}

void Polygon::setFillColor(const unsigned int myFillColor)
{
    fillColor = myFillColor;
}

const unsigned int Polygon::getBorderColor()
{
    return borderColor;
}

const unsigned int Polygon::getFillColor()
{
    return fillColor;
}

void Polygon::draw()
{
    // Draw each segment of the polygon
    for (int i = 0; i < getNumSides() - 1; i++) {
        Tft.drawLine(points.get(i)->getX(), points.get(i)->getY(), points.get(i+1)->getX(), points.get(i+1)->getY(), borderColor);
    }
    Tft.drawLine(points.get(getNumSides()-1)->getX(), points.get(getNumSides()-1)->getY(), points.get(0)->getX(), points.get(0)->getY(), borderColor);
}

/*
----------------------------------------------------------------
				  Rectangle Subclass
----------------------------------------------------------------
*/

Point2D *updateVertices(const Point2D &a, const Point2D &b, const Point2D &c, const Point2D &d = Point2D(0, 0))
{
    Point2D vertices[4];
    vertices[0] = a;
    vertices[1] = b;
    vertices[2] = c;
    vertices[3] = d;
    return vertices;
}


Rectangle::Rectangle() : Polygon(updateVertices(Point2D(), Point2D(), Point2D(), Point2D()), 4, 0xffff, 0x0000)
{
}

Rectangle::Rectangle(Point2D &upperLeft, Point2D &lowerRight, const unsigned int myBorderColor, const unsigned int myFillColor)
    : Polygon(updateVertices(upperLeft, Point2D(lowerRight.getX(), upperLeft.getY()), lowerRight, Point2D(upperLeft.getX(), lowerRight.getY())),
              4, myBorderColor, myFillColor)
{
}

Rectangle::Rectangle(const int myXStart, const int myYStart, const int myWidth, const int myHeight, const unsigned int myBorderColor, const unsigned int myFillColor)
    : Polygon(updateVertices(Point2D(myXStart, myYStart), Point2D(myXStart + myWidth, myYStart), Point2D(myXStart + myWidth, myYStart + myHeight), Point2D(myXStart, myYStart + myHeight)),
              4,  myBorderColor, myFillColor)
{
}

void Rectangle::setUpperLeft(const int myXStart, const int myYStart)
{
    const int tempWidth = getWidth();
    const int tempHeight = getHeight();
    points.setPoint(0, Point2D(myXStart, myYStart));
    points.setPoint(1, Point2D(myXStart + tempWidth, myYStart));
    points.setPoint(2, Point2D(myXStart + tempWidth, myYStart + tempHeight));
    points.setPoint(3, Point2D(myXStart, myYStart + tempHeight));
}

void Rectangle::setValues(const int myXStart, const int myYStart, const int myWidth, const int myHeight)
{
    setUpperLeft(myXStart, myYStart);
    setSize(myWidth, myHeight);
}

void Rectangle::setSize(const int myWidth, const int myHeight)
{
    int tempXStart = getXStart();
    int tempYStart = getYStart();
    points.setPoint(1, Point2D(tempXStart + myWidth, tempYStart));
    points.setPoint(2, Point2D(tempXStart + myWidth, tempYStart + myHeight));
    points.setPoint(3, Point2D(tempXStart, tempYStart + myHeight));
}

const int Rectangle::getXStart()
{
    return points.get(0)->getX();
}

const int Rectangle::getYStart()
{
    return points.get(0)->getY();
}

const int Rectangle::getXEnd()
{
    return points.get(2)->getX();
}

const int Rectangle::getYEnd()
{
    return points.get(2)->getY();
}

const int Rectangle::getWidth()
{
    return abs(getXEnd() - getXStart());
}

const int Rectangle::getHeight()
{
    return abs(getYEnd() - getYStart());
}

void Rectangle::fill()
{
    Tft.fillRectangle(getXStart() + 1, getYStart() + 1, getWidth() - 1, getHeight() - 1, fillColor);
}

void Rectangle::move(const int dx, const int dy)
{
    unsigned int previousBorderColor = borderColor; // Saves the border color.
    unsigned int previousFillColor = fillColor; // Saves the fill color
    setBorderColor(0x0000); // Sets the border color to black.
    setFillColor(0x0000); // Sets the fill color to black
    draw(); // Erases the rectangle on the screen.
    fill(); // Erases the rectangle on the screen
    setUpperLeft(getXStart() + dx, getYStart() + dy); // Translate the x-coordinates of the rectangle
    setBorderColor(previousBorderColor); // Sets the border color back to its original state.
    setFillColor(previousFillColor); // Sets the fill color back to its original state.
    draw(); // Redraws the rectangle at its translated coordinates.
    fill(); // Redraws the rectangle at its translated coordinates.
}

void Rectangle::scale(const float factor)
{
    unsigned int previousBorderColor = borderColor; // Saves the border color.
    unsigned int previousFillColor = fillColor; // Saves the fill color
    setBorderColor(0x0000); // Sets the border color to black.
    setFillColor(0x0000); // Sets the fill color to black
    draw(); // Erases the rectangle on the screen.
    fill(); // Erases the rectangle on the screen
    setSize(int(factor * getWidth()), int(factor * getHeight())); // Resizes the rectangle.
    setBorderColor(previousBorderColor); // Sets the border color back to its original state.
    setFillColor(previousFillColor); // Sets the fill color back to its original state.
    draw(); // Redraws the rectangle at its translated coordinates.
    fill(); // Redraws the rectangle at its translated coordinates.
}


/*
----------------------------------------------------------------
				  Triangle Subclass
----------------------------------------------------------------
*/
Triangle::Triangle(const Point2D &a, const Point2D &b, const Point2D &c, const unsigned myBorderColor, const unsigned int myFillColor)
    : Polygon(updateVertices(a, b, c), 3, myBorderColor, myFillColor)
{
}

/*
----------------------------------------------------------------
				  Circle Subclass
----------------------------------------------------------------
*/

Circle::Circle(const Point2D &myCentroid, const int myRadius, const unsigned int myBorderColor, const unsigned int myFillColor)
{
    centroid = myCentroid;
    radius = myRadius;
    borderColor = myBorderColor;
    fillColor = myFillColor;
}

Circle::Circle(const int myXStart, const int myYStart, const int myRadius, const unsigned int myBorderColor, const unsigned int myFillColor)
{
    centroid.setX(myXStart);
    centroid.setY(myYStart);
    radius = myRadius;
    borderColor = myBorderColor;
    fillColor = myFillColor;
}

void Circle::setRadius(const int myRadius)
{
    radius = myRadius;
}

const int Circle::getRadius()
{
    return radius;
}

void Circle::setBorderColor(unsigned int myBorderColor )
{
    borderColor = myBorderColor;
}

void Circle::setFillColor(unsigned int myFillColor)
{
    fillColor = myFillColor;
}

const unsigned int Circle::getBorderColor()
{
    return borderColor;
}

const unsigned int Circle::getFillColor()
{
    return fillColor;
}

void Circle::draw()
{
    Tft.drawCircle(centroid.getX(), centroid.getY(), radius, borderColor);
}

void Circle::fill()
{
    Tft.fillCircle(centroid.getX(), centroid.getY(), radius - 1, fillColor);
}

void Circle::move(const int dx, const int dy)
{
    unsigned int previousBorderColor = borderColor; // Saves the border color.
    unsigned int previousFillColor = fillColor; // Saves the fill color
    setBorderColor(0x0000); // Sets the border color to black.
    setFillColor(0x0000); // Sets the fill color to black
    draw(); // Erases the circle on the screen.
    fill(); // Erases the rectangle on the screen
    centroid.setX(centroid.getX() + dx);
    centroid.setY(centroid.getY() - dy);
    setBorderColor(previousBorderColor); // Sets the border color back to its original state.
    setFillColor(previousFillColor); // Sets the fill color back to its original state.
    draw(); // Redraws the circle at its translated coordinates.
    fill(); // Redraws the rectangle at its translated coordinates.
}

void Circle::scale(const float factor)
{
    unsigned int previousBorderColor = borderColor; // Saves the border color.
    unsigned int previousFillColor = fillColor; // Saves the fill color
    setBorderColor(0x0000); // Sets the border color to black.
    setFillColor(0x0000); // Sets the fill color to black
    fill(); // Erases the rectangle on the screen
    draw(); // Erases the rectangle on the screen.
    setRadius(int(factor * radius)); // Resizes the rectangle.
    setBorderColor(previousBorderColor); // Sets the border color back to its original state.
    setFillColor(previousFillColor); // Sets the fill color back to its original state.
    draw(); // Redraws the rectangle at its translated coordinates.
    fill(); // Redraws the rectangle at its translated coordinates.
}








