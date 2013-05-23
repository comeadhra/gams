/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

/*********************************************************************
 * BridgeAlgorithm.cpp - Defines the methods for the algorithm
 * that creates a bridge and decides which drone should move where.
 *********************************************************************/

#include <cmath>
#include <list>
#include <vector>
#include <map>
#include "BridgeAlgorithm.h"

using namespace std;

// Compares two distance tuples by the distance field.
bool compareByDistance(DistanceTuple first, DistanceTuple second)
{
	return first.distance < second.distance;
}

// Creates a bridge given the source, sink, and available drone locations. 
// Returns the position this drone should go to, or NULL if this drone was not selected for the bridge.
Position* BridgeAlgorithm::getPositionInBridge(int myId, double commRange, Position sourcePosition, Position sinkPosition, map<int, Position> availableDronePositions)
{
	Position* myNewPosition = NULL;

	// Calculate how many drones we need for the bridge.
	double bridgeLength = sqrt(pow(sourcePosition.x - sinkPosition.x, 2) + pow(sourcePosition.y - sinkPosition.y, 2));
	int numberOfRelays = (int) ceil(bridgeLength/commRange - 1);
	//printf("test");

	// Calculate the locations of each relay drone.
	vector<Position> relayList(numberOfRelays);
	for(int i=0; i<numberOfRelays; i++)
	{
		Position relayPosition;
		relayPosition.x = sinkPosition.x + (sourcePosition.x - sinkPosition.x)/(numberOfRelays + 1)*(i+1);
		relayPosition.y = sinkPosition.y + (sourcePosition.y - sinkPosition.y)/(numberOfRelays + 1)*(i+1);
		relayList[i] = relayPosition;
	}

	// List to mark when a drone has been assigned.
	map<int, bool> assignedDrones;

	// Calculate the distances between each drone and each relay location.
	list<DistanceTuple> distanceTuples;
	typedef std::map<int, Position>::iterator it_type;
	for(it_type iterator = availableDronePositions.begin(); iterator != availableDronePositions.end(); iterator++) 
	{
		// Get info for current drone ID and position, and mark this drone as not assigned for now.
		int currentDroneId = iterator->first;
		Position currentDronePos = iterator->second;
		assignedDrones[currentDroneId] = false;

		for(int j=0; j<numberOfRelays; j++)
		{
			// Calculate the distance betwee drone i and location j.
			Position currentRelayPos = relayList[j];
			double distanceToLocation = sqrt(pow(currentDronePos.x - currentRelayPos.x,2) + pow(currentDronePos.y - currentRelayPos.y,2));

			// Store the distance and the information about the drone and location in a list to be sorted later.
			DistanceTuple currentDistanceTuple;
			currentDistanceTuple.droneId = currentDroneId;
			currentDistanceTuple.relayLocationId = j;
			currentDistanceTuple.distance = distanceToLocation;
			distanceTuples.push_back(currentDistanceTuple);
		}
	}

	// Sort the list with all the distances.
	distanceTuples.sort(compareByDistance);

	// List to store the drone assigned to each location.
	vector<int> droneAssignedToLocation(numberOfRelays);
	for(int i=0; i<numberOfRelays; i++)
	{
		droneAssignedToLocation[i] = -1;
	}

	// Find the best drone-location pairs, with the smallest distances.
	list<DistanceTuple>::iterator currentTuple;
	for(currentTuple=distanceTuples.begin(); currentTuple!=distanceTuples.end(); ++currentTuple)
	{
		// Check if this drone-location pair is useful, if both drone and location have not been assigned yet.
		bool droneAndRelayLocationNotAssigned = !assignedDrones[currentTuple->droneId] && (droneAssignedToLocation[currentTuple->relayLocationId]==-1);
		if(droneAndRelayLocationNotAssigned)
		{
			// Mark this drone as assigned, and store the drone assigned to that location.
			assignedDrones[currentTuple->droneId] = true;
			droneAssignedToLocation[currentTuple->relayLocationId] = currentTuple->droneId;

			// Check if this is me to stop search right away?
			if(currentTuple->droneId == myId)
			{
				myNewPosition = new Position();
				myNewPosition->x = relayList[currentTuple->relayLocationId].x;
				myNewPosition->y = relayList[currentTuple->relayLocationId].y;

				// break;
			}
		}
	}

	// This will be null if I didnt end up inside the bridge.
	return myNewPosition;
}