#include <SDL.h>
#include <vector>
#include <string>
#include <chrono>
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
    DISPLAYING,
    PAUSED,
    PLAYING
  };

  SDL_Player(const char*, AVStream* stream, int,int);
  ~SDL_Player();

  std::vector<SDL_Texture*> TextureVector;

  //void get_frametextures_from_file(std::string filename);

  void display_avframe(AVFrame* frame);

  SDL_Texture* get_texture_from_frame(AVFrame* frame);

  void save_frame_into_texture(AVFrame* frame);
  void set_nb_frames(int nb);
  int get_nb_frames();


  // Player basics
  void play();
  void play_pause();
  void stop();


  // Event related
  void poll_event();
  void quit_all();

  struct vs
  {
    int current_frame = 0;
    std::chrono::time_point<std::chrono::system_clock> time_last_frame;
    State state;
    AVRational avg_frame_rate;
    AVRational speed_factor{1,1};
  } video_state;

private:
  void display_texture(int idx);
  void display_texture_mini(int idx);
  int width;
  int height;
  int nb_frames;
  SDL_Event event;
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  //SDL_Texture* texturemini;

  void update_last_frame_time();
  uint64_t elapsed_since_last_frame();
  uint64_t get_frame_period(AVRational rate, AVRational sf) const;
  void increase_sf();
  void decrease_sf();
};
