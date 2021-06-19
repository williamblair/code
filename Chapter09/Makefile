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
       $(BUILDDIR)/AudioComponent.o \
       $(BUILDDIR)/AudioSystem.o \
       $(BUILDDIR)/CameraComponent.o \
       $(BUILDDIR)/Component.o \
       $(BUILDDIR)/CircleComponent.o \
       $(BUILDDIR)/FollowActor.o \
       $(BUILDDIR)/FollowCamera.o \
       $(BUILDDIR)/FPSActor.o \
       $(BUILDDIR)/FPSCamera.o \
       $(BUILDDIR)/Math.o \
       $(BUILDDIR)/Mesh.o \
       $(BUILDDIR)/MeshComponent.o \
       $(BUILDDIR)/MoveComponent.o \
       $(BUILDDIR)/OrbitActor.o \
       $(BUILDDIR)/OrbitCamera.o \
       $(BUILDDIR)/PlaneActor.o \
       $(BUILDDIR)/Renderer.o \
       $(BUILDDIR)/Shader.o \
       $(BUILDDIR)/SoundEvent.o \
       $(BUILDDIR)/SplineActor.o \
       $(BUILDDIR)/SplineCamera.o \
       $(BUILDDIR)/SpriteComponent.o \
       $(BUILDDIR)/Texture.o \
       $(BUILDDIR)/VertexArray.o
LIBDIRS = 
INCDIRS =
LIBS = `pkg-config --libs sdl2` \
       `pkg-config --libs SDL2_image` \
       `pkg-config --libs glew` \
       -lSOIL -lfmod -lfmodstudio

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

