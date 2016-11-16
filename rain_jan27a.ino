#include <Sprite.h>
#include <Matrix.h>

// cgmelvin 1/27/2015
// libraries downloaded here: http://www.pjrc.com/teensy/arduino_libraries/Matrix_Sprite.zip


const int loadPin  = 2;
const int clockPin = 3;
const int dataPin  = 4;
Matrix myMatrix = Matrix(dataPin, clockPin, loadPin);

int matsize = 8;
int raindrop_pos[] = {0,0,0,0,0,0,0,0};
long raindrop_interval[]={2000,2000,2000,2000,2000,2000,2000,2000};

/* create a new Sprite instance
   8 pixels wide, 8 pixels tall
*/
Sprite rain = Sprite(
  8, 8,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
);
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 3000;           // interval at which to blink (milliseconds)
long raindrop_start_interval =500L;
long raindrop_typical_interval = 100L;
long rd_interval = raindrop_start_interval;
long generate_raindrops();
long currentMillis;
 int loop_cnt=1;
 int angle_cnt=1;
 int rain_start_incr=2;
 
#define SINSIZE 255
#define TWOPIOVERSINSIZE    (PI*2)/(float)SINSIZE
unsigned char sins_of_our_fathers[SINSIZE];
int sine_ptr;

void setup()
{
   randomSeed(1);  //this will give the same sequence each time
  Serial.begin(9600);
  for (int i=0;i<matsize;i++)
  {
    raindrop_interval[i] = millis() + raindrop_start_interval + (random(raindrop_start_interval)*10);
  } 
  
  for(unsigned char i=0;i<SINSIZE;i++) 
  {
    sins_of_our_fathers[i] = (128.0*sin( (float)i*TWOPIOVERSINSIZE) ) + 127;
    Serial.print(".");
  }
  Serial.println("Done.");
  
  myMatrix.clear();               // clear the screen for next animation frame
 myMatrix.setBrightness(30);               // set brightness max
  
}

void loop()
{
  loop_cnt++;
  if(loop_cnt % 50 ==0) {
    sine_ptr++;
    if(sine_ptr>254) sine_ptr=0;
    raindrop_start_interval=sins_of_our_fathers[sine_ptr] *2;
    
//    raindrop_start_interval=raindrop_start_interval+(raindrop_start_interval * rain_start_incr /10);
//    if(raindrop_start_interval <5 || raindrop_start_interval >800){
//      rain_start_incr=rain_start_incr*-1;
//      if(raindrop_start_interval<5) raindrop_start_interval=5;
//      else raindrop_start_interval=800;
//    }
//     Serial.print("loop_cnt: ");
//     Serial.print(loop_cnt);
//     Serial.print(" sine_ptr: ");
//     Serial.print(sine_ptr);
//     Serial.print(" sins_of_our_fathers[sine_ptr]: ");
//    Serial.println(sins_of_our_fathers[sine_ptr]);

  }    
  currentMillis=millis();
 
for (int column=0;column<matsize;column++)
  {
    /* go through each column checking on our raindrop for that column,
    if it is time for a change */
 //   Serial.print(", row: ");
//    Serial.println(row);
    if(currentMillis>raindrop_interval[column]){
 //       Serial.print("column: ");
 //    Serial.print(column);
 //     Serial.print("currentMillis: ");
 //     Serial.println(currentMillis);
      raindrop_interval[column] = currentMillis +generate_raindrops(column);
 //      Serial.print("column: ");
 //    Serial.print(column);
 //     Serial.print("raindrop interval: ");
 //     Serial.println(raindrop_interval[column]);
    }
  } //end column loop
  myMatrix.write(0, 0, rain);     // place sprite on screen/
}
long generate_raindrops(int colnum){
  long rd_interval=raindrop_start_interval;
  int raindrop_pos=-1;
  int nrows=rain.width();
  /* where is our raindrop? */
   for (int thisrow=0;thisrow<nrows;thisrow++){
 //    Serial.print(rain.read(thisrow,colnum));
 //    Serial.print(".");
      if(rain.read(thisrow,colnum)==1) raindrop_pos=thisrow;
   }//end read rows loop
 //  Serial.print(" raindrop_pos before: ");
 //  Serial.print(raindrop_pos);
   if(raindrop_pos==-1){ //no raindrop, start at the top
     raindrop_pos=7;
   }else {
 /* remove old & increment */
         rain.write(raindrop_pos,colnum,0);
         raindrop_pos--;
   }
   if(raindrop_pos==-1){
        rd_interval=raindrop_start_interval+(random(raindrop_start_interval)*10);
   }else{
       rain.write(raindrop_pos,colnum,1);
       rd_interval=raindrop_typical_interval;
//       rd_interval=(raindrop_typical_interval/2L)+ random(raindrop_typical_interval) ;
   }

 //   Serial.print(" raindrop_pos after: ");
 //  Serial.print(raindrop_pos);
 //  Serial.print(" new rows :");
//for (int thisrow=0;thisrow<nrows;thisrow++){
  //   Serial.print(rain.read(thisrow,colnum));
  //   Serial.print(".");
//  }
// Serial.print("col: ");
// Serial.println(colnum); 
    return(rd_interval);
}
