
#include "Bitmap.h"
#include <fstream>
#include <cassert>

namespace Bitmap
{
	//definition of Bitmap Loader Function
	ImageData LoadBitmap( const char* filename )
	{
		std::ifstream bmpFile( filename, std::ios::binary );

		char signature[ 2 ];
		bmpFile.read( signature, 2 );
		assert( signature[ 0 ] == 'B' && signature[ 1 ] == 'M' && "File is not a BMP file." );

		BitmapFileHeader fileHeader{};
		bmpFile.read( reinterpret_cast< char* >( &fileHeader ), sizeof( fileHeader ) );

		BitmapInfoHeader infoHeader{};
		bmpFile.read( reinterpret_cast< char* >( &infoHeader ), sizeof( infoHeader ) );
		assert( infoHeader.bits == 24 && "Only 24 bit bitmaps supported." );

		bmpFile.seekg( fileHeader.offsetToPixelData, std::ios::beg );

		ImageData image{
			infoHeader.width,
			infoHeader.height,
			std::make_unique<Color[]>( std::size_t( infoHeader.width * infoHeader.height ) )
		};

		const int nPaddingBytesPerRow = ( 4 - ( ( infoHeader.width * 3 ) % 4 ) ) % 4;
		for( int y = image.height - 1; y >= 0; y-- )
		{
			const auto rIndex = ( y * image.width );
			for( auto x = 0u; x < image.width; x++ )
			{
				Pixel24 pixel;
				bmpFile.read( reinterpret_cast< char* >( &pixel ), sizeof( pixel ) );

				image.pixels[ x + rIndex ] =
					Color( pixel.red, pixel.green, pixel.blue );
			}

			bmpFile.seekg( nPaddingBytesPerRow, std::ios::cur );
		}

		bmpFile.close();

		return image;
	}
}