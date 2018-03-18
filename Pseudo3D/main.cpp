#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

int		width	= 1024;
int		height	= 768;
int		roadW	= 2000;
int		segL	= 200; //segment length
float	camD	= 0.84; //camera depth

struct Line
{
	float x, y, z; //3D center of line
	float X, Y, W; // screen coord
	float scale;

	Line() {x=y=z=0;}

	//from world to screen coordinates
	void project(int camX, int camY, int camZ)
	{
		scale = camD/(z-camZ);
		X = (1 + scale * (x - camX)) * width / 2;
		Y = (1 - scale * (y - camY)) * height / 2;
		W = scale * roadW * width / 2;
	}
};
 
void drawQuad (RenderWindow &w, Color c,float x1,float y1,float w1,float x2,float y2,float w2)
{
	ConvexShape shape(4);
	shape.setFillColor(c);
	shape.setPoint(0, Vector2f(x1-w1,y1));
	shape.setPoint(1, Vector2f(x2-w2,y2));
	shape.setPoint(2, Vector2f(x2+w2,y2));
	shape.setPoint(3, Vector2f(x1+w1,y1));
	w.draw(shape);
}


int main ()
{
	RenderWindow app (VideoMode(width, height), "Pseudo3DRacing");
	app.setFramerateLimit(60);

	vector<Line> lines;

	for (int i=0; i<1600; i++)
	{
		Line line;
		line.z = i * segL;

		lines.push_back(line);
	}

	int N = lines.size();

	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				app.close();
			}
		}
		app.clear();
		//drawQuad(app, Color::Color(41,95,19,255), 500, 500, 200, 500, 300, 100);

			////////draw road////////
		for (int n = 0; n < 300; n++)
		{
			Line &l = lines[n%N];
			l.project(0, 1500, 0);

			Color grass		= (n / 3) %2 ? Color(16, 200, 16) : Color(0, 154, 0);
			Color rumble	= (n / 3) %2 ? Color(255, 255, 255) : Color(0, 0, 0);
			Color road		= (n / 3) %2 ? Color(107, 107, 107) : Color(105, 105, 105);

			Line p = lines[(n - 1) %N]; //previous line

			drawQuad(app, grass,	0,	p.Y, width, 0, l.Y, width);
			drawQuad(app, rumble,	p.X, p.Y, p.W * 1.2, l.X, l.Y, l.W * 1.2);
			drawQuad(app, road,		p.X, p.Y, p.W, l.X, l.Y, l.W);
		}

		app.display();
		}

	return 0;
}