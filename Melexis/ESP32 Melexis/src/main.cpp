#include <Arduino.h>
#include <MLX90621_API.h>

void setup() {
    Serial.begin(115200);
}

void loop() {
    float Ta;
    static uint8_t eeMLX90621[256]; 
    static uint16_t mlx90621Frame[66]; 
    paramsMLX90621 mlx90621;
    int status;
    status = MLX90621_DumpEE(eeMLX90621);
    status = MLX90621_ExtractParameters(eeMLX90621,&mlx90621);
    status = MLX90621_GetFrameData(mlx90621Frame);  
    Ta = MLX90621_GetTa(mlx90621Frame, &mlx90621);

    Serial.println(status);

    delay(1000);
}