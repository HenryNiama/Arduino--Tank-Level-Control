//Entradas (sensores)
int sA = 22; //Sensor Parte superior (ESTADO ALARMA)
int sB = 23; //Sensor del medio (ESTADO LLENO)
int sC = 26; //Sensor ultimo (ESTADO NORMAL)

//Salidas (Bombas y alarma)

int bNoSumergible = 28; //El relé activa bomba No Sumergible/Bomba de entrada.
int bSumergible = 30; //El relé activa bomba Sumergible/Bomba de salida.
int alarma = 32; // pin del buzzer pasivo

//Variables para controlar cada sensor
int estadoNormal; //sA
int estadoLleno; //sC
int estadoAlarma; //sC

//------------------------------------------------------------------
//-----variables del buzzer pasivo (alarma)
int cuarto = 1000 / 4; // tiempo de 1/4 de la nota
int octavo = 1000 / 8; // tiempo de 1/8 de la nota
double pausa = 1.30; // pausar el 30% de la nota

// La frecuencia min que puede producir el Arduino Uno es 31Hz.
// La frecuencia max que puede producir el Arduino Uno es 65,525Hz.
int NOTA_C4 = 262; // Frecuencia de la nota C4 es 262Hz
int NOTA_A4 = 440; // Frecuencia de la nota A4 es 440Hz
int NOTA_E5 = 659; // Frecuencia de la nota E5 es 659Hz
//-----------------------------------------------------------------

void setup() {

  pinMode(sA, INPUT);
  pinMode(sB, INPUT);
  pinMode(sC, INPUT);

  pinMode(bSumergible, OUTPUT);
  pinMode(bNoSumergible, OUTPUT);
  pinMode(alarma, OUTPUT);

  Serial.begin(9600); //abre el puerto de serie para la transmision de ceros
}

void loop() {

  estadoNormal = digitalRead(sC);
  estadoLleno = digitalRead(sB);
  estadoAlarma = digitalRead(sA);

  //Sensor A                Sensor B           Sensor C
  if (estadoAlarma == 0 && estadoLleno == 0 && estadoNormal == 0) {

    while (estadoNormal == 0) {

      digitalWrite(bSumergible, HIGH); //Mi bomba de salida va a estar desactivada
      digitalWrite(bNoSumergible, LOW); //La B.N.sumergible (de entrada) está activa.
      Serial.println("Estado 0 0 0 bomba no sumergible accionada.");
      /*Cuando el agua alcance al sensor C, se guardara en estadonormal el
        numero 1, y saldrá del blucle while.
      */
      estadoNormal = digitalRead(sC);
    }
    digitalWrite(bNoSumergible, LOW); // Bomba de llenado (de entrada)continua prendida.
    digitalWrite(bSumergible, LOW); //Bomba de salida SE ACTIVA
    digitalWrite(alarma, LOW); //Alarma apagada

  } else if (estadoAlarma == 0 && estadoLleno == 0 && estadoNormal == 1) {
   
    while ( estadoLleno == 0 && estadoNormal == 1) {

      digitalWrite(bNoSumergible, LOW); //Bomba de salida activada
      digitalWrite(bSumergible, LOW); //Bomba de entrada activada
      Serial.println("Estado 0 0 1 bomba no sumergible y sumergible accionadas.");
      //estadoAlarma = digitalRead(sA);
      estadoLleno = digitalRead(sB);
      estadoNormal = digitalRead(sC);
    }

   if(estadoLleno==1){
    digitalWrite(bSumergible, LOW);//Se activa
    digitalWrite(bNoSumergible, HIGH); //Se apagan
   }else if(estadoNormal==0){
    digitalWrite(bSumergible, HIGH);//Se apaga
    digitalWrite(bNoSumergible, LOW); //Se activa
   }
     digitalWrite(alarma, LOW); //Sigue apagado 

  } else if (estadoAlarma == 0 && estadoLleno == 1 && estadoNormal == 1) {
   
    while (estadoAlarma == 0 && estadoLleno == 1 && estadoNormal == 1) {

      digitalWrite(bSumergible, LOW); //La B.sumergi se activa para que se valla el agua que está demás.
      /*Cuando el agua baje lo suficiente para que el sensor B (estado lleno) vuelva a 0. El
         bucle termina.
      */
      Serial.println("Estado 0 1 1 bomba sumergible accionada.");
      estadoLleno = digitalRead(sB);
      estadoNormal = digitalRead(sC);
      estadoAlarma = digitalRead(sA);
    }
   
    if (estadoLleno == 0) {

      delay(70000); //esperar hasta que el agua baje al nivel sC
       digitalWrite(bNoSumergible, LOW); //Bomba de entrada se activa
        digitalWrite(bSumergible, LOW);//finalmente apago la bomba sumergi. por que ya llego al sC ==1 y sB==0.
        digitalWrite(alarma, LOW); //Alarma sigue apagada
    }


  } else if (estadoAlarma == 1 && estadoLleno == 1 && estadoNormal == 1)  {

    while (estadoAlarma == 1 ) {
      digitalWrite(bNoSumergible, HIGH); 
      digitalWrite(bSumergible, LOW); //La B.sumergi se activa para que se valla el agua demás.
      /*Cuando el agua baje lo suficiente para que el sensor A (estado alarma) vuelva a 0. El
         bucle termina.
      */
      digitalWrite(alarma, HIGH);    //La alarma se activa mientras estadoAlarma(sA)=1
      estadoAlarma = digitalRead(sA);

      Serial.println("Estado 1 1 1 bomba sumergible y alarma accionadas.");

      // Melodía del buzzer
      tone(alarma, NOTA_C4, cuarto); //un cuarto de la nota C4.
      delay(cuarto * pausa); // silenciar por una duración de la nota previa + 30% de la duración de esa nota
      tone(alarma, NOTA_A4, octavo); //un cuarto de la nota A4.
      delay(octavo * pausa); // silenciar por una duración de la nota previa + 30% de la duración de esa nota
      tone(alarma, NOTA_E5, cuarto); //un cuarto de la nota E5.
      delay(cuarto * pausa); // silenciar por una duración de la nota previa + 30% de la duración de esa nota
      //---------------------------------------------------------------------------------------
    }

    //una vez sale del bucle, quiere decir que el sensor a volvió a 0, sA==0 entonces:
    digitalWrite(bNoSumergible, HIGH); //Bomba de entrada sigue desactivada como antes
    // digitalWrite(bSumergible, HIGH);//
    digitalWrite(alarma, LOW); //Alarma finalmente se apaga por que sA==0.
    delay(80000); //esperar hasta que el agua baje al nivel sC

  }
}
