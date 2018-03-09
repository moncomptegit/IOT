#include<Wire.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Time.h>
#include <ESP8266WebServer.h>

#define PIN D3
#define LED_NUM 8
#define BRUIT 1000 // pour ne pas prendre en compte le bruit (accelerometre trop sensible)
#define counter_scale 8 //nombre de valeurs à prendre avec l accelerometre entre chaque iteration dans le jeu

ESP8266WebServer server ( 80 );

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, PIN, NEO_GRB + NEO_KHZ800);
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ;
int counter=0; //compteur
int level=0,score=0;
int dir_colors[6][3]={{0,255,0},{0,0,255},{255,255,255},{255,0,0},{255,255,0},{238,130,238}}; //links entre les couleurs et les directions 
//dir_colors[0] = {0,255,0}; //green, left
//dir_colors[1] = {0,0,255}; //blue,backward
//dir_colors[2] = {255,255,255};// white, up 
//dir_colors[3] = {255,0,0}; //red , right
//dir_colors[4] = {255,255,0}; //yellow, forward
//dir_colors[5] = {238,130,238}; //purple, dawn


int16_t last_x_value=0,last_y_value=0, last_z_value=0; //valeurs prises dans l iteration precedente (pour calculer le delta)
int next_dir=-1,game_end=1;
int dir_pl=-1;
float max_v=0.;
unsigned long begin_time, end_time;

String PageStart ="<!DOCTYPE html><html lang=\"en\"><head><title>A really bad game</title><meta charset=\"utf-8\"><style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style></head><body><h1>This is the site of a really bad game</h1><p>To start the game, click on the button start</p><form action='/' method = 'POST'><input type = \"submit\" name = \"start\" value = 'start'><div class=\"container3\"><p>stats</p></div></body></html>"; 

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  strip.begin();
  strip.show();
  randomSeed(analogRead(0));
  setup_server_wifi();

}


void loop() {
  server.handleClient();
  delay(500);
}

void initGame(float x, float y, float z){
  Serial.print(" init Game \n");
  //elapsed_time=0;
  level=0;
  last_x_value=x;
  last_y_value=y;
  last_z_value=z;
  game_end=0;
  next_dir=-1;
  dir_pl=-1;
  max_v=0.;
  strip.begin();
  for (int i = 0; i < LED_NUM; i++ ) { //eteindre les leds
  strip.setPixelColor(i, 0, 0, 0);
  }
  begin_time = now();
  }

  
void generate_next_dir(){
  Serial.print(" generate next dir ");
  long dir = random(0, 6);
  next_dir = (int)dir;
  dir_pl=-1;
  max_v=0.;
  Serial.print(next_dir);
  Serial.print("  \n");
  strip.setPixelColor(level,dir_colors[next_dir][0] , dir_colors[next_dir][1], dir_colors[next_dir][2]);
  }

int next_step(float dx, float dy, float dz){
  int dir_axis=-1;
  float max_value=0.;
  Serial.print(" next step \n");Serial.print(dx);Serial.print(" ");Serial.print(dy);Serial.print("  ");Serial.print(dz);Serial.print(" \n");
  int dir_played= -1, opposite=0; //axe et direction
  
  //obtenir l axe en fonction du max (valeur absolu)
  if(abs(dx)>=abs(dy)){
  if(abs(dx)>=abs(dz)){
    dir_axis=0;
    max_value=dx;
  }else{
    dir_axis=2;
    max_value=dz;
  }
  }else{
  if(abs(dy)>=abs(dz)){
    dir_axis=1;
    max_value=dy;
  }else{
    dir_axis=2;
    max_value=dz;
  }
  }
  
  // obtention de la direcction en fonction du signe du max
  if(abs(max_value)>BRUIT){
  if(max_value<0)
    opposite=3;
  else
    opposite=0;
    
  dir_played = dir_axis+opposite; // direction joue
  if(abs(max_value)>abs(max_v)){
    max_v=abs(max_value);
    dir_pl=dir_played;
  }
  if((counter % counter_scale)==0){
    if(dir_pl==next_dir){
    level++;
    generate_next_dir();
    return 1;
    }else
    return -1;
    }
  }else
    return 0;
  }

  
void handleRoot() {
  if(server.hasArg("start")){
  handleSubmit();
  }else{
  server.send(200, "text/html",PageStart);
  }
}


void handleSubmit(){
  initGame(0,0,0);
  generate_next_dir();
  loop_game();
  server.send(200,"text/html", createStatsPage());
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
  message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}

void setup_server_wifi()
{
  
  WiFi.begin("Adrien", "445066464");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
  delay(500);
  Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  // page web
  server.on ( "/", handleRoot );

  server.on("/inline", [](){
  server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop_game(){
  int firstIt=0;
  while(game_end!=1){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);

  if(firstIt!=0){ //deltas (dx,dy,dz) trop eleves (cause:initialisation a 0) 
    int action = next_step(AcX-last_x_value, AcY-last_y_value, AcZ-last_z_value);
    Serial.print(" action "); Serial.println(action);Serial.println("level");Serial.println(level);  Serial.print(" \n");
  
    if(action == -1){ // mauvaise direction
      score=level;
      initGame(0,0,0);
      game_end=1;
      end_time = now();
      Serial.print(" game over \n");
    }
  }else{ //initialiser avec les valeurs de la premiere prise avec l accelerometre
    last_x_value=AcX;
    last_y_value=AcY;
    last_z_value=AcZ;
    firstIt=1;
    }
    
  strip.show();
  counter++;
  delay(500);
  }
}

String createStatsPage(){
  Serial.print("end time"+end_time);
  Serial.print("begin time"+begin_time);
  int elapsed_time = (int)((end_time - begin_time)/1000);
  return String("<!DOCTYPE html><html lang=\"en\"><head><title>Stats</title><meta charset=\"utf-8\"><style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style></head><body><h1>Score: ")+String(score)+"</h1><h1>elapsed time: "+String(end_time)+" hhh"+String(begin_time)+" seconds</h1></body></html>";
}

