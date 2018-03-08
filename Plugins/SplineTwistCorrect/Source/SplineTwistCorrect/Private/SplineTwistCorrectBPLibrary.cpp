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

void USplineTwistCorrectBPLibrary::CalcRotFromUp(float &Rotation, const class USplineComponent* Spline, const int Index, const float Length)
{
	ESplineCoordinateSpace::Type W = ESplineCoordinateSpace::World;
	FVector Tan = Spline->GetTangentAtDistanceAlongSpline((Index+1)*Length, W);
	FVector Crossed1 = FVector::CrossProduct(Tan.GetSafeNormal(),Spline->GetUpVectorAtDistanceAlongSpline(Index*Length,W));
	FVector Crossed2 = FVector::CrossProduct(Tan.GetSafeNormal(),Spline->GetUpVectorAtDistanceAlongSpline((Index+1)*Length,W));	
	FVector Crossed3 = FVector::CrossProduct(Crossed1, Crossed2).GetSafeNormal();
	float Dot1 = FVector::DotProduct(Crossed1.GetSafeNormal(), Crossed2.GetSafeNormal()); 
	float Dot2 = FVector::DotProduct(Crossed3.GetSafeNormal(), Tan);
    Rotation = ((UKismetMathLibrary::SignOfFloat(Dot2))*(-1)*(FGenericPlatformMath::Acos(Dot1))); 
}
    
void USplineTwistCorrectBPLibrary::ConfigSplineMesh(
	const int &Index, 
	const float &Length, 
	const class USplineComponent *SplineFinal, 
    class USplineMeshComponent *SplineMesh, 
	const class AActor *Actor, 
	class UStaticMesh *StaticMesh)
{
	FVector locStart = FVector(0, 0, 0),
			locEnd = FVector(100, 0, 0),
			tanStart = FVector(100, 0, 0),
			tanEnd = FVector(100, 0, 0);
	CalcStartEnd(SplineFinal, locStart, tanStart, locEnd, tanEnd, Index, Length);
	SplineMesh->SetStartAndEnd(locStart, tanStart, locEnd, tanEnd, false);

	ESplineCoordinateSpace::Type L = ESplineCoordinateSpace::World;
	FVector upDir = SplineFinal->GetUpVectorAtDistanceAlongSpline(Index*Length, L);
	FTransform transformDir = Actor->GetActorTransform();
	FVector transformed = transformDir.InverseTransformVectorNoScale(upDir);
	SplineMesh->SetSplineUpDir(transformed, true);

	float Rotation = 0; 
	CalcRotFromUp(Rotation, SplineFinal, Index, Length);
	SplineMesh->SetEndRoll(Rotation, false);

    SplineMesh->SetStaticMesh(StaticMesh);

}