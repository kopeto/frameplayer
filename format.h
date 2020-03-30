#pragma once

extern "C"
{
  #include <libavformat/avformat.h>
  #include <libavcodec/avcodec.h>
  #include <libswscale/swscale.h>
}

#include "stream.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class BaseStream;
class Packet;
class Codec;

class Format
{

public:
  Format(const std::string);
  ~Format();
  AVFormatContext *get_context() const;
  const char *get_filename() const;
  unsigned nbStreams() const;
  BaseStream *streams(unsigned idx);
  VideoStream *best_video_stream();
  std::unique_ptr<Packet> read();

private:
  // AVCodecContext  *codecContext;
  AVFormatContext *context=NULL;
  std::vector<std::unique_ptr<BaseStream>> streamsList;
};
