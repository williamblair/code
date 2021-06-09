# assumes SDL2 is installed globally (sudo apt install libsdl2-dev)
# doesn't use the included ../External/SDL folder
# tested with
#   gcc 9.3.0
#   Ubuntu 20.04.2 LTS
CC = g++
TARGET = main
CFLAGS = `pkg-config --cflags sdl2`
BUILDDIR = ./build
OBJS = $(BUILDDIR)/Main.o \
       $(BUILDDIR)/Game.o \
       $(BUILDDIR)/Actor.o \
       $(BUILDDIR)/AnimSpriteComponent.o \
       $(BUILDDIR)/BGSpriteComponent.o \
       $(BUILDDIR)/Component.o \
       $(BUILDDIR)/Math.o \
       $(BUILDDIR)/Ship.o \
       $(BUILDDIR)/SpriteComponent.o \
       $(BUILDDIR)/Character.o \
       $(BUILDDIR)/TileMapComponent.o \
       $(BUILDDIR)/TileMap.o
LIBDIRS = 
INCDIRS =
LIBS = `pkg-config --libs sdl2` \
       `pkg-config --libs SDL2_image`

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBDIRS) $(LIBS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: %.cpp $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCDIRS)

run:
	./$(TARGET)

clean:
	rm -rf $(BUILDDIR) $(TARGET)

