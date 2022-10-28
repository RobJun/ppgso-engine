// Task 3 - Implement Bresenham drawing alg.
//        - Draw a star using lines
//        - Make use of std::vector to define the shape
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include <ppgso/ppgso.h>

# define M_PI           3.14159265358979323846

// Size of the framebuffer
const unsigned int SIZE = 512;

struct Point {
  int x,y;

  Point distance(const Point& to) {
      return { abs(x - to.x), abs(y - to.y) };
  }

};
Point pCalc(const float& x, const float& y) {
    return { 256 + (int)(x * 255),256 + (int)(y * 255) };
}

void EightWaySymmetricPlot(ppgso::Image& framebuffer, Point c, Point p)
{
    framebuffer.setPixel(p.x + c.x, p.y + c.y, 255,255,255);
    framebuffer.setPixel(p.x + c.x, -p.y + c.y, 255, 255, 255);
    framebuffer.setPixel(-p.x + c.x, -p.y + c.y, 255, 255, 255);
    framebuffer.setPixel(-p.x + c.x, p.y + c.y, 255, 255, 255);
    framebuffer.setPixel(p.y + c.x, p.x + c.y, 255, 255, 255);
    framebuffer.setPixel(p.y + c.x, -p.x + c.y, 255, 255, 255);
    framebuffer.setPixel(-p.y + c.x, -p.x + c.y, 255, 255, 255);
    framebuffer.setPixel(-p.y + c.x, p.x + c.y, 255, 255, 255);
}


// Bresenham drawing algorithm
void drawLine(ppgso::Image& framebuffer, Point& from, Point& to) {
  // TODO: Implement Bresenham drawing algorithm
    Point d = from.distance(to);
    if (d.x > d.y) {
        int y = from.y;
        int x = 0;
        int e = 2 * d.y - d.x;
        while (x <= d.x) {
            framebuffer.setPixel(from.x + (from.x < to.x ? x : -x), y, 255, 255, 255);
            if (e >= 0) {
                y = y + (from.y < to.y ? 1 : -1);
                e += 2 * d.y - 2 * d.x;
            }
            else {
                e += 2 * d.y;
            }
            x++;
        }
    }
    else {
        int y = 0;
        int x = from.x;
        int e = 2 * d.x - d.y;
        while (y <= d.y) {
            framebuffer.setPixel(x,from.y + (from.y < to.y ? y : -y), 255, 255, 255);
            if (e >= 0) {
                x = x + (from.x < to.x ? 1 : -1);
                e += 2 * d.x - 2 * d.y;
            }
            else {
                e += 2 * d.x;
            }
            y++;
        }
    }
}

void drawCircle(ppgso::Image& framebuffer,Point center, int radius) {
    int d = 3 - 2 * radius;
    Point p = { 0,radius };

    while (p.x <= p.y) {
        if (d <= 0)
        {
            d = d + (4 * p.x) + 6;
        }
        else
        {
            d = d + (4 * p.x) - (4 * p.y) + 10;
            p.y = p.y - 1;
        }
        p.x++;
        EightWaySymmetricPlot(framebuffer,center,p);
    }
}

int main()
{
  // Use ppgso::Image as our framebuffer
  ppgso::Image framebuffer(SIZE, SIZE);
  // TODO: Generate star points
  std::vector<Point> points = {
                pCalc(1.f,0.f) ,
                pCalc(0.5f*sin(M_PI/4),0.5f * cos(M_PI / 4)),
                pCalc(0,1.f),
                pCalc(-0.5f * sin(M_PI / 4),0.5f * cos(M_PI / 4)),
                pCalc(-1.f,0.f),
                pCalc(-0.5f * sin(M_PI / 4),-0.5f * cos(M_PI / 4)),
                pCalc(0.f,-1.f),
                pCalc(0.5f * sin(M_PI / 4),-0.5f * cos(M_PI / 4)),
                pCalc(1.f,0.f) };

  

  //std::vector<Point> points = { {511,256},{346,346},{256,511},{166,346},{0,256},{166,166},{256,0},{346,166},{511,256},{511,256} };
  // Draw lines
  for(unsigned int i = 0; i < points.size() - 1; i++)
    drawLine(framebuffer, points[i], points[i+1]);

  drawCircle(framebuffer, pCalc(0.f, 0.f), 125);

  // Save the result
  std::cout << "Generating task2_bresenham.bmp file ..." << std::endl;
  ppgso::image::saveBMP(framebuffer, "task2_bresenham.bmp");

  std::cout << "Done." << std::endl;
  return EXIT_SUCCESS;
}

