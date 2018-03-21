// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Runtime/Engine/Classes/Components/SplineMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "SplineTwistCorrectBPLibrary.generated.h"
 

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/



UCLASS()
class USplineTwistCorrectBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

    /** Input a spline and ideal subdivision length to get number of subdivisions and calculated length. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Calculate Rail Length", Keywords = "SplineTwistCorrect TwistCorrect SplineCorrect"), Category = "SplineTwistCorrect")
	static void CalcRailLength(
        const class USplineComponent* Spline, 
        int &number, 
        float &length,
        const float IdealLength=10);

    /** Input a spline with index and section length to calculate start and end locations and tangents for a spline mesh */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Calculate Start End", Keywords = "SplineTwistCorrect TwistCorrect SplineCorrect"), Category = "SplineTwistCorrect")
    static void CalcStartEnd(
        const class USplineComponent* Spline, 
        FVector& LocStart, 
        FVector& TanStart, 
        FVector& LocEnd, 
        FVector& TanEnd, 
        const int Index, 
        const float Length=10);
	
    /** Input a spline to calculate the rotation of a spline mesh from the current 'Up Vector' to its end location. */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Calc Rot from Up Vector", Keywords = "SplineTwistCorrect TwistCorrect SplineCorrect"), Category = "SplineTwistCorrect")
    static void CalcRotFromUp(
        float &Rotation, 
        const class USplineComponent* Spline, 
        const int Index, 
        const float Length=10);

    /** Input a spline and splinemesh to set its start and end along with twisting */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Configure SplineMesh", Keywords = "SplineTwistCorrect TwistCorrect SplineCorrect"), Category = "SplineTwistCorrect")
    static void ConfigSplineMesh(
        const int &Index, 
        const float &Length, 
        const class USplineComponent *SplineFinal, 
        class USplineMeshComponent *SplineMesh, 
        const class AActor *Actor, 
        class UStaticMesh *StaticMesh);

    /** Input a Spline to offset with offset distance and rotation from the spline's up vector at each point */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Build Offset Spline", Keywords = "SplineTwistCorrect TwistCorrect SplineCorrect"), Category = "SplineTwistCorrect")
    static void BuildOffsetSpline(
        const class USplineComponent *SplineUser,
        class USplineComponent *SplineOffset,
        const float RotFromUp = 0,
        const float OffsetDist = 100);

    /** Input an offset spline to fix the tangents by comparing length of user made spline */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fix Tangents of Offset Spline", Keywords = "SplineTwistCorrect TwistCorrect SplineCorrect"), Category = "SplineTwistCorrect")
    static void FixTangents(
        const class USplineComponent *SplineUser,
        class USplineComponent *SplineOffset);

        
        
    /** Builds a final spline using a user spline and offset spline. Final spline should be heavily subdivided to avoid twisting */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Build Corrected Spline", Keywords = "SplineTwistCorrect TwistCorrect SplineCorrect"), Category = "SplineTwistCorrect")
    static void BuildCorrectedSpline(
        const class USplineComponent *SplineUser,
        const class USplineComponent *SplineOffset, 
        class USplineComponent *SplineFinal,
        const float IdealLength=100);
};
