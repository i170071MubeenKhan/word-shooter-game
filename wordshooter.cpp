//============================================================================
// Name        : cookie-crush.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Cookie  Crush...
//============================================================================
#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include<fstream>
#include "util.h"
using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 60

string * dictionary;
int dictionarysize = 370099;
#define KEY_ESC 27 // A

// 20,30,30
const int bradius = 30; // ball radius in pixels...

int width = 930, height = 660;
int byoffset = bradius;

int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2; // initially number of full rows //
float score = 0;
int **board; // 2D-arrays for holding the data...
int bwidth = 130;
int bheight = 10;
int bsx, bsy;
const int nalphabets = 26;
enum alphabets {
	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};
GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
"x.bmp", "y.bmp", "z.bmp" };
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...

/** SELF MADE **/
int col_num, row_num;
int cookies[10][15];
int shoot_num;
int shoot_num_second;
float secs = 120;
int dicstart, dicend;
float grdx=0;
float grdy=0;
int newcorx = width/2, newcory = 10;
string forward;
string backward;
float move_rad;
float finalx1 = 495, finaly1 = 100, ratio1;
float finalx2 = 495, finaly2 = 120, ratio2;
float finalx3 = 495, finaly3 = 140, ratio3;
string matchword = "", longestword = "";
string maxword = "";

//Dictionary part selection
/** dictionary has been divided in smaller parts (for example, all the words starting with a are in one part). This minimizes the calculations done by the computer. So it results in overall better performance. This function is used to select the correct part of the dictionary **/ 
void dicpart(string a){
	switch (int(a[0] - 97))
	{
		case 0:  dicstart =      0; dicend =  25417; break;
		case 1:  dicstart =  25418; dicend =  43830; break;
		case 2:  dicstart =  43831; dicend =  75936; break;
		case 3:  dicstart =  75937; dicend =  94668; break;
		case 4:  dicstart =  94669; dicend = 108865; break;
		case 5:  dicstart = 108866; dicend = 120758; break;
		case 6:  dicstart = 120759; dicend = 131711; break;
		case 7:  dicstart = 131712; dicend = 145455; break;
		case 8:  dicstart = 145456; dicend = 159796; break;
		case 9:  dicstart = 159797; dicend = 162636; break;
		case 10: dicstart = 162637; dicend = 165688; break;
		case 11: dicstart = 165689; dicend = 176590; break;
		case 12: dicstart = 176591; dicend = 196395; break;
		case 13: dicstart = 196396; dicend = 209853; break;
		case 14: dicstart = 209854; dicend = 222534; break;
		case 15: dicstart = 222535; dicend = 257394; break;
		case 16: dicstart = 257395; dicend = 259187; break;
		case 17: dicstart = 259188; dicend = 275970; break;
		case 18: dicstart = 275971; dicend = 314731; break;
		case 19: dicstart = 314732; dicend = 333549; break;
		case 20: dicstart = 333550; dicend = 356316; break;
		case 21: dicstart = 356317; dicend = 361645; break;
		case 22: dicstart = 361646; dicend = 368204; break;
		case 23: dicstart = 368205; dicend = 368711; break;
		case 24: dicstart = 145456; dicend = 159796; break;
		case 25: dicstart = 368712; dicend = 370099; break;
	}
}

//Word combo functions
void maxlengthword_h()
{
	for (int i = 0; i < 10; i += 1)							//Checks words in each row
	{
		for (int j = 0; j < 15; j += 1)
		{
			for (int k = 14; k >= j+2; k -= 1)
			{
				forward = "";
				for (int l = j; l <= k; l += 1)				//forward word concatenation
				{
					forward += char(cookies[i][l] + 97);
				}
				dicpart(forward);
				for (int l = dicstart; l <= dicend; l += 1)
				{
					if (forward == dictionary[l])
					{	
						for (int m = j; m <= k; m += 1)		//forward word check
						{
							if (i > 1){
							matchword = forward;
							if (matchword.size() >= maxword.size())
							{
								maxword = matchword;
							}
							cookies[i][m] = -1;
							score ++;}
						}
					}
				}
				backward = "";
				for (int l = k; l >= j; l -= 1)				//backward word concatenation
				{
					backward += char(cookies[i][l] + 97);
				}
				dicpart(backward);
				for (int l = dicstart; l <= dicend; l += 1)
				{
					if (backward == dictionary[l])
					{
						for (int m = k; m >= j; m -= 1)		//backward word check
						{
							if (i > 1){
							matchword = backward;
							if (matchword.size() >= maxword.size())
							{
								maxword = matchword;
							}
							cookies[i][m] = -1;
							score ++;}
						}
					}
				}
			}
		}
	}
}

void maxlengthword_v()
{
	for (int i = 0; i < 15; i += 1)							//Checks words in each column
	{
		for (int j = 0; j < 10; j += 1)
		{
			for (int k = 10; k >= j+2; k -= 1)
			{
				forward = "";
				for (int l = j; l <= k; l += 1)				//forward word concatenation
				{
					forward += char(cookies[l][i] + 97);
				}
				dicpart(forward);
				for (int l = dicstart; l <= dicend; l += 1)
				{
					if (forward == dictionary[l])
					{
						for (int m = j; m <= k; m += 1)		//forward word check
						{
							if (k > 1){
							matchword = forward;
							if (matchword.size() >= maxword.size())
							{
								maxword = matchword;
							}
							cookies[m][i] = -1;
							score ++;}
						}
					}
				}
				backward = "";
				for (int l = k; l >= j; l -= 1)				//backward word concatenation
				{
					backward += char(cookies[l][i] + 97);
				}
				dicpart(backward);
				for (int l = dicstart; l <= dicend; l += 1)
				{
					if (backward == dictionary[l])
					{
						for (int m = k; m >= j; m -= 1)		//backward word check
						{
							if (k > 1){
							matchword = backward;
							if (matchword.size() >= maxword.size())
							{
								maxword = matchword;
							}
							cookies[m][i] = -1;
							score ++;}
						}
					}
				}
			}
		}
	}
}


//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i) {

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			ofile.write((char*)&length, sizeof(int));
		}
		ofile.write((char*)&data[0], sizeof(char) * data.size());

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();

}
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i) {
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char*)&data[0], sizeof(char)* length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}
void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels
	* cheight= height of displayed cookiei pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
int GetAlphabet() {
	return GetRandInRange(0, 25);
}

void Pixels2Cell(int px, int py, int & cx, int &cy) {
}
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
{
}
void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
/*
* Main Canvas drawing function.
* */
void DisplayFunction() {
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
	//#if 0
	glClearColor(1/*Red Component*/, 1.0/*Green Component*/,
		1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	//write your drawing commands here or call your drawing functions...
	if (secs > 0)
	{
		for (int i = 0; i < 10; i += 1)				//Display alphabets of cookie-array
		{
			for (int j = 0, wgap = 15; j < 15; j += 1, wgap += 60)
			{
				DrawAlphabet((alphabets)cookies[i][j], wgap, height-(90+(i*60)), awidth, aheight);
			}
		}

		
		/** DRAWING SHOOTER BALLS START **/

		if (newcory > 450)
		{
			for (int i = 0; i < 15; i++)			//Determines the column for the shooter ball in array
			{	
				if (newcorx < (60*(i+1)-15))
				{
					col_num = i; break;
				}
				else if (newcorx >= (60*(i+1)-15) && newcorx < (60*(i+1)+15))
				{
					col_num = i+1; break;
				}
				
			}
			for (int i = 0; i < 10; i++)			//Determines the empty row for the shooter ball
			{
				if (cookies[i][col_num] == -1)
				{
					row_num = i; break;
				}
			}
			cookies[row_num][col_num] = shoot_num;			//Saves current ball to array
			shoot_num = shoot_num_second;				//Shifts next ball to current ball
			shoot_num_second = GetAlphabet();			//Generates new alphabet for second shooter ball
			newcorx = 465; newcory = 10; grdx = 0; grdy = 0;	//Resets shooter ball to original location
			DrawAlphabet((alphabets)shoot_num, newcorx, newcory, awidth, aheight);
			DrawAlphabet((alphabets)shoot_num_second, 800, 10, awidth, aheight);

			maxlengthword_h();					//Horizontal word check
			maxlengthword_v();					//Vertical word check
		}
		else
		{
			DrawAlphabet((alphabets)shoot_num, newcorx, newcory, awidth, aheight);
			DrawAlphabet((alphabets)shoot_num_second, 800, 10, awidth, aheight);
		}

		/** Draws Aimer **/
		DrawString(finalx3, finaly3, width, height, "o", colors[SKY_BLUE]);
		DrawString(finalx2, finaly2, width, height, "o", colors[BLUE]);
		DrawString(finalx1, finaly1, width, height, "o", colors[DARK_BLUE]);
		/** Draws Aimer **/

		/** Displays all the strings **/
		DrawString(40, 80, width, height + 5, "Longest Match found: " + maxword, colors[CHOCOLATE]);		
		DrawString(40, 40, width, height + 5, "Last Match found: " + matchword, colors[CHOCOLATE]);
		DrawString(780, 80, width, height + 5, "Next Ball:", colors[NAVY]);
		DrawString(40, height - 20, width, height + 5, "Score " + Num2Str(score), colors[BLUE_VIOLET]);
	}
	else {
		DrawString(width/2 - 60, height/2 + 60, width, height + 5, "Longest Match found: " + maxword, colors[CHOCOLATE]);
		DrawString(width/2 - 35, height/2, width, height + 5, "GAME OVER!", colors[DARK_RED]);
		DrawString(width/2 - 10, height/2 + 30, width, height + 5, "SCORE: " + Num2Str(score), colors[BLUE_VIOLET]);
	}
	/** DRAWING SHOOTER BALLS END **/

	DrawString(width / 2 - 30, height - 25, width, height,
		"Time Left: " + Num2Str(secs) + " secs", colors[RED]);

	// #----------------- Write your code till here ----------------------------#
	//DO NOT MODIFY THESE LINES
	DrawShooter((width / 2) - 35, 0, bwidth, bheight);
	glutSwapBuffers();
	//DO NOT MODIFY THESE LINES..
}

/* Function sets canvas size (drawing area) in pixels...
*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height) {
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
* is pressed from the keyboard
*
* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
*
* */

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...

	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	* this function*/
	/*
	glutPostRedisplay();
	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting
*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) {
	//If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls

	/** CACULATION OF AIMERS COORDINATES **/
	move_rad = pow((pow((x-495), 2)+pow((650-y), 2)),0.5);
	ratio1 = move_rad/100.0;
	ratio2 = move_rad/120.0;
	ratio3 = move_rad/140.0;

	finalx1 = ((x-495)/ratio1) + 495;
	finalx2 = ((x-495)/ratio2) + 495;
	finalx3 = ((x-495)/ratio3) + 495;

	finaly1 = ((650-y)/ratio1) - 10;
	finaly2 = ((650-y)/ratio2) - 10;
	finaly3 = ((650-y)/ratio3) - 10;
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
* You will have to add the necessary code here for shooting, etc.
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */

void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		if (state == GLUT_UP)
		{	
			if(grdx == 0 && grdy == 0)			// Calculation of slope
			{
				grdx = (x - 465)/50.0;
				grdy = (650 - y)/50.0;
			}
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{

	}
	glutPostRedisplay();
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyboard
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
}

/*
* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).
* You can use this function to animate objects and control the
* speed of different moving objects by varying the constant FPS.
*
* */
void Timer(int m) {
	if (newcory <= 450)			//Increments the coordinates till reaching the top two rows
	{
		if (newcorx >= 870 || newcorx <= 0)		//Bounces ball back in the canvas
		{
			grdx = -1*grdx;
		}
		newcory += grdy;
		newcorx += grdx;
	}

	if(secs > 0)				//Decreases remaining time
	{
		secs -= 1.0/60.0;
	}
	glutPostRedisplay();
	glutTimerFunc(1000.0/FPS, Timer, 0);
}

/*
* our gateway main function
* */
int main(int argc, char*argv[]) {
	InitRandomizer(); // seed the random number generator...

	//Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize];
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
	//print first 5 words from the dictionary
	for(int i=0; i < 5; ++i)
		cout<< " word "<< i << " =" << dictionary[i] <<endl;

	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets
	for (int i = 0; i < 10; i += 1)					//adds random numbers to cookies
	{
		for (int j = 0; j < 15; j += 1)
		{
			if (i < 2)					//sets first two rows to alphabet numbers
			{
				cookies[i][j] = GetAlphabet();
			}
			else						//sets other rows to non-alphabet numbers
			{
				cookies[i][j] = -1;
			}
		}
	}

	shoot_num = GetAlphabet();
	shoot_num_second = GetAlphabet();
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("ITCs Word Shooter"); // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0/FPS, Timer, 0);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();

	return 1;
}
#endif /* */
