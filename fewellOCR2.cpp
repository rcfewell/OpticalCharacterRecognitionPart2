#include "EasyBMP.cpp"
#include "EasyBMP.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <iomanip>
using namespace std;


struct letterData
{
  int holes;
  float gPixelCount;
  float ratio;
  float area;
  float pixelReg;
  float quad1;
  float quad2;
  float quad3;
  float quad4;
  float region1;
  float region2;
  float region3;
  float region4;
  float region5;
  float region6;
  float region7;
};


int findPic (BMP & I, int xleft, int yleft, int boxWidth, int boxHeight, letterData data[], int letterCount);
void print (int letterCount, letterData data[]);
//float percent (int letterCount, vector<float>area, queue & q, int width, int height);

//default size of the queue
const int MSIZE = 10000;

// allows for a pixels x and y values to be 
//declared as a point
class point {
private:
  int x; int y;

public:

  //declares the point and assigns
  //x and y values
  point(int p, int q) {
    x = p; y = q;
  }

  int getx() {
    return x;
  }

  int gety() {
    return y;
  }
};

//allows for a queue to be inistialized
// making it easy to perform specific tasks
//such as insert and delete
class queue {

private:
  point* Q[MSIZE];

  int front, rear, size;

public:
  queue() {
    // initialize an empty queue
    front = 0; rear = 0; size = 0;
    for (int j=0; j < MSIZE; ++j)
      Q[j] = 0;
  }

  void insert(point* x) {
    if (size != MSIZE) {
      front++; size++;
      if (front == MSIZE) front = 0;
      Q[front] = x;
    }
  }

  point del() {
    if (size != 0) {
      rear++; if (rear == MSIZE) rear = 0;
      point temp(Q[rear]->getx(), Q[rear]->gety());
      size--;
      return temp;
    };
  }

  void print() {
    for (int j = 1; j <= size; ++j) {
      int i = front - j + 1;
      cout << "x = " << Q[i]->getx() << " y = " << Q[i]->gety() << endl;
    }
    cout << "end of queue" << endl;
  }

  bool isEmpty() {
    return (size == 0);
  }
};


void Conversion(BMP & imageIn, BMP & imageOut, letterData data [])
{
  
  //RGB color declaration of green and red
  /***************/
  RGBApixel red;
  red.Red = 255;
  red.Green = 0;
  red.Blue = 0;
  /**************/
  RGBApixel green;
  green.Red = 0;
  green.Green = 255;
  green.Blue = 0;
  /*************/

  // declaration of variables to count
  //amount of lines and letters in the image
  int letterCount = 0;
  int lineCount = 0;

  //Declares the height and width of image
  int width = imageIn.TellWidth();
  int height = imageIn.TellHeight();
  

  imageOut.SetSize(width, height);

  //boolean 2d-array to verify if a pixel has
  // been visited or not
  bool visited[width][height];

  //inistializes the array setting all values equal
  //to false
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      visited[j][i] = false;

  //x and y high and low values of a pixel
  float xlow = 0;
  float xhigh = width;

  float ylow = 0;
  float yhigh = height;

  // Declares a value for the top of a line an
  // bottom of a line
  int lineBottom = 0;
  int lineTop = 0;

  queue Q;

  //int numHoles = -1;

  //int pixelCount = 0;

  float ratio = 0;

  vector<int>holes;
  vector<float>AR;
  vector<float>area;
  vector<int>gPixel;
  queue GQ;

  float greenPixel;

  // Nested loops to loop over every pixel in the image
  for (int j = 0; j < height; j++){// Start for1
    for (int k = 0; k < width; k++){// Start for 2
      if (visited[k][j] == true)
	continue;
      //Checks to see if the pixel is black
      if (imageIn(k, j)->Red >= 20){
	visited[k][j] = true;
	continue;
      }
      greenPixel = 0;  
      letterCount++;

      float q1 = 0;
      float q2 = 0;
      float q3 = 0;
      float q4 = 0;


      xlow = k;
      ylow = j;
      xhigh = k;
      yhigh = j;

      Q.insert(new point(k, j));

      int currentTop = ylow;
      int currentBottom = yhigh;

      visited[k][j] = true;

      //data[letterCount].area = (xhigh - xlow) * (yhigh - ylow);

      while(!Q.isEmpty()){// Start while
	//gets point value of a pixel
	greenPixel++;
	point p = Q.del();
	int x = p.getx();
	int y = p.gety();
	//cout << x << ' ' << y << endl;
	int w = ((xhigh - xlow) / 2) + xlow;
	int h = ((yhigh - ylow) / 2) + xlow;
	//cout << w << ' ' << h << endl;
	if ( x <= w && x >= xlow ){
	  if ( y <= h && y >= ylow )
	    q1++;
	  if ( y >= h && y <= yhigh )
	    q3++;
	}
	if ( x >= w && x <= xhigh ){
	  if ( y <= h && y >= ylow )
	    q2++;
	  if ( y >= h && y <= yhigh )
	    q4++;
	}


	GQ.insert(new point(x, y));

	imageOut.SetPixel(x, y, green);

	if(x < xlow)
	  xlow = x;
	if(y < ylow)
	  ylow = y;
	if (x > xhigh)
	  xhigh = x;
	if (y > yhigh)
	  yhigh = y;

	//finds the neighbors of a pixel by
	// finding the pixels around it that are black
	for (int l = y - 1; l <= y + 1; l++){// Start for3
	  for (int m = x - 1; m <= x + 1; m++){// Start for 4
	    if (visited[m][l] == false){// Start if1
	      visited [m][l] = true;
	      if (imageIn(m, l)->Red == 0)
		Q.insert(new point(m, l));
	    }// End if1
	  }// End for4
	}// End for 3
      }// End whil

      //draws bounding boxes around each letter
      for (int i = xlow - 1; i <= xhigh + 1; i++){
	imageOut.SetPixel(i, ylow - 1, red);
	imageOut.SetPixel(i, yhigh + 1, red);
      }

      for (int j = ylow - 1; j <= yhigh + 1; j++){
	imageOut.SetPixel(xlow - 1, j, red);
	imageOut.SetPixel(xhigh + 1, j, red);
      }

      //finds the number of lines in the image
      if (currentTop > lineBottom){
	lineCount++;
	lineTop = currentTop;
	lineBottom = currentBottom;
      }

      //cout << xhigh - xlow << ' ' << yhigh - ylow << endl;


      int numHoles = findPic (imageOut, xlow, ylow, xhigh - xlow, yhigh - ylow, data, letterCount);
      data[letterCount].holes = numHoles;
      data[letterCount].gPixelCount = greenPixel;

      data[letterCount].ratio = (xhigh - xlow)/(yhigh - ylow);

      data[letterCount].quad1 = q1;
      data[letterCount].quad2 = q2;
      data[letterCount].quad3 = q3;
      data[letterCount].quad4 = q4;

      data[letterCount].area = (xhigh - xlow) * (yhigh - ylow);
      cout << data[letterCount].area << endl;

      /*** for the vector *******/
      holes.push_back(numHoles);

      AR.push_back(ratio);
      gPixel.push_back(greenPixel);

      /****** Finished **********/

      //cout << data[k - k + 1].pixelCount << endl;

      //cout << data[letterCount].quad1 << ' ' << data[letterCount].quad2 << ' ' << data[letterCount].quad3 << 
      //' ' << data[letterCount].quad4 << ' ' << endl;

    }// End for2
  }// End for1

  //cout << "Letter Count: " << letterCount << endl;
  //cout << "Line Count: " << lineCount << endl;
  //cout << numHoles << endl;
  print(letterCount, data);
  // cout << greenPixel << endl;
  //percent(letterCount, area, GQ, xhigh - xlow, yhigh - ylow);
}


int findPic(BMP & I, int xleft, int yleft, int boxWidth, int boxHeight, letterData data[], int letterCount)
{
  // cout << xleft << ' ' << yleft << ' ' << boxWidth << ' ' << boxHeight << ' ' << endl;
  /*****************/
  RGBApixel blue;
  blue.Red = 0;
  blue.Green = 0;
  blue.Blue = 255;
  /*****************/
  RGBApixel other;
  other.Red = 255;
  other.Green = 167;
  other.Blue = 0;
  /*****************/
  RGBApixel other1;
  other1.Red = 75;
  other1.Green = 194;
  other1.Blue = 188;
  /*****************/
  RGBApixel other2;
  other2.Red = 223;
  other2.Green = 252;
  other2.Blue = 18;
  /****************/
  RGBApixel other3;
  other3.Red = 252;
  other3.Green = 18;
  other3.Blue = 228;
  /****************/
  RGBApixel other4;
  other4.Red = 149;
  other4.Green = 86;
  other4.Blue = 83;
  /****************/
  RGBApixel other5;
  other5.Red = 255;
  other5.Green = 0;
  other5.Blue = 255;
  /****************/



  int width = I.TellWidth();
  int height = I.TellHeight();

  bool visited[width][height];

  for (int j = 0; j < height; j++)
    for (int i = 0; i < width; i++)
      visited[i][j] = false;

  int numReg = 0;
  queue Q1;

  int regCount;

  float reg1;
  float reg2;
  float reg3;
  float reg4;
  float reg5;
  float reg6;
  float reg7;

  for (int k = yleft; k < yleft + boxHeight + 1; k++){
    for (int j = xleft; j < xleft + boxWidth + 1; j++){
      if (visited[j - xleft][k - yleft] == true)
	continue;
      if(I(j, k)->Red >= 200 && I(j, k)->Green >= 200
	 && I(j, k)->Blue >= 200){
	numReg ++;

	regCount = 0;
	
	Q1.insert(new point(j, k));
	visited[j- xleft][k- yleft] = true;

	reg1 = 0;
	reg2 = 0;
	reg3 = 0;
	reg4 = 0;
	reg5 = 0;
	reg6 = 0;
	reg7 = 0;

	while(!Q1.isEmpty()){
	  point p = Q1.del();
	  int x = p.getx();
	  int y = p.gety();

	  regCount++;

	  if(numReg == 1){
	    I.SetPixel(x, y, blue);
	    reg1++;
	  }
	  else if (numReg == 2){
	    I.SetPixel(x, y, other);
	    reg2++;
	  }
	  else if (numReg == 3){
            I.SetPixel(x, y, other1);
	    reg3++;
	  }
	  else if (numReg == 4){
            I.SetPixel(x, y, other2);
	    reg4++;
	  }
	  else if (numReg == 5){
            I.SetPixel(x, y, other3);
	    reg5++;
	  }
	  else if (numReg == 6){
            I.SetPixel(x, y, other4);
	    reg6++;
	  }
	  else if (numReg == 7){
            I.SetPixel(x, y, other5);
	    reg7++;
	  }

	  

	  for (int m = y - 1; m <= y + 1; m++){
	    for (int l = x - 1; l <= x + 1; l++){
	      if (visited[l][m] == false){
		visited[l][m] = true;
		if (I(l, m)->Blue >= 200 && I(l, m)->Red >= 200 && I(l, m)->Green >= 200)
		  Q1.insert(new point(l, m));
	      }
	    }
	  }

	}
      }
      cout << reg1 << ' ' << reg1 << endl;
      data[letterCount].region1 = reg1;
      data[letterCount].region2 = reg2;
      data[letterCount].region3 = reg3;
      data[letterCount].region4 = reg4;
      data[letterCount].region5 = reg5;
      data[letterCount].region6 = reg6;
      data[letterCount].region7 = reg7;
      //cout << regCount << endl;
      //other.Red -= 64;
      //other.Green -= 64;
      //other.Blue += 64;
    }
  } 
  return numReg;
}

void print(int letterCount, letterData data [])
{
  int holeCount = 0;
  cout << setw (10) << left << " Letter # |";
  cout << setw (14) << " Aspect Ratio |";
  cout << setw (14) << " # of regions |";
  cout << setw (28) << "      % of green pixels        |";
  cout << setw (22) << " # of pixels in region |" << endl;

  for (int i = 0; i < 97; i++)
    cout << '-';
  cout << endl;

  for (int i = 1; i < letterCount + 1; i++){
    cout << setw (10) << setprecision(2) << left << i << "|";
    cout << setw (14) << data[i].ratio<< "|";
    cout << setw (14) << data[i].holes << "|";
    cout << setw (7) << data[i].quad1 / data[i].area << ' ';
    cout << setw(7) <<  data[i].quad2 / data[i].area <<  ' '; 
    cout << setw (7)<< data[i].quad3 / data[i].area << ' ';
    cout << setw (7) << data[i].quad4 / data[i].area << "|";
    cout << setw (7) << data[i].region1 / data[i].area << "|" << endl;

  } 
  
}
 

int main (int argc, char* argv[])
{

  BMP image;
  image.ReadFromFile("input.bmp");

  int width = image.TellWidth();
  int height = image.TellHeight();

  letterData data[10000];

  BMP imageOut;
  imageOut.SetSize(width, height);

  Conversion(image, imageOut, data);
  

  imageOut.WriteToFile("output.bmp");
  return 0;
}
