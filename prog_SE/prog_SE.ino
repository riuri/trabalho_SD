#define NUM_MOTORES 2
#define ANALOG_MAX 255
int in1[NUM_MOTORES] = {8, 11};
int in2[NUM_MOTORES] = {9, 10};
int vel[NUM_MOTORES] = {0, 0};
int led = 13;
int vstep = 32;

void setMotorPins()
{
  for(int i=0; i<NUM_MOTORES; i++) {
    pinMode(in1[i], OUTPUT);
    pinMode(in2[i], OUTPUT);
  }
}

void setVel(int motor, int vel)
{
  int an = vel;
  if(vel>=0) {
    an = vel;
    digitalWrite(in1[motor], 0);
    analogWrite(in2[motor], an);
  }
  else if(vel<0) {
    an = ANALOG_MAX+vel;
    digitalWrite(in1[motor], 1);
    analogWrite(in2[motor], an);
  }
}

void alteraVel(int somaA, int somaB)
{
  vel[0] += somaA;
  vel[1] += somaB;
  for(int i=0; i<NUM_MOTORES; i++) {
    if(vel[i] > ANALOG_MAX) vel[i] = ANALOG_MAX;
    else if(vel[i] < -ANALOG_MAX) vel[i] = -ANALOG_MAX;
    setVel(i, vel[i]);
  }
}

void acao(char c)
{
  switch(c)
  {
    case 'F': // Para frente
      alteraVel(+vstep, +vstep);
      break;
    case 'B': // Para trás
      alteraVel(-vstep, -vstep);
      break;
    case 'R': // Direita
      alteraVel(-vstep, +vstep);
      break;
    case 'L': // Esquerda
      alteraVel(+vstep, -vstep);
      break;
    case 'V': // Emergência
    case 'v':
      alteraVel(-vel[0], -vel[1]);
      break;
  }
}

void programaBluetooth()
{
  if(Serial.available()>0) {
    char c = Serial.read();
    acao(c);
  }
}

void setup()
{
  setMotorPins();
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  programaBluetooth();
}
