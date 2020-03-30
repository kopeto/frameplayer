#include "stream.h"
#include "packet.h"
#include "format.h"
#include "codec.h"
#include "swscaler.h"
#include <iostream>
#include <iomanip>

//   save_gray_frame(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height, frame_filename);
void save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, char *filename)
{
    FILE *f;
    int i;
    f = fopen(filename,"w");
    // writing the minimal required header for a pgm file format
    // portable graymap format -> https://en.wikipedia.org/wiki/Netpbm_format#PGM_example
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

    // writing line by line
    for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}

void print_frame_info(AVFrame* pFrame, AVCodecContext* codecContext)
{
  printf(
    "Frame %c (%d) pts %ld(%.03f) dts %ld key_frame %d [coded_picture_number %d, display_picture_number %d]\n",
    av_get_picture_type_char(pFrame->pict_type),
    codecContext->frame_number,
    pFrame->pts,
    pFrame->pts/av_q2d(codecContext->time_base),
    pFrame->pkt_dts,
    pFrame->key_frame,
    pFrame->coded_picture_number,
    pFrame->display_picture_number
  );
}

void print_stream_info(BaseStream* mystream)
{
  std::cout<<"Dimensions: "<<mystream->get_codec_context()->width<<"x"<<mystream->get_codec_context()->height<<'\n';
  std::cout<<"Codec name: "<<mystream->get_codec()->long_name<<'\n';
  std::cout<<"NUmber of frames: "<<mystream->get_stream()->nb_frames<<'\n';
  std::cout<<"Frame rate(avg): "<<
    (double)mystream->get_stream()->avg_frame_rate.num/mystream->get_stream()->avg_frame_rate.den
    <<" fps\n";
  std::cout<<"Duration: "<<std::setprecision(2)<< std::fixed<<mystream->real_duration()<<" seconds"<<std::endl;
  std::cout<<"Bit rate(avg): "<<(double)mystream->get_codec_params()->bit_rate/1000.0<<"Kb/s\n";

}
