/*Övningsuppgift 1 - Trafikljus
Dagens trafiksignalssystem i Sverige använder sig av PLC och RTU moduler för att hantera signalerna. 
Dessa system är oftast skrivna i Ladder, Visual Basic eller C#. 
Dock används samma kommunikationsmedel som du kommer använda dig utav för att överföra signalerna från ett trafikljus till ett annat etc. 
I denna övningsuppgift ska du just göra detta. Du ska simulera en korsning med trafiksignaler.

Delmoment 1:
Du ska i detta delmoment bygga ett trafikljus (grönt, gult, rött) som ska fungera på följande sätt. 

När en bil har identifierats så ska trafikljuset slå om till grönt genom följande steg:

Trafikljuset lyser rött med fast sken.
Trafikljuset ändrar så att både rött och gul ljus lyser i fast sken. Detta ska lysa i 3 sekunder.
Sedan slår trafikljuset över till att bara lysa grönt i fast sken. Det gröna ljuset ska lysa i 1 minut. 
Sedan slår trafikljuset över till att bara lysa gult i fast sken. Detta ska lysa i 3 sekunder.
Sedan ska trafikljuset slå om till rött med fast sken igen.
För att simulera en bil kan du använda dig av en pushknapp eller använda dig av någon form av distanssensor. Såsom ultraljudsensorn.

Delmoment 2:

I detta delmoment ska du utöka funktionaliteten i ditt system. Du ska nu se till att det finns två olika trafikljus. Trafikljusen ska kunna kommunicera med varandra genom antingen I2C eller UART (vanligtvis används CanBus, men det kräver speciell hårdvara).

När ingen bil finns ska båda trafikljusen vara röda. När en bil har identifierats hos ett trafikljus ska ljuset slå om till grönt och det andra trafikljuset ska vara rött. 
När cyklen har gått så får vilket trafikljus som helst ändra sig till grönt om det är så att det finns en bil vid trafikljuset. 

Ni får självklart sammarbeta med varandra för att lösa uppgiften. Denna uppgift ska du även skicka in. Dock är detta inte betygsgrundande.
*/

#define RED 4 
#define GREEN 2 
#define YELLOW 3
#define BUTTON 7

#define SIZEI 7
#define SIZEJ 3

bool isRunning = false;
bool buttonPressed = false;

bool otherLight = false;

int shortWait = 3000;
int longWait = 6000; //600000

int cycle[SIZEI][SIZEJ]={
                {YELLOW, HIGH, shortWait},
                {RED, LOW, 1},
                {YELLOW, LOW, 1},
                {GREEN, HIGH, longWait},
                {GREEN, LOW, 1},
                {YELLOW, HIGH, shortWait},
                {YELLOW, LOW, 1}
                };


void setup(){
    pinMode(RED, OUTPUT);
    pinMode(YELLOW, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BUTTON, INPUT);

    Serial.begin(9600);
}

void loop(){ 
    CheckSerial();
   
    int button = digitalRead(BUTTON);

    if (!button && !buttonPressed){
            buttonPressed = true;
    }
    if (buttonPressed && !isRunning && !otherLight){
        isRunning = true;
    }

    if(isRunning){
        Serial.write('1');
        CycleGreenDelay();
        Serial.write('0');
    }else{
        digitalWrite(RED, HIGH);
        digitalWrite(YELLOW, LOW);
        digitalWrite(GREEN, LOW);
    }
}

void CycleGreenDelay(){
    for (int i=0; i<SIZEI; i++){
            digitalWrite(cycle[i][0], cycle[i][1]);
            delay(cycle[i][2]);
        }
    isRunning = false;
    buttonPressed = false;
}

void CheckSerial(){
    if(Serial.available()){
        char serialData = Serial.read();
        char prevSerialData = '2';
        if (serialData != prevSerialData){
            prevSerialData = serialData;
        }
            if (serialData == '1'){
                otherLight = true;
            }else{
                otherLight = false;
            }
    }
}