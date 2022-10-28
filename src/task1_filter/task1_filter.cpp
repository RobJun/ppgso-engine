// Task 1 - Load a 512x512 image lena.raw
//        - Apply specified per-pixel transformation to each pixel
//        - Save as result.raw
#include <fstream>
#include <iostream>

// Size of the framebuffer
const unsigned int SIZE = 512;
const double R = 0.f, G = 0.f, B = 1.f;

// A simple RGB struct will represent a pixel in the framebuffer
struct Pixel {
  // TODO: Define correct data type for r, g, b channel
    char r;
    char g;
    char b;
};

int main()
{
  // Initialize a framebuffer
  auto framebuffer = new Pixel[SIZE][SIZE];

  // TODO: Open file lena.raw (this is 512x512 RAW GRB format)
  std::ifstream image_stream("./lena.raw", std::ios::binary);
  if (!image_stream.is_open()) {
      std::cout << "error reading a file";
      return EXIT_FAILURE;
  }
  // TODO: Read data to framebuffer and close the file
  image_stream.read((char*)framebuffer, SIZE * SIZE*3);
  std::cout << "written to dataframe" << std::endl;
  image_stream.close();

  // Traverse the framebuffer
  for (unsigned int y = 0; y < SIZE; y++) {
    for (unsigned int x = 0; x < SIZE; x++) {
        framebuffer[y][x].r = (char)(((framebuffer[y][x].r / 255.f) * R + ((x / 511.f)) * (1 - R)) * 255);
        framebuffer[y][x].g = (char)(((framebuffer[y][x].g / 255.f) * G + ((x / 511.f)) * (1 - G)) * 255);
        framebuffer[y][x].b = (char)(((framebuffer[y][x].b / 255.f) * B + ((x / 511.f)) * (1 - B)) * 255);
    }
  }

  // TODO: Open file result.raw
  std::cout << "Generating result.raw file ..." << std::endl;
  std::ofstream output("./result.raw", std::ios::binary);
  output.write((char*)framebuffer, SIZE * SIZE * 3);


  output.close();

  std::cout << "Done." << std::endl;
  delete[] framebuffer;
  return EXIT_SUCCESS;
}
