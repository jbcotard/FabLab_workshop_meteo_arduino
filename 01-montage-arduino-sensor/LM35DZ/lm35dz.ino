/*
 * Code d'exemple pour le capteur LM35 (2°C ~ +110°C).
 */
 
// Fonction setup(), appelée au démarrage de la carte Arduino
void setup() {
 
  // Initialise la communication avec le PC
  Serial.begin(9600);
}
 
// Fonction loop(), appelée continuellement en boucle tant que la carte Arduino est alimentée
void loop() {
  
  // Mesure la tension sur la broche A0
  int valeur_brute = analogRead(A0);
  
  // Transforme la mesure (nombre entier) en température via un produit en croix
  float temperature_celcius = valeur_brute * (5.0 / 1023.0 * 100.0);
  
  // Envoi la mesure au PC pour affichage et attends 250ms
  Serial.println(temperature_celcius);
  delay(250);
}

