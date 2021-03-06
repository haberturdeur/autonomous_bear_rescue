#pragma config(Sensor, S2,     infra1,         sensorEV3_IRSensor)
#pragma config(Sensor, S3,     infra2,         sensorEV3_IRSensor)
#pragma config(Motor,  motorA,          motorL,        tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          klepetoL,      tmotorEV3_Medium, PIDControl, encoder)
#pragma config(Motor,  motorC,          motorR,        tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          klepetoR,      tmotorEV3_Medium, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// 04 - zakladni prujezd S, neobsahuje PID ani rampu

const int speed = 40; // pri jizde bez rampy max rychlost 40, aby se zbytecne nenicil motor a prevodovka pri rozjezdech a brzdeni
const float degToMm = 2.335;  // speed je rychlost teziste
const int dist = 84;       // vzdalenost stredu os kol robota od stredu leveho nebo praveho kola - pro zataceni
//float distance = 300;      // ultrazvuk v cm
int otacky_vpravo90st = 283;

float g_distPredni = 0, g_distLevy = 0, g_distPravy = 0;
float encoder = 0; //vzdalenost od zacatku hledani robota
int e1 = 0;
int e2 = 0;
int trueorfalse = 0;

// vsechny rozmery hriste a delky jizdy jsou v mm
// const int delkaStart = 200; // delka startovaci oblasti
const int delka1 = 470; // delka prvniho useku "S" - kolik robot pojede ze startovaci pozice
const int delka2 = 0; // delka druheho useku "S"
const int delka3 = 700; // delka tretiho useku "S" - robot jede bez korekci z ultrazvuku, pouze na enkodery
const int delka4 = 480; //delka pro stred ctverce
const int delka5 = 600; //od konce S k prvnimu oblouku
const int polomer1 = 200; // polomer prvni zatacky od startu
const int polomer2 = -300; // polomer druhe zatacky od startu
const int polomer3 = 300;
const int polomer4 = -200;

task encoderTask(){
  encoder = (getMotorEncoder(motorL) + getMotorEncoder(motorR))/2;
}

task senzory(){
	g_distPredni = getIRDistance(S1);
	g_distLevy = getIRDistance(S2);
	g_distPravy = getIRDistance(S3);
	delay(2);
}

// zaporna rychlost - jede vzad; zaporna draha - jede porad VPRED !!
void jizda( int draha, int rychlost){ // jed rovne danou drahu danou rychlosti
	moveMotorTarget(motorL, draha*degToMm , rychlost); // ktery motor, stupne, rychlost - mel by tocit vpravo o 90 stupnu
	moveMotorTarget(motorR, draha*degToMm , rychlost);
	waitUntilMotorStop(motorL);  // pokud nepockate na dojeti motoru, tak program ihned pokracuje dal a vzapeti skonci
	waitUntilMotorStop(motorR);
}

void oblouk( int polomer, int stupne, int rychlost){  // projed oblouk o danem polomeru, danem poctu stupnu a dane rychlosti
																									 		// polomer > 0 - zataci vpravo; polomer < 0 - zataci vlevo;
  bool zataci_vlevo = false;

	if (polomer < 0) {
		zataci_vlevo = true;
		polomer = abs(polomer); // pro dalsi vypocty je potreba kladny polomer
  }
	int rR = polomer + dist; // pro rychlejsi kolo - polomer zataceni vetsi
	int rP = polomer - dist; // pro pomalejsi kolo - polomer zataceni mensi
	float vR = rychlost*rR/polomer; // rychlost rychlejsiho kola
	float vP = rychlost*rP/polomer; // rychlost pomalejsiho kola
	float vzdP = 6.2*rP*degToMm*stupne/360; // vzdalenost pro pomalejsi kolo - 6,2832 = 2*pi, 6,2 - z pokusu pro rychlost 40
	float vzdR = 6.12*rR*degToMm*stupne/360; // vzdalenost pro rychlejsi kolo - 6,0597 - z pokusu pro rychlost 40

	if (zataci_vlevo) {
		moveMotorTarget(motorR, vzdP , vP);
		moveMotorTarget(motorL, vzdR , vR);
  }
  else {
  	moveMotorTarget(motorL, vzdP , vP);
		moveMotorTarget(motorR, vzdR , vR);
	}
	waitUntilMotorStop(motorL);  // pokud nepockate na dojeti motoru, tak program ihned pokracuje dal a vzapeti skonci
	waitUntilMotorStop(motorR);
}
void Jizda1(){  // hleda konec S
	float distance2 = 300;
	 //while (true) {
	do {
		distance2 = getIRDistance(S2); // levy infra
		displayBigTextLine(1, "A %i", distance2);
		setMotorSpeed(motorL, speed);
		setMotorSpeed(motorR, speed);
	} while (distance2 < 50);
		  	stopMultipleMotors(motorL , motorR);
		  	sleep(400);
}

void klepeta_otevri(){
	 resetMotorEncoder(klepetoL);
   resetMotorEncoder(klepetoR);
    setMotorTarget(klepetoL, 45, 10);
	  setMotorTarget(klepetoR, 45, -10);
	   waitUntilMotorStop(klepetoL);
	   waitUntilMotorStop(klepetoR);
}

void klepeta_zavri(){
	 resetMotorEncoder(klepetoL);
   resetMotorEncoder(klepetoR);
	 setMotorTarget(klepetoL, 45, -10);
	 setMotorTarget(klepetoR, 45, 10);
   sleep(1000);
}

void ZatackaVlevo() {
	 moveMotorTarget(motorL, otacky_vpravo90st, speed); // ktery motor, stupne, rychlost - mel by tocit vpravo o 90 stupnu
	moveMotorTarget(motorR, otacky_vpravo90st, - speed); // ktery motor, stupne, rychlost - mel by tocit vpravo o 90 stupnu
	waitUntilMotorStop(motorL);  // kdyz tam tohle nebude, tak se program skonci driv, nez se motor vubec rozjede
	waitUntilMotorStop(motorR);
}

void Zatacka180() {
	 moveMotorTarget(motorL, otacky_vpravo90st*2, speed); // ktery motor, stupne, rychlost - mel by tocit vpravo o 90 stupnu
	moveMotorTarget(motorR, otacky_vpravo90st*2, - speed); // ktery motor, stupne, rychlost - mel by tocit vpravo o 90 stupnu
	waitUntilMotorStop(motorL);  // kdyz tam tohle nebude, tak se program skonci driv, nez se motor vubec rozjede
	waitUntilMotorStop(motorR);
}
void ZatackaVpravo() {
	 moveMotorTarget(motorL, otacky_vpravo90st, - speed); // ktery motor, stupne, rychlost - mel by tocit vpravo o 90 stupnu
	moveMotorTarget(motorR, otacky_vpravo90st, speed); // ktery motor, stupne, rychlost - mel by tocit vpravo o 90 stupnu
	waitUntilMotorStop(motorL);  // kdyz tam tohle nebude, tak se program skonci driv, nez se motor vubec rozjede
	waitUntilMotorStop(motorR);
}
float HledaniMedveda() {
	 float distLastPredni = 300;
	 float distLastLevy = 300;
	 float distLastPravy = 300;
	 setMotorSpeed(motorL, speed);
	 setMotorSpeed(motorR, speed);
   do {
	   distLastPredni = g_distPredni;
	   distLastLevy = g_distLevy;
	   distLastPravy = g_distPravy;
	   delay(20); // ???
	   displayBigTextLine(1, "P %i", distLastPredni);
	   displayBigTextLine(3, "L %i", distLastLevy);
	   displayBigTextLine(5, "R %i", distLastPravy);
      } while((g_distLevy - distLastLevy < 10) && (g_distPravy - distLastPravy < 10));
	 stopMultipleMotors(motorL , motorR);
	 sleep(100);
	 if(g_distLevy - distLastLevy > 9){
	   ZatackaVlevo();
	   displayBigTextLine(7, "vlevo");
	   return 0;
	   }
	 else {
		  	ZatackaVpravo();
		  	displayBigTextLine(7, "vpravo");
				return 1;
   }
}

void HledaniMedveda1() {
	 float distance1 = 300;
	 setMotorSpeed(motorL, speed);
	 setMotorSpeed(motorR, speed);
	 do {
		 distance1 = getIRDistance(S1);
		 displayBigTextLine(1, "F %i", distance1);
		  } while(distance1 > 3);
		 stopMultipleMotors(motorL , motorR);

		 sleep(200);
}
void JizdaSmeremS(){
  if(trueorfalse == 0){
		Zatacka180();
    moveMotorTarget(motorL, delka4*degToMm+e1 , speed);
	  moveMotorTarget(motorR, delka4*degToMm+e1 , speed);
	  waitUntilMotorStop(motorL)
	  waitUntilMotorStop(motorR)
  }
	else{
		moveMotorTarget(motorL, delka4*degToMm-e1 , speed);
	  moveMotorTarget(motorR, delka4*degToMm-e1 , speed);
	  waitUntilMotorStop(motorL)
	  waitUntilMotorStop(motorR)
  }
}
void jizdaSmeremS1(){
	float distance2 = 300;
	do {
		distance2 = getIRDistance(S3); // levy infra
		displayBigTextLine(1, "A %i", distance2);
		setMotorSpeed(motorL, speed);
		setMotorSpeed(motorR, speed);
	} while (distance2 > 50);
		  	stopMultipleMotors(motorL , motorR);
		  	sleep(400);
}

void klepeta(){
    setMotorTarget(klepetoL, 40, -10);
    setMotorTarget(klepetoR, 40, 10);
    waitUntilMotorStop(klepetoL);
	  waitUntilMotorStop(klepetoR);
	    sleep(2000);
	  setMotorTarget(klepetoL, 0, 10);
	  setMotorTarget(klepetoR, 0, 10);

     waitUntilMotorStop(klepetoL);
	  // waitUntilMotorStop(klepetoR);
}

float medved1(){
  float d = getIRDistance(S1);
  displayBigTextLine(1, "A %i", d);
  return d;
}
float medved2(){
  float d = getIRDistance(S2);
  displayBigTextLine(3, "B %i", d);
  return d;
}
float medved3(){
  float d = getIRDistance(S3);
  displayBigTextLine(5, "C %i", d);
  return d;
}

task main(){
	startTask(encoderTask);
	startTask(senzory);
   //klepeta();
	//jizda( delka1, speed);
	//oblouk( polomer1, 180, speed);
	//jizda( delka2, speed);
	//oblouk( polomer2, 170, speed);
	 //jizda( delka3, speed);
  //Jizda1();  // hleda konec S
  //ZatackaVlevo();
  //jizda(delka4, speed); // do stredu hriste
  //ZatackaVpravo();
  //jizda(70, speed); // popojet, aby nebral mantinel
  //trueorfalse = HledaniMedveda();
   resetMotorEncoder(motorL);
   resetMotorEncoder(motorR);
 // klepeta_otevri();
 // HledaniMedveda1();
 //   e1 = (getmotorencoder(motorL) + getmotorencoder(motorR))/2 - otacky_vpravo90st;
	//klepeta_zavri();
	//JizdaSmeremS();
	//ZatackaVpravo();
  JizdaSmeremS1();
  jizda( delka5, speed)
  oblouk( polomer3, 170, speed);
  sleep(100);
  oblouk( polomer4, 180, speed);
  jizda( delka1, speed);
	//ZatackaVpravo();
	//jizda(delka4, speed);
	while (true) {}//distance = medved1(); delay(300);
	//  						 //distance = medved2(); delay(300);
	//  						 //distance = medved3(); delay(300); };
	////setLEDColor(ledGreen); // hotovo :-)
	//delay(10);
	//}

  stopTask(senzory);
  stopTask(encoderTask);
}
