  #include<LiquidCrystal.h>
  #include<Keypad.h>
  #include<string.h>
  int i=0;
  int var;
  int arrow;
  int game_time_min=1, game_time_sec=55;
  char buff_gmin[3], buff_gsec[3];
  int game_arr[4]={0,0,0,0};
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
  // struct menu1
  // {
  //   char* name="Game Time";
  //   struct time
  //   {
  //     int minutes=1, seconds=55;
  //   }game_time;
  // }option1;
  // struct menu2
  // {
  //   char* name="Arm Time";
  //   int seconds=3;
  // }option2;
  // struct menu3
  // {
  //   char* name="Bomb Time";
  //   struct time
  //   {
  //     int minutes=0, seconds=40;
  //   }bomb_time;
  // }option3;

  void bufferize()
  {
    // Buffer to hold the formatted number (including null terminator)
    sprintf(buff_gmin, "%02d", game_time_min);
    sprintf(buff_gsec, "%02d", game_time_sec);
  }

  void display(int n)
  {
    switch (n)
    {
      case 1:
      cls();
      lcd.print("*Game time");
      lcd.setCursor(0,1);
      lcd.print(" Arm time");
      break;
      case 2:
      cls();
      if(arrow==bt_dw)
      {
      lcd.print(" Game time");
      lcd.setCursor(0,1);
      lcd.print("*Arm time");
      }
      else if(arrow==bt_up)
      {
      cls();
      lcd.print("*Arm time");
      lcd.setCursor(0,1);
      lcd.print(" Bomb time");
      }
      break;
      case 3:
      cls();
      lcd.print(" Arm time");
      lcd.setCursor(0,1);
      lcd.print("*Bomb time");
      break;
    }
  }
  void menu()
  {
    cls();
    int cnt=1;
    display(1);
    while(1)
    {
      arrow=keypad.waitForKey();
      
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
        if(cnt==1)
        {
        input:
          int cnt1=0;
          cls();
          lcd.print(buff_gmin);
          lcd.print(":");
          lcd.print(buff_gsec);
          lcd.print("    MM:SS");
          while(cnt1<=3)
          {
              
              if(cnt1<=1)
              {
                lcd.setCursor(cnt1,1);
                lcd.print("-");
              }
              else
              {
                lcd.setCursor((cnt1+1),1);
                lcd.print("-");
              }
            
              while(1){
                var=keypad.waitForKey();
                if(var=='*')
                {
                  goto panic;
                }
                else if(var=='#')
                {
                  continue;
                }
                else if(cnt1==2 && var>='6')
                {
                  continue;
                }
                else
                  {
                    break;
                  }
              }
              
              game_arr[cnt1]=var-48;
              cls();
              lcd.print(game_arr[0]);
              lcd.print(game_arr[1]);
              lcd.print(":");
              lcd.print(game_arr[2]);
              lcd.print(game_arr[3]);
              lcd.print("    MM:SS");
              cnt1++;
          }
          lcd.setCursor(0,1);
          lcd.print("Re(*)");
          lcd.print("    Cnfm(#)");

          var=keypad.waitForKey();
          if(var=='#'){

            // Parse Time
          int min = game_arr[0]*10 + game_arr[1];
          int sec = game_arr[2]*10 + game_arr[3];

          game_time_min = min;
          game_time_sec = sec;
          bufferize();
          break;
          }
          else if(var=='*'){
            goto input;
          }
          continue;
          panic:
          break;
        }

        
      }
    }
  }

  void setup() //put setup code here to run once:
  { 
    bufferize();
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