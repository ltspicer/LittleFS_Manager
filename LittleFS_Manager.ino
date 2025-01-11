// LittleFS Manager //
//   Version 2.0    //
// by D. Luginbuehl //
//    (C) 2025      //

#include <LittleFS.h>

String language = "de";
String currentDirectory = "/"; // Der aktuelle Pfad

void setup() {
  Serial.begin(9600);
  delay(5000);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS konnte nicht initialisiert werden.");
    return;
  }

  Serial.println();
  Serial.println(language == "de" ? "LittleFS initialisiert" : "LittleFS initialized");

  help();

  Serial.print(language == "de" ? "Aktuelles Verzeichnis: " : "Current directory: ");

  Serial.println(currentDirectory); // Ausgabe des neuen Pfads

  // Verzeichnisse und Dateien auflisten
  listFiles(LittleFS, "/", 1);

  // Endlosschleife für Benutzerinteraktion
  while (true) {
    if (language == "de") {
      Serial.println("\nGib den Dateinamen zum Löschen oder einen Verzeichnisnamen zum Auflisten ein (ENTER für erneutes Auflisten):");
    } else {
      Serial.println("\nEnter the file name to delete or a directory name to list (ENTER to list again):");
    }

    while (Serial.available() == 0) {
      // Warten auf Eingabe
    }

    String eingabe = Serial.readString();
    eingabe.trim(); // Entfernt führende und nachfolgende Leerzeichen
    Serial.println(language == "de" ? "Eingabe: " : "Entered: ");
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
      Serial.print(language == "de" ? "Aktuelles Verzeichnis: " : "Current directory: ");
      Serial.println(currentDirectory);
      listFiles(LittleFS, currentDirectory.c_str(), 1);
      continue;
    }

    if (eingabe.startsWith("-language ")) {
      String params = eingabe.substring(10); // Entfernt "-language " aus der Eingabe
      if (params == "de") {
        language = "de";
        Serial.println("Deutsche Sprache eingestellt");
      }
      if (params == "en") {
        language = "en";
        Serial.println("Set to English language");
      }
      continue;
    }

    if (eingabe.startsWith("-help")) {
      help();
      continue;
    }

    // Benutzer gibt "rename <alter Name> <neuer Name>" ein
    if (eingabe.startsWith("-rename ")) {
      String params = eingabe.substring(8); // Entfernt "-rename " aus der Eingabe
      int spaceIndex = params.indexOf(' ');
      if (spaceIndex == -1) {
        if (language == "de") {
          Serial.println("Ungültige Eingabe. Verwende: rename <alter Name> <neuer Name>");
        } else {
          Serial.println("Invalid input. Use: rename <old name> <new name>");
        }
        continue;
      }
    
      String oldName = params.substring(0, spaceIndex);
      String newName = params.substring(spaceIndex + 1);
    
      // Vollständige Pfade generieren
      String fullOldPath = currentDirectory + (currentDirectory.endsWith("/") ? "" : "/") + oldName;
      String fullNewPath = currentDirectory + (currentDirectory.endsWith("/") ? "" : "/") + newName;
    
      renameFile(LittleFS, fullOldPath.c_str(), fullNewPath.c_str());
      continue;
    }

    // Prüfen, ob der Pfad zu einer Datei oder einem Verzeichnis gehört
    if (LittleFS.exists(fullPath)) {
      File file = LittleFS.open(fullPath, "r");
      if (file.isDirectory()) {
        // Wenn es ein Verzeichnis ist, auflisten
        Serial.println(language == "de" ? "Verzeichnis existiert. Auflisten der Dateien:" : "Directory exists. List the files:");
        currentDirectory = fullPath; // Wechsle zum angegebenen Verzeichnis
        currentDirectory = normalizePath(currentDirectory); // Bereinigt den Pfad
        Serial.println();
        Serial.print(language == "de" ? "Aktuelles Verzeichnis: " : "Current directory: ");
        Serial.println(currentDirectory);
        listFiles(LittleFS, fullPath.c_str(), 1);
      } else {
        // Wenn es eine Datei ist, löschen
        Serial.println(language == "de" ? "Datei gefunden. Lösche..." : "File found. Delete...");
        if (LittleFS.remove(fullPath)) {
          Serial.println(language == "de" ? "Datei erfolgreich gelöscht." : "File successfully deleted.");
          listFiles(LittleFS, currentDirectory.c_str(), 1); // Inhalt nach dem Löschen auflisten
        } else {
          Serial.println(language == "de" ? "Fehler beim Löschen der Datei." : "Error deleting the file.");
        }
      }
    } else {
      if (language == "de") {
        Serial.println("Die angegebene Datei oder das Verzeichnis existiert nicht.");
      } else {
        Serial.println("The specified file or directory does not exist.");
      }
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
    Serial.println(language == "de" ? "Das Verzeichnis ist leer." : "The directory is empty.");
    if (String(dirname) != "/") {  // Kein Wechsel im Root-Verzeichnis
      changeToParentDirectory(currentDirectory);  // Wechsel zum übergeordneten Verzeichnis
    }
  } else {
    Serial.println(language == "de" ? "\nInhalt:" : "\nContent:");
  }
  
  while (file) {
    if (file.isDirectory()) {
      Serial.print(language == "de" ? "Verzeichnis: " : "Directory: ");
      Serial.println(file.name());
      if (levels > 1) {
        listFiles(fs, file.name(), levels - 1); // Rekursive Auflistung für Unterverzeichnisse
      }
    } else {
      Serial.print(language == "de" ? "Datei:       " : "File:      ");
      Serial.print(file.name());
      Serial.print(language == "de" ? "\t\tGrösse: " : "\t\tSize: ");
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
    Serial.println(language == "de" ? "Bereits im Root-Verzeichnis." : "Already in the root directory.");
    return;
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
  Serial.println(language == "de" ? "Wechsel zu übergeordnetem Verzeichnis..." : "Change to parent directory...");
  Serial.println();
  Serial.print(language == "de" ? "Aktuelles Verzeichnis: " : "Current directory: ");
  Serial.println(currentDirectory);

  // Inhalt des aktuellen Verzeichnisses auflisten
  listFiles(LittleFS, currentDirectory.c_str(), 1);
}

void renameFile(fs::FS &fs, const char *oldName, const char *newName) {
  if (!fs.exists(oldName)) {
    Serial.println(language == "de" ? "Die Datei oder das Verzeichnis existiert nicht." : "The file or directory does not exist.");
    return;
  }

  if (fs.exists(newName)) {
    Serial.println(language == "de" ? "Der neue Name existiert bereits. Umbenennen nicht möglich." : "The new name already exists. Renaming is not possible.");
    return;
  }

  Serial.print(language == "de" ? "Umbenennen von " : "Rename ");
  Serial.print(oldName);
  Serial.print(language == "de" ? " nach " : " to ");
  Serial.println(newName);

  if (fs.rename(oldName, newName)) {
    Serial.println(language == "de" ? "Erfolgreich umbenannt." : "Successfully renamed.");
  } else {
    Serial.println(language == "de" ? "Fehler beim Umbenennen." : "Renaming error.");
  }
  // Ausgabe des aktuellen Verzeichnisses
  Serial.println();
  Serial.print(language == "de" ? "Aktuelles Verzeichnis: " : "Current directory: ");
  Serial.println(currentDirectory);
  // Inhalt des aktuellen Verzeichnisses auflisten
  listFiles(LittleFS, currentDirectory.c_str(), 1);
}

void help() {
    Serial.println();
  if (language == "de") {
    Serial.println("-help zeigt die Hilfe an");
    Serial.println("-language en       set language to english");
    Serial.println("-rename <alter Name> <neuer Name>");
    Serial.println("ENTER zeigt den Inhalt des LittleFS");
    Serial.println("Eingabe eines Dateinamens löscht diese Datei");
    Serial.println("Eingabe eines Verzeichnisnamens wechselt in dieses Verzeichnis");
    Serial.println("Eingabe von .. wechselt in das übergeordnete Verzeichnis");
    Serial.println("Wenn ein Verzeichnis leer ist, wird dieses gelöscht!");
  } else {
    Serial.println("-help for help");
    Serial.println("-language de       setzt Sprache auf deutsch");
    Serial.println("-rename <old name> <new name>");
    Serial.println("ENTER shows the content of the LittleFS");
    Serial.println("Entering a filename deletes this file");
    Serial.println("Entering a directory name changes to this directory");
    Serial.println("Entering .. changes to the parent directory");
    Serial.println("If a directory is empty, it will be deleted!");
  }
Serial.println();
}

