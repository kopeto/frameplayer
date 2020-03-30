#include "stream.h"
#include "packet.h"
#include "format.h"
#include "codec.h"
#include "swscaler.h"
#include "sdlscreen.h"

#include <iostream>
#include <iomanip>

#include "utils.h"
#include "timer.h"

// const int WIDTH = 800;
// const int HEIGHT = 600;

const char* filename = "in.mp4";

int main(int argc, char** argv)
{
  const char *input;
  if(argc<2)
    input=filename;
  else
    input = argv[1];

  Format format(input);
  av_dump_format(format.get_context(),0, format.get_filename(), 0);
  auto* mystream = format.best_video_stream();

  const int WIDTH = 960;
  const int HEIGHT = 540;

  SWScaler my_scaler(mystream,WIDTH,HEIGHT);
  SDLScreen my_screen("My Screen",WIDTH,HEIGHT);

  std::vector<AVFrame*> frames;

  if(mystream)
  {
    mystream->open_codec();

    print_stream_info(mystream);

    AVPacket *packet = av_packet_alloc();

    while (av_read_frame(format.get_context(), packet) >= 0) {
      if(packet->stream_index!=mystream->get_stream()->index)
      {
        continue;
      }
      // framelist[f_count] = av_frame_alloc();
      auto *frame = av_frame_alloc();

      avcodec_send_packet(mystream->get_codec_context(), packet);
      avcodec_receive_frame(mystream->get_codec_context(), frame);
      // if(frame->key_frame)

      if(av_get_picture_type_char(frame->pict_type)=='?')
      {
        printf("\"Wrong\" frame info:\n");
        print_frame_info(frame,mystream->get_codec_context());
        continue;
      }


      if(1)
      {

        auto *scaled = my_scaler.scale(frame);
        my_screen.display_frame(scaled,WIDTH,HEIGHT);
        my_screen.save_frame_into_texture(scaled,WIDTH,HEIGHT);
        av_frame_free(&scaled);

      }


      //frames.push_back(scaled);
      //print_frame_info(frame, mystream->get_codec_context());
      //framelist[f_count] =  my_scaler.scale(frame);

      // my_screen.poll_event();

      av_packet_unref(packet);
      av_frame_free(&frame);

    }
  }

  printf("%lu key frames recorded.\n",my_screen.TextureVector.size());

  while(my_screen.TextureVector.size()>0)
  {
    my_screen.poll_event();
  }
}
