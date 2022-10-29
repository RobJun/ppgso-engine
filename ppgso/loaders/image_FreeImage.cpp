#pragma once
#include "image_FreeImage.h"

namespace ppgso {
	namespace image {
		ppgso::Image loadFI(const std::string& filename) {
			FIBITMAP* dib = FreeImage_Load(FreeImage_GetFileType(filename.c_str()), filename.c_str());
			//dib = FreeImage_ConvertTo24Bits(dib);
			std::cout << (int)FreeImage_GetWidth(dib) <<" "<< (int)FreeImage_GetHeight(dib) << std::endl;
			int width = FreeImage_GetWidth(dib), height = FreeImage_GetHeight(dib);
			Image image{ width ,height };
			FreeImage_FlipVertical(dib);
			dib = FreeImage_ConvertTo24Bits(dib);

			auto& framebuffer = image.getFramebuffer();
			auto* pixels = FreeImage_GetBits(dib);
			for (int pix = 0; pix < width * height; pix++)
			{
			    framebuffer[pix].r = pixels[pix * 3 + 2];
				framebuffer[pix].g = pixels[pix * 3 + 1];
				framebuffer[pix].b = pixels[pix * 3 + 0];
			}
			FreeImage_Unload(dib);
			return image;
		}

		void saveFI(ppgso::Image& image, const std::string& filename) {}
	}
}