# Projet IOT : Memtest

<h2>Description du jeu</h2>

<p>Memtest est un jeu de mémoire disposant 8 LEDs et de 6 couleurs.

Chaque couleur correspond à une direction,du coup, il y a 6 direction possibles : Haut, Bas, Gauche, Droite, Devant , Derrière.

Une fois le jeu est lancer la première LED s’allume avec une couleur.

Il faudra deviner la bonne direction pour faire allumer la LED suivante qui allume à son tour une couleur 

La but de Memtest est tester la mémoire du joueur et voir le meilleur score de celui qui parvient à allumer les 8 LEDs
</p>



<h2>1 - Setup</h2>
<p>
-Ajouter l'url suivante dans Fichier -> Préférences-> URL de gestionnaire de cartes supplémentaires:
http://arduino.esp8266.com/stable/package_esp8266com_index.json<br>
-installer le pilote de la carte ESP8266 dans Outils->Type de carte-> Gestionnaire de carte<br>
-Telecharger la lib Neo_Pixel (https://github.com/adafruit/Adafruit_NeoPixel), et la dézipper dans le répertoire libraries de Arduino<br>
-Modifier le nom du réseau et le mot de passe du réseau WIFI dans le fichier MemTest.ino<br>
  WiFi.begin("Nom Reseau", "Password");<br>
</p>
<h2>2 - Hard ware</h2>
<b>Liste de composants : <b>
  <P>   WEMOS D1 mini (Un mini board de wifi basé sur ESP-8266EX) X 1</p>
  <img src="https://github.com/HiduBrh/IOT/blob/master/images/d1_mini.jpg" width="500">
  
  
  <p>   MPU-6050 Accelerometer X 1 </p>
  <img src="https://github.com/HiduBrh/IOT/blob/master/images/mpu6050.jpg" width="500">
  
  
  <p>   Neopixel ws2812 X 1</p>
  <img src="https://github.com/HiduBrh/IOT/blob/master/images/ws2812.jpg" width="500">
  
  
  <p>   Les cables X N</P>
  <img src="https://github.com/HiduBrh/IOT/blob/master/images/cables.jpg" width="500">
  
<p>Schema de montage</p> 
<img src="https://github.com/HiduBrh/IOT/blob/master/Schema.png">
<h2>3 - Web interface</h2>


<h2>Réference</h2>

<b>Installations du Arduino : </b><p>http://barzilouik.cat/wiki/doku.php?id=introduction_arduino_-_blink_sur_le_wemos_d1_mini</p>

<b>Web Server : </b><p>https://www.arduino.cc/en/Tutorial/WebServer</p>

<b>Controller de LED : </b><p>https://www.kzenjoy.net/2015/ws2812-et-la-lumiere-fut/</p>

<b>MPU-6050 Accelerometer + Gyro : </b><p>https://playground.arduino.cc/Main/MPU-6050</p>
