I have created a rectangle class for blocks and paddle in my game. 
The rectangle class is derived from class Fl_Widget and implements a draw method with attributes found in data members.
The attributes are position, dimensions and color of the rectangle. 
The object is drawn using an overriden definition of the function draw() which is virtual in parent class Fl_Widget().
Member functions of the class can be used to access its position and dimensions from outside the class or its daughter classes.
These will be useful to coordinate events in the game.

I have created two classes - Paddle and Block, by extending the class Rectangle.
These classes inherit member functions, data members and draw() function of the parent class, a very POWERFUL INHERITANCE IDEA.
Paddle class has a new member function move, which is called by handle function.
It also has an overridden definition of handle function which is virtual in parent Rectangle.
The handle function is used to take keyboard input, Right and Left arrow keys. 
It uses this input to call move() function which moves the paddle left or right.
To use FL_KEYBOARD event handler, the paddle is set to focus when it is created, so it is the focus of keyboard events.

For the moving ball, I have created the class Ball inherited from Fl_Widget. 
The ball has attributes position, radius and velocity as data members.
The ball has a draw() functions which overrides the virtual function in Fl_Widget, and has a move() function to move.
The move function is called by the game loop, and updates the position of the ball as it moves on the canvas.
It takes care that the ball stays within the boundary and handles collisions with the wall.
Member functions can be used to access position and velocity etc from outside, to coordinate the game.

A collide function has been created which checks for collision between its 2 arguments - a block and a ball.
This avoids mess in the actual game loop and keeps things tidy and packaged.
It returns 0, 1, 2, 3 or 4 depending on the collision direction or if there was a collision at all.

Finally, a game class has been created which sort of brings together all these individual components :
ball, paddle and blocks into one neat, nicely coordinated package.
It has pointers to the components as data members. The play() function of this class has the main game loop.
It makes calls to the ball's move() function, it checks for collision of ball with blocks or paddles. 
If collision is found with a block, it deletes that block and changes direction of the ball.
Similarly it controls the ball's interaction with the paddle.
It checks for the conditions for the game to be over, and if the game is over, reports it back to the main().

The main() initialises the objects needed for the game - border, ball, the (colorful) blocks, and the paddle.
It passes pointers to these to the constructor of class Game, and calls the play() function for the game to begin!
Depending on the result of the game, a window showing the result is shown.
---------------------------------------------------------------------------------------------------------------------
So I observe that through use of classes to create objects, and using custom designed handlers for these objects, a nice game can be created.
FLTK is a powerful library for that purpose with lots of classes and plenty of room to extend these classes and create useful ones of our own.
Event handlers in FLTK like FL_PUSH and FL_KEYBOARD among others are great ways to interact with objects on our window.
Overriding virtual functions of parent class makes inheritance very powerful and easy to use programming practice.
Using functions to create specific tasks irrespective of what is using it, is a great abstraction that saves us clutter and helps tidy up the code.
