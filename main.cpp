#include "stream.h"
#include "packet.h"
#include "format.h"
#include "codec.h"
#include "swscaler.h"
#include "SDL_Player.h"

#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>

#include "utils.h"
#include "timer.h"

// const int WIDTH = 800;
// const int HEIGHT = 600;

const char* filename = "ander.mp4";
const int WIDTH = 960;
const int HEIGHT = 540;

std::mutex texture_mutex;


void thread_decode(const char* filename, std::vector<AVFrame*>& frame_list, int seek_frame, int part_frames );

int main(int argc, char** argv)
{
  const char *input;
  if(argc<2)
    input=filename;
  else
    input = argv[1];

  Format format(input);
  VideoStream* stream = format.best_video_stream();
  stream->open_codec(NULL);
  SWScalerYUV420 my_scaler(stream,WIDTH,HEIGHT);

  SDL_Player player("My Screen", stream->get_stream(),WIDTH,HEIGHT);

  int nb_frames = stream->nb_frames();
  int nb_cores = std::thread::hardware_concurrency();

  int part_frames = nb_frames / nb_cores;
  if(nb_frames % nb_cores)
    part_frames ++;  

  { Timer t;

  std::vector<std::thread> threads(nb_cores);
  std::vector<std::vector<AVFrame*>> frames_vvector(nb_cores);

  for(int i=0; i<nb_cores; ++i)
  {
    std::thread t{thread_decode, input, std::ref(frames_vvector[i]), i*part_frames, part_frames};
    threads[i] = std::move(t);
  }
  for(int i=0; i<nb_cores; ++i)
  {
    threads[i].join();
  }

  for(int i=0; i<nb_cores; ++i)
  {
    for(AVFrame* frame: frames_vvector[i])
    {
      if(!player.TextureVector[frame->pts])
        player.save_frame_into_texture(frame);
      av_frame_free(&frame);
    }
  }


  } // TIMER

  // Check if missing frames
  for(int i=0; i<nb_frames; ++i)
  {
    if(!player.TextureVector[i])
      printf("Frame %u missing\n",i);
  }

  player.play();

  while(1)
  { 
    player.poll_event();
  }

  //END.
}


void thread_decode(const char* filename, std::vector<AVFrame*>& frame_list, int seek_frame, int part_frames )
{
  Format format(filename);
  VideoStream* stream = format.best_video_stream();
  stream->open_codec(NULL);
  SWScalerYUV420 my_scaler(stream,WIDTH,HEIGHT);

  auto *packet = av_packet_alloc();
  auto *frame = av_frame_alloc();

  avformat_seek_file(format.get_context(), stream->get_stream()->index, 0, seek_frame, seek_frame, 0);

  while (av_read_frame(format.get_context(), packet) >= 0) {
    
    if(packet->stream_index!=stream->get_stream()->index)
    {
      continue;
    }
    // DECODE:
    avcodec_send_packet(stream->get_codec_context(), packet);
    avcodec_receive_frame(stream->get_codec_context(), frame);
    if(av_get_picture_type_char(frame->pict_type)=='?')
    {
      continue;
    }
    AVFrame* scaled = my_scaler.scale(frame);
    frame_list.push_back(scaled);    

    if(frame->pts >= seek_frame+part_frames)
      break;
  }

  av_packet_unref(packet);
  //av_frame_free(&frame);
}