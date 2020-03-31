#pragma once

extern "C"
{
  #include <libavformat/avformat.h>
  #include <libavcodec/avcodec.h>
  #include <libswscale/swscale.h>
}

#include "stream.h"
typedef unsigned char Uint8;

class SWScalerYUV420
{

public:
  SWScalerYUV420(const VideoStream *stream, int dest_width, int dest_height);
  ~SWScalerYUV420();
  SwsContext* get_context();
  AVFrame* scale(AVFrame* src_frame);

private:
  SwsContext* context;
  AVPixelFormat pix_format = AV_PIX_FMT_YUV420P;
  int flags = SWS_BICUBIC;
  int dest_width;
  int dest_height;

  // Pixel formatting variables
  Uint8 *yPlane, *uPlane, *vPlane;
  size_t yPlaneSz, uvPlaneSz;
  int uvPitch;
};
