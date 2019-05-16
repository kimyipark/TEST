#define TRIG 8  //TRIGGER 
#define ECHO 7 //ECHO
#define INTERVAL 10 //

unsigned int distance[INTERVAL];

unsigned int distanceMeas()
{
  int j = 0;
  int k = 0;
  int cnt = 0;
  unsigned int currenTime = 0;
  unsigned int prevTime = 0;
  unsigned int intermedi = 0;
  unsigned int meanIntermedi = 0; //중간단계 평균 값: 고스트에코 필터링을 위한 값을 저장한다.
  unsigned int meanDistance = 0; //최종 평균 값: 고스트에코 필터를 거친 값을 저장한다.
  
  for(j = 0; j < INTERVAL; j++)
  {
    digitalWrite(TRIG,LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG,HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG,LOW);
    while(digitalRead(ECHO) == LOW);
    prevTime = micros();
    while(digitalRead(ECHO) == HIGH);
    currenTime = micros();
    //ECHO핀의 출력이 LOW인 상태와 HIGH인 상태를 지속한 시간을 각각 prevTime과 currenTime에 저장한다.
    distance[j] = (currenTime - prevTime)/58.2;
    intermedi += distance[j];
    if(j == INTERVAL-1) //distance[]에 모든 값이 채워지면 수행된다.
    {
      meanIntermedi = intermedi/INTERVAL; //고스트에코를 포함한 평균 값을 저장한다.
      for(k = 0; k < INTERVAL; k++)
      {
        if(meanIntermedi < distance[k]) //평균 값 보다 더 큰 값은 산술평균에서 제외한다.
        {
          intermedi -= distance[k];
          cnt++; //산술평균에서 제외된 항 개수
        }
        if((cnt+1) == INTERVAL) //모든 항이 평균보다 크다면 거리는 0으로 고정한다.
        {
            meanDistance = 0;
        }
        if(k == INTERVAL-1) //distance[]의 모든 항에 대한 평균 값과의 비교가 완료되면 최종 값을 계산하여 'meanDistance' 변수에 저장한다. 
        {
          meanDistance = intermedi/(INTERVAL-cnt);
          cnt = 0;
        }
      }
    }
  }
  return meanDistance;
}

void setup()
{ 
  int i = 0;
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  for(i = 21; i < 26; i++)
  {
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }
  Serial.begin(9600);
}

void loop()
{
  unsigned int distance = 0;

  wiringPiSetup();
  setup();
  while(1)
  {
    distance = distanceMeas();
    if(distance >=5 && distance <= 10) 
    {
      digitalWrite(21,HIGH);
      digitalWrite(22,LOW);
      digitalWrite(23,LOW);
      digitalWrite(24,LOW);
      digitalWrite(25,LOW);
    }
    else if (distance >10 && distance <= 15)
    { 
      digitalWrite(21,HIGH);
      digitalWrite(22,HIGH);
      digitalWrite(23,LOW);
      digitalWrite(24,LOW);
      digitalWrite(25,LOW);
    }
    else if (distance > 15 && distance <= 20) 
    {
      digitalWrite(21,HIGH);
      digitalWrite(22,HIGH);
      digitalWrite(23,HIGH);
      digitalWrite(24,LOW);
      digitalWrite(25,LOW);
    }
    else if (distance > 20 && distance <= 25) 
    {
      digitalWrite(21,HIGH);
      digitalWrite(22,HIGH);
      digitalWrite(23,HIGH);
      digitalWrite(24,HIGH);
      digitalWrite(25,LOW);
    }
    delay(100);
  }
}
  