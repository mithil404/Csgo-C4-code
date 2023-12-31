  #include <LiquidCrystal.h>
  #include <Keypad.h>

  int i=0;
  int var;
  int arrow;
  int game_time_min=1, game_time_sec=55, arm_time_sec=3, bomb_time_min=0, bomb_time_sec=40;
  char buff_gmin[3], buff_gsec[3], buff_asec[3], buff_bmin[3], buff_bsec[3];
  int game_arr[4]={0,0,0,0};
  const int YELLOW=13; //led pins:
  const int RED=12; //led pins:
  const int GREEN=14; //led pins:
  const int BUZZER=27; //led pins:
  bool defusing;
  int nokit=10; //no defuse kit purchased time: 10 seconds
  int defkit=5; //defuse kit purchased time: 5 seconds

  byte arrow_dw[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b01110,
	0b00100
  };

  byte arrow_up[8] = {
	0b00100,
	0b01110,
	0b11111,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

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

  void bufferize()
  {
    // Buffer to hold the formatted number (including null terminator)
    sprintf(buff_gmin, "%02d", game_time_min);
    sprintf(buff_gsec, "%02d", game_time_sec);
    sprintf(buff_asec, "%02d", arm_time_sec);
    sprintf(buff_bmin, "%02d", bomb_time_min);
    sprintf(buff_bsec, "%02d", bomb_time_sec);

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
      lcd.setCursor(15,1);
      lcd.write((uint8_t)1);
      break;
      case 2:
      cls();
      if(arrow==bt_dw)
      {
      lcd.print(" Game time");
      lcd.setCursor(15,0);
      lcd.write((uint8_t)0);
      lcd.setCursor(0,1);
      lcd.print("*Arm time");
      lcd.setCursor(15,1);
      lcd.write((uint8_t)1);
      }
      else if(arrow==bt_up)
      {
      cls();
      lcd.print("*Arm time");
      lcd.setCursor(15,0);
      lcd.write((uint8_t)0);
      lcd.setCursor(0,1);
      lcd.print(" Bomb time");
      lcd.setCursor(15,1);
      lcd.write((uint8_t)1);
      }
      break;
      case 3:
      cls();
      lcd.print(" Arm time");
      lcd.setCursor(15,0);
      lcd.write((uint8_t)0);
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
        input1:
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
                  goto panic1;
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
          cls();
          lcd.setCursor(4,0);
          lcd.print("Changed!");
          delay(1000);
          break;
          }
          else if(var=='*'){
            goto input1;
          }
          continue;
          panic1:
          cls();
          lcd.setCursor(3,0);
          lcd.print("Cancelled!");
          delay(1000);
          break;
        }

        else if(cnt==2)
        {
        input2:
          int cnt1=0;
          cls();
          lcd.print(buff_asec);
          lcd.print(" Seconds");
          while(cnt1<=1)
          {
              
                lcd.setCursor(cnt1,1);
                lcd.print("-");
            
              while(1){
                var=keypad.waitForKey();
                if(var=='*')
                {
                  goto panic2;
                }
                else if(var=='#')
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
              lcd.print(" Seconds");
              cnt1++;
          }
          lcd.setCursor(0,1);
          lcd.print("Re(*)");
          lcd.print("    Cnfm(#)");

          var=keypad.waitForKey();
          if(var=='#'){

            // Parse Time
          int sec = game_arr[0]*10 + game_arr[1];

          arm_time_sec = sec;
          bufferize();
          cls();
          lcd.setCursor(4,0);
          lcd.print("Changed!");
          delay(1000);
          break;
          }
          else if(var=='*'){
            goto input2;
          }
          continue;
          panic2:
          cls();
          lcd.setCursor(3,0);
          lcd.print("Cancelled!");
          delay(1000);
          break;
        }

        if(cnt==3)
        {
        input3:
          int cnt1=0;
          cls();
          lcd.print(buff_bmin);
          lcd.print(":");
          lcd.print(buff_bsec);
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
                  goto panic3;
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

          bomb_time_min = min;
          bomb_time_sec = sec;
          bufferize();
          cls();
          lcd.setCursor(4,0);
          lcd.print("Changed!");
          delay(1000);
          break;
          }
          else if(var=='*'){
            goto input3;
          }
          continue;
          panic3:
          cls();
          lcd.setCursor(3,0);
          lcd.print("Cancelled!");
          delay(1000);
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

    lcd.createChar(0, arrow_up);
    lcd.createChar(1, arrow_dw);
  }

  void loop() // put your main code here, to run repeatedly: 
  {
    menu();
  }
