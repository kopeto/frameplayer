#include "sdlscreen.h"

SDLScreen::~SDLScreen()
{
  quit_all();
}

SDLScreen::SDLScreen(const char* window_name, int w, int h):
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

  texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_YV12,
    SDL_TEXTUREACCESS_STREAMING,
    width,height
  );
}

void SDLScreen::display_frame(AVFrame* frame,int w,int h)
{
  //TODO: locl/unlock texture durin update
  SDL_UpdateYUVTexture(
    texture,
    NULL,
    frame->data[0], //yPlane,
    w,
    frame->data[1], //uPlane,
    frame->linesize[2], // uvPitch,
    frame->data[2], //vPlane,
    frame->linesize[1] //uvPitch
  );
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void SDLScreen::save_frame_into_texture(AVFrame* frame,int w,int h)
{
  TextureVector.push_back(
    SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_YV12,
      SDL_TEXTUREACCESS_STREAMING,
      width,height
    )
  );

  SDL_UpdateYUVTexture(
      TextureVector.back(),
      NULL,
      frame->data[0], //yPlane,
      w,
      frame->data[1], //uPlane,
      frame->linesize[2], // uvPitch,
      frame->data[2], //vPlane,
      frame->linesize[1] //uvPitch
  );
}

void SDLScreen::display_texture(unsigned i)
{
  if(i<TextureVector.size())
  {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, TextureVector[i], NULL, NULL);
    SDL_RenderPresent(renderer);  }
}

void SDLScreen::poll_event()
{
  SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
    {
      SDL_DestroyTexture(texture);
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      SDL_Quit();
      exit(0);
      break;
    }
    case SDL_MOUSEBUTTONDOWN: // if the event is mouse click
    {
      // printf("%d %d\n",event.button.x, event.button.y);
      break;
    }
    case SDL_MOUSEMOTION:
    {
      auto idx = (unsigned)(((double)event.motion.x / width)*(double)TextureVector.size());
      //printf("%d\n",idx);
      display_texture(idx);
      break;
    }
    default:
      break;
    }
  SDL_Delay(5);
}

void SDLScreen::quit_all()
{
  for(auto p: TextureVector)
    SDL_DestroyTexture(p);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(0);
}
