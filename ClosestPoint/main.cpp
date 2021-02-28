#include <iostream>
#include <SDL.h>
#include <ctime> 
using namespace std;
const int ScreenWidth = 640;
const int ScreenHeight = 320;
SDL_Window* window = SDL_CreateWindow("3D Demo ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, 0);
SDL_Surface* screen = SDL_GetWindowSurface(window);
SDL_Event event;
class Point
{
public:
    int x, y;
};
struct returnedThing
{
    Point i = {0,0};
    Point j = {0,0};
    float min = 0;
};
void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
    if (x < ScreenWidth && x>0 && y < ScreenHeight && y>0)
    {
        Uint32* pixels = (Uint32*)surface->pixels;
        pixels[(y * surface->w) + x] = pixel;
    }
}
void DrawLine(int x1, int y1, int x2, int y2)
{
    int m_new = 2 * (y2 - y1);
    int slope_error_new = m_new - (x2 - x1);
    for (int x = x1, y = y1; x <= x2; x++)
    {
        putpixel(screen, x, y, 8143103);

        slope_error_new += m_new;

        if (slope_error_new >= 0)
        {
            y++;
            slope_error_new -= 2 * (x2 - x1);
        }
    }
}
void DrawRedLine(int x1, int y1, int x2, int y2)
{
    int m_new = 2 * (y2 - y1);
    int slope_error_new = m_new - (x2 - x1);
    for (int x = x1, y = y1; x <= x2; x++)
    {
        putpixel(screen, x, y, 16711680);

        slope_error_new += m_new;

        if (slope_error_new >= 0)
        {
            y++;
            slope_error_new -= 2 * (x2 - x1);
        }
    }
}
void circlebres(float x1, float y1, float r)
{
    float x, y, p;
    x = 0;
    y = r;
    p = 3 - (2 * r);
    while (x <= y)
    {
        putpixel(screen,x1 + x, y1 + y, 16711680);
        putpixel(screen,x1 - x, y1 + y, 16711680);
        putpixel(screen,x1 + x, y1 - y, 16711680);
        putpixel(screen,x1 - x, y1 - y, 16711680);
        putpixel(screen,x1 + y, y1 + x, 16711680);
        putpixel(screen, x1 + y, y1 - x, 16711680);
        putpixel(screen, x1 - y, y1 + x, 16711680);
        putpixel(screen, x1 - y, y1 - x, 16711680);
        x = x + 1;
        if (p < 0)
        {
            p = p + 4 * (x)+6;
        }
        else
        {
            p = p + 4 * (x - y) + 10;
            y = y - 1;



        }
    }
}
void SDL_RenderFillCircle(int x, int y, int radius)
{
    int offsetx, offsety, d;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;

    while (offsety >= offsetx) {

        DrawLine( x - offsety, y + offsetx,
            x + offsety, y + offsetx);
        DrawLine( x - offsetx, y + offsety,
            x + offsetx, y + offsety);
        DrawLine( x - offsetx, y - offsety,
            x + offsetx, y - offsety);
        DrawLine( x - offsety, y - offsetx,
            x + offsety, y - offsetx);


        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}
int compareX(const void* a, const void* b)
{
    Point* p1 = (Point*)a, * p2 = (Point*)b;
    return (p1->x - p2->x);
}

int compareY(const void* a, const void* b)
{
    Point* p1 = (Point*)a, * p2 = (Point*)b;
    return (p1->y - p2->y);
}
float min(float x, float y)
{
    return (x < y) ? x : y;
}
float dist(Point p1, Point p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) +(p1.y - p2.y) * (p1.y - p2.y));
}
returnedThing bruteforce(Point P[], int n)
{
    returnedThing a;
    Point jr;
    Point ir;
    float min = FLT_MAX;
    for (int i = 0;i < n;++i)
    {
        for (int j = i + 1;j < n;++j)
        {
            if (dist(P[i], P[j]) < min)
            {
                min = dist(P[i], P[j]);
                ir = P[i];
                jr = P[j];
            }
        }
    }
    a.i = ir;
    a.j = jr;
    a.min = min;
    return a;
}
returnedThing stripClosest(Point strip[],int size,float d)
{
    returnedThing a;
    Point jr = { 0,0 };
    Point ir = { 0,0 };
    float min = d;

    qsort(strip, size, sizeof(Point), compareY);

    for (int i = 0; i < size; ++i)
    {
        for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < min; ++j)
        {
            if (dist(strip[i], strip[j]) < min)
            {
                min = dist(strip[i], strip[j]);
                ir = strip[i];
                jr = strip[j];
            }
        }
    }

    a.i = ir;
    a.j = jr;
    a.min = min;
    return a;
}
returnedThing closestUtil(Point P[], int n)
{
    returnedThing a;
    returnedThing dmin;
    float d;
    if (n <= 3)
    {
        return bruteforce(P,n);
    }
    int mid = n / 2;
    Point midpoint = P[mid];
    returnedThing dl = closestUtil(P,mid);
    returnedThing dr = closestUtil(P + mid, n - mid);
    if (dl.min < dr.min)
    {
        dmin = dl;
        d = dl.min;
    }
    else
    {
        dmin = dr;
        d = dr.min;
    }
    d = min(dl.min, dr.min);
    Point* strip = new Point[n];
    int j = 0;
    for (int i = 0;i < n;i++)
    {
        if (abs(P[i].x - midpoint.x) < d)
        {
            strip[j] = P[i];
            j++;
        }
    }
    returnedThing dstrip = stripClosest(strip, j, d);
    if (dstrip.min < dmin.min)
    {
        return dstrip;
    }
    else
    {
        return dmin;
    }
}
returnedThing closest(Point P[],int n)
{
    qsort(P,n,sizeof(Point),compareX);
    return closestUtil(P, n);
}
int main(int argc, char* argv[])
{
    bool quit = false;
    srand((unsigned)time(0));
    const int Size = 100;
    Point points[Size];
    for (int i = 0;i < Size;i++)
    {
        points[i].x = (rand() % 640);
        points[i].y = (rand() % 320);
        //cout <<"("<< points[i].x<< ","<< points[i].y<<")";
    }
    while (!quit)
    {
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }
        returnedThing closestPointInfo = closest(points, Size);
        for (Point point : points)
        {
            SDL_RenderFillCircle(point.x, point.y, 4);
        }
        DrawRedLine(closestPointInfo.i.x, closestPointInfo.i.y, closestPointInfo.j.x, closestPointInfo.j.y);
        circlebres((closestPointInfo.i.x+ closestPointInfo.j.x)/2, (closestPointInfo.i.y + closestPointInfo.j.y) / 2, 25);
        SDL_UpdateWindowSurface(window);
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    }
    SDL_FreeSurface(screen);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}