#include<stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#ifdef __WIN64
    #include <windows.h>  //for Sleep() only
    #include<GL/glut.h>
#elif __WIN32
    #include <windows.h>  //for Sleep() only
    #include<GL/glut.h>
#elif __APPLE_CC__
    #include <GLUT/glut.h>
#elif __linux
    #include<GL/glut.h>
#elif __unix
    #include<GL/glut.h>
#endif
#include<limits.h>
#include<unistd.h>
#include<time.h>
#include<math.h>
#include<cstring>
#include<iostream>
#include<vector>


#define MAX_CARS 7

void drawCar();
void drawFootPath();
void drawDivider();
void stopGame();
void drawOtherCars(int);
void drawOther();
void setCars();
void moveOtherCars();
int detectCollision();
void drawString(std::string);
void drawMainMenu();
void drawExitMenu();
void drawTime();
void showCoin();
void showFuel();
void showDist();
void stopGame();
void resumeGame();
void drawScore(int);
void drawEnd();
void moveEnd();
void drawFuelBar();
void fuelMessage();
void drawFuel();
void drawInstruct();
void drawAbout();
void moveFuel();
void drawCoin();
void moveCoin();
int foot_y = 250;
int div_y = 220;
int end_y = 160;

int a = 0;
int speed = 1;
int steerSpeed = 1;
int temp = 0;
int seconds = 0;
int distance = 1000;
int fuel_x = 0;
int fuel_y = -80;
int coin_x = 0;
int coin_y = -80;
int fuel = 250;
int game_state = 0;
int coin=0;

void moveDivider();
void moveRoad();
void moveCar();
void periodicFunction(int);


int lane[MAX_CARS];
int pos[MAX_CARS];
int speeds[MAX_CARS];
int speedsTemp[MAX_CARS];

GLdouble width=1400, height=800;

bool carMoveLeft = false, carMoveRight = false, carMoveFast = false, carStopped = false, gameStopped = false, horn = false;
bool highlightStart = false, highlightExit = false;
bool reachedEnd = false, gameCompleted = false ,fuelOver = false;;
//Car variables
int car_x = 0;
int car_y = -100;



void maindisp()
{

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

    switch(game_state)
	{
        case 1 : glClearColor(0, 0, 0, 0);
                 drawFootPath();

                 drawDivider();
                 drawEnd();
                 drawCar();
                 drawOther();
                 drawTime();
                 showCoin();
                 showFuel();
                 showDist();
                 drawFuel();
                 drawCoin();

                 break;

        case 0 : drawMainMenu();

                 break;

        case 2 : glClearColor(0, 0, 0, 0);
                 drawFootPath();

                 drawDivider();
                 if(gameCompleted)
                    drawEnd();

                if(fuelOver)
                     fuelMessage();
                     drawCar();
                     drawOther();
                     drawExitMenu();
                     drawTime();
                     showCoin();
                     showFuel();
                     showDist();
                     break;

            case 3 : drawAbout();
                        break;

            case 4 : drawInstruct();
                        break;
	}

	glFlush();
	glutSwapBuffers();
}




void keyBoardFunc(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT : carMoveLeft = true; carMoveRight = false; break;
		case GLUT_KEY_RIGHT : carMoveRight = true; carMoveLeft = false; break;
		case GLUT_KEY_UP 	: carMoveFast = true; break;
	}
}



void keyboard_up_func(int k, int x, int y)
{
    switch(k)
    {
        case GLUT_KEY_LEFT: carMoveLeft=false;
                            break;
        case GLUT_KEY_RIGHT:carMoveRight=false;
                            break;

		case GLUT_KEY_UP : carMoveFast = false; break;
		case GLUT_KEY_DOWN : carStopped = false; break;
    }
}


void mouse_func(int button, int state, int x, int y)
{
    switch(game_state)
    {
        case 0 : if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
                 {
                    if(x >=  630 && x <= 810 && y >= 320 && y <= 405)
                    {
                        //gameStopped = false;
                        setCars();
                        game_state = 1;
                    }

                    else if(x >=  630 && x <= 810 && y >= 490 && y <= 575)
                        exit(0);
                 }

                    else if(x >=  630 && x <= 810 && y >= 635 && y <= 785)
                    {
                        drawAbout();
                        game_state = 3;
                    }
                    else if(x >=  630 && x <= 810 && y >= 145 && y <= 225)
                    {
                        drawInstruct();
                        game_state = 4;
                    }
                 break;




        case 2 : if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
                 {
                    if(x >=  630 && x <= 810 && y >= 320 && y <= 405)
                    {
                        gameStopped = false;
                        gameCompleted = false;
                        reachedEnd = false;
                        seconds = 0;
                        distance = 1000;
                        fuel = 250;
                        coin=0;
                        fuelOver = false;
                        end_y = 160;
                        setCars();
                        game_state = 1;
                        car_x = 0;
                    }

                    else if(x >=  630 && x <= 810 && y >= 490 && y <= 575)
                        exit(0);
                 }
                 break;

        case 3: if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
                 {
                        game_state = 0;
                 }
                 break;

        case 4:if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
                 {
                        game_state = 0;
                 }

    }

}



int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize((int) width, (int) height);
	glutCreateWindow("Galaxian");
	glutFullScreen();


	gluOrtho2D(-240.0, 240.0, -160.0, 160.0);

	glutDisplayFunc(maindisp);
	glutTimerFunc(100, periodicFunction, 0);

    glutSpecialFunc(keyBoardFunc);
    glutSpecialUpFunc(keyboard_up_func);

    glutMouseFunc(mouse_func);

	glutMainLoop();
	return 0;
}

//This function draws a car at (car_x, car_y)
void drawCar(void)
{
    glPushMatrix();

    glTranslated(car_x, car_y, 0.0);

    glColor3f(1.0, 0.70, 1.0);

    glRectd(2, 10, -2, -10);
    glRectf(-8, 12, 8, 13);
    glRectf(-10.0, 9.0, -8.0, 15.0);
    glRectf(8.0, 9.0, 10.0, 15.0);

    glBegin(GL_LINES);
        glVertex2f(8.0,11.0);
        glVertex2f(6.0,7.0);
        glVertex2f(6.0,7.0);
        glVertex2f(6.0,-7.0);
        glVertex2f(6.0,-7.0);
        glVertex2f(10.0,-12.0);
        glVertex2f(-8.0,11.0);
        glVertex2f(-6.0,7.0);
        glVertex2f(-6.0,7.0);
        glVertex2f(-6.0,-7.0);
        glVertex2f(-6.0,-7.0);
        glVertex2f(-10.0,-12.0);
        glVertex2f(10.0,-12.0);
        glVertex2f(-10.0,-12.0);
    glEnd();

    glRectf(-11.0, -16.0, -9.0, -8.0);
    glRectf(9.0, -16.0, 11.0, -8.0);
    glRectf(-7.0, -14.0, -5.0, -12.0);
    glRectf(5.0, -14.0, 7.0, -12.0);


	glPopMatrix();
}





void drawOtherCars(int i)
{
    glPushMatrix();

    glTranslated((lane[i] - 3)*40, pos[i], 0.0);

    switch(speeds[i])
    {
        case 2 : glColor3f(0.0, 1.0, 0.0); break;
        case 3 : glColor3f(1.0, 0.0, 1.0); break;
        case 4 : glColor3f(1.0, 1.0, 1.0); break;
        case 5 : glColor3f(0.0, 1.0, 1.0); break;
        case 6 : glColor3f(1.0, 1.0, 0.0); break;
        case 7 : glColor3f(1.0, 0.0, 0.0); break;
        case 0 : glColor3f(0.0, 0.0, 0.0); break;
    }

    glRectd(2, 10, -2, -10);
    glRectf(-8, 12, 8, 13);
    glRectf(-10.0, 9.0, -8.0, 15.0);
    glRectf(8.0, 9.0, 10.0, 15.0);

    glBegin(GL_LINES);
        glVertex2f(8.0,11.0);
        glVertex2f(6.0,7.0);
        glVertex2f(6.0,7.0);
        glVertex2f(6.0,-7.0);
        glVertex2f(6.0,-7.0);
        glVertex2f(10.0,-12.0);
        glVertex2f(-8.0,11.0);
        glVertex2f(-6.0,7.0);
        glVertex2f(-6.0,7.0);
        glVertex2f(-6.0,-7.0);
        glVertex2f(-6.0,-7.0);
        glVertex2f(-10.0,-12.0);
        glVertex2f(10.0,-12.0);
        glVertex2f(-10.0,-12.0);
    glEnd();

    glRectf(-11.0, -16.0, -9.0, -8.0);
    glRectf(9.0, -16.0, 11.0, -8.0);
    glRectf(-7.0, -14.0, -5.0, -12.0);
    glRectf(5.0, -14.0, 7.0, -12.0);


	glPopMatrix();
}







void drawFootPath()
{
	int i, y;
	glPushMatrix();

	//Draw right side foot path
	glTranslated(150, 0, 0);
	y = foot_y + 20;

	for(i = 0; i < 20; i++)
	{
		if(a == 0)
		{
			if(i % 2 == 0)
				glColor3f(0.0, 1.0, 0.0);
			else
				glColor3f(1.0, 0.0, 0.0);
		}

		else
		{
			if(i % 2 == 1)
				glColor3f(0.0, 0.0, 1.0);
			else
				glColor3f(1.0, 1.0, 0.0);
		}


		y -= 20;
		glRectd(5, y, -5, y - 20);
	}
	glPopMatrix();

	//Draw left side foot path
	glPushMatrix();
	glTranslated(-150, 0, 0);

	y = foot_y + 20;
	for(i = 0; i < 20; i++)
	{

		if(a == 0)
		{
			if(i % 2 == 0)
				glColor3f(0.0, 1.0, 0.0);
			else
				glColor3f(1.0, 0.0, 0.0);
		}

		else
		{
			if(i % 2 == 1)
				glColor3f(0.0, 0.0, 1.0);
			else
				glColor3f(1.0, 1.0, 0.0);
		}

		y -= 20;
		glRectd(5, y, -5, y - 20);
	}
	glPopMatrix();

}








void drawDivider()
{
	int i;
	int y = div_y + 280;
	glColor3f(1.0, 1.0, 1.0);

	for(i = 0 ; i < 8; i++)
	{
		y-=80;
		glRectd(22, y, 18, y-40);
        glRectd(62, y, 58, y-40);
        glRectd(102, y, 98, y-40);
        glRectd(-18, y, -22, y-40);
        glRectd(-58, y, -54, y-40);
        glRectd(-98, y, -94, y-40);
	}

}





void stopGame()
{
	speed = 0;
	steerSpeed = 0;
}






void resumeGame()
{
    speed = 2;
    steerSpeed = 1;
}






void drawOther()
{
	int i;
	for(i = 0; i < MAX_CARS; i++)
	{
		drawOtherCars(i);
	}
}






void setCars()
{
	int i;
	for(i = 0; i < MAX_CARS; i++)
	{
		lane[i] = i;
		pos[i] = 10 + rand()%100;
        speeds[i] = 1 + i + rand()%4;
	}
}






void moveDivider()
{
	div_y -= speed;
	if(div_y < 120 && distance > 0)
    {
        div_y = 200;

        if(carMoveFast)
        {
            distance -= 1.5;
            fuel -=1.5;
        }
        else
        {

            distance -= 0.5;
            fuel -= 0.5;
        }

        if(fuel < 0)
            fuelOver = true;

        if(distance < 5)
            reachedEnd = true;

    }
}





void moveCar()
{
	if(carMoveLeft)
		car_x -= steerSpeed;


	else if(carMoveRight)
		car_x += steerSpeed;

	if(car_x > 130 || car_x < -130)
    {
        game_state = 2;
        gameStopped = true;
    }
}





void moveRoad()
{
	foot_y -= speed;
	if(foot_y < 160)
	{
		foot_y = 180;
		if(a == 0)
			a = 1;
		else
			a = 0;
	}
}




void periodicFunction(int v)
{
    if(gameStopped)
        stopGame();

    else
        resumeGame();

	if(speed != 0)
	{
		if(carMoveFast)
            speed = 6;

		else
            speed = 2;

	}

	if(fuelOver)
	{
        gameStopped = true;
        game_state = 2;

	}

	moveRoad();
	moveDivider();
	moveCar();
    moveOtherCars();
    moveEnd();
    moveFuel();
    moveCoin();

    if(!detectCollision() && !gameStopped && game_state == 1)
        temp += 15;

	if(temp > 1000)
    {
        temp  = 0;
        seconds++;
    }

    if(coin)
        speed+=1;
    else
        speed+=0;

	glutPostRedisplay();
	glutTimerFunc(10, periodicFunction, v);
}







void moveOtherCars()
{
    int i;
    for(i = 0; i < MAX_CARS; i++)
    {
        pos[i] += -speed + speeds[i];

        if(pos[i] < -500 || pos[i] > 500)
        {
            pos[i] = 500 + rand()%1000;
            speeds[i] = 2 + rand()%4;
        }
    }

}







int detectCollision()
{
    if(game_state != 1)
        return 0;

    int i, limit;
    for(i = 0; i < MAX_CARS; i++)
    {
        if(pos[i] < -70 && pos[i] > -130)
        {
            limit = (i - 3)*40;
            if(car_x < limit + 22 && car_x > limit - 22)
            {
                speeds[i] = 0;
                gameStopped = true;
                game_state = 2;
                return 1;
            }
        }
    }

        if((fuel_x > car_x && fuel_x - car_x < 20) || (fuel_x < car_x && car_x - fuel_x < 20))
    {
        if(fuel_y < -80 && fuel_y > -120)
        {
            fuel+=40;
            fuel_y = 100 + rand()%150;
            fuel_x = (rand()%7 - 3)*40;
        }
    }

    if((coin_x > car_x && coin_x - car_x < 20) || (coin_x < car_x && car_x - coin_x < 20))
    {
        if(coin_y < -80 && coin_y > -120)
        {
            coin+=1;
            if(coin > 250)
                coin = 250;
            coin_y = 100 + rand()%150;
            coin_x = (rand()%7 - 3)*40;
        }
    }

    return 0;
}




void draw_string(std::string str)
{
    for(unsigned int i=0;i<str.length();i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *(str.begin()+i));
    }
}








void drawMainMenu()
{

        glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(0, 90, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-20, 90, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("INST'S");

    glPopMatrix();
    //Draw start button
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(0, 30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-20, 30, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("START");

    glPopMatrix();

    //Draw exit button
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslated(0, -30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-15, -30, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("EXIT");

    glPopMatrix();



    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(0, -90, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-20, -90, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("ABOUT");

    glPopMatrix();

}
//-----------------------------------------------------------------------------------------------------------------




void drawExitMenu()
{
    //Draw start button
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(0, 30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-25, 30, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("RESTART");

    glPopMatrix();

    //Draw exit button
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslated(0, -30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-15, -30, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("EXIT");

    glPopMatrix();


}


// draw the end point ------------------------------------------------------------------------------------


void drawEnd()
{
    int i, j;
    for(i = 0; i < 5; i++)
    {
        if(i % 2 == 0)
            glColor3f(0.0, 0.0, 0.0);
        else
            glColor3f(1.0, 1.0, 1.0);
        glPushMatrix();

        glTranslated(-55 , end_y + 10*i, 0);

        for(j = 0; j < 11; j++)
        {
            if(i%2)
            {
                if(j % 2 == 0)
                    glColor3f(0.0, 0.0, 0.0);
                else
                    glColor3f(1.0, 1.0, 1.0);

            }

            else
            {
                if(j % 2)
                    glColor3f(0.0, 0.0, 0.0);
                else
                    glColor3f(1.0, 1.0, 1.0);
            }

            glRectd(10 * j,0,10*(j+1),10);
        }

        glPopMatrix();
    }
}



void moveEnd()
{
    if(reachedEnd)
        end_y-= speed;

    if(end_y < -125)
    {
        gameStopped = true;
        gameCompleted = true;
        game_state = 2;
    }
}


// do fuel and coin stuff --------------------------------------------------------------------------------

void fuelMessage()
{
    glPushMatrix();
    glTranslated(170, -70, 0);
    glScalef(0.07,0.07,0.07);
    glColor3f(1.0, 0,0);
    draw_string("Fuel Exhausted\n");
    glPopMatrix();
}



void drawFuel()
{
    glPushMatrix();
    glTranslated(fuel_x, fuel_y, 0);
    glColor3f(0.0, 1.0, 0.0);
    glRectf(5, 5, -5, -5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(fuel_x, fuel_y+5, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(5, 0);
        glVertex2f(5, 5);
        glVertex2f(-3, 5);
        glVertex2d(-5, 0);
    glEnd();
    glPopMatrix();

}

void moveFuel()
{
    fuel_y-=speed;

    if(fuel_y < -200)
    {
        fuel_y = 100 + rand()%150;
        fuel_x = (rand()%7 - 3)*40;
    }
}

void drawCoin()
{
    glPushMatrix();
    glTranslated(coin_x, coin_y, 0);
    glColor3f(1.0, 1.0, 0.0);
    glRectf(5, 5, -5, -5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(coin_x, coin_y+5, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(5, 0);
        glVertex2f(5, 5);
        glVertex2f(-3, 5);
        glVertex2d(-5, 0);
    glEnd();
    glPopMatrix();

}

void moveCoin()
{
    coin_y-=speed;

    if(coin_y < -200)
    {
        coin_y = 100 + rand()%150;
        coin_x = (rand()%7 - 3)*40;
    }
}




// display current scores etc ----------------------------------------------------------------------------

void drawScore(int score)
{
    int temp = score;
    int str[20],i=0;

    while(temp>0)
    {
        str[i++] = (temp%10);
        temp /= 10;
    }
    i--;
    temp+=temp;
    while(i>=0)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i--]+'0');
    }
    if(score == 0) glutStrokeCharacter(GLUT_STROKE_ROMAN, '0');
}

void showCoin()
{
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(-200, 50, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(40, 65);
        glVertex2f(40, 35);
        glVertex2f(-35, 35);
        glVertex2d(-35, 65);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-230, 95, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("Coins: ");
    glPopMatrix();

    glPushMatrix();
    glTranslated(-190, 95, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 0.0, 0.0);
    drawScore(coin);
    glPopMatrix();
}


void showFuel()
{
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(200,90, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(40, 65);
        glVertex2f(40, 35);
        glVertex2f(-40, 35);
        glVertex2d(-40, 65);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(165, 135, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("Fuel: ");
    glPopMatrix();

    glPushMatrix();
    glTranslated(195, 135, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 0.0, 0.0);
    drawScore(fuel);
    glPopMatrix();
}


void showDist()
{
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(200,50, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(40, 65);
        glVertex2f(40, 35);
        glVertex2f(-40, 35);
        glVertex2d(-40, 65);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(160, 95 ,0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("Distance: ");
    glPopMatrix();

    glPushMatrix();
    glTranslated(215, 95, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 0.0, 0.0);
    drawScore(distance);
    glPopMatrix();
}



void drawTime()
{
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(-200, 90, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(40, 65);
        glVertex2f(40, 35);
        glVertex2f(-35, 35);
        glVertex2d(-35, 65);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-230, 135, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("Time: ");
    glPopMatrix();

    glPushMatrix();
    glTranslated(-190, 135, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 0.0, 0.0);
    drawScore(seconds);
    glPopMatrix();
}



void drawAbout()
{
// draw instructions--------------------------------------------------------------------------------------------

    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslated(200, -70, 0);
    glBegin(GL_LINE_LOOP);                              //placement of the rectangle
        glVertex2f(-430, 225);
        glVertex2f(-430, -85);
        glVertex2f(30, -85);
        glVertex2d(30, 225);
        glColor3f(1,1,0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-25, 50, 0);
    glScalef(0.08, 0.08, 0.08);
    draw_string("GUIDED BY");

    glTranslated(-900, -280, 0);
    draw_string("MR PUNEET RP");

    glTranslated(-1600, -280, 0);
    draw_string("ASSISTANT PROFESSOR GRADE 2");

    glTranslated(-2850,-280, 0);
    draw_string("DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING");

    glPopMatrix();

//--------------------------------------------------------------------------------------------------------------------


    glPushMatrix();
    glTranslated(-40, -50, 0);
    glScalef(0.08, 0.08, 0.07);
    draw_string("SUBMITTED BY");


    glTranslated(-1500, -200, 0);                                           // placement of the text
    glScalef(1,1,1);                                              //change text size
    draw_string("PAVAN D JAVALI - 4NM18CS111");


    glTranslated(-2000, -210, 0);                                           // placement of the text
    glScalef(1,1,1);                                              //change text size
    draw_string("PRAJWAL T - 4NM18CS113");


    glTranslated(-1750, -220, 0);
    glScalef(1, 1, 1);
    draw_string("COURSE CODE : 18CS603");


    glTranslated(-2100, -230, 0);                                           // placement of the text
    glScalef(1, 1, 1);                                              //change text size
    draw_string("COURSE NAME : COMPUTER GRAPHICS");


    glTranslated(-2300, -240, 0);                                           // placement of the text
    glScalef(1,1,1);                                              //change text size
    draw_string("SEMESTER : VI  |  SECTION : C");


    glPopMatrix();



    //Title-----------------------------------------------------------------------------------------------------------

    glPushMatrix();
    glTranslated(-100, 140, 0);
    glScalef(0.07, 0.07, 0.07);
    draw_string("NMAM INSTITUTE OF TECHNOGLOGY NITTE");

    glTranslated(-3200, -250, 0);
    glScalef(1, 1, 1);
    draw_string("DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING");

    glTranslated(-2700, -260, 0);
    glScalef(1, 1, 1);
    draw_string("A MINI PROJECT ON");

    glTranslated(-1000, -250, 0);
    glScalef(1, 1, 1);
    draw_string("SPORTSCAR");

    glPopMatrix();

}

void drawInstruct()
{

 //Draw rules button---------------------------------------------------------------------------------------------
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslated(200, -70, 0);
    glBegin(GL_LINE_LOOP);                              //placement of the rectangle
        glVertex2f(-430, 225);
        glVertex2f(-430, -85);
        glVertex2f(30, -85);
        glVertex2d(30, 225);
        glColor3f(1,1,0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-20, 140, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("RULES");

    glTranslated(-2400, -200, 0);
    glScalef(0.7, 0.7, 0.7);
    draw_string("1. DO NOT HIT THE FOOTPATH");


    glTranslated(-2120, -250, 0);
    draw_string("2. DO NOT HIT OTHER CARS ON THE WAY");


    glTranslated(-2980, -250, 0);
    draw_string("3. REACH THE FINISH LINE BEFORE THE FUEL RUNS OUT");


    glTranslated(-4000, -250, 0);
    draw_string("4. COLLECT GREEN CUBES TO INCREASE FUEL AND YELLOW CUBES FOR POINTS");

    glTranslated(-5550, -250, 0);
    draw_string("5. TRY TO BEAT THE GAME IN THE LEAST AMOUNT OF TIME BY COLLECTING THE CUBES");


    glTranslated(-3700, -500, 0);
    glScalef(1.5, 1.5, 1.5);
    draw_string("INSTRUCTIONS");

    glTranslated(-900, -200, 0);
    glScalef(0.5, 0.5, 0.5);
    draw_string("1) HOLD ^ TO STEER UP");


    glTranslated(-1800, -300, 0);
    draw_string("2) HOLD > TO STEER RIGHT");


    glTranslated(-2000, -300, 0);
    draw_string("3) HOLD < TO STEER LEFT");

    glTranslated(-1600, -600, 0);
    glScalef(2, 2, 2);
    draw_string("GAMEPLAY");

    glTranslated(-2200, -200, 0);
    glScalef(0.5, 0.5, 0.5);
    draw_string("A BASIC SPORTS CAR GAME WHICH INVOLVES MANY OBSTACLES ON ITS WAY TO REACH ITS FINISH LINE");


    glTranslated(-7700, -300, 0);
    glScalef(1,1,1);
    draw_string("A PLAYER SHOULD TRY TO REACH THE END POINT WITHOUT HITTING OTHER CARS AND BEFORE THE FUEL GETS EMPTY");

    glColor3f(0.0, 1.0, 1.0);
    glTranslated(-2800, -720, 0);
    glScalef(1,1,1);
    draw_string("RIGHT CLICK TO GO BACK TO THE MENU");


    glPopMatrix();
}
