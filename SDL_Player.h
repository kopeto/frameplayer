#include <SDL.h>
#include <vector>
#include <string>

extern "C"
{
  #include <libavformat/avformat.h>
  #include <libavcodec/avcodec.h>
  #include <libswscale/swscale.h>
}

class SDL_Player
{
public:
  enum class State
  {
    STOP = 0,
    DISPLAYING
  };

  SDL_Player(const char*, AVStream* stream, int,int);
  ~SDL_Player();

  std::vector<SDL_Texture*> TextureVector;

  void get_frametextures_from_file(std::string filename);

  void display_avframe(AVFrame* frame);
  void display_texture(unsigned i, bool mini);

  SDL_Texture* get_texture_from_frame(AVFrame* frame);

  void save_frame_into_texture(AVFrame* frame);
  void set_nb_frames(int nb);
  int get_nb_frames();
  void poll_event();

  void quit_all();

  struct vs
  {
    int current_frame = 0;
    State state;
  } video_state;

private:
  void display_texture(int idx);

  int width;
  int height;
  int nb_frames;
  SDL_Event event;
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  //SDL_Texture* texturemini;

};
