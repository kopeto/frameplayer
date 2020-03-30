#pragma once

extern "C"
{
  #include <libavformat/avformat.h>
  #include <libavcodec/avcodec.h>
  #include <libswscale/swscale.h>
}

#include "stream.h"
typedef unsigned char Uint8;

class SWScaler
{

public:
  SWScaler(const VideoStream *stream, int dest_width, int dest_height);
  ~SWScaler();
  SwsContext* get_context();
  AVFrame* scale(AVFrame* src_frame);

private:
  SwsContext* context;
  AVPixelFormat pix_format = AV_PIX_FMT_YUV420P;
  int flags = SWS_BICUBIC;
  int src_height;
  int dest_width;
  int dest_height;

  // Pixel formatting variables
  Uint8 *yPlane, *uPlane, *vPlane;
  size_t yPlaneSz, uvPlaneSz;
  int uvPitch;
};
