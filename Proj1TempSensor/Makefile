CC = g++
CFLAGS = -Wall -g
INCLUDES = -I.
SRCS = raspbPiTempSensor.cpp  
OBJS = raspbPiTempSensor.cpp

TEMPSENSOR = temperature-sensor
LDFLAGS = -ldl

all:	$(TEMPSENSOR)

$(TEMPSENSOR):$(OBJS) 
	$(CC)  -o $(TEMPSENSOR) $(CFLAGS) $(INCLUDES) $(OBJS) $(LDFLAGS) 

.c.o: 
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(TEMPSENSOR) 

