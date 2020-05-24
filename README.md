# Programming Assignment 3 
| Submitted to: | Prof/ Inas A. Yassine |
| ----------- | ----------- |
| **By** | - Adel Moustafa |
|     | - Mahmoud Abdelrhman |
|     | - Mohamed Kamal |
|     | - Mohamed Yasser |

# what we have done:

1. Robotic body animation.
2. Interfacing with an object.
3. Texture mapping on floor.
4. loading external 5 different objects.
5. The ability to change the position and dimensions of any object using keyboard keys.
6. Lighting and coloring.
7. Report.

# Control keys:

- " (q,Q),(w,W),(e,E)" these keys are changing the scale of the object in three dimensions space.
- "(a,A),(s,S),(d,D)" these keys are moving the object in the three dimensions space.
- "(x,X),(y,Y),(z,Z)" these keys are rotating the object around the three coordinates.
- ("5","2") these keys are moving the camera forward and backward.
- "four arrows" are used to move the camera up, down, left, and right.
- "mouse" is used to rotate the robot in any direction.

# The idea of the animation:

First the robot is doing exercise in place by moving its arm and legs up and down like jumping in place, second it is walking two steps forward, the third animation is that the robot is jumping across an obstacle.  

# The three animations:

## 1. Robot is jumping up and down in place:

![](Screenshots\20.png)

![](Screenshots\21.png)

## 2. Robot is walking:

![](Screenshots\22.png)

![](Screenshots\23.png)

![](Screenshots\25.png)

## 3. Robot is crossing the obstacle:

![](Screenshots\1.png)

![](Screenshots\2.png)

![](Screenshots\10.png)

![](Screenshots\11.png)

# The objects menu and loaded objects:

![Objects menu](Screenshots\13.png)

![Aircraft](Screenshots\14.png)

![Cat](Screenshots\15.png)

![Swan](Screenshots\16.png)

![Scooter](Screenshots\17.png)

![Obstacle](Screenshots\4.png)



# The implementation code:

In the "init" function we are setting the light source properties and enabling it.

![](Screenshots\init.png)

here in the "initRendering" function we are loading the ground's texture and adjust the lighting.

![](Screenshots\loading floor & adjust color.png)

In "screen_menu" function we are loading our 5 different objects.

![](Screenshots\object menu.png)

In "glClearColor" we changed the background's color.

![](Screenshots\background color.png)

Here we have drawn a square as a floor and put a texture on it, to be our ground.

![](Screenshots\ground.png)

From line 519 we start drawing our robot from old code in last assignment.

![](Screenshots\robot drawing.png)

In "keyboard" function we used some keys to control camera movements and objects position and dimensions.

![](Screenshots\keys.png)

This is our main function, where we call all used functions and naming our objects.

![](Screenshots\main.png)

And finally we made a timer function to make our animation and it is iterating in the "poss" array to make the movements of robot.  

# Problems:

1. The main problem was the we used code blocks on windows to write our program, but the was a lot of errors in loading texture and we send lots of time trying to fixe it, but we couldn't. until we asked Eng.Ayman Anwer and he told us the solution, but it seemed little complicated, so we turned on Ubuntu easier and it worked fine!   
2. In lighting we had some issues, because our code didn't work and there is no light. so we toked code from the section code and it worded fine.
3. In the implementation of texture the was a lot of problems: first it didn't appear when we run the code because we forgot to put "glPushMatrix();" before it until we realized this mistake and but it. second the texture didn't fit correctly like the image that we are loading because its size we small and we zoomed it too much, and when we realized this mistake too we changed it with a bigger one and finally it worked fine.
4. loading the objects, first it was difficult to find free 3D objects with its .obj and .mtl files. and after finding free objects with its files it didn't appear after running the code until we realized that we forgot to put some lines in the Display function.
5. The interface between object and robot was to difficult, because the object's movement didn't work as we wanted, and we tried a lot, so we decided to depend on the movement of robot and loading a specific object to work with that movement.  