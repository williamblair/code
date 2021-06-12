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
       $(BUILDDIR)/Board.o \
       $(BUILDDIR)/Component.o \
       $(BUILDDIR)/CircleComponent.o \
       $(BUILDDIR)/Math.o \
       $(BUILDDIR)/Random.o \
       $(BUILDDIR)/SpriteComponent.o \
       $(BUILDDIR)/MoveComponent.o \
       $(BUILDDIR)/AlphaBeta.o
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

