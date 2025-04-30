
// Ορίζουμε τα pins για τον οδηγό μοτέρ
int xstepPin = 2;
int xdirPin = 5;
int ystepPin = 3;
int ydirPin = 6;
int zstepPin = 4;
int zdirPin = 7;
// αρχικά δεδoμένα κινητηρων
int xkat = LOW;
long xstr = 0;
int ykat = LOW;
int ystr = 0;
int zkat = LOW;
long zstr = 0;

long h = 0, m = 0;  // Αριθμητικές μεταβλητές
char j = ' ';  // Χαρακτήρες

void setup() {
    Serial.begin(9600);  // Εκκίνηση σειριακής επικοινωνίας
    Serial.println("Δώσε τις 3 τιμές: j, h, m (j='A,K' και h=ΟΡΟΦΟΣ, m=ΣΤΗΛΗ )");
}

void loop() {
    if (Serial.available() > 0) { // Έλεγχος για εισαγωγή δεδομένων
                
        Serial.println("Δώσε την τιμή του j (γράμμα):");
        while (Serial.available() == 0); // Περιμένει είσοδο
        j = Serial.read(); // Διαβάζει έναν χαρακτήρα (Ανοδος - Κάθοδος)

        Serial.println("Δώσε την τιμή του h για χ:");
        while (Serial.available() == 0);
        h = Serial.parseInt();

        Serial.println("Δώσε την τιμή του m για y:");
        while (Serial.available() == 0);
        m = Serial.parseInt();

        

        // Απορρίπτει περιττά νέα γραμμή ή κενά
        while (Serial.available() > 0) Serial.read();

        if (j == 'A'||j=='K'  && 1 <= h <= 3  && 1 <= m <= 2  ) { // Έλεγχος σωστών τιμών
            Serial.println("Εκκίνηση λειτουργίας...");

            for (int i = 1; i <= 5; i++) { // Εκτέλεση όλων των κινήσεων

                if (i == 1 && j=='A') { 
                    xstr = (h * 14000)+1500; ystr = (m-1) * 22500; zstr = 000; 
                    xkat = HIGH; ykat = HIGH; zkat = HIGH; 
                }  
                if (i == 2 && j=='A') { 
                    xstr = 3000; ystr = 0; zstr = 27000; xkat = HIGH; zkat = HIGH;
                }  
                if (i == 3 && j=='A') { 
                   xstr = 3000; ystr = 0; zstr = 27000; xkat = LOW; zkat = LOW;
                }  
                if (i == 4 && j=='A') { 
                  xstr = 0;  ystr = (m-1) * 22500; zstr = 0; ykat = LOW;
                }  
                if (i == 5 && j=='A') { 
                    xstr = (h * 14000)+1500; ystr = 0; zstr = 0; xkat = LOW;
                }

                if (i == 1 && j=='K') { 
                    xstr = (h * 14000)+1500; ystr = (m-1) * 22500; zstr = 27000; 
                    xkat = HIGH; ykat = HIGH; zkat = HIGH; 
                }

                if (i == 2 && j=='K') { 
                    xstr = 3000; ystr = 0; zstr = 0; 
                    xkat = HIGH; ykat = HIGH; zkat = HIGH; 
                }

                if (i == 3 && j=='K') { 
                    xstr = 0; ystr = 0; zstr = 27000; 
                    xkat = HIGH; ykat = HIGH; zkat = LOW; 
                }

                if (i == 4 && j=='K') { 
                    xstr = 3000; ystr = (m-1) * 22500; zstr = 0; 
                    xkat = LOW; ykat = LOW; zkat = HIGH; 
                }

                if (i == 5 && j=='K') { 
                    xstr = (h * 14000)+1500; ystr = 0; zstr = 0; 
                    xkat = LOW; ykat = HIGH; zkat = HIGH; 
                }
                

                // Κίνηση κινητήρων
                digitalWrite(xdirPin, xkat);
                for (int x = 0; x < xstr; x++) {
                    digitalWrite(xstepPin, HIGH);
                    delayMicroseconds(700);
                    digitalWrite(xstepPin, LOW);
                    delayMicroseconds(700);
                }

                delay(10);

                digitalWrite(ydirPin, ykat);
                for (int x = 0; x < ystr; x++) {
                    digitalWrite(ystepPin, HIGH);
                    delayMicroseconds(700);
                    digitalWrite(ystepPin, LOW);
                    delayMicroseconds(700);
                }

                delay(10);

                digitalWrite(zdirPin, zkat);
                for (int x = 0; x < zstr; x++) {
                    digitalWrite(zstepPin, HIGH);
                    delayMicroseconds(600);
                    digitalWrite(zstepPin, LOW);
                    delayMicroseconds(600);
                }

                delay(10);
            }

            // Επαναφορά τιμών
            xstr = 0;
            ystr = 0;
            zstr = 0;

            Serial.println("Κύκλος εκτέλεσης ολοκληρώθηκε. Δώσε νέες τιμές για j, h, m,:");
        } else {
            Serial.println("Λάθος τιμές! Πρέπει j='A', h=1, m=1. Προσπάθησε ξανά.");
        }
    }
}
