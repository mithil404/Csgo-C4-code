#include<LiquidCrystal.h>
#include<Keypad.h>
#include<string.h>
int i=0;
const int YELLOW=13; //led pins:
const int RED=12; //led pins:
const int GREEN=14; //led pins:
const int BUZZER=27; //led pins:
bool defusing;
int nokit=10; //no defuse kit purchased time: 10 seconds
int defkit=5; //defuse kit purchased time: 5 seconds

LiquidCrystal lcd(19, 21, 3, 1, 22, 23);
const byte ROWS=4;
const byte COLS=3;
char keys[ROWS][COLS]={
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS]={18,5,17,16};
byte colPins[COLS]={4,2,15};
Keypad keypad=Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char bt_up='2'; //up navigate
char bt_dw='8'; //down navigate
char bt_sel='#'; //select key
char bt_cnl='*'; //cancel key

void keypadEvent(KeypadEvent key)
{
  switch (keypad.getState())
  {
    case RELEASED:
      switch (key)
      {
        case '#': defusing=false;
        break;
      }
    break;
    case HOLD:
      switch (key)
      {
        case '#': defusing= true;
        break;
      }
    break;
  }
}
void cls()
{
  lcd.clear();
  lcd.setCursor(0,0);
}
char pass[7];
struct menu1
{
  char* name="Game Time";
  struct time
  {
    int minutes=1, seconds=55;
  }game_time;
}option1;
struct menu2
{
  char* name="Arm Time";
  int seconds=3;
}option2;
struct menu3
{
  char* name="Bomb Time";
  struct time
  {
    int minutes=0, seconds=40;
  }bomb_time;
}option3;
void display(int n)
{
  switch (n)
  {
    case 1:
    cls();
    lcd.print("Game time");
    break;
    case 2:
    cls();
    lcd.print("Arm time");
    break;
    case 3:
    cls();
    lcd.print("Bomb Time");
    break;
  }
}
void menu()
{
  lcd.setCursor(0,0);
  lcd.print("Game Time");
  int cnt=1;
  while(1)
  {
    int arrow=keypad.waitForKey();
    if(arrow==bt_up && cnt>1)
    {
      cnt--;
      display(cnt);
    }
    else if(arrow==bt_dw && cnt<3)
    {
      cnt++;
      display(cnt);
    }
    else if(arrow==bt_sel)
    {
      int cnt1=3;
      cls();
      lcd.print("01:55 mm:ss");
      int arr[4]={0,0,0,0};
      while(1)
      {
        int var=keypad.waitForKey();
        arr[cnt1]=var-48;
        cls();
        lcd.print(arr[3]);
        lcd.print(arr[2]);
        lcd.print(":");
        lcd.print(arr[1]);
        lcd.print(arr[0]);
        cnt1--;
      }
    }
  }
}


void setup() //put setup code here to run once:
{
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  keypad.setHoldTime(50);
  keypad.setDebounceTime(50);
  keypad.addEventListener(keypadEvent);
  // pinmodes
  pinMode(YELLOW, OUTPUT); //YELLOW
  pinMode(RED, OUTPUT); //RED
  pinMode(GREEN, OUTPUT); //GREEN
  pinMode(BUZZER, OUTPUT); //BUZZER
}

void loop() // put your main code here, to run repeatedly: 
{
  menu();
}
