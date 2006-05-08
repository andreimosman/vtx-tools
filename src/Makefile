

CXX=g++
CXXFLAGS=-g3 
#CXXFLAGS=-O3 -fno-enforce-eh-specs
INCLUDES=-I . -I /usr/local/include 


INSTALL_INC_PATH=/usr/include/mosman
INSTALL_LIB_PATH=/usr/lib

INSTALL_VIRTEX=/mosman/virtex
INSTALL_VIRTEX_ETC=$(INSTALL_VIRTEX)/etc
INSTALL_VIRTEX_BIN=$(INSTALL_VIRTEX)/bin


LIBS=-L . -L /usr/local/lib -l mframework -lpqxx -lpq -lboost_regex 

TARGET=libmframework.a vtx-spool vtx-stats vtx-radius vtx-pppoe
#teste 
OBJLIB=aplicativo.o config.o rede.o so_freebsd.o sistema_operacional.o
OBJETOS=apppppoe.o appradius.o appstats.o appteste.o appspool.o atuador_bandalarga.o atuador_redesuporte.o atuador.o 

vtx-spool_OBJ=appspool.o atuador_bandalarga.o atuador_redesuporte.o atuador.o
vtx-stats_OBJ=appstats.o 
vtx-radius_OBJ=appradius.o 
vtx-pppoe_OBJ=apppppoe.o so_freebsd.o sistema_operacional.o

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<
.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<
.cpp:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $? $(LIBS)
#	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(OBJETOS) $(LIBS)

all:  $(TARGET) $(OBJETOS) $(OBJLIB)

libmframework.a: $(OBJLIB)
	ar -cr libmframework.a $(OBJLIB)

teste: teste.cpp $(OBJETOS) libmframework.a

vtx-spool: vtx-spool.cpp $(vtx-spool_OBJ)
vtx-stats: vtx-stats.cpp $(vtx-stats_OBJ)
vtx-radius: vtx-radius.cpp $(vtx-radius_OBJ)
vtx-pppoe: vtx-pppoe.cpp $(vtx-pppoe_OBJ)

clean:
	rm -f $(TARGET) $(OBJETOS) $(OBJLIB)

apptest: teste
	./teste



install:
	install -d $(INSTALL_VIRTEX_BIN)
	install vtx-spool $(INSTALL_VIRTEX_BIN)
	install vtx-radius $(INSTALL_VIRTEX_BIN)
	
install-config:
	install -d $(INSTALL_VIRTEX_ETC)
	install virtex.ini $(INSTALL_VIRTEX_ETC)

lib-install: libmframework.a
	install -d $(INSTALL_INC_PATH)
	install -d $(INSTALL_LIB_PATH)
	install config.h $(INSTALL_INC_PATH)/
	install aplicativo.h $(INSTALL_INC_PATH)/
	install libmframework.a $(INSTALL_LIB_PATH)/
