#include <SDL.h>
#include <vector>

extern "C"
{
  #include <libavformat/avformat.h>
  #include <libavcodec/avcodec.h>
  #include <libswscale/swscale.h>
}

class SDLScreen
{
public:
  SDLScreen(const char*, int,int);
  ~SDLScreen();

  std::vector<SDL_Texture*> TextureVector;
  std::vector<SDL_Texture*> TextureminiVector;

  void display_frame(AVFrame* frame,int w,int h);
  void display_texture(unsigned i);
  void display_texturemini(unsigned i);

  void save_frame_into_texture(AVFrame* frame,int w,int h);
  void save_frame_into_texturemini(AVFrame* frame,int w,int h);
  void poll_event();


  void quit_all();

private:
  int width;
  int height;
  SDL_Event event;
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  //SDL_Texture* texturemini;

};
