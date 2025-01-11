// LittleFS Manager //
//   Version 1.0    //
// by D. Luginbuehl //
//    (C) 2025      //

#include <LittleFS.h>

String currentDirectory = "/"; // Der aktuelle Pfad

void setup() {
  Serial.begin(9600);
  delay(5000);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS konnte nicht initialisiert werden.");
    return;
  }

  Serial.println();
  Serial.println("LittleFS initialisiert");

  Serial.println();
  Serial.print("Aktuelles Verzeichnis: ");
  Serial.println(currentDirectory); // Ausgabe des neuen Pfads

  // Verzeichnisse und Dateien auflisten
  listFiles(LittleFS, "/", 1);

  // Endlosschleife für Benutzerinteraktion
  while (true) {
    Serial.println("\nGib den Dateinamen zum Löschen oder einen Verzeichnisnamen zum Auflisten ein (ENTER für erneutes Auflisten):");

    while (Serial.available() == 0) {
      // Warten auf Eingabe
    }

    String eingabe = Serial.readString();
    eingabe.trim(); // Entfernt führende und nachfolgende Leerzeichen
    Serial.print("Eingabe: ");
    Serial.println(eingabe);

    // Wenn der Benutzer eine Datei oder ein Verzeichnis angibt
    String fullPath = currentDirectory + (currentDirectory.endsWith("/") ? "" : "/") + eingabe; // Verhindert doppelte Schrägstriche

    if (eingabe == "..") {
      changeToParentDirectory(currentDirectory);
      continue;
    }

    if (eingabe.length() == 0) {
      // Wenn nur ENTER gedrückt wurde, Verzeichnisse erneut auflisten
      Serial.println();
      Serial.print("Aktuelles Verzeichnis: ");
      Serial.println(currentDirectory);
      listFiles(LittleFS, currentDirectory.c_str(), 1);
      continue;
    }

    // Prüfen, ob der Pfad zu einer Datei oder einem Verzeichnis gehört
    if (LittleFS.exists(fullPath)) {
      File file = LittleFS.open(fullPath, "r");
      if (file.isDirectory()) {
        // Wenn es ein Verzeichnis ist, auflisten
        Serial.println("Verzeichnis existiert. Auflisten der Dateien:");
        currentDirectory = fullPath; // Wechsle zum angegebenen Verzeichnis
        currentDirectory = normalizePath(currentDirectory); // Bereinigt den Pfad
        Serial.println();
        Serial.print("Aktuelles Verzeichnis: ");
        Serial.println(currentDirectory);
        listFiles(LittleFS, fullPath.c_str(), 1);
      } else {
        // Wenn es eine Datei ist, löschen
        Serial.println("Datei gefunden. Lösche...");
        if (LittleFS.remove(fullPath)) {
          Serial.println("Datei erfolgreich gelöscht.");
          listFiles(LittleFS, currentDirectory.c_str(), 1); // Inhalt nach dem Löschen auflisten
        } else {
          Serial.println("Fehler beim Löschen der Datei.");
        }
      }
    } else {
      Serial.println("Die angegebene Datei oder das Verzeichnis existiert nicht.");
    }
    delay(500); // Nach der Verarbeitung warten
  }
}

void loop() {
  // Leere Schleife, da wir alles im setup erledigen
}

void listFiles(fs::FS &fs, const char *dirname, uint8_t levels) {
  File root = fs.open(dirname, "r");

  File file = root.openNextFile();

  if (!file) {
    Serial.println("Das Verzeichnis ist leer.");
    if (String(dirname) != "/") {  // Kein Wechsel im Root-Verzeichnis
      changeToParentDirectory(currentDirectory);  // Wechsel zum übergeordneten Verzeichnis
    }
  } else {
    Serial.println("\nInhalt:");
  }
  
  while (file) {
    if (file.isDirectory()) {
      Serial.print("Verzeichnis: ");
      Serial.println(file.name());
      if (levels > 1) {
        listFiles(fs, file.name(), levels - 1); // Rekursive Auflistung für Unterverzeichnisse
      }
    } else {
      Serial.print("Datei:       ");
      Serial.print(file.name());
      Serial.print("\tGrösse: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

// Bereinigt den Pfad und entfernt unnötige ".."
String normalizePath(String path) {
  if (path == "/") {
    return path;
  }

  String normalizedPath = "/";
  String part;
  int startIdx = 0;

  while (startIdx < path.length()) {
    int endIdx = path.indexOf('/', startIdx);
    if (endIdx == -1) {
      part = path.substring(startIdx);
      startIdx = path.length();
    } else {
      part = path.substring(startIdx, endIdx);
      startIdx = endIdx + 1;
    }

    if (part == "..") {
      int lastSlashIdx = normalizedPath.lastIndexOf('/');
      if (lastSlashIdx > 0) {
        normalizedPath = normalizedPath.substring(0, lastSlashIdx);
      }
    } else if (part != "." && part != "") {
      if (normalizedPath != "/") {
        normalizedPath += "/";
      }
      normalizedPath += part;
    }
  }

  return normalizedPath;
}

void changeToParentDirectory(String &currentDirectory) {
  if (currentDirectory == "/") { 
    // Überprüfen, ob wir uns im Root-Verzeichnis befinden
    Serial.println("Bereits im Root-Verzeichnis.");
    return;  // Kein Wechsel möglich
  }

  int lastSlash = currentDirectory.lastIndexOf('/');
  if (lastSlash > 0) {
    // Entfernt alles nach dem letzten Slash
    currentDirectory = currentDirectory.substring(0, lastSlash);
  } else {
    // Wenn wir uns z. B. in "/subdir" befinden
    currentDirectory = "/";
  }

  // Pfad normalisieren
  currentDirectory = normalizePath(currentDirectory);

  // Ausgabe des aktuellen Verzeichnisses
  Serial.println("Wechsel zu übergeordnetem Verzeichnis...");
  Serial.println();
  Serial.print("Aktuelles Verzeichnis: ");
  Serial.println(currentDirectory);

  // Inhalt des neuen aktuellen Verzeichnisses auflisten
  listFiles(LittleFS, currentDirectory.c_str(), 1);
}

