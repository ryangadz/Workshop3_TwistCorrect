// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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

    //  USTRUCT(BlueprintType)
    //  struct MyStruct {
    //  GENERATED_USTRUCT_BODY()
    //      MyStruct(float l = 0f, int n = 0) : length(l), num(n) {}
     
    //      UPROPERTY(BlueprintReadWrite, Category = Vars)
    //          float length;
    //      UPROPERTY(BlueprintReadWrite, Category = Vars)
    //          int num;
    //  };

UCLASS()
class USplineTwistCorrectBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "SplineTwistCorrect sample test testing"), Category = "SplineTwistCorrectTesting")
	static float SplineTwistCorrectSampleFunction(float Param);


    /** Input a spline and ideal subdivision length to get number of subdivisions and calculated length. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Calculate Rail Length", Keywords = "SplineTwistCorrect sample test testing"), Category = "SplineTwistCorrectTesting")
	static void CalcRailLength(const class USplineComponent* Spline, int &number, float &length,const float IdealLength=10);

    /** Input a spline with index and section length to calculate start and end locations and tangents for a spline mesh */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Calculate Start End", Keywords = "SplineTwistCorrect sample test testing"), Category = "SplineTwistCorrectTesting")
    static void CalcStartEnd(const class USplineComponent* Spline, FVector& LocStart, FVector& TanStart, FVector& LocEnd, FVector& TanEnd, const int Index, const float Length=10);
	
};
