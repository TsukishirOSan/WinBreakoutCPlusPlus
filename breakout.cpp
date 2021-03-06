/**
*
*   WinBreakoutC++
*
*   breakout.cpp
*
*   2015 a collaboration of CS50x students
*
*/

#include "breakout.h"

/**   declare some global object variables   **/
theBALL ballInfo;
RECT mwinRect;    // main window rectangle
RECT ballRect;    // ball rectangle
RECT ballRectInvaild;    // ball rectangle for future and past positions
                         // needed to redraw only where needed
RECT textRect;    // text rectangle
RECT paddleRect;    // paddle rectangle
RECT paddleRectInvaild;    // rectangle need to for paddle redraw

POINTS mpoint_x;    // POINTS structure to relay mouse data to paddle


/**   need to keep track of paddle length beyond it's creation   **/
/**   maybe a paddle info structure like ball                    **/
int paddlelength = 0;


/**   make the main window   **/
HWND createMainWin(HINSTANCE hThisInstance, int sizeX, int sizeY){

    /**   declare a class name. Why? Because.  **/
    TCHAR szClassName[ ] = _T("Student Project");

    /**   declare the main window handle   **/
    HWND hwnd = NULL;

    /**   declare data structure object variable for the window class   **/
    WNDCLASSEX wincl;

    /**   initialize the window structure   **/
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;   // This function is called by windows
    wincl.style = CS_DBLCLKS;     // Catch double-clicks
    wincl.cbSize = sizeof (WNDCLASSEX);

    /**  use default icon and mouse-pointer  **/
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);

    /**   no menu   **/
    wincl.lpszMenuName = NULL;

    /**   no extra bytes after the window class structure
          or the window instance                            */
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;

    /**   add a custom background color   */
    wincl.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

    /**   Register the window class, if it fails quit the program   */
    if(!RegisterClassEx(&wincl))
        return 0;

    /**   the class is registered,
          let's create the programs main window   */
    hwnd = CreateWindowEx(
           0,                  // extended possibilities for variation
           szClassName,            // Class name
           _T(" WinBreakoutC++ "),   // Title Text
           WS_OVERLAPPEDWINDOW,     // default window style
           CW_USEDEFAULT,       // windows decides the position
           CW_USEDEFAULT,       // where the window ends up on the screen
           sizeX,             // the window width
           sizeY,             // and height in pixels
           HWND_DESKTOP,      // the window is a child-window to desktop
           NULL,              // no menu
           hThisInstance,     // the program instance handle
           NULL               // no window creation data
           );

    /**   if window creation successful return the main window handle
          any work in the window will need this handle                  **/
    if(hwnd)
        return hwnd;

    /**   if we get this far we have failed to create
          a window for our program, returning 0 will
          cause a message to the user and then quit the process.   **/

    return 0;
}

/**   pay sound   **/
void playSound(int sound){

    switch(sound){
        case 1:
        PlaySound(MAKEINTRESOURCE(IDW_MYBALLSOUND),
                GetModuleHandle(NULL), SND_ASYNC|SND_RESOURCE);
        break;
        default:;
    }

}

/**   make the ball   **/
void createBall(int sizeDia, int x, int y){

    /**   set up the ball rectangles   **/
    ballRect.left = x - sizeDia / 2;
    ballRect.top = y - sizeDia / 2;
    ballRect.right = x + sizeDia / 2;
    ballRect.bottom = y + sizeDia / 2;
    ballInfo.sizeDia = sizeDia;
    ballInfo.x = ballRect.left;
    ballInfo.y = ballRect.top;

    /**   seed random number gen   **/
    srand(GetTickCount());

    /**   provide some random kick to the ball velocity   **/
    // TODO: this method can result in zero
    ballInfo.vx = 2 + (int)(double)rand() / (RAND_MAX + 1) * (3 - -3) + -3;
    ballInfo.vy = 4 + (int)(double)rand() / (RAND_MAX + 1) * (3 - -3) + -3;

    /**   play a starting sound / a natural pause   **/
    playSound(1);
}

/**   make the paddle   **/
void createPaddle(int posy, int length, int height){

    /**   preserve the length of the paddle   **/
    paddlelength = length;

    /**   load paddle rectangle  **/
    paddleRect.left = mwinRect.right / 2 - length / 2;
    paddleRect.top = posy;
    paddleRect.right = paddleRect.left + length;
    paddleRect.bottom = paddleRect.top + height;

    /**   load the paddle invalidate rectangle   **/
    paddleRectInvaild = paddleRect;
    paddleRectInvaild.left = mwinRect.left;
    paddleRectInvaild.right = mwinRect.right;
}

/**   update paddle rectangle when ever mouse moves   **/
void updatePaddle(){

      /**   alter paddle position from mouse position   **/
      paddleRect.left = mpoint_x.x - paddlelength / 2;
      paddleRect.right = paddleRect.left + paddlelength;
}

/****************************
 *                          *
 *    update game data      *
 *                          *
 ****************************/
/*    the whole game is a bunch of rectangles   */
/*    manipulated here                          */
void updateGame(HWND hwnd){

   /**********************
    *                    *
    *    Move The Ball   *
    *                    *
    **********************/

    /**   MS's Offset Rectangle function make moving the rectangle easy   **/
    OffsetRect(&ballRect, ballInfo.vx, ballInfo.vy);

    /**   copy new ball rectangle   **/
    ballRectInvaild = ballRect;

   /*************************************************************************
    *   Adjust the RECT used to invalidate the part of the screen we need   *
    *   to redraw for the ball.  The region needed to be redrawn is where   *
    *   the ball is and where it is going to be on the next screen redraw   *
    *************************************************************************/
    if(ballInfo.vx > 0){
        ballRectInvaild.left = ballRect.left - ballInfo.vx;
        ballRectInvaild.right = ballRect.right + ballInfo.vx;
    }
    else{
        ballRectInvaild.left = ballRect.left + ballInfo.vx;
        ballRectInvaild.right = ballRect.right - ballInfo.vx;
    }

    if(ballInfo.vy > 0){
        ballRectInvaild.top = ballRect.top - ballInfo.vy;
        ballRectInvaild.bottom = ballRect.bottom + ballInfo.vy;
    }
    else{
        ballRectInvaild.top = ballRect.top + ballInfo.vy;
        ballRectInvaild.bottom = ballRect.bottom - ballInfo.vy;
    }

   /******************************
    *                            *
    *   Detect Ball Collisions   *
    *                            *
    ******************************/
    detectCollisions(hwnd);
}

/**   detect collisions of the ball
      with window walls, paddle and bricks        **/
void detectCollisions(HWND hwnd){

    /**   get main window rectangle   **/
    GetClientRect(hwnd, &mwinRect);

    /**   ball can collide in corner any one or   **/
    /**   two of the following are possible       **/

    /**   if collision left   **/
    if(ballRect.left <= mwinRect.left){
       ballInfo.vx = -ballInfo.vx;
       ballRect.left = mwinRect.left + 1;
       ballRect.right = ballInfo.sizeDia + 1;
       playSound(1);
    }
    /**   if collision top   **/
    if(ballRect.top <= mwinRect.top){
        ballInfo.vy = -ballInfo.vy;
        ballRect.top = mwinRect.top + 1;
        ballRect.bottom = ballInfo.sizeDia + 1;
        playSound(1);
    }
    /**   if collision right   **/
    if(ballRect.right >= mwinRect.right){
        ballInfo.vx = -ballInfo.vx;
        ballRect.left = mwinRect.right - ballInfo.sizeDia - 1;
        ballRect.right = mwinRect.right - 1;
        playSound(1);
    }
    /**   if collision bottom   **/
    // TODO (ebob): add code to flag event "ball lost"
    if(ballRect.bottom >= mwinRect.bottom){
        ballInfo.vy = -ballInfo.vy;
        ballRect.top = mwinRect.bottom - ballInfo.sizeDia - 1;
        ballRect.bottom = mwinRect.bottom - 1;
        playSound(1);
    }

    //  TODO:  this is messy and not complete
    /**   detect paddle collisions   **/
    if(ballRect.bottom > paddleRect.top){

        if(paddleRect.left < ballRect.right &&
                             paddleRect.right > ballRect.left){

            ballInfo.vy = -ballInfo.vy;
            playSound(1);
        }
    }

}

/**   paint text in window   **/
void paintText(HDC hdc, LPCTSTR tsText){

    /**   write character string to location x, y   **/

    /* TODO: This code needs work, our exact needs for text display is
             not completely defined yet                                  */
    int width = 150;
    int height = 60;

    /**   like everything we draw to the screen   **/
    /**   create the text rectangle               **/
    textRect.left = (mwinRect.right - width) / 2 ;
    textRect.top = 60;
    textRect.right = textRect.left + width;
    textRect.bottom = textRect.top + height;

    /**   draw the text using rectangle   **/
    DrawTextEx(hdc, (LPSTR)tsText, _tcslen(tsText), &textRect, DT_CENTER, 0);

}


/**   refresh the window here we call all drawing functions   **/
void refreshWindow(HWND hwnd, LPCTSTR lpOptionalText){

    /**   we need to deal with optional text           **/
    /**   calling function can omit the text           **/
    /**   this is demonstrative at this point           **/

    /**   declare a wide CHAR buffer for the Unicode crowd   **/
    /**   for some one you this is a string                  **/
    TCHAR tcText[64];
    // TODO: there is a wide character issue here, important ??
    sprintf_s(tcText, sizeof(tcText),
              "%s\n x = %d y = %d\n paddle x = %d" ,
              "ball position",
              ballRect.left, ballRect.top, mpoint_x.x);

    /**   if calling function sends text, us it   **/
    if(lpOptionalText)
        lstrcpy((LPSTR)tcText, lpOptionalText);

    /**   declare the paint structure object variable   **/
    /**   needed by the device context                    **/
    PAINTSTRUCT ps;

    /**   get the handle of the device context for our window needed   **/
    /**   by functions that paint or draw to the window                **/
    HDC hdc = BeginPaint(hwnd, &ps);

   /*******************
    *                 *
    *   DRAW PADDLE   *
    *                 *
    *******************/
    Rectangle(hdc,
              paddleRect.left,
              paddleRect.top,
              paddleRect.right,
              paddleRect.bottom);


   /**********************************
    *                                *
    *    PAINT TEXT TO THE WINDOW    *
    *                                *
    **********************************/
    paintText(hdc, (LPCTSTR)tcText);


    /**   using the device context we can change              **/
    /**   the color of pen and brush for the device context   **/
    /**   changing before drawing the ball causes the ball    **/
    /**   to change to red in this example                    **/

    /**   Note: if you don't select on object, PEN, BRUSH   **/
    /**         you can't change it                         **/
    SelectObject(hdc, GetStockObject(DC_PEN));
    SetDCPenColor(hdc, RGB(255, 0, 0));

    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(255, 0, 0));

   /*********************
    *                   *
    *   DRAW THE BALL   *
    *                   *
    *********************/

    Ellipse(hdc,
            ballRect.left,     // int nLeftRect
            ballRect.top,      // int nTopRect
            ballRect.right,    // int nRightRect
            ballRect.bottom);  // int nBottomRect


    /**   much like free we use end paint to return        **/
    /**   the device context to the system.  if we         **/
    /**   don't release the DC the resource will be lost   **/
    /**   as a memory leak                                 **/
    EndPaint(hwnd, &ps);
}
