# REQUIRES TWIST CORRECT PLUGIN ON MARKETPLACE
## https://www.unrealengine.com/marketplace/spline-twist-correct
This is a sample project to go with the twist correct plugin on the UE4 marketplace

## Get live help on Discord: https://discord.gg/WDZk9MT


## Use cases:
1. When adding spline mesh components to a spline that does loops or twists
2. Finding an offset spline for geometry or effects
3. Want to simplify setting up a spline mesh
4. Want to subdivide a spline with minimal control points

![TwistCorrect](/assets/TwistCorrect_r1venq50n.png)

## How To use with 'Spline With Mesh' component: (NEW!!)
1. Add the component to any actor 
2. Adjust the parameters for the mesh and materials to use
3. That is all!!

![SplineWithMeshComponentExamples](/assets/SplineWithMeshComponentExamples.jpg)


## How To use with functions:
1. Set up a spline in the editor (SplineUser) 
2. Add a second spline to the actor and set it as a variable (SplineOffset)
    - Its a good time use "Build Offset Spline" which will set the                  locationd and tangents of all the points on SplineOffset
3. Add a third spline to the actor and set it as a variable (SplineFinal)
    - We can "Build Corrected Spline" now which will subdivide the SplineUser and   set all the points of SplineFinal with 'Up Vectors' pointing toward the OffsetSpline
4. We need the number of sections and calculated length from "Calculate Rail Length" to configure our final spline mesh. 
5. Loop through adding spline meshes to the actor and "Configure SplineMesh" to see all of their locations, tangents, and rotations automatically.  

![TwistCorrectScreenshotsFinal](/assets/TwistCorrectScreenshotsFinal.jpg)

## Demo Room 
Shows a progression using more and more functions in the plugin

![TwistCorrectDemoRoom](/assets/TwistCorrectDemoRoom.png)

# Available Functions
Currently 7 functions that can all be used in Blueprints or C++

![TwistCorrectScreenshots](/assets/TwistCorrectScreenshots.jpg)

I will be going over this project at our monthly UE4 meetup
https://www.meetup.com/Seattle-Indie-Game-Dev-Workshop/events/247613617/

