#include <pitches.h> //스피커 라이브러리 
#include <LiquidCrystal.h> //LCD 라이브러리 
//타임라이브러리 
#include<Time.h>
#include<TimeAlarms.h>
unsigned long time_prev,time_curr; //예전시간, 현재시간 
boolean t_f; //데이터 입력 종료(타이머) 
boolean t_f1; //데이터 입력 종료(버퍼) 
String buffer=""; //수신 데이터 버퍼
int state=1; //초기화면 설정을 위함 (4segment) 
int hourt=1; //타이머시
int mint=1; //타이머분
int sect=1; //타이머초 
int RGB_LED[]={5,6,7}; //LED 연결핀 
int yearc,monthc,dayc; //날짜,월,일 
int hourc,minc,sec1; //시계의 시간,분,초 
unsigned long timer; //타이머를 위한 초단위 시간 
int pin_LED[] = {13,12,11,10}; //LED 연결 핀  
int noteDuration = 4; //4분 음표
int speakerPin = 57; //스피커핀 
int melody[] = { //학교종 초반 멜로디, 0은 쉼표
  NOTE_G4,NOTE_G4,NOTE_A4,NOTE_A4,NOTE_G4,NOTE_G4,NOTE_E4,0,
  NOTE_G4,NOTE_G4,NOTE_E4,NOTE_E4,NOTE_D4,0,  
};
int buttons[] = {14,15,16}; //버튼으로 모드 설정 
boolean state_previous[] = {false,false,false}; //버튼들의 이전상태 
boolean state_current[3]; //현재 버튼 상태  
boolean clockbool=false; //시계실행여부 
boolean memobool=false; //메모실행여부 
boolean timerbool=false; //타이머실행여부 
char modeSel; // 모드 설정 (타이머,시계...) 

//0에서 9까지 숫자 표현을 위한
//세그먼트 a,b,c,d,e,f,g,dp의 패턴 
 byte patterns[] {
  0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE4,0xFE,0xE6
 };  
int digit_select_pin[] = {66,67,68,69}; //자릿수 선택 핀
int segment_pin[]={58,59,60,61,62,63,64,65}; //7세그먼트 모듈 연결핀(a~dp오름차순) 
int SEGMENT_DELAY=5; //숫자 표시 사이의 시간 간격 
// 핀 번호 (RS, E, DB4, DB5, DB6, DB7)
LiquidCrystal lcd(44, 45, 46, 47, 48, 49); // LCD 연결

void(* resetFunc) (void) = 0; //declare reset function @ address 0 

void setup() {
  Serial.begin(9600); //시리얼 통신 초기화 
  Serial1.begin(9600); //블루투스 시리얼 통신 초기화 
  for(int i=0; i<4; i++) { //자릿 수 선택 핀을 출력으로 설정 
    pinMode(digit_select_pin[i],OUTPUT); 
  }

  for(int i=0; i<8; i++) { //세그먼트 제어 핀을 출력으로 설정 
   pinMode(segment_pin[i],OUTPUT);
 }

  lcd.begin(16,2); //LCD 초기화 
  lcd.print("I'M TIME HELPER"); //문자열 출력 
  for(int i=0; i<3; i++) {
    pinMode(buttons[i],INPUT); //버튼 연결 핀들을 입력으로 설정 
  }
  for(int i=0; i<4; i++) {
    pinMode(pin_LED[i],OUTPUT);  //LED 연결 핀을 출력으로 설정 
    digitalWrite(pin_LED[i],LOW); //꺼진 상태로 시작 
  }

  for(int i=0; i<3; i++) {
    pinMode(RGB_LED[i],OUTPUT); //LED를 출력으로 설정 
  }
  

  setTime(14,1,0,4,2,22); //22년 2월 4일, 14시 1분 0초로 시간 설정 
  time_prev = millis(); //시작 시간 
}

//해당 자리에 숫자 하나를 표현하는 함수
void show_digit(int pos,int number) { //(위치, 출력할 숫자)
  for(int i=0; i<4; i++) {
    if(i+1==pos) 
      digitalWrite(digit_select_pin[i],LOW); //해당 자릿수 선택 핀만 LOW로 설정 
      
    else
      digitalWrite(digit_select_pin[i],HIGH); //나머지는 HIGH로 설정 
    }

    for(int i=0; i<8; i++) {
      boolean on_off = bitRead(patterns[number],7-i); //패턴 읽어오기 
      digitalWrite(segment_pin[i],on_off); //세그먼트 제어핀을 패턴대로 설정 
      }
  }


//4자리 세그먼트 표시 장치에 4자리 숫자를 표시하는 함수 
void show_4_digit(int number) {
  number = number %10000;  //4자리로 제한 
  int thousands = number/ 1000; //천자리 
  number = number %1000;
  int hundreads = number/100; //백자리
  number = number % 100;
  int tens = number /10; //십자리 
  int ones = number %10; //일자리 
  show_digit(1,thousands); //천의자리 표시
  delay(SEGMENT_DELAY); //delay 5밀리초 
  show_digit(2,hundreads); //백자리 
  delay(SEGMENT_DELAY); //delay 5밀리초 
  show_digit(3,tens); //십자리 
  delay(SEGMENT_DELAY); //delay 
  show_digit(4,ones); //일자리 
  delay(SEGMENT_DELAY); //delay 
  }

//타이머표시 함수 
void digitalClockDisplayTimer() {

 
  lcd.begin(16, 2); //LCD 초기화 
  lcd.print(hourt); //시  
  printDigits(mint); //분 
  printDigits(sect); //초 
}

//시계표시 함수
void digitalClockDisplay() 
{
  lcd.begin(16, 2); //LCD 초기화 
  lcd.print(year()); //년 
  lcd.print(".");
  lcd.print(month()); //월 
  lcd.print(".");
  lcd.print(day()); //일 
  lcd.setCursor(0, 1); //다음 줄로 
  lcd.print(hour()); //시간 
  printDigits(minute()); //분 
  printDigits(second()); //초 
  //delay(100); //alarm 라이브러리의 경우 delay 가 필요함 
}


//시간 자릿수 함수 
void printDigits(int digits) {

  lcd.print(":");  
  if(digits < 10) //10보다 작으면 
    lcd.print('0'); //0붙여주기 
  lcd.print(digits);
}


void timer1() {
  state=0;
  time_curr = millis(); //현재시간 
  timerbool=true; //타이머실행여부 
  if(clockbool==true) { //시계가 실행되었으면 
    clockbool=false; //시계실행여부 false 설정후 
    reset1(); //리셋 
  }
  if(memobool==true) { //메모가 실행되었으면 
    memobool=false; //메모실행여부 false설정후 
    reset1(); //리셋 
  }
  
  while(Serial1.available()) { //개행문자 '\n' 만날때까지 
    char data = Serial1.read(); //읽기 
    if(data=='\n') { 
      t_f = true; //문자열 종료 
      break;
    }
    else {
      buffer+=data; //버퍼에 연결 
      int index1 = buffer.indexOf(':'); //:를 찾아서 인덱스 값 반환 
      int index2 = buffer.indexOf(':',index1+1); //:를 찾기, 이는 index1의 1만큼 뒤부터 찾는다 
      int index3 = buffer.length(); //버퍼 끝의 인덱스 

      hourt = buffer.substring(0,index1).toInt(); //0부터 index1까지 자른 후 정수 변환 뒤 시간 저장 
      mint = buffer.substring(index1+1,index2).toInt(); //index1의 1뒤부터 index2까지 자른 후 정수 변환 후 분 저장 
      sect = buffer.substring(index2+1,index3).toInt(); //index2의 1뒤부터 index3까지 자른 후 정수 변환 후 분 저장 

      timer = ((long)hourt*3600+mint*60+sect); //초로 변환한다 1시간 -> 3600초 1분 -> 60초 
    }
    
  }

  if(t_f) { //문자열 끝나면 
    //1초 이상 시간 경과 경우 
    if(time_curr - time_prev >= 1000) {
      time_prev = time_curr; //시작 시간 갱신 
      Serial1.println(timer); //시리얼로 초 확인 
      timer--; //초에서 -1씩 
      int hour1,sec1,min1; //나머지 저장 함수(분,초계산 위함)
      hourt = (timer/3600); //시 구하기 
      hour1 = timer%3600; //나머지구하기 
      mint = hour1/60; //분 구하기 
      min1 = hour1%60; //나머지 구하기 
      sect = min1%60; //초 구하기 
      Serial1.println(String(hourt)+":"+String(mint)+":"+String(sect)); //시리얼로 확인 
      digitalClockDisplayTimer(); //LCD 출력 
    }
    
  }
  show_4_digit(timer); //4자리 7segment 출력

  if(hourt==0&&mint==0&&sect==0) { 
    show_4_digit(timer);
    //모두 0으로 돌아오면 
    //시,분,초 다 0으로 초기화 
    for(int i=0; i<4; i++) {
      digitalWrite(pin_LED[i],HIGH); //led점등
    }
    for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(int); thisNote++) { //부저 울림
     //음표 길이를 시간으로 변환
     int noteLength = 1000 / noteDuration;
     //단음 재생
     tone(speakerPin, melody[thisNote], noteLength);
     delay(noteLength);
     noTone(speakerPin); //현재 음 재생 중지 
    }
    modeSel = '0'; //reset1 함수로 가도록 

  }
}



void Clock1(){ //시계함수 
  state=0;
  int year1 = year(); //년도 불러오기 
  time_curr = millis(); //현재 시간 
  clockbool = true; //시계 실행여부 true 
   //1초 경과 하였으면 
   if(time_curr - time_prev >=1000 ) { 
     time_prev = time_curr; //이전 시간 change 
     digitalClockDisplay(); //시계표시 lcd 함수 
 }
 show_4_digit(year1); //4자리 segment에 년도 표시 
}

void memo() {  //메모함수 
  state=0;
  memobool=true; //메모 실행여부 
  time_curr = millis(); //현재시간 
  if(clockbool==true) { //시계 실행 여부 검사 
    clockbool=false; //실행 여부 false로 초기화 
    reset1(); //시계가 실행되었으면 리셋 실행 
  } 
  if(timerbool==true) { //시계 실행 여부 검사 
   timerbool=false; //타이머 실행 여부 검사 
   reset1(); //타이머가 실행되었으면 리셋 실행 
  }
  while(Serial1.available()) { //개행문자 '\n' 만날때까지 
    char data = Serial1.read(); //읽기 
    if(data=='\n') { 
      t_f = true; //문자열 종료 
      break;
    }
    else {
      buffer+=data; //버퍼에 연결 
    }
  }

  if(t_f) { //메모 문자열 끝나면 
    if(time_curr-time_prev>=1000){  //1초가 지난경우 
      time_prev=time_curr; //
     lcd.begin(16, 2); //LCD 초기화 
     lcd.print(buffer);  //lcd에 버퍼를 프린트한다(메모출력) 
  }
 }
 show_4_digit(0); //4segment에 0으로만 표시 
}

void reset1() {
   digitalWrite(pin_LED,LOW); //led 점등off 
   if(time_curr - time_prev >= 1000) {
     time_prev = time_curr; //시작 시간 갱신 
     t_f = false; //문자열 flag false로(garbage값 issue) 
     Serial1.println(String(hourt)+":"+String(mint)+":"+String(sect)); //시리얼로 확인 
     digitalClockDisplayTimer(); //LCD 출력 
 }
 show_4_digit(timer); //4자리 7segment 출력
 resetFunc(); //초기화함수(처음으로 돌아감) 
 state=1; //초기상태 변수 
}


void loop(){
  int light = analogRead(A0); //조도센서 측정값 아날로그로 
  time_curr = millis(); //현재 시간 측정 
  Serial.print(light);
  if(state==1) {//초기상태 (전역변수 state=1로 설정해주었음으로)
   show_4_digit(0); //state가 초기값 1이면 0으로 4자리 표시 
  }
  if(light>=400) { //빛의 값이 400이상이면 
   digitalWrite(RGB_LED[1],HIGH); //green off 
   digitalWrite(RGB_LED[2],HIGH); //red off 
   digitalWrite(RGB_LED[0],LOW);  //blue 색상 on 
   }
  else{
   digitalWrite(RGB_LED[1],HIGH); //green off 
   digitalWrite(RGB_LED[2],HIGH); //red off 
   digitalWrite(RGB_LED[0],HIGH); //blue off 
  }
  for(int i=0; i<3; i++) { //버튼의 상태 읽기 
   state_current[i] = digitalRead(buttons[i]);
  }
  //0번째 버튼을 누른경우 이전과 비교하여 상태가 바뀐 경우 아래 코드 실행 
  if((state_current[0] == 1) && (state_previous[0] == 0 )) {
    state_previous[0]=1; //이전 경우 change 
    state=0; //초기변수 
    modeSel = '1'; //시계 설정 변수
  }
  //0번째 버튼의 현재상태가 0인데 이전상태가 1일 때를 뜻함 
   if((state_current[0] == 0 ) && (state_previous[0] == 1 )) { 
    state_previous[0]=0; //이전상태를 다시 0으로 
  }

  //1번째 버튼을 누른경우 이전과 비교하여 상태가 바뀐 경우 아래 코드 실행
  if((state_current[1] == 1) && (state_previous[1] == 0 )) { //1번째 버튼을 누르면 
    state_previous[1]=1; //이전 경우 change 
    state=0; //초기변수 
    modeSel = '2';  //타이머 설정 변수
  }
  //1번째 버튼의 현재상태가 0인데 이전상태가 1일 때를 뜻함
  if((state_current[1] == 0 ) && (state_previous[1] == 1 )) {
    state_previous[1]=0;
  }

  //2번째 버튼을 누른경우 이전과 비교하여 상태가 바뀐 경우 아래 코드 실행
  if((state_current[2] == 1) && (state_previous[2] == 0 )) { //2번째 버튼을 누르면 
    state_previous[2]=1; //이전 경우 change 
    state=0; //초기변수 
    modeSel = '3';  //메모 설정 변수 
  }
  //2번째 버튼의 현재상태가 0인데 이전상태가 1일 때를 뜻함
  if((state_current[2] == 0 ) && (state_previous[2] == 1 )) {
    state_previous[2]=0;
  }

  
  if(modeSel=='0') { //리셋함수로 유도 
  
    reset1();
  }

  if(modeSel == '1' ) { //시계 함수로 유도 
    Clock1(); 
  }
  
  if(modeSel=='2') { //타이머 함수로 유도 
    timer1();
  }

 if(modeSel=='3') { //메모 함수로 유도 
    memo();
  }

 }

  
  

 

  
