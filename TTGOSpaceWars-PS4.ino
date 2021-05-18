#define USE_PS4_CONTROLLER

#ifdef USE_PS4_CONTROLLER
#include "PS4Controller.h"
constexpr int8_t kPs4StickThreshold = 30;
#endif  // USE_PS4_CONTROLLER

#ifdef USE_BUZZER
#include <Tone32.h>
#define BUZZER_PIN 27
#define BUZZER_CHANNEL 0
#endif  // USE_BUZZER

#include <SPI.h>
#include <TFT_eSPI.h>  // Hardware-specific library
#include "back2.h"
#include "brod1.h"
#include "bulet.h"
#include "buum.h"
#include "earth.h"
#include "ebullet.h"
#include "ex.h"
#include "ex2.h"
#include "gameOver.h"
#include "life.h"
#include "ricon.h"
#include "rocket.h"
#include "rover.h"
#include "sens.h"

#define TFT_GREY 0x5AEB
#define lightblue 0x2D18
#define orange 0xFB60
#define purple 0xFB9B

TFT_eSPI tft = TFT_eSPI();
int brojac = 0;  // Invoke custom library
float buletX[10] = {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20};
float buletY[10] = {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20};

float EbuletX[10] = {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20};
float EbuletY[10] = {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20};

float rocketX[10] = {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20};
float rocketY[10] = {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20};
float rocketSpeed = 0.22;
int rockets = 3;

int counter = 0;
int rcounter = 0;
int Ecounter = 0;
int level = 1;
float x = 10;
float y = 20;

float ey = 18;
float ex = 170;

float es = 0.1;

float bx = -50;
float by = 0;

int pom = 0;   // pressdebounce for fire
int pom2 = 0;  // pressdebounce for rockets
float speed = 0.42;
int blinkTime = 0;
int eHealth = 50;
int mHealth = eHealth;
int lives = 4;
int ly[4] = {0, 0, 0, 0};
int ri[3] = {0, 0, 0};
int fireTime = 100;
int fireCount = 0;
float EbulletSpeed = 0.42;
int rDamage = 8;  // rocket damage
int tr = 0;

int pom3 = 0;
bool sound = 1;  // sound on or off

float spaceX[30];
float spaceY[30];

enum GameState { kIntro = 0, kGamePlay, kGameOver };

GameState game_state{kIntro};

//------------------------------------------------------------------------------
void init_ps4() {
#ifdef USE_PS4_CONTROLLER
  PS4.begin("03:03:03:03:03:03");
#endif  // USE_PS4_CONTROLLER
}  // init_ps4()
//------------------------------------------------------------------------------
bool joystick_left() {
  return (digitalRead(2) == 0)
#ifdef USE_PS4_CONTROLLER
         || (PS4.isConnected() &&
             (PS4.Left() || (PS4.LStickX() < -kPs4StickThreshold)))
#endif  // USE_PS4_CONTROLLER
      ;
}  // joystick_left()
//------------------------------------------------------------------------------
bool joystick_right() {
  return (digitalRead(17) == 0)
#ifdef USE_PS4_CONTROLLER
         || (PS4.isConnected() &&
             (PS4.Right() || (PS4.LStickX() > kPs4StickThreshold)))
#endif  // USE_PS4_CONTROLLER
      ;
}  // joystick_right()
//------------------------------------------------------------------------------
bool joystick_up() {
  return (digitalRead(22) == 0)
#ifdef USE_PS4_CONTROLLER
         || (PS4.isConnected() &&
             (PS4.Up() || (PS4.LStickY() > kPs4StickThreshold)))
#endif  // USE_PS4_CONTROLLER
      ;
}  // joystick_up()
//------------------------------------------------------------------------------
bool joystick_down() {
  return (digitalRead(21) == 0)
#ifdef USE_PS4_CONTROLLER
         || (PS4.isConnected() &&
             (PS4.Down() || (PS4.LStickY() < -kPs4StickThreshold)))
#endif  // USE_PS4_CONTROLLER
      ;
}  // joystick_down()
//------------------------------------------------------------------------------
bool joystick_button_a() {
  return (digitalRead(13) == 0 || digitalRead(0) == 0)
#ifdef USE_PS4_CONTROLLER
         || (PS4.isConnected() && PS4.Cross())
#endif  // USE_PS4_CONTROLLER
      ;
}  // joystick_button_a()
//------------------------------------------------------------------------------
bool joystick_button_b() {
  return (digitalRead(12) == 0)
#ifdef USE_PS4_CONTROLLER
         || (PS4.isConnected() && PS4.Circle())
#endif  // USE_PS4_CONTROLLER
      ;
}  // joystick_button_c()
//------------------------------------------------------------------------------
bool joystick_button_c() {
  return (digitalRead(35) == 0)
#ifdef USE_PS4_CONTROLLER
         || (PS4.isConnected() && PS4.Square())
#endif  // USE_PS4_CONTROLLER
      ;
}  // joystick_button_c()
//------------------------------------------------------------------------------
void wait_for_joystick_button_a_down() {
  while (!joystick_button_a()) {
    delay(5);
  }
}  // wait_for_joystick_button_a_down()
//------------------------------------------------------------------------------
void wait_for_joystick_button_a_up() {
  while (joystick_button_a()) {
    delay(5);
  }
}  // wait_for_joystick_button_a_up()
//------------------------------------------------------------------------------
void move_player(const float& dx, const float& dy) {
  const float new_x = x + dx;
  const float new_y = y + dy;
  if (new_x >= 0.0F && new_x <= 125.0F && new_y >= 18.0F && new_y <= 94.0F) {
    x = new_x;
    y = new_y;
  }
}  // move_player()
//------------------------------------------------------------------------------
void setup(void) {
  init_ps4();
  pinMode(21, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);  // fire2 B
  pinMode(13, INPUT_PULLUP);  // fire1 A
  pinMode(25, OUTPUT);        // led2
  pinMode(33, OUTPUT);        // led1
  pinMode(26, OUTPUT);        // led3
  pinMode(15, INPUT_PULLUP);  // stisak
  pinMode(13, INPUT_PULLUP);  // buttonB
  pinMode(0, INPUT_PULLUP);   // LORA built in buttons
  pinMode(35, INPUT_PULLUP);
  digitalWrite(26, 1);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, 240, 135, back2);

  for (int i = 0; i < 30; i++) {
    spaceX[i] = random(5, 235);
    spaceY[i] = random(18, 132);
  }

  wait_for_joystick_button_a_down();
  digitalWrite(26, 0);
}  // setup()
//------------------------------------------------------------------------------
void restart() {
  counter = 0;
  rcounter = 0;
  Ecounter = 0;
  level = 1;
  x = 10;
  y = 20;
  ey = 18;
  ex = 170;
  es = 0.1;
  bx = -50;
  by = 0;

  rockets = 3;
  rDamage = 8;
  lives = 4;
  brojac = 0;
  ri[0] = 0;
  ri[1] = 0;
  ri[2] = 0;
  ly[0] = 0;
  ly[1] = 0;
  ly[2] = 0;
  ly[3] = 0;
  ey = 44;
  speed = 0.42;
  eHealth = 50;
  mHealth = eHealth;
  EbulletSpeed = 0.42;
  rocketSpeed = 0.22;

  for (int i = 0; i < 10; i++) {
    buletX[i] = -20;
    EbuletX[i] = -20;
    rocketX[i] = -20;
  }
}  // restart()
//------------------------------------------------------------------------------
void new_level() {
  level++;
  speed = speed + 0.05;
  EbulletSpeed = EbulletSpeed + 0.1;
  eHealth = 50 + (level * 5);
  mHealth = eHealth;
  es = 0.05 + (0.035 * level);

  rockets = 3;
  rDamage = 8 + (level * 2);
  rocketSpeed = 0.22 + (level * 0.02);
  ri[0] = 0;
  ri[1] = 0;
  ri[2] = 0;

  ey = 44;

  for (int i = 0; i < 10; i++) {
    buletX[i] = -20;
    EbuletX[i] = -20;
    rocketX[i] = -20;
  }

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0, 4);
  tft.print("Level " + String(level));
  tft.setCursor(0, 22, 2);

  tft.println("Enemy speed : " + String(es));
  tft.println("Enemy health : " + String(eHealth));
  tft.println("Enemy bullet speed : " + String(EbulletSpeed));
  tft.println("Remaining lives: " + String(lives));
  tft.println("My speed : " + String(speed));
  tft.println("Rocket damage : " + String(rDamage));
  tft.println("Rocket speed : " + String(rocketSpeed));

  tft.pushImage(170, 5, 55, 54, earth[level - 1]);
  tft.pushImage(170, 61, 72, 72, sens);
  delay(2600);

  wait_for_joystick_button_a_down();
  tft.fillScreen(TFT_BLACK);

  tft.drawLine(0, 16, 240, 16, lightblue);
  tft.drawLine(0, 134, 240, 134, lightblue);

  tft.setCursor(200, 0, 2);
  tft.print(brojac);

  tft.fillRect(120, 3, 70, 7, TFT_GREEN);
  tft.drawRect(119, 2, 72, 9, TFT_GREY);
}  // new_level()
//------------------------------------------------------------------------------
void show_intro() {
  restart();
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, 240, 135, back2);
  wait_for_joystick_button_a_down();
  tft.fillScreen(TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0, 4);
  tft.print("Level " + String(level));
  tft.setCursor(0, 22, 2);

  tft.println("Enemy speed : " + String(es));
  tft.println("Enemy health : " + String(eHealth));
  tft.println("Enemy bullet speed : " + String(EbulletSpeed));
  tft.println("Remaining lives: " + String(lives));
  tft.println("My speed : " + String(speed));
  tft.println("Rocket damage : " + String(rDamage));
  tft.println("Rocket speed : " + String(rocketSpeed));

  tft.pushImage(170, 5, 55, 54, earth[level - 1]);
  tft.pushImage(170, 61, 72, 72, sens);
  delay(1000);

  wait_for_joystick_button_a_down();
  tft.fillScreen(TFT_BLACK);

  tft.drawLine(0, 16, 240, 16, lightblue);
  tft.drawLine(0, 134, 240, 134, lightblue);

  tft.setCursor(200, 0, 2);
  tft.print(brojac);

  tft.fillRect(120, 3, 70, 7, TFT_GREEN);
  tft.drawRect(119, 2, 72, 9, TFT_GREY);
}  // show_intro()
//------------------------------------------------------------------------------
void play_game() {
  if (joystick_down()) move_player(0.0F, speed);
  if (joystick_up()) move_player(0.0F, -speed);
  if (joystick_left()) move_player(-speed, 0.0F);
  if (joystick_right()) move_player(speed, 0.0F);
  if (joystick_button_a()) {
    if (pom == 0) {
      pom = 1;
      buletX[counter] = x + 34;
      buletY[counter] = y + 15;
      counter = counter + 1;
    }
  } else
    pom = 0;
  if (joystick_button_b() && rockets > 0) {
    // Rocket button B button
    if (pom2 == 0) {
      pom2 = 1;
      rockets--;
      rocketX[rcounter] = x + 34;
      rocketY[rcounter] = y + 14;
      rcounter = rcounter + 1;
      ri[rockets] = -100;
      tft.fillRect(70 + (rockets * 14), 0, 8, 14, TFT_BLACK);
    }
  } else
    pom2 = 0;
  if (joystick_button_c()) {  // on and off sound
    if (pom3 == 0) {
      pom3 = 1;
      sound = !sound;
    }
  } else
    pom3 = 0;

  for (int i = 0; i < 30; i++) {
    // drawStars..........................................
    tft.drawPixel(spaceX[i], spaceY[i], TFT_BLACK);
    spaceX[i] = spaceX[i] - 0.5;
    tft.drawPixel(spaceX[i], spaceY[i], TFT_GREY);
    if (spaceX[i] < 0) {
      tft.drawPixel(spaceX[i], spaceY[i], TFT_BLACK);
      spaceX[i] = 244;
    }
  }

  tft.pushImage(x, y, 49, 40, brod1);
  tft.pushImage(ex, ey, 55, 54, earth[level - 1]);

  for (int i = 0; i < 10; i++) {  // firing buletts
    if (buletX[i] > 0) {
      tft.pushImage(buletX[i], buletY[i], 8, 8, bulet);
      buletX[i] = buletX[i] + 0.6;
    }
    if (buletX[i] > 240) buletX[i] = -30;
  }

  for (int i = 0; i < 10; i++) {  // firing rockets
    if (rocketX[i] > 0) {
      tft.pushImage(rocketX[i], rocketY[i], 24, 12, rocket);
      rocketX[i] = rocketX[i] + rocketSpeed;
    }
    if (rocketX[i] > 240) rocketX[i] = -30;
  }

  // delay(1);

  for (int j = 0; j < 10; j++)  // did my bulet hit enemy
  {
    if (buletX[j] > ex + 20 && buletY[j] > ey + 2 && buletY[j] < ey + 52) {
      tft.pushImage(buletX[j], buletY[j], 12, 12, ex2);
#ifdef USE_BUZZER
      if (sound == 1) {
        tone(BUZZER_PIN, NOTE_C5, 12, BUZZER_CHANNEL);
        noTone(BUZZER_PIN, BUZZER_CHANNEL);
      } else {
        delay(12);
      }
#else   // USE_BUZZER
      delay(12);
#endif  // USE_BUZZER

      tft.fillRect(buletX[j], buletY[j], 12, 12, TFT_BLACK);
      buletX[j] = -50;
      brojac = brojac + 1;
      tft.setCursor(200, 0, 2);
      tft.print(brojac);
      eHealth--;
      tr = map(eHealth, 0, mHealth, 0, 70);
      tft.fillRect(120, 3, 70, 7, TFT_BLACK);
      tft.fillRect(120, 3, tr, 7, TFT_GREEN);

      if (eHealth <= 0) {
        tft.pushImage(ex, ey, 55, 55, buum);
#ifdef USE_BUZZER
        tone(BUZZER_PIN, NOTE_E4, 100, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_D4, 80, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_G5, 100, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_C3, 80, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_F4, 280, BUZZER_CHANNEL);
        noTone(BUZZER_PIN, BUZZER_CHANNEL);
#endif  // USE_BUZZER
        delay(700);
        new_level();
      }
      digitalWrite(25, 1);
      blinkTime = 1;
    }
  }

  for (int j = 0; j < 10; j++)  // did my ROCKET hit enemy
  {
    if (rocketX[j] + 18 > ex && rocketY[j] > ey + 2 && rocketY[j] < ey + 52) {
      tft.pushImage(rocketX[j], rocketY[j], 24, 24, explosion);
#ifdef USE_BUZZER
      if (sound == 1) {
        tone(BUZZER_PIN, NOTE_C3, 40, BUZZER_CHANNEL);
        noTone(BUZZER_PIN, BUZZER_CHANNEL);
      } else {
        delay(40);
      }
#else
      delay(40);
#endif  // USE_BUZZER

      tft.fillRect(rocketX[j], rocketY[j], 24, 24, TFT_BLACK);
      // delay(30);

      rocketX[j] = -50;
      brojac = brojac + 12;
      tft.setCursor(200, 0, 2);
      tft.print(brojac);
      eHealth = eHealth - rDamage;
      tr = map(eHealth, 0, mHealth, 0, 70);
      tft.fillRect(120, 3, 70, 7, TFT_BLACK);
      tft.fillRect(120, 3, tr, 7, TFT_GREEN);

      if (eHealth <= 0) {
        tft.pushImage(ex, ey, 55, 55, buum);
#ifdef USE_BUZZER
        tone(BUZZER_PIN, NOTE_E4, 100, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_D4, 80, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_G5, 100, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_C3, 80, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_F4, 280, BUZZER_CHANNEL);
        noTone(BUZZER_PIN, BUZZER_CHANNEL);
#endif  // USE_BUZZER
        delay(700);
        new_level();
      }
      digitalWrite(25, 0);
      blinkTime = 1;
    }
  }

  for (int j = 0; j < 10; j++)  // Am I hit
  {
    if (EbuletX[j] < x + 30 && EbuletX[j] > x + 4 && EbuletY[j] > y + 4 &&
        EbuletY[j] < y + 36) {
      EbuletX[j] = -50;
      ly[lives - 1] = -40;
      tft.fillRect((lives - 1) * 14, 0, 14, 14, TFT_BLACK);
      lives--;
      if (lives == 0) {
        tft.pushImage(x, y, 55, 55, buum);
#ifdef USE_BUZZER
        tone(BUZZER_PIN, NOTE_G4, 100, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_B4, 80, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_C5, 100, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_A4, 80, BUZZER_CHANNEL);
        tone(BUZZER_PIN, NOTE_F4, 280, BUZZER_CHANNEL);
        noTone(BUZZER_PIN, BUZZER_CHANNEL);
#endif  // USE_BUZZER
        delay(500);
        tft.fillScreen(TFT_BLACK);
        game_state = kGameOver;
      }

      digitalWrite(33, 1);
      blinkTime = 1;
#ifdef USE_BUZZER
      if (sound == 1) {
        tone(BUZZER_PIN, NOTE_C6, 4, BUZZER_CHANNEL);
        noTone(BUZZER_PIN, BUZZER_CHANNEL);
      } else {
        delay(4);
      }
#else
      delay(4);
#endif  // USE_BUZZER
    }
  }

  ey = ey + es;
  if (ey > 80) es = es * -1;
  if (ey < 18) es = es * -1;
  if (blinkTime > 0) blinkTime++;
  if (blinkTime > 2) {
    digitalWrite(25, 0);
    digitalWrite(33, 0);
    blinkTime = 0;
  }

  for (int i = 0; i < 10; i++) {  // enemy shoots
    if (EbuletX[i] > -10) {
      tft.pushImage(EbuletX[i], EbuletY[i], 7, 7, ebullet);
      EbuletX[i] = EbuletX[i] - EbulletSpeed;
    }
  }

  for (int i = 0; i < 4; i++)  // draw lifes
    tft.pushImage(i * 14, ly[i], 12, 11, life);
  for (int i = 0; i < 3; i++)  // draw lifes
    tft.pushImage(70 + (i * 14), ri[i], 8, 14, ricon);

  fireCount++;
  if (fireTime == fireCount) {
    EbuletX[Ecounter] = ex + 5;
    EbuletY[Ecounter] = ey + 24;
    fireCount = 0;
    fireTime = random(110 - (level * 15), 360 - (level * 30));
    Ecounter++;
  }
  if (counter == 9) counter = 0;
  if (rcounter == 3) rcounter = 0;
  if (Ecounter == 9) Ecounter = 0;
}  // play_game()
//------------------------------------------------------------------------------
void show_game_over_screen() {
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(0, 0, 240, 135, gameOver);
  tft.setCursor(24, 54, 2);
  tft.print("Score : " + String(brojac));
  tft.setCursor(24, 69, 2);
  tft.print("Level : " + String(level));
}  // show_game_over_screen()
//------------------------------------------------------------------------------
void loop() {
  switch (game_state) {
    case kIntro:
      show_intro();
      game_state = kGamePlay;
      break;
    case kGamePlay:
      play_game();
      break;
    case kGameOver:
      show_game_over_screen();
      wait_for_joystick_button_a_down();
      wait_for_joystick_button_a_up();
      game_state = kIntro;
      break;
  }
}  // loop()
