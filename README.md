# LittleFS_Manager

Sketch for managing the LittleFS memory on an ESP

Sketch zur Verwaltung des LittleFS Speichers auf einem ESPs

-----------------------------------

Upload **LittleFS_Manager.ino** to the ESP. I use **Arduino IDE** for this.
Then open the **serial monitor**.
- **ENTER** shows the content of the LittleFS
- Entering a **filename** deletes this file
- Entering a **directory name** changes to this directory
- Entering **..** changes to the parent directory
- If a directory is empty, it will be deleted

-----------------------------------

**LittleFS_Manager.ino** auf den ESP hochladen. Ich nutze dazu **Arduino IDE**.
Anschliessend den **Seriellen Monitor** öffnen.
- **ENTER** zeigt den Inhalt des LittleFS
- Eingabe eines **Dateinamens** löscht diese Datei
- Eingabe eines **Verzeichnisnamens** wechselt in dieses Verzeichnis
- Eingabe von **..** wechselt in das übergeordnete Verzeichnis
- Wenn ein Verzeichnis leer ist, wird dieses gelöscht
