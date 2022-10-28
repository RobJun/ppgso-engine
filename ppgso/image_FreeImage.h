#pragma once
#include <FreeImage/FreeImage.h>
#include <iostream>
#include "image.h"

namespace ppgso {
	namespace image {
		/*!
		 * Load RAW image from file. Only uncompressed RGB format is supported.
		 *
		 * @param raw - File path to a RAW image.
		 */
		ppgso::Image loadFI(const std::string& filename);

		/*!
		 * Save as RAW image.
		 * @param image - Image to save.
		 * @param raw - Name of the RAW file to save image to.
		 */
		void saveFI(ppgso::Image& image, const std::string& filename);
	}
}