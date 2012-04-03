#pragma once

#include "Intention.h"
#include "Racer.h"
#include "Waypoint.h"
#include <iostream>
#include <fstream>

using namespace std;

class WaypointEditor
{
public:
	WaypointEditor(Renderer* renderer);
	~WaypointEditor(void);
	void update(Racer* racer);
	void openFile();
	void closeFile();
	void writeToFile(Waypoint* waypoints[], int numberOfWaypoints, std::string fileName);
private:
	int waypointNumber;
	ofstream file;
	Renderer* renderer;
	int prevX;
	int prevY;
	int prevZ;
};

