void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi ..");
  lcd.setCursor(0,0);
  lcd.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  lcd.setCursor(0,1);
  lcd.print("WIFI Connected");
  Serial.println("WIFI Connected");

}

void mqBegin() {
  MQ5.setRegressionMethod(1);
  MQ5.setA(1163.8);
  MQ5.setB(-3.874);

  /*
    Exponential regression:
  Gas    | a      | b
  H2     | 1163.8 | -3.874
  LPG    | 80.897 | -2.431
  CH4    | 177.65 | -2.56
  CO     | 491204 | -5.826
  Alcohol| 97124  | -4.918
  */

  MQ5.init();
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ5.update();
    calcR0 += MQ5.calibrate(RatioMQ5CleanAir);
    Serial.print(".");
  }
  MQ5.setR0(calcR0 / 10);
  Serial.println("  done!.");

  if (isinf(calcR0)) {
    Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
    while (1)
      ;
  }
  if (calcR0 == 0) {
    Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
    while (1)
      ;
  }
  /*****************************  MQ CAlibration ********************************************/
  MQ5.serialDebug(true);
}

void run() {
  MQ5.update();
  // MQ5.readSensor();
  // MQ5.serialDebug();
  frValue = digitalRead(fr);
  mqValue = MQ5.readSensor();
  Serial.println("Nilai MQ-5 : " + (String)mqValue);
  Serial.println("Nilai sApi : " + (String)frValue);

  // Kondisi Buzzer Menyala
  if (frValue == 0 || mqValue > 50) {
    digitalWrite(buzzer, HIGH);
    statusFr = "Menyala";
  } else {
    digitalWrite(buzzer, LOW);
    statusFr = "Mati";
  }

  // Mengirim data ke Blynk 
  Blynk.virtualWrite(V0, mqValue);
  Blynk.virtualWrite(V1, statusFr); 


  lcd.setCursor(0, 0);
  lcd.print("---- SMART HOME ----");
  lcd.setCursor(0, 1);
  lcd.print("Nilai MQ-5 : " + (String)mqValue);
  lcd.setCursor(0, 2);
  lcd.print("Nilai sApi : " + (String)statusFr);
  lcd.setCursor(0, 3);
  lcd.print("--------------------");

  // LCD Clear
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis) >= interval) {
    lcd.clear();
    previousMillis = millis();
  }
}

BLYNK_WRITE(V2) {
  int fan = param.asInt(); //Cek Data Button
  Serial.print("Fan 1:"); Serial.println(fan);
  if (fan == 1)digitalWrite(fan1, LOW); //1-ON , 0-OFF
  if (fan == 0)digitalWrite(fan1, HIGH);
}

BLYNK_WRITE(V3) {
  int fan = param.asInt(); //Cek Data Button
  Serial.print("Fan 2:"); Serial.println(fan);
  if (fan == 1)digitalWrite(fan2, LOW); //1-ON , 0-OFF
  if (fan == 0)digitalWrite(fan2, HIGH);
}