/* Code based on perlin function in this source
 kasperkamperman.com 16-09-2012
 
 written as a birthday gift for Stephanie Brunner 9/28/2013
 by Jakob Marsico
 */




unsigned long currentTime;     // for time measuring purposes
unsigned long passedTime;     // for time measuring purposes
unsigned long longestTime = 0; // for time measuring purposes

float perlinTimeInc      = 0.01;
float perlinXInc         = 0.1;  
float perlinTimePosition;
int mappedVal= 0;
int minVal = 30;
int PWMpins[] = {4,5,9,10};     //array to hold pin locations (these are the PWM pins uses on teensey)



void setup(){   
  Serial.begin(9600);
  
  pinMode(10, OUTPUT);
}

void loop(){   
  currentTime = micros(); // store current time
  
  for(byte i=0;i< 5;i++)
  { 
      float x = float(i)*perlinXInc;               // input for x value in the renderNoise function
      byte val = renderNoise(x, perlinTimePosition+i*i);   //render the noise... i*i to get noise further in time position
      mappedVal=map(val,0,255, minVal, 255);   //map the value so it doesn't hit zero
      analogWrite(PWMpins[i],mappedVal);       //light the LED
  }

 
  perlinTimePosition = perlinTimePosition + perlinTimeInc;   // go a step further in time (input for y function in perlin noise)
  passedTime = micros()-currentTime;          // calculate the time the whole calculation took
  if(passedTime>longestTime) longestTime = passedTime;            // because times will variate, remember the maximum time it took
  delay(20);          //give processor a break
}


//***************** noise functions ***************************

// returns a value between 0 - 255 for lights
byte renderNoise(float x, float y)
{	
  float noise;

  // 2 octaves
  //noise = perlin_function(x,y) + perlin_function(x*2,y*2);

  noise = perlin_function(x,y); // gives noise in the range of -1 to +1
  noise = noise *128+127;       // scale to a number between 0 - 255

  return (byte) noise;  
}


float perlin_function(float x, float y)
{
  int fx = floor(x);
  int fy = floor(y);

  float s,t,u,v;
  s=perlin_noise_2d(fx,fy);
  t=perlin_noise_2d(fx+1,fy);
  u=perlin_noise_2d(fx,fy+1);
  v=perlin_noise_2d(fx+1,fy+1);

  float inter1 = lerp(s,t,x-fx);
  float inter2 = lerp(u,v,x-fx);  

  return lerp(inter1,inter2,y-fy);
}

float perlin_noise_2d(int x, int y) {
  long n=(long)x+(long)y*57;
  n = (n<<13)^ n;
  return 1.0 - (((n *(n * n * 15731L + 789221L) + 1376312589L)  & 0x7fffffff) / 1073741824.0);    
}

float lerp(float a, float b, float x)
{ 
  return a + x * (b - a);
}





