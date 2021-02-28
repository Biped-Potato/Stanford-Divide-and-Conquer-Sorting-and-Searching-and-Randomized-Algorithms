#include <iostream>
#include <algorithm>
#include <array> 
#include <random>
#include <chrono>
#include <SDL.h>
using namespace std;
const int ScreenWidth = 1980;
const int ScreenHeight = 1000;
SDL_Window* window = SDL_CreateWindow("Quicksort", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, 0);
SDL_Surface* screen = SDL_GetWindowSurface(window);
SDL_Event event;
SDL_Rect Rect;
const int Size = 990;
int Partition(int A[], int l, int r)
{
	int p = A[r];
	int i = l - 1;
	for (int j = l;j <= r - 1;j++)
	{
		if (A[j] <= p)
		{
			i++;
			swap(A[i], A[j]);
			for (int k = 0; k < Size; k++)
			{
				if (k == i || k == j)
				{
					Rect.x = k * 2;
					Rect.y = ScreenHeight - (A[k]) - 10;
					Rect.w = 3;
					Rect.h = A[k];
					SDL_FillRect(screen, &Rect, 5253513);
				}
				else
				{
					Rect.x = k * 2;
					Rect.y = ScreenHeight - (A[k]) - 10;
					Rect.w = 3;
					Rect.h = A[k];
					SDL_FillRect(screen, &Rect, 16777215);
				}
			}
			SDL_UpdateWindowSurface(window);
			SDL_FillRect(screen, NULL, 0);
		}
	}
	swap(A[i + 1], A[r]);
	for (int k = 0; k < Size; k++)
	{
		if (k == i+1 || k == r)
		{
			Rect.x = k * 2;
			Rect.y = ScreenHeight - (A[k]) - 10;
			Rect.w = 3;
			Rect.h = A[k];
			SDL_FillRect(screen, &Rect, 5253513);
		}
		else
		{
			Rect.x = k * 2;
			Rect.y = ScreenHeight - (A[k]) - 10;
			Rect.w = 3;
			Rect.h = A[k];
			SDL_FillRect(screen, &Rect, 16777215);
		}
	}
	SDL_UpdateWindowSurface(window);
	SDL_FillRect(screen, NULL, 0);
	return (i + 1);
}
void QuickSort(int A[], int low, int high)
{
	if (low < high)
	{
		int Pivot = Partition(A, low, high);
		QuickSort(A, low, Pivot - 1);
		QuickSort(A, Pivot + 1, high);
	}
}

int main(int argc, char* argv[])
{
	srand((unsigned)time(0));
	bool quit = false;
	int arr[Size];
	for (int i = 0;i < Size;i++)
	{
		arr[i] = i;
	}
	random_shuffle(begin(arr),end(arr));
	QuickSort(arr, 0, Size-1);
	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}
	}
	SDL_FreeSurface(screen);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}