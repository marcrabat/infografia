#include "image.h"


Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

//copy constructor
Image::Image(const Image& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*sizeof(Color));
	}
}

//assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if(c.pixels)
	{
		pixels = new Color[width*height*sizeof(Color)];
		memcpy(pixels, c.pixels, width*height*sizeof(Color));
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}



//change image size (the old one will remain in the top-left corner)
void Image::resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = getPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

void Image::DDA(Vector2& inici, Vector2& final, const Color& c) {
	float difx = final.x - inici.x, dify = final.y - inici.y;
	float dist, x, y;
	float dx, dy;
	if (abs(difx) >= abs(dify))
		dist = abs(difx);
	else
		dist = abs(dify);

	dx = difx / dist;
	dy = dify / dist;

	x = inici.x + sgn(inici.x)*0.5;
	y = inici.y + sgn(inici.x)*0.5;

	for (int i = 0; i <= dist; i++) {
		setPixel(floor(x), floor(y), c);
		x += dx;
		y += dy;
	}
}

void Image::Bresenham(Vector2& inici, Vector2& final, const Color& c) {
	
	int dx, dy, inc_E, inc_NE, d, x, y;
	int incX = 1, incY = 1;

	if (abs(final.y - inici.y) > abs(final.x - inici.x)) {	
		//Aquí s'hauria "d'invertir" l'algoritme per fer el mateix però desplaçant-se per les y's
	}

	if (inici.x > final.x) {	//dibuixa els octants de l'esquerra amb pendents no superiors a +-1
		std::swap(inici, final);
	}

	dx = final.x - inici.x;
	dy = final.y - inici.y;

	if (dy < 0) {	//dibuixa l'octant baix-dreta
		dy = -dy;
		incY = -1;
	}

	x = inici.x;
	y = inici.y;

	setPixelSafe(x, y, c);
	inc_E = 2 * dy;
	inc_NE = 2 * (dy - dx);
	d = 2 * dy - dx;


	while (x != final.x)
	{
		x = x + incX;
		if (d <= 0) { //Escollim E
			d = d + inc_E;
			//x = x + incX;
		}
		else { //Escollim NE
			d = d + inc_NE;
			//x = x + incX;
			y = y + incY;
		}
		setPixelSafe(x, y, c);
	}
	
	
}

void Image::BresenhamCircle(Vector2& centre, Vector2& r, const Color& c)
{
	int radi = r.distance(centre);
	int x, y; int v;
	x = 0;
	y = radi;
	v = 1 - radi;
	
	while (y >=x) {
		setPixelSafe(x + centre.x, y + centre.y, c);
		setPixelSafe(x + centre.x, -y + centre.y, c);
		setPixelSafe(-x + centre.x, y + centre.y, c);
		setPixelSafe(-x + centre.x, -y + centre.y, c);
		setPixelSafe(y + centre.x, x + centre.y, c);
		setPixelSafe(-y + centre.x, x + centre.y, c);
		setPixelSafe(y + centre.x, -x + centre.y, c);
		setPixelSafe(-y + centre.x, -x + centre.y, c);
		if (v < 0) {
			v = v + 2 * x + 3;
			x++;
		}
		else {
			v = v + 2 * (x - y) + 5;
			x++;
			y--;
		}
		
	}
}


void Image::drawRectangle(Vector2 & lup, Vector2  & rdwn,int plus, const Color& c) {	
	//we understand lup for left-up, rdwn for right-down, rup for right-up and ldwn for left-down vertexs
	Vector2 rup = { lup.x,rdwn.y};
	Vector2 ldwn = { rdwn.x,lup.y};
	DDA(lup, rup, c);
	DDA(rup, rdwn, c);
	DDA(rdwn, ldwn, c);
	DDA(lup, ldwn, c);
}
void Image::drawTriangle(Vector2 & init, Vector2  & final, const Color& c) {
	DDA(init, final, c);
	Vector2 f = { init.x ,final.y };
	DDA(init, f, c);
	DDA(f, final, c);

}

//raster a triangle
void Image::TAA(int x1, int y1, int x2, int y2, int x3, int y3, Image* img, Color c)
{
	std::vector<sCelda> tabla;
	tabla.resize(img->height);


	//inicializar tabla
	for (unsigned int i = 0; i < tabla.size(); i++)
	{
		tabla[i].minx = 100000; //numero muy grande
		tabla[i].maxx = -100000; //numero muy pequeño
	}
	for (int x = 0; x < img->width; x++) {
		for (int y = 0; y < img->height; y++) {
			img->getPixel(x, y);
		}
	}
	//… algoritmo de raster de las 3 aristas
}
//change image size and scale the content
void Image::scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = getPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::getArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++x)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.setPixel( x, y, getPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::flipX()
{
	for(unsigned int x = 0; x < width * 0.5; ++x)
		for(unsigned int y = 0; y < height; ++y)
		{
			Color temp = getPixel(width - x - 1, y);
			setPixel( width - x - 1, y, getPixel(x,y));
			setPixel( x, y, temp );
		}
}

void Image::flipY()
{
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height * 0.5; ++y)
		{
			Color temp = getPixel(x, height - y - 1);
			setPixel( x, height - y - 1, getPixel(x,y) );
			setPixel( x, y, temp );
		}
}


//Loads an image from a TGA file
bool Image::loadTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int bytesPerPixel;
	unsigned int imageSize;

	FILE * file = fopen(filename, "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << filename << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		std::cerr << "TGA file seems to have errors or it is compressed, only uncompressed TGAs supported" << std::endl;
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	//save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	//convert to float all pixels
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			this->setPixel(x , height - y - 1, Color( tgainfo->data[pos+2], tgainfo->data[pos+1], tgainfo->data[pos]) );
		}

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::saveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	FILE *file = fopen(filename, "wb");
	if ( file == NULL )
	{
		fclose(file);
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	//tgainfo->width = header[1] * 256 + header[0];
	//tgainfo->height = header[3] * 256 + header[2];

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	//convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[(height-y-1)*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);
	return true;
}

#ifndef IGNORE_LAMBDAS

//you can apply and algorithm for two images and store the result in the first one
//forEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void forEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif