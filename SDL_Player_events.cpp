#include "SDL_Player.h"

void SDL_Player::poll_event()
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

        case SDLK_LEFT:
        {
          if(video_state.current_frame)
            video_state.current_frame--;
          if(TextureVector[video_state.current_frame])
          {
            int w,h;
            SDL_QueryTexture(TextureVector[video_state.current_frame],NULL,NULL,&w,&h);
            //printf("Displaying frame %d [%dx%d]\n", video_state.current_frame, w, h);
            display_texture(video_state.current_frame,event.motion.y > height - height/8);
          }
      
          break;
        }
        case SDLK_RIGHT:
        {
          video_state.current_frame++;
          if(video_state.current_frame==nb_frames)
            video_state.current_frame--;
          if(TextureVector[video_state.current_frame])
          {
            int w,h;
            SDL_QueryTexture(TextureVector[video_state.current_frame],NULL,NULL,&w,&h);
            //printf("Displaying frame %d [%dx%d]\n", video_state.current_frame, w, h);
            display_texture(video_state.current_frame,event.motion.y > height - height/8);
          }
      
          break;
        }
      }
      break;
    }

    case SDL_MOUSEWHEEL:
    {
      //printf("Wheel event: ");
      if(event.wheel.y > 0) // scroll up
      { 
          //printf("UP\n");
          video_state.current_frame++;
          if(video_state.current_frame==nb_frames)
            video_state.current_frame = nb_frames-1;;
      }
      else if(event.wheel.y < 0) // scroll down
      {
          //printf("DOWN\n");
          video_state.current_frame--;
          if(video_state.current_frame<0)
            video_state.current_frame = 0;
      }
      if(TextureVector[video_state.current_frame])
      {
        int w,h;
        SDL_QueryTexture(TextureVector[video_state.current_frame],NULL,NULL,&w,&h);
        //printf("Displaying frame %d [%dx%d]\n", video_state.current_frame, w, h);
        display_texture(video_state.current_frame,true);
      }
      break;
    }

    case SDL_MOUSEMOTION:
    {
      if(video_state.state != State::STOP)
      {
        if(lastx == event.motion.x) break;
        lastx=event.motion.x;
        video_state.current_frame = (unsigned)(((double)event.motion.x / width)*(double)TextureVector.size());
        if(TextureVector[video_state.current_frame])
        {
          int w,h;
          SDL_QueryTexture(TextureVector[video_state.current_frame],NULL,NULL,&w,&h);
          //printf("Displaying frame %d [%dx%d]\n", video_state.current_frame, w, h);
          display_texture(video_state.current_frame,event.motion.y > height - height/8);
        }
      }
      break;
    }
    default:
      break;
    }
}