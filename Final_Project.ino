#include <Keypad.h>
#include <LiquidCrystal.h>

// Define the correct password
const char* password = "030023"; 
int position = 0;                                           // Tracks the current digit being entered
bool isLocked = true;                                       // System starts in a locked state
bool isReadyForInput = false;                               // Tracks if the system is ready to accept password input

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[4] = {7, 0, 1, 3};                               // Rows connected to pins 7, 0, 1, 3
byte colPins[4] = {2, 4, 5, 6};                               // Columns connected to pins 2, 4, 5, 6

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LCD configuration (RS, EN, D4, D5, D6, D7)
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

String enteredPassword = "";                                // String to store the entered password
String enteredSequence = "";                                // String to store the entered sequence for special checks

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setCursor((16 - 13) / 2, 0);
  lcd.print("Final Project");
  lcd.setCursor((16 - 5) / 2, 1);
  lcd.print("CS443");
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    Serial.print("Key Pressed: ");
    Serial.println(key);
    
    if (!isReadyForInput) {   // Only check for the '####' sequence when not entering a password
      enteredSequence += key;
      if (enteredSequence.length() > 4) {
        enteredSequence = enteredSequence.substring(1);  // Keep only the last 4 characters
      }

      if (enteredSequence == "####") {
        showThankYouMessage();
        enteredSequence = "";  // Reset the sequence
        return;
      }
    }

    if (key == '*') {
      lcd.clear();                      // Reset the display and prompt for password entry
      position = 0;
      enteredPassword = "";             // Clear entered password
      isReadyForInput = true;           // Allow input after pressing '*'
      lcd.setCursor((16 - 14) / 2, 0);
      lcd.print("Enter password");
      lcd.setCursor(0, 1);
    } 
    else if (isReadyForInput && key >= '0' && key <= '9') {
      handlePasswordEntry(key);                              // Only handle digits ('0' to '9') if ready for input
    }
  }
}

// Function to handle password entry
void handlePasswordEntry(char key) {
  lcd.setCursor(position, 1);
  lcd.print('*');                                     // Display '*' for each digit entered
  enteredPassword += key;                             // Append the entered key to the enteredPassword string
  position++;

  if (position == 6) {
    checkPassword();                                  // After 6 digits are entered, verify the password
  }
}

// Function to check if the entered password is correct
void checkPassword() {
  Serial.print("Entered Password: ");
  Serial.println(enteredPassword);
  if (enteredPassword == password) {                  // Password entered correctly
    lcd.clear();
    lcd.setCursor((16 - 14) / 2, 0);
    lcd.print("That's Correct");
    lcd.setCursor((16 - 10) / 2, 1);
    lcd.print("Password!!");
    
    isLocked = false;                                 // Unlock the system
    isReadyForInput = false;                          // Reset ready state
    delay(5000);
    
    lcd.clear();
    isLocked = true;
    lcd.setCursor((16 - 16) / 2, 0);
    lcd.print("Press * to enter");
    lcd.setCursor((16 - 14) / 2, 1);
    lcd.print("password again");
    position = 0;                                     // Reset for a new attempt
    enteredPassword = "";
  } 
  else {                                              // Incorrect password
    lcd.clear();
    lcd.setCursor((16 - 9) / 2, 0);
    lcd.print("Incorrect");
    lcd.setCursor((16 - 10) / 2, 1);
    lcd.print("Password!!");
    
    delay(3000);
    
    lcd.clear();
    isReadyForInput = false;                          // Reset ready state
    lcd.setCursor((16 - 10) / 2, 0);
    lcd.print("Press * to");
    lcd.setCursor((16 - 14) / 2, 1);
    lcd.print("enter password");
    position = 0;                                     // Reset for a new attempt
    enteredPassword = "";                             // Clear entered password
  }
}

// Function to display "Thank you" message
void showThankYouMessage() {
  lcd.clear();
  lcd.setCursor((16 - 9) / 2, 0);
  lcd.print("Thank You");
  
  delay(3000);                                        // Display the message for 2 seconds
  
  lcd.clear();
  lcd.setCursor((16 - 13) / 2, 0);
  lcd.print("Final Project");
  lcd.setCursor((16 - 5) / 2, 1);
  lcd.print("CS443");
}
