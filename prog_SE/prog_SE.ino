#include <stdin.c>

const uint8_t u8_AnalogMax = 255;
const uint8_t u8_NumMotores = 2;
const uint8_t a_SentidoMotor[u8_NumMotores] = {8, 11};  // in1
const uint8_t a_PWMMotor[u8_NumMotores] = {9, 10};      // in2
const uint8_t u8_VelStep = 8;
const uint8_t u8_Buzina_1 = 3;
const uint8_t u8_Buzina_2 = 4;
const uint8_t u8_FarolDiant = 2;
const uint8_t u8_FarolTras = 5;
const char c_Frente = 'F';
const char c_Tras = 'B';
const char c_Direita = 'R';
const char c_Esquerda = 'L';
const char c_Parada = 'X', c_ParadaAlt = 'x';
const char c_Buzina = 'V', c_BuzinaAlt = 'v';
const char c_FarolDiantLiga = 'W', c_FarolDiantDesliga = 'w';
const char c_FarolTrasLiga = 'U', c_FarolTrasDesliga = 'u';


int16_t a_Vel[u8_NumMotores] = {0, 0};

/* Aciona os farois (liga ou desliga) */

void AcionaFarol(char comando)
{
  switch (comando)
  {
    case c_FarolDiantLiga:
    digitalWrite(u8_FarolDiant,1);
    break;
    case c_FarolDiantDesliga:
    digitalWrite(u8_FarolDiant,0);
    break;
    case c_FarolTrasLiga:
    digitalWrite(u8_FarolTras,1);
    break;
    case c_FarolTrasDesliga:
    digitalWrite(u8_FarolTras,0);
    break;
  }
}

/* Aciona buzina */

void Buzina(){
  int i, j;
  
  for(j=0;j<2;j++){
    AcionaFarol(c_FarolDiantLiga);
    AcionaFarol(c_FarolTrasLiga);
    for(i=0;i<(75+75*j);i++){
      digitalWrite(u8_Buzina_1, 1);
      digitalWrite(u8_Buzina_2, 0);
      delay(2);
      digitalWrite(u8_Buzina_1, 0);
      digitalWrite(u8_Buzina_2, 1);
      delay(2);
    }
    AcionaFarol(c_FarolDiantDesliga);
    AcionaFarol(c_FarolTrasDesliga);
   delay(200);
  }
}

/* Determina os pinos de sentido e velocidade dos motores */

void SetPins()
{
  pinMode(u8_Buzina_1, OUTPUT);
  pinMode(u8_Buzina_2, OUTPUT);
  pinMode(u8_FarolDiant, OUTPUT);
  pinMode(u8_FarolTras, OUTPUT);
  for(int i=0; i<u8_NumMotores; i++)
  {
    pinMode(a_SentidoMotor[i], OUTPUT);
    pinMode(a_PWMMotor[i], OUTPUT);
  }
}

/* Estabelece os valores para os pinos de sentido e velocidade dos motores */

void SetVel(int motor, int vel)
{
  int an = vel;
  if(vel>=0)
  {
    an = vel;
    digitalWrite(a_SentidoMotor[motor], 0);
    analogWrite(a_PWMMotor[motor], an);
  }
  else if(vel<0)
  {
    an = u8_AnalogMax+vel;
    digitalWrite(a_SentidoMotor[motor], 1);
    analogWrite(a_PWMMotor[motor], an);
  }
}

/* Calcula a nova velocidade de cada motor */

void AlteraVel(int somaA, int somaB)
{
  a_Vel[0] += somaA;
  a_Vel[1] += somaB;
  for(int i=0; i<u8_NumMotores; i++)
  {
    if(a_Vel[i] > u8_AnalogMax)
    {
      a_Vel[i] = u8_AnalogMax;
    }
    else if(a_Vel[i] < -u8_AnalogMax)
    {
      a_Vel[i] = -u8_AnalogMax;
    }
    SetVel(i, a_Vel[i]);
  }
}

/* Altera a velocidade dos motores de acordo com o comando */

void Acao(char comando)
{
  switch(comando)
  {
    case c_Frente: // Para frente
    AlteraVel(+u8_VelStep, +u8_VelStep);
    break;
    case c_Tras: // Para trás
    AlteraVel(-u8_VelStep, -u8_VelStep);
    break;
    case c_Direita: // Direita
    AlteraVel(-u8_VelStep, +u8_VelStep);
    break;
    case c_Esquerda: // Esquerda
    AlteraVel(+u8_VelStep, -u8_VelStep);
    break;
    case c_Parada: // Emergência
    case c_ParadaAlt:
    AlteraVel(-a_Vel[0], -a_Vel[1]);
    break;
    case c_BuzinaAlt: // Buzina
    case c_Buzina:
    Buzina();
    break;
    case c_FarolDiantLiga:
    case c_FarolDiantDesliga:
    case c_FarolTrasLiga:
    case c_FarolTrasDesliga:
    AcionaFarol(comando);
    break;
    
  }
}

/* Recebe comando por Bluetooth e executa acao correspondente */

void ProgramaBluetooth()
{
  if(Serial.available()>0)
  {
    char comando = Serial.read();
    Acao(comando);
  }
}

void setup()
{
  SetPins();
//  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  ProgramaBluetooth();
}
