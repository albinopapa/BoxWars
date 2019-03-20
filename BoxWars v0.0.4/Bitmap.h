//make sure that the header file is included only once into each one of the .cpp files
#pragma once

#include "Color.h"

#include <memory>

namespace Bitmap
{
	struct BitmapFileHeader
	{
		std::uint32_t fileSize;
		std::uint16_t reserved1, reserved2;
		std::uint32_t offsetToPixelData;
	};
	struct BitmapInfoHeader
	{	
		std::uint32_t headerSize;
		std::uint32_t width, height;

		std::uint16_t planes;
		std::uint16_t bits;

		std::uint32_t compression;
		std::uint32_t imageSize;

		std::int32_t xResolution, yResolution;

		std::uint32_t nColors;
		std::uint32_t importantColours;
	};

	struct Pixel24
	{
		std::uint8_t blue;
		std::uint8_t green;
		std::uint8_t red;
	};

	struct ImageData
	{
		std::uint32_t width = 0u, height = 0u;
		std::unique_ptr<Color[]> pixels;
	};

	//we name it 'Loadbmp' and not 'LoadBitmap' to avoid a NAME-SPACE collision [two functions with the same name confuse the compiler]
	ImageData LoadBitmap( const char* filename );
}
