OUTPUT=output
GENERATED=generated
# runtime is where you downloaded and extracted https://www.antlr.org/download/antlr4-cpp-runtime-4.8-macos.zip
RUNTIME=/usr/local
ANTLRJARDIR=/home/anton/ANTLR-VER
CCARGS=-c -I $(RUNTIME)/include/antlr4-runtime/ -I $(GENERATED) -I .  -std=c++17 -g
LDFLAGS := $(LDFLAGS) -g  -lpthread -lrt
LIBS=$(RUNTIME)/lib/libantlr4-runtime.a
JAVA=/usr/bin/java
CC=g++
GRAMMAR=dynadl
# This assumes you have antlr-4.8-complete.jar in the current directory.
#ANTLR4=$(JAVA) -jar $(ANTLRJARDIR)/antlr-4.11.1-complete.jar
ANTLR4=$(JAVA) -jar $(ANTLRJARDIR)/antlr-4.9.3-complete.jar
#ANTLR4=antlr4

ANTLRGEN=BaseListener Lexer Listener Parser 
OBJS=$(addsuffix .o,$(addprefix $(OUTPUT)/$(GRAMMAR),$(ANTLRGEN)))
GSOURCES=$(addsuffix .cpp,$(addprefix $(GENERATED)/$(GRAMMAR),$(ANTLRGEN)))

.precious: $(GSOURCES)

all: dynadl 

dynadl: dirs antlr4 dynadl.cpp dynadlWalkListener.cpp dynadlDataManager.cpp $(OBJS)
	$(CC) $(CCARGS) dynadl.cpp  -o $(OUTPUT)/dynadl.o 
	$(CC) $(CCARGS) dynadlWalkListener.cpp  -o $(OUTPUT)/dynadlWalkListener.o 
	$(CC) $(CCARGS) dynadlDataManager.cpp  -o $(OUTPUT)/dynadlDataManager.o
	$(CC) $(OUTPUT)/dynadl.o $(OUTPUT)/dynadlWalkListener.o $(OUTPUT)/dynadlDataManager.o $(OBJS) $(LIBS) $(LDFLAGS) -o dynadl 

antlr4: $(GENERATED)/.generated;
 
$(GENERATED)/.generated: $(GRAMMAR).g4
	$(ANTLR4) -Dlanguage=Cpp -o $(GENERATED) $(GRAMMAR).g4
	@touch $(GENERATED)/.generated

$(OUTPUT)/%.o : $(GENERATED)/%.cpp
	$(CC) $(CCARGS) $< -o $@

$(GENERATED)/%.cpp: $(GENERATED)/.generated;

dirs:; mkdir -p $(OUTPUT) $(GENERATED) 
clean:; rm -rf dynadl Debug $(OUTPUT) $(GENERATED) *.msg *.action *.srv *.cfg *.sta *.log package* *.bin

