/**********************************************************

	Project: EngineSuffer
	Desc:    Utility functions

**********************************************************/
#include "stdafx.h"

float Difference(float source, float destination)
{
	if(destination == source)
		return 0;
	if(destination < source)
	{
		return -(source-destination);
	}
	else
	{
		return (destination-source);
	}
	return (destination-source);
}

float LinearInterpolate(float source, float destination, float timepassed)
{
	float newvalue = source + (timepassed*(destination - source));
	return newvalue;
}
