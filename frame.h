#pragma once

extern "C"
{
  #include <libavformat/avformat.h>
  #include <libavutil/rational.h>
  #include <libavcodec/codec.h>
}

#include <memory>
#include <vector>

class Frame
{
private:
  AVFrame* frame;
}
