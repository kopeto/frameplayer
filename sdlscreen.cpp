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

  // texture = SDL_CreateTexture(
  //   renderer,
  //   SDL_PIXELFORMAT_YV12,
  //   SDL_TEXTUREACCESS_STREAMING,
  //   width,height
  // );

  texture = NULL;
  state = State::STOP;
}

void SDLScreen::display_frame(AVFrame* frame,int w,int h)
{
  if(texture)
    SDL_DestroyTexture(texture);
  texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_YV12,
    SDL_TEXTUREACCESS_STREAMING,
    w,h
  );
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
  SDL_Rect dst{0,0,width,height};
  SDL_Rect src{0,0,w,h};
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, &src, &dst);
  SDL_RenderPresent(renderer);
}

void SDLScreen::save_frame_into_texture(AVFrame* frame,int w,int h)
{
  TextureVector.push_back(
    SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_YV12,
      SDL_TEXTUREACCESS_STREAMING,
      w,h
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


void SDLScreen::display_texture(unsigned i, bool mini)
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

void SDLScreen::save_frame_into_texturemini(AVFrame* frame,int w,int h)
{
  TextureminiVector.push_back(
    SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_YV12,
      SDL_TEXTUREACCESS_STREAMING,
      w,h
    )
  );

  SDL_UpdateYUVTexture(
    TextureminiVector.back(),
    NULL,
    frame->data[0], //yPlane,
    w,
    frame->data[1], //uPlane,
    frame->linesize[2], // uvPitch,
    frame->data[2], //vPlane,
    frame->linesize[1] //uvPitch
  );
}
void SDLScreen::display_texturemini(unsigned i)
{
  if(i<TextureminiVector.size())
  {
    // calc position
    int x,y,w,h;
    SDL_QueryTexture(TextureminiVector[i],
      NULL, NULL,
      &w,&h
    );

    x = (int) ((double)i / (double)TextureminiVector.size() * (double)width) - w/2;
    if(x<0) x = 0;
    if(x+w>width) x=width-w;
    y = height - h;

    SDL_Rect dest{x,y,w,h};
    //SDL_Rect src{0,0,w,h};

    //SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, TextureminiVector[i], NULL, &dest);
    SDL_RenderPresent(renderer);
  }
}


void SDLScreen::poll_event()
{
  static int lastx=-1;

  SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
    {
      quit_all();
      break;
    }
    case SDL_MOUSEBUTTONDOWN: // if the event is mouse click
    {
      break;
    }
    case SDL_KEYDOWN:
    {
      switch(event.key.keysym.sym)
      {
        case SDLK_q:
        case SDLK_ESCAPE:
        quit_all();
      }
      break;
    }
    case SDL_MOUSEMOTION:
    {
      if(state != State::STOP)
      {
        if(lastx == event.motion.x) break;
        lastx=event.motion.x;
        auto idx = (unsigned)(((double)event.motion.x / width)*(double)TextureVector.size());
        display_texture(idx,event.motion.y > height - height/8);
      }
      break;
    }
    default:
      break;
    }
}

void SDLScreen::quit_all()
{
  for(auto p: TextureVector)
    SDL_DestroyTexture(p);
  for(auto p: TextureminiVector)
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
