#include "swscaler.h"
#include <vector>
#include <iomanip>


SWScaler::~SWScaler()
{

}

SWScaler::SWScaler(const VideoStream *stream, int dest_width, int dest_height):
    src_height(stream->get_codec_context()->height),
    dest_width(dest_width),
    dest_height(dest_height),
    yPlane((Uint8*)malloc(dest_width * dest_height)),
    uPlane((Uint8*)malloc(dest_width * dest_height / 4)),
    vPlane((Uint8*)malloc(dest_width * dest_height / 4)),
    yPlaneSz(dest_width * dest_height),
    uvPlaneSz(dest_width * dest_height / 4),
    uvPitch(dest_width/2)
{
  context = sws_getContext(
   stream->get_codec_context()->width,
   stream->get_codec_context()->height,
   stream->get_codec_context()->pix_fmt,
   dest_width,   // Destination width
   dest_height,  // Destination height
   pix_format,  //YUV420 by default
   flags,      // BICUBIC by default
   NULL,
   NULL,
   NULL
 );
}

AVFrame* SWScaler::scale(AVFrame* src_frame)
{
  AVFrame* scaled = av_frame_alloc();
  scaled->data[0] = yPlane;
  scaled->data[1] = uPlane;
  scaled->data[2] = vPlane;
  scaled->linesize[0] = dest_width;
  scaled->linesize[1] = uvPitch;
  scaled->linesize[2] = uvPitch;

  sws_scale(context,
    src_frame->data,
    src_frame->linesize,
    0,
    src_height,
    scaled->data,
    scaled->linesize
  );

  scaled->pts = src_frame->pts;
  //scaled->duration = src_frame->duration;

  return scaled;
}
