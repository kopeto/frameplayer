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


void SDL_Player::display_texture(unsigned i, bool mini)
{
  if(i<TextureVector.size())
  {
    SDL_RenderClear(renderer);
    SDL_Rect dst{0,0,width,height};
    SDL_RenderCopy(renderer, TextureVector[i], NULL, &dst);

    if(mini) // minitexture
    {
      //display_texturemini(unsigned i);
      // calc position
      int x,y,w,h;
      w = width/8;
      h = height/8;
      // SDL_QueryTexture(TextureminiVector[i],
      //   NULL, NULL,
      //   &w,&h
      // );

      x = (int) ((double)i / (double)TextureVector.size() * (double)width) - w/2;
      if(x<0) x = 0;
      if(x+w>width) x=width-w;
      y = height - h;

      SDL_Rect dest{x,y,w,h};
      SDL_RenderCopy(renderer, TextureVector[i], NULL, &dest);
    }

    SDL_RenderPresent(renderer);
  }
}


void SDL_Player::quit_all()
{
  for(auto p: TextureVector)
    SDL_DestroyTexture(p);
  if(texture)
    SDL_DestroyTexture(texture);
  if(renderer)
    SDL_DestroyRenderer(renderer);
  if(window)
    SDL_DestroyWindow(window);
  SDL_Quit();
  exit(0);
}
