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


void USplineTwistCorrectBPLibrary::CalcRailLength(const class USplineComponent* Spline, int &number, float &length,const float IdealLength)
{
	number = (Spline->GetSplineLength()/IdealLength);
	length = (Spline->GetSplineLength()/number);
}

void USplineTwistCorrectBPLibrary::CalcStartEnd(const class USplineComponent* Spline, FVector& LocStart, FVector& TanStart, FVector& LocEnd, FVector& TanEnd, const int Index, const float Length)
{
    ESplineCoordinateSpace::Type L = ESplineCoordinateSpace::Local;

	LocStart = Spline->GetLocationAtDistanceAlongSpline(Index*Length, L);
	LocEnd = Spline->GetLocationAtDistanceAlongSpline((Index+1)*Length, L);

	FVector Tan1 = Spline->GetTangentAtDistanceAlongSpline(Index*Length,L);
	TanStart = Tan1.GetSafeNormal()*Length;

	FVector Tan2 = Spline->GetTangentAtDistanceAlongSpline((Index+1)*Length,L);
	TanEnd = Tan2.GetSafeNormal()*Length;
}