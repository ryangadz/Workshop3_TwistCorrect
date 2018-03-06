// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SplineTwistCorrectBPLibrary.h"
#include "SplineTwistCorrect.h"


USplineTwistCorrectBPLibrary::USplineTwistCorrectBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float USplineTwistCorrectBPLibrary::SplineTwistCorrectSampleFunction(float Param)
{
	return -1;
}


void USplineTwistCorrectBPLibrary::FindRailLength(const class USplineComponent* Spline, int &number, float &length,const float IdealLength)
{
	number = (Spline->GetSplineLength()/IdealLength);
	length = (Spline->GetSplineLength()/number);
}