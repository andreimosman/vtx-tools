

CXX=g++
CXXFLAGS=-g3
INCLUDES=-I . -I /usr/local/include 


INSTALL_INC_PATH=/usr/include/mosman
INSTALL_LIB_PATH=/usr/lib
LIBS=-L . -L /usr/local/lib -l mframework -lpqxx -lpq -lboost_regex

TARGET=libmframework.a teste vtx-spool
OBJLIB=aplicativo.o config.o rede.o
OBJETOS=appteste.o appspool.o atuador_bandalarga.o atuador.o



vtx-spool_OBJ=appspool.o atuador_bandalarga.o

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<
.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<
.cpp:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(OBJETOS) $(LIBS)

all:  $(TARGET) $(OBJETOS) $(OBJLIB)

libmframework.a: $(OBJLIB)
	ar -cr libmframework.a $(OBJLIB)

teste: teste.cpp $(OBJETOS) libmframework.a

vtx-spool: vtx-spool.cpp $(vtx-spool_OBJ)

clean:
	rm -f $(TARGET) $(OBJETOS) $(OBJLIB)

apptest: teste
	./teste





install: libmframework.a
	install -d $(INSTALL_INC_PATH)
	install -d $(INSTALL_LIB_PATH)
	install config.h $(INSTALL_INC_PATH)/
	install aplicativo.h $(INSTALL_INC_PATH)/
	install libmframework.a $(INSTALL_LIB_PATH)/
