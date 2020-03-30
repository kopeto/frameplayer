#pragma once

extern "C"
{
  #include <libavformat/avformat.h>
  #include <libavcodec/avcodec.h>

}

class Packet
{
  friend class Format;
public:
  AVPacket get_packet();
  AVPacket packet;
};
