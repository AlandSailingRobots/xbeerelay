#####################################################
#                                                   #
#    Aland Sailing Robot                            #
#    ===========================================    #
#    xBeeRelay                                		#
#    -------------------------------------------    #
#                                                   #
#####################################################


BUILD_DR 		= ../build/XbeeRemote

TARGET			=./XbeeRemote.run

SRC 			= main.cpp udpclient.cpp XbeeRemote.cpp

OBJECTS 		= $(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.o))
OBJECTS 		+= ../build/Network/DataLink.o ../build/Network/LinuxSerialDataLink.o ../build/Network/XbeePacketNetwork.o ../build/utility/SysClock.o ../build/SystemServices/Logger.o ../build/Messages/MessageSerialiser.o ../build/Messages/MessageDeserialiser.o

INCLUDE			= -I../


WIRING_PI = libwiringPi.so

all : $(TARGET)
	 
$(TARGET): $(OBJECTS)
	@mv ../$(WIRING_PI) $(WIRING_PI)
	@$(CXX) $(CPPFLAGS) $(OBJECTS) -Wl,-rpath=./,-rpath=../ $(WIRING_PI) -o $(TARGET) $(LIBS) -lrt
	@echo Built $(TARGET)
	 
# Compile CPP files into the build folder
$(BUILD_DIR)/%.o:%.cpp
	@mkdir -p $(dir $@)
	@echo Compiling CPP File: $@
	$(CXX) -c $(CPPFLAGS) $(INC) $(INCLUDE) -o ./$@ $< -DTOOLCHAIN=$(TOOLCHAIN) $(LIBS)

clean :
	rm -f ./*.o
	rm xbeerelay
