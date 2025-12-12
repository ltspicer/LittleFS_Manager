# LittleFS_Manager

Sketch for managing the LittleFS memory on an ESP

Sketch zur Verwaltung des LittleFS Speichers auf einem ESP

-----------------------------------

Upload **LittleFS_Manager.ino** to the ESP. I use **Arduino IDE** for this.

Then open the **serial monitor**. Set baud rate to 9600.

Commands:
- **-help** for help
- **-language en** set language to english
- **-rename** <old_name> <new_name>
- **ENTER** shows the content of the LittleFS
- Entering a **filename** deletes this file
- Entering a **directory name** changes to this directory
- Entering **..** changes to the parent directory
- If a directory is empty, it will be deleted

-----------------------------------

**LittleFS_Manager.ino** auf den ESP hochladen. Ich nutze dazu **Arduino IDE**.

Anschliessend den **Seriellen Monitor** öffnen. Setze Baudrate auf 9600.

Befehle:
- **-help** zeigt die Hilfe an
- **-language de** setzt Sprache auf deutsch
- **-rename** <alter_name> <neuer_name>
- **ENTER** zeigt den Inhalt des LittleFS
- Eingabe eines **Dateinamens** löscht diese Datei
- Eingabe eines **Verzeichnisnamens** wechselt in dieses Verzeichnis
- Eingabe von **..** wechselt in das übergeordnete Verzeichnis
- Wenn ein Verzeichnis leer ist, wird dieses gelöscht

-----------------------------------

-----------------------------------

### Flashing:

**Option 1:**

Upload the ino file using Arduino IDE.

**Option 2:**

Upload the bin file using **esptool.exe (Windows)** or **esptool (Linux)**.

Example for Windows:

**esptool.exe -vv -cd nodemcu -cb 115200 -bz 4M -cp COM7 -ca 0x00000 -cf LittleFS_Manager.ino.bin**

Example for Linux:

**sudo ./esptool -vv -cd nodemcu -cb 115200 -bz 4M -cp /dev/ttyUSB0 -ca 0x00000 -cf LittleFS_Manager.ino.bin**


**Port** ( /dev/ttyUSB0 or COM7 ) **adjust** if necessary.

-----------------------------------

### Flashen:

**Variante 1:**

Mittels Arduino IDE die ino Datei hochladen.

**Variante 2:**

Mittels **esptool.exe (Windows)** oder **esptool (Linux)** die bin Datei hochladen.

Beispiel für Windows:

**esptool.exe -vv -cd nodemcu -cb 115200 -bz 4M -cp COM7 -ca 0x00000 -cf LittleFS_Manager.ino.bin**

Beispiel für Linux:

**sudo ./esptool -vv -cd nodemcu -cb 115200 -bz 4M -cp /dev/ttyUSB0 -ca 0x00000 -cf LittleFS_Manager.ino.bin**


**Port** ( /dev/ttyUSB0 bzw COM7 ) ggf. **anpassen**.

