#include "application.h"
#include "utils.h"
#include "image.h"

Vector2 inicial; //initial figure vertex
Vector2 fi; //final figure vertex
Color colour = colour.WHITE; //default color
Image framebuffer(800, 600);
Image llegenda(101, 61);
bool point;
bool line;
bool circle;
bool rectangle;
bool triangle;
int plus=0; //augmentar/disminuir tamany
bool clear; //clear framebuffer
bool erase; //activate erase option



Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(NULL);
}

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "Initiating app..." << std::endl;
	//here add your init stuff
	
	//LEGEND
	Vector2 a = { 3,3 };
	Vector2 b = { 97,57 };

	//fill legend with gray and draw a rectangle
	llegenda.fill(Color(180, 180, 180));
	llegenda.drawRectangle(a , b , plus, colour.BLACK);
	//set legend colors
	llegenda.drawCircle(a = { 20,20 }, b = { 21,21 }, 4, colour.WHITE);
	llegenda.drawCircle(a = { 40,20 }, b = { 41,21 }, 4, colour.GRAY);
	llegenda.drawCircle(a = { 60,20 }, b = { 61,21 }, 4, colour.RED);
	llegenda.drawCircle(a = { 80,20 }, b = { 81,21 }, 4, colour.YELLOW);
	llegenda.drawCircle(a = { 20,40 }, b = { 21,41 }, 4, colour.GREEN);
	llegenda.drawCircle(a = { 40,40 }, b = { 40,41 }, 4, colour.CYAN);
	llegenda.drawCircle(a = { 60,40 }, b = { 61,41 }, 4, colour.BLUE);
	llegenda.drawCircle(a = { 80,40 }, b = { 81,41 }, 4, colour.PURPLE);
	//--------------------------------------------------------
}

//render one frame
void Application::render(void)
{
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	showImage(&framebuffer);
	showImage(&llegenda);
}

//called after render
void Application::update(double seconds_elapsed)
{
	int i = 0;
	if (point || erase) { 
		while (i <= plus) {
			Vector2 cc = { mouse_position.x,mouse_position.y };
			framebuffer.drawCircle(cc, fi = { cc.x +i,cc.y }, 0, colour);
			i++;
		}
	}
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	
	if (clear) { //clear the framebuffer with an animation

		//if we change the coordinates of the argument of the cosine, we are be able to print horizontal lines, vertical or diagonal
		//if we change the factor multiplying the coordinates, we are changing the amplitude of the cosine
		//if we change the factor multiplying the variable time, we are changing the speed
		for (unsigned int x = 0; x < framebuffer.width; x++)
			for (unsigned int y = 0; y < framebuffer.height; y++)
				framebuffer.setPixelSafe(x, y, Color(abs(sin(((x+y)*0.2) + time * 5) * 0), abs(sin(((x + y)*0.2) + time * 5) * 200), abs(sin(((x + y)*0.2) + time * 5) * 50)));
	}

	if (keystate[SDL_SCANCODE_KP_PLUS]) plus ++ ; //with '+' increase 
	if (keystate[SDL_SCANCODE_KP_MINUS]) { //with '-' decrease
		if (plus == 0) {
			return;
		}
		else {
			plus--;
		}
	}
	//to read mouse position use mouse_position
}


//keyboard press event 
void Application::onKeyDown( SDL_KeyboardEvent event )
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch(event.keysym.scancode)
	{
		case SDL_SCANCODE_ESCAPE:
			exit(0); 
			break; //ESC key, kill the app

		case SDL_SCANCODE_DELETE:
			if (clear == true) {
				clear = false;
				framebuffer.fill(colour.BLACK);
				plus = 0;
			}
			else clear = true;
			break;

		case SDL_SCANCODE_L: //draw line
			line = true;
			point =false;
			break;

		case SDL_SCANCODE_C: //draw circle
			circle = true;
			break;

		case SDL_SCANCODE_R: //draw rectangle
			rectangle = true;
			break;

		case SDL_SCANCODE_T: //draw triangle
			triangle = true;
			break;
	}
}

//keyboard key up event 
void Application::onKeyUp(SDL_KeyboardEvent event)
{
	//...
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event ){

	if (mouse_position.x <= llegenda.width && mouse_position.y <= llegenda.height) {
		plus = 0;
		if (event.clicks == 2) {
			colour = llegenda.getPixel(mouse_position.x, mouse_position.y);
		}
	}
	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{
		point = true;
		if (line||rectangle|| triangle||circle) {
			point = false;
			inicial.x = mouse_position.x;
			inicial.y = mouse_position.y;
		}
	}
	if (event.button == SDL_BUTTON_RIGHT)
	{
		colour = colour.BLACK;
		erase = true;
	}

}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{
		point=false;

		if (line||rectangle||triangle||circle) {
			fi.x = mouse_position.x;
			fi.y = mouse_position.y;
			if (line) {
				framebuffer.drawLine(inicial, fi, colour);
				line = false;
			}
			if (rectangle) {
				framebuffer.drawRectangle(inicial, fi, plus, colour);
				rectangle = false;
			}
			if (triangle) {
				framebuffer.drawTriangle(inicial, fi, colour);
				triangle = false;
			}
			if (circle)
			{
				framebuffer.drawCircle(inicial,fi, plus, colour);
				circle = false;
			}
		
		}
	}
	if (event.button == SDL_BUTTON_RIGHT) {
		erase = false;
		colour = colour.WHITE;
		plus = 0;
	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}
