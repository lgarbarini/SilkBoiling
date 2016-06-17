## Variables      
+ step_ttg – Step Time To Go (TTG) (modular
arithmetic needed)        
+toggle_time
+ default_program (e.g. {fill, heat, boil, drain,
fill, rinse, drain, fill, rinse, drain, fill, rinse, drain, dry})        
+ array_length        
+ current_program
+ step_index
+ State Variable Enum 
  - 0 – no current operation
  - 1 – test 1
  - 2 – test 2
  - 3 – fill
  - 4 - heating
  - 5 – boiling
  - 6 – drain/squeeze
  - 7 – rinse
  - 8 – dry
  - 20 – OverTemp      
+ fill_time
+ heat_time     
+ drain_time
+ temp_limit

## Startup

1.  All Pins Low
2.  Flash Green LED with i2C ID
3.  Connect to i2C with Hello Message
  a.  Red LED on while transmitting
  b.  Transmit temperature & float

## Main  
+ Check Temp above limit If it is shutdown with error

### Test 1
+ Solenoids Open and Close every 5 seconds
+ Temp sent every 5 seconds over i2c
+ Runs for 30 sec

### Test 2
+ Heater on for 30 s
+ Requires override button

### Fill
+ Fill until fill switch or fill time is reached
+ If fill_time is reached, warning
+ If filled, close

### Heat

+ Start heater
+  If heat_time is reached, warning
+  If temp reached, state to boil

### Boil
+ Lower bucket
+ Agitate
+ 30 min timer

### Drain
+ Drain for drain time
+ Electromagnet
  - If float switch doesn’t change, warning
+ Rinse
+ 20 min timer

### Dry
+ Open Electromagnet
+ Heat to 40 C +-10C for 2 hours
+ stop

### Done
+ Wait until silk unloaded

 

 

 
