#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, 30.f);

#define DRAW_SPHERE_COLOR(Location,Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, Color, false, 5.f);

#define DRAW_SPHERE_SingleFrame(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);
#define DRAW_LINE(StartPoint,EndPoint) if (GetWorld()) DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_LINE_SingleFrame(StartPoint,EndPoint) if (GetWorld()) DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, -1.f, 0, 1.f);
#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location , 15.f, FColor::Red, true);
#define DRAW_POINT_SingleFrame(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location , 15.f, FColor::Red, false, -1.f);

#define DRAW_VECTOR(StartPoint,EndPoint) if (GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, true, -1.f, 0, 1.f);\
		DrawDebugPoint(GetWorld(), EndPoint, 15.f, FColor::Red, true);\
	}

#define DRAW_VECTOR_SingleFrame(StartPoint,EndPoint) if (GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, -1.f, 0, 1.f);\
		DrawDebugPoint(GetWorld(), EndPoint, 15.f, FColor::Red,  false, -1.f);\
	}

/*
#define DRAW_SPHERE_COLOR(Location,Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 20.f, 12, Color, false, 30.f);

#define DRAW_ARROW(StartPoint,EndPoint) if (GetWorld()) DrawDebugDirectionalArrow(GetWorld(), StartPoint,EndPoint, 10.f, FColor::Green, true);
*/