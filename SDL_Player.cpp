#include "SDL_Player.h"

SDL_Player::~SDL_Player()
{
  quit_all();
}

SDL_Player::SDL_Player(const char* window_name, AVStream* stream, int w, int h):
  width(w), height(h)
{
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow(
    window_name,
    SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
    width,height,
    0
  );
  renderer = SDL_CreateRenderer(
    window,
    0,
    SDL_RENDERER_ACCELERATED
  );

  texture = NULL;
  video_state.state = State::STOP;
  video_state.current_frame = 0;
  video_state.avg_frame_rate = stream->avg_frame_rate;
  update_last_frame_time();

  nb_frames = stream->nb_frames;

  TextureVector = std::vector<SDL_Texture*>(nb_frames,NULL);

}

void SDL_Player::set_nb_frames(int nb)
{
  nb_frames = nb;
  TextureVector.reserve(nb);
}

int SDL_Player::get_nb_frames()
{
  return nb_frames;
}

void SDL_Player::display_avframe(AVFrame* frame)
{
  if(!frame)
    return;
  if(texture)
    SDL_DestroyTexture(texture);

  texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_YV12,
    SDL_TEXTUREACCESS_STREAMING,
    frame->width,
    frame->height
  );
  //TODO: locl/unlock texture durin update
  SDL_UpdateYUVTexture(
    texture,
    NULL,
    frame->data[0], //yPlane,
    frame->width,
    frame->data[1], //uPlane,
    frame->linesize[2], // uvPitch,
    frame->data[2], //vPlane,
    frame->linesize[1] //uvPitch
  );
  SDL_Rect dst{0,0,width,height};
  SDL_Rect src{0,0,frame->width,frame->height};
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, &src, &dst);
  SDL_RenderPresent(renderer);
}

SDL_Texture* SDL_Player::get_texture_from_frame(AVFrame* frame)
{
    SDL_Texture* texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_YV12,
    SDL_TEXTUREACCESS_STREAMING,
    frame->width,
    frame->height
  );
  //TODO: locl/unlock texture durin update
  SDL_UpdateYUVTexture(
    texture,
    NULL,
    frame->data[0], //yPlane,
    frame->width,
    frame->data[1], //uPlane,
    frame->linesize[2], // uvPitch,
    frame->data[2], //vPlane,
    frame->linesize[1] //uvPitch
  );

  return texture;
}

void SDL_Player::save_frame_into_texture(AVFrame* frame)
{
  int position = frame->pts;
  TextureVector[position] = 
    SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_YV12,
      SDL_TEXTUREACCESS_STREAMING,
      frame->width,
      frame->height
    ) ;

  SDL_UpdateYUVTexture(
      TextureVector[position],
      NULL,
      frame->data[0], //yPlane,
      frame->width,
      frame->data[1], //uPlane,
      frame->linesize[2], // uvPitch,
      frame->data[2], //vPlane,
      frame->linesize[1] //uvPitch
  );
}


void SDL_Player::display_texture(int i)
{
  if(i<(int)TextureVector.size() && TextureVector[i])
  {
    SDL_RenderClear(renderer);
    SDL_Rect dst{0,0,width,height};
    SDL_RenderCopy(renderer, TextureVector[i], NULL, &dst);
    SDL_RenderPresent(renderer);
  }
}

void SDL_Player::display_texture_mini(int i)
{
  if(i<(int)TextureVector.size() && TextureVector[i])
  {
    int x,y,w,h;
    w = width/8;
    h = height/8;
    x = (int) ((double)i / (double)TextureVector.size() * (double)width) - w/2;
    if(x<0) x = 0;
    if(x+w>width) x=width-w;
    y = height - h;

    SDL_Rect dest{x,y,w,h};
    SDL_RenderCopy(renderer, TextureVector[i], NULL, &dest);
    SDL_RenderPresent(renderer);

  }

}

void SDL_Player::play()
{
  video_state.state = State::PLAYING;
  update_last_frame_time();
}


void SDL_Player::play_pause()
{
  if(video_state.state == State::PLAYING)
  {
    video_state.state = State::PAUSED;
  }
  else if(video_state.state == State::PAUSED)
  {
    video_state.state = State::PLAYING;
    update_last_frame_time();
  }
  
}

void SDL_Player::update_last_frame_time()
{
  video_state.time_last_frame=std::chrono::system_clock::now();
}

uint64_t SDL_Player::elapsed_since_last_frame()
{
  return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now()-video_state.time_last_frame).count();
}

uint64_t SDL_Player::get_frame_period(AVRational rate, AVRational sf) const
{
  return rate.den * 1000000 / rate.num * sf.num / sf.den;
}

void SDL_Player::decrease_sf()
{
  if(video_state.speed_factor.den > 1)
    video_state.speed_factor.den /= 2;
  else
    video_state.speed_factor.num *= 2;
}

void SDL_Player::increase_sf()
{
  if(video_state.speed_factor.num > 1)
    video_state.speed_factor.num /= 2;
  else
    video_state.speed_factor.den *= 2;
}