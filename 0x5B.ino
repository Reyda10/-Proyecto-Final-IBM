#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define NEW_ADDRESS 0x5B // Cambia esto por la nueva dirección (0x5B, 0x5C, etc.)

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);

  if (!mlx.begin()) {
    Serial.println("Sensor no encontrado. Verifica la conexión.");
    while (1);
  }
  
  // Cambiar dirección del sensor
  if (changeI2CAddress(NEW_ADDRESS)) {
    Serial.print("Dirección cambiada a: 0x");
    Serial.println(NEW_ADDRESS, HEX);
  } else {
    Serial.println("Error al cambiar la dirección.");
  }
}

void loop() {
  // No hay necesidad de hacer algo en loop después del cambio de dirección
}

// Función para cambiar la dirección del sensor
bool changeI2CAddress(uint8_t newAddress) {
  if (newAddress < 0x08 || newAddress > 0x77) {
    Serial.println("Dirección no válida.");
    return false;
  }

  uint16_t data = (newAddress << 1); // La dirección I2C es 7 bits, se almacena en 8 bits
  Wire.beginTransmission(0x5A);      // Dirección por defecto del sensor
  Wire.write(0x2E);                  // Comando para escribir en EEPROM
  Wire.write(lowByte(data));         // Byte menos significativo
  Wire.write(highByte(data));        // Byte más significativo
  uint8_t ack = Wire.endTransmission();

  delay(100); // Esperar para que se aplique el cambio

  if (ack == 0) {
    return true;
  } else {
    Serial.println("No se pudo escribir en la EEPROM.");
    return false;
  }
}