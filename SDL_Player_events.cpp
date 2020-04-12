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
      int x,y;
      SDL_GetMouseState(&x,&y);
      if(y > height - height/8)
      {
        video_state.current_frame = (int)(((double)event.motion.x / width)*(double)nb_frames);
        display_texture(video_state.current_frame);
        update_last_frame_time();
        display_texture_mini(video_state.current_frame);
      }
      break;
    }
    case SDL_KEYDOWN:
    {
      switch(event.key.keysym.sym)
      {
        case SDLK_q:
        case SDLK_ESCAPE:
        {
          quit_all();
          break;
        }

        case SDLK_SPACE:
        case SDLK_p:
        {
          play_pause();
          break;
        }

        case SDLK_LEFT:
        {
          video_state.current_frame--;
          if(video_state.current_frame<0)
            video_state.current_frame = 0;
          if(TextureVector[video_state.current_frame])
          {
            display_texture(video_state.current_frame);
            display_texture_mini(video_state.current_frame);
            //update_last_frame_time();
          }
      
          break;
        }
        case SDLK_RIGHT:
        {
          video_state.current_frame++;
          if(video_state.current_frame>=nb_frames)
            {
              video_state.current_frame--;
              break;
            }
          if(TextureVector[video_state.current_frame])
          {
            display_texture(video_state.current_frame);
            display_texture_mini(video_state.current_frame);
            //update_last_frame_time();
          }
      
          break;
        }
        case SDLK_COMMA:
        {
          decrease_sf();
          break;
        }
        case SDLK_PERIOD:
        {
          increase_sf();
          break;
        }
        
      }
      break;
    }

    case SDL_MOUSEWHEEL:
    {
      // //printf("Wheel event: ");
      // if(event.wheel.y > 0) // scroll up
      // { 
      //     //printf("UP\n");
      //     video_state.current_frame++;
      //     if(video_state.current_frame==nb_frames)
      //       video_state.current_frame = nb_frames-1;;
      // }
      // else if(event.wheel.y < 0) // scroll down
      // {
      //     //printf("DOWN\n");
      //     video_state.current_frame--;
      //     if(video_state.current_frame<0)
      //       video_state.current_frame = 0;
      // }
      // if(TextureVector[video_state.current_frame])
      // {
      //   display_texture(video_state.current_frame);
      //   display_texture_mini(video_state.current_frame);
      //   //update_last_frame_time();
      // }
      break;
    }

    case SDL_MOUSEMOTION:
    {
      if(video_state.state != State::STOP)
      {
        if(lastx == event.motion.x) break;
        lastx=event.motion.x;
        int i = (int)(((double)event.motion.x / width)*(double)nb_frames);
        if(TextureVector[i] && event.motion.y > height - height/8)
        {
          display_texture(video_state.current_frame);
          display_texture_mini(i);
          //update_last_frame_time();
        }
      break;
    }
    default:
      break;
    }
  }

  if(video_state.state == State::PLAYING)
    {
      if(elapsed_since_last_frame()>=get_frame_period(video_state.avg_frame_rate, video_state.speed_factor))
      {
        video_state.current_frame++;
        if(video_state.current_frame < nb_frames)
        {
          display_texture(video_state.current_frame);
          update_last_frame_time();
          if(event.motion.y > height - height/8)
            display_texture_mini((int)(((double)event.motion.x / width)*(double)nb_frames));
        }
        else
        {
          video_state.current_frame=nb_frames-1;
          video_state.state = State::PAUSED;
        }
      }
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