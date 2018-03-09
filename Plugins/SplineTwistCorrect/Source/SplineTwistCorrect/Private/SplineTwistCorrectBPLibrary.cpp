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


void USplineTwistCorrectBPLibrary::CalcRailLength(
	const class USplineComponent* Spline, 
	int &number, 
	float &length,
	const float IdealLength)
{
	//TODO needs to return if no spline input
	number = (Spline->GetSplineLength()/IdealLength);
	length = (Spline->GetSplineLength()/number);
}

void USplineTwistCorrectBPLibrary::CalcStartEnd(
	const class USplineComponent* Spline, 
	FVector& LocStart, 
	FVector& TanStart, 
	FVector& LocEnd, 
	FVector& TanEnd, 
	const int Index, 
	const float Length)
{
	//TODO needs to return if no spline input
    ESplineCoordinateSpace::Type L = ESplineCoordinateSpace::Local;

	LocStart = Spline->GetLocationAtDistanceAlongSpline(Index*Length, L);
	LocEnd = Spline->GetLocationAtDistanceAlongSpline((Index+1)*Length, L);

	FVector Tan1 = Spline->GetTangentAtDistanceAlongSpline(Index*Length,L);
	TanStart = Tan1.GetSafeNormal()*Length;

	FVector Tan2 = Spline->GetTangentAtDistanceAlongSpline((Index+1)*Length,L);
	TanEnd = Tan2.GetSafeNormal()*Length;
}

void USplineTwistCorrectBPLibrary::CalcRotFromUp(
	float &Rotation,
	const class USplineComponent *Spline,
	const int Index,
	const float Length)
{
	//TODO needs to return if no spline input
	ESplineCoordinateSpace::Type W = ESplineCoordinateSpace::World;
	FVector Tan = Spline->GetTangentAtDistanceAlongSpline((Index + 1) * Length, W);
	FVector Crossed1 = FVector::CrossProduct(Tan.GetSafeNormal(), Spline->GetUpVectorAtDistanceAlongSpline(Index * Length, W));
	FVector Crossed2 = FVector::CrossProduct(Tan.GetSafeNormal(), Spline->GetUpVectorAtDistanceAlongSpline((Index + 1) * Length, W));
	FVector Crossed3 = FVector::CrossProduct(Crossed1, Crossed2).GetSafeNormal();
	float Dot1 = FVector::DotProduct(Crossed1.GetSafeNormal(), Crossed2.GetSafeNormal());
	float Dot2 = FVector::DotProduct(Crossed3.GetSafeNormal(), Tan);
	Rotation = ((UKismetMathLibrary::SignOfFloat(Dot2)) * (-1) * (FGenericPlatformMath::Acos(Dot1)));
}

void USplineTwistCorrectBPLibrary::ConfigSplineMesh(
	const int &Index,
	const float &Length,
	const class USplineComponent *SplineFinal,
	class USplineMeshComponent *SplineMesh,
	const class AActor *Actor,
	class UStaticMesh *StaticMesh)
{
	//TODO needs to return if no spline input
	FVector locStart = FVector(0, 0, 0),
			locEnd = FVector(100, 0, 0),
			tanStart = FVector(100, 0, 0),
			tanEnd = FVector(100, 0, 0);
	CalcStartEnd(SplineFinal, locStart, tanStart, locEnd, tanEnd, Index, Length);
	SplineMesh->SetStartAndEnd(locStart, tanStart, locEnd, tanEnd, false);

	ESplineCoordinateSpace::Type W = ESplineCoordinateSpace::World;
	FVector upDir = SplineFinal->GetUpVectorAtDistanceAlongSpline(Index * Length, W);
	FTransform transformDir = Actor->GetActorTransform();
	FVector transformed = transformDir.InverseTransformVectorNoScale(upDir);
	SplineMesh->SetSplineUpDir(transformed, true);

	float Rotation = 0;
	CalcRotFromUp(Rotation, SplineFinal, Index, Length);
	SplineMesh->SetEndRoll(Rotation, false);

	SplineMesh->SetStaticMesh(StaticMesh);
}

void USplineTwistCorrectBPLibrary::OffsetASpline(
	const class USplineComponent *SplineUser,
	class USplineComponent *SplineOffset,
	const float RotFromUp,
	const float OffsetDist)
{
	//TODO needs to return if no spline input
	ESplineCoordinateSpace::Type W = ESplineCoordinateSpace::World;
	SplineOffset->ClearSplinePoints(true);
	float lastIndex = SplineUser->GetNumberOfSplinePoints() - 1;

	FVector upVectorScaled = FVector(0, 0, 0);
	FVector tanAtPoint = FVector(0, 0, 0);
	FVector offsetVector = FVector(0, 0, 0);
	FVector pointPos = FVector(0, 0, 0);

	for (int i = 0; i <= lastIndex; i++)
	{
		upVectorScaled = ((SplineUser->GetUpVectorAtSplinePoint(i, W)) * OffsetDist);
		tanAtPoint = SplineUser->GetTangentAtSplinePoint(i, W);
		offsetVector = upVectorScaled.RotateAngleAxis(RotFromUp, tanAtPoint);
		pointPos = SplineUser->GetLocationAtSplinePoint(i, W) + offsetVector;
		SplineOffset->AddSplinePointAtIndex(pointPos, i, W, false);

		SplineOffset->SetTangentAtSplinePoint(i, tanAtPoint, W, false);

		SplineOffset->SetSplinePointType(i, SplineUser->GetSplinePointType(i), false);
	}
	SplineOffset->UpdateSpline();
	FixTangents(SplineUser, SplineOffset);
}

void USplineTwistCorrectBPLibrary::FixTangents(
	const class USplineComponent *SplineUser,
	class USplineComponent *SplineOffset)
{
	//TODO needs to return if no spline input
	ESplineCoordinateSpace::Type W = ESplineCoordinateSpace::World;
	FVector arriveTan = FVector(10, 0, 0);
	FVector leaveTan = FVector(10, 0, 0);
	FVector userTan = FVector(10, 0, 0);
	float lastIndex = SplineUser->GetNumberOfSplinePoints() - 1;
	int32 aindex = 0;  //arrive index
	int32 aindexl = 0; //arrive looping index
	int32 lindex = 0; //leave looping index
	for (int a = 0; a <= 2; a++)
	{
		for (int i = 0; i <= lastIndex; i++)
		{
			userTan = SplineUser->GetTangentAtSplinePoint(i, W);

			aindex = FMath::Clamp(int32(i - 1), 0, int32(lastIndex + 1));
			aindexl = lastIndex * ((i - 1) < 0);
			arriveTan = userTan * ((SplineOffset->GetDistanceAlongSplineAtSplinePoint(i) -
									(SplineOffset->GetDistanceAlongSplineAtSplinePoint(aindex) +
									 SplineOffset->GetDistanceAlongSplineAtSplinePoint(aindexl))) /
								   (SplineUser->GetDistanceAlongSplineAtSplinePoint(i) -
									(SplineUser->GetDistanceAlongSplineAtSplinePoint(aindex) +
									 SplineUser->GetDistanceAlongSplineAtSplinePoint(aindexl))));

			lindex = (lastIndex) == (i);
			leaveTan = userTan * ((SplineOffset->GetDistanceAlongSplineAtSplinePoint(i) -
									(SplineOffset->GetDistanceAlongSplineAtSplinePoint(i + 1) +
									 (SplineOffset->GetSplineLength() * lindex))) /
								   (SplineUser->GetDistanceAlongSplineAtSplinePoint(i) -
									(SplineUser->GetDistanceAlongSplineAtSplinePoint(i + 1) +
									 (SplineUser->GetSplineLength() * lindex))));

			SplineOffset->SetTangentsAtSplinePoint(i, arriveTan, leaveTan, W, false);
		}
		SplineOffset->UpdateSpline();
	}
}