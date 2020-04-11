#include "swscaler.h"
#include <vector>
#include <iomanip>


SWScalerYUV420::~SWScalerYUV420()
{

}

SWScalerYUV420::SWScalerYUV420(const VideoStream *stream, int _dest_width, int _dest_height):
    dest_width(_dest_width),
    dest_height(_dest_height)
{
  if( !dest_width && !dest_height)
  {
    // DONT scale, just set pixel format
    dest_width = stream->get_codec_context()->width;
    dest_height = stream->get_codec_context()->height;
  }
  else if((!dest_width && dest_height) || (dest_width && !dest_height))
  {
    // keep porportions
    double prop = (double)stream->get_codec_context()->width / (double)stream->get_codec_context()->height;
    if(dest_width)
    {
      dest_height = (int) ((double)dest_width / prop);
    }
    else
    {
      dest_width = (int) ((double)dest_height * prop);
    }
  }



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

AVFrame* SWScalerYUV420::scale(AVFrame* src_frame)
{
  AVFrame* scaled = av_frame_alloc();

  yPlane = (Uint8*)malloc(dest_width * dest_height);
  uPlane = (Uint8*)malloc(dest_width * dest_height / 4);
  vPlane = (Uint8*)malloc(dest_width * dest_height / 4);
  yPlaneSz = dest_width * dest_height;
  uvPlaneSz = dest_width * dest_height / 4;
  uvPitch = dest_width/2;

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
    src_frame->height,
    scaled->data,
    scaled->linesize
  );

  scaled->pts = src_frame->pts;
  scaled->width = dest_width;
  scaled->height = dest_height;

  return scaled;
}
