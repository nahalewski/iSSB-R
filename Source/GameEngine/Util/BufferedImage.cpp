
#include "BufferedImage.h"
#include "Image.h"
#include "../Output/Console.h"
#include <SDL_image.h>
#include "../Application.h"
#include <cstdio>

namespace GameEngine
{
	BufferedImage::BufferedImage()
	{
		textureDensity = 1;
		texture = NULL;
		width = 0;
		height = 0;
	}
	
	BufferedImage::BufferedImage(const Image&img)
	{
		textureDensity = 1;
		texture = SDL_CreateTexture(Application::renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, (int)img.getWidth(), (int)img.getHeight());
		int pitch;
		
		unsigned int total = img.width*img.height;
		
		pixels.resize(total);
		
		void*pixsVoid = NULL;
		if(SDL_LockTexture(texture, NULL, &pixsVoid, &pitch)<0)
		{
			Console::WriteLine(SDL_GetError());
		}
		ValueRGBA*pixs = (ValueRGBA*)pixsVoid;
		for(unsigned int i=0; i<total; i++)
		{
			ValueRGBA&px = img.pixels[i];
			if(px.a>0)
			{
				pixels[i] = true;
			}
			else
			{
				pixels[i] = false;
			}
			pixs[i] = px;
		}
		
		SDL_UnlockTexture(texture);
		
		width = img.width;
		height = img.height;
	}
	
	BufferedImage::~BufferedImage()
	{
		pixels.resize(0);
		if(texture!=NULL)
		{
			SDL_DestroyTexture(texture);
		}
		width = 0;
		height = 0;
	}
	
	bool BufferedImage::loadFromFile(const String&fileName)
	{
		textureDensity = 1;
		if(texture!=NULL)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
			width = 0;
			height = 0;
		}
		SDL_Surface*img = IMG_Load(fileName);
		if(img!=NULL)
		{
			texture = SDL_CreateTextureFromSurface(Application::renderer, img);
			if(texture == NULL)
			{
				Console::WriteLine((String)"Error BufferedImage::loadFromFile(String) " + IMG_GetError());
				SDL_FreeSurface(img);
				return false;
			}
			width = img->w;
			height = img->h;
			unsigned int total = width*height;
			int bpp = img->format->BytesPerPixel;
			if(bpp==4)
			{
				ValueRGBA*pixs = (ValueRGBA*)img->pixels;
				pixels.resize(total);
				for(unsigned int i=0; i<total; i++)
				{
					ValueRGBA&point = pixs[i];
					if(point.a==0)
					{
						pixels[i] = false;
					}
					else
					{
						pixels[i] = true;
					}
				}
			}
			else
			{
				pixels.resize(total);
				for(unsigned int i=0; i<total; i++)
				{
					pixels[i] = true;
				}
			}
			SDL_FreeSurface(img);
			// Optional HD metadata keeps animation geometry and collision masks in
			// original game pixels while the GPU uses the higher-resolution texture.
			// SDL's file API also reads packaged Android assets.
			SDL_RWops* metadata = SDL_RWFromFile((fileName + ".hd"), "rb");
			if(metadata != NULL)
			{
				unsigned int density = 0, logicalWidth = 0, logicalHeight = 0;
				char header[128];
				unsigned int headerSize = 0;
				bool hasNewline = false;
				while(headerSize < sizeof(header)-1)
				{
					char ch;
					if(SDL_RWread(metadata, &ch, 1, 1) != 1) { break; }
					if(ch == '\n') { hasNewline = true; break; }
					header[headerSize++] = ch;
				}
				header[headerSize] = '\0';
				bool valid = hasNewline && std::sscanf(header, "ISSBHD1 %u %u %u", &density, &logicalWidth, &logicalHeight) == 3;
				valid = valid && density >= 1 && density <= 16 && logicalWidth > 0 && logicalHeight > 0;
				valid = valid && width % density == 0 && height % density == 0
					&& width / density == logicalWidth && height / density == logicalHeight;
				std::vector<bool> logicalPixels;
				if(valid)
				{
					logicalPixels.resize(logicalWidth * logicalHeight);
					for(unsigned int i = 0; i < logicalPixels.size(); i++)
					{
						unsigned char pixel;
						if(SDL_RWread(metadata, &pixel, 1, 1) != 1 || pixel > 1) { valid = false; break; }
						logicalPixels[i] = pixel == 1;
					}
				}
				SDL_RWclose(metadata);
				if(!valid)
				{
					Console::WriteLine((String)"Invalid HD sprite metadata: " + fileName);
					SDL_DestroyTexture(texture);
					texture = NULL;
					width = height = 0;
					pixels.clear();
					return false;
				}
				textureDensity = density;
				width = logicalWidth;
				height = logicalHeight;
				pixels.swap(logicalPixels);
			}
			return true;
		}
		Console::WriteLine((String)"Error BufferedImage::loadFromFile(String) " + IMG_GetError());
		return false;
	}
	
	unsigned int BufferedImage::getWidth() const
	{
		return width;
	}
	
	unsigned int BufferedImage::getHeight() const
	{
		return height;
	}

	unsigned int BufferedImage::getTextureDensity() const
	{
		return textureDensity;
	}
	
	bool BufferedImage::pixelAtPoint(unsigned int index) const
	{
		return pixels[index];
	}
	
	bool BufferedImage::pixelAtPoint(unsigned int x,unsigned int y) const
	{
		return pixels[(width*y)+x];
	}
	
	SDL_Texture*BufferedImage::getTexture() const
	{
		return texture;
	}
}