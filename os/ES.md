## freeRTOS

```c
void print_message(void *taskParameter){
    const char * msg = (const char*) taskParameter;
    
    while(1){
        puts(msg);
        vTaskDelay(1000);
    }
}

int main(void){
    xTaskCreate(print_message, "printTask", 1, "hello", PRIORITY_LOW, NULL);
    vTaskStartScheduler();
    return 0;
}
```


## Writing to registers

```c

int main(void){
    // Enable clock for ports A, B, C, D, E
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK |
    SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK );
    
    // Set pin function to GPIO
    PORTE->PCR[H2O_IN_PIN] = PORT_PCR_MUX(1);
    PORTC->PCR[LEVEL50_PIN] = PORT_PCR_MUX(1);
    
    // Set pin direction
    PTE->PDDR |= (1 << H2O_IN_PIN);
    
    // Sensors
    PTC->PDDR &= ~(1 << LEVEL50_PIN);
    
    turnOnWater();
    return 0;
}

void turnOnWater(){ // get to 50%
PTE->PSOR = (1 << H2O_IN_PIN);

while ((PTC->PDIR & (1 << LEVEL50_PIN)) == 0);
PTE->PCOR = (1 << H2O_IN_PIN);
}
```
