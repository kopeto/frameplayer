#pragma once

extern "C"
{
  #include <libavformat/avformat.h>
  #include <libavutil/rational.h>
}

#include <memory>
#include <vector>
#include <iostream>
#include <string>

class Packet;
class Frame;
class Codec;

class BaseStream
{
public:
  //friend class Format;
  BaseStream(AVStream *stream);
  virtual ~BaseStream();

  AVStream* get_stream() const;
  int64_t duration() const;
  int64_t nb_frames() const;
  AVRational time_base() const;

  double real_duration() const;

  void open_codec(AVDictionary *dict = NULL);

  //Both Same thing
  Packet *packets(unsigned idx);
  //std::vector<Packet*> packetPtrList;
  std::vector<Frame *> framePtrList;

  AVCodec*            get_codec() const;
  AVCodecContext*     get_codec_context() const;
  AVCodecParameters*  get_codec_params() const;

private:
  AVStream*           stream;
  AVCodec*            codec; //AVCodec* avcodec_find_decoder 	( 	enum AVCodecID  	id	)
  AVCodecContext*     codec_context;
  AVCodecParameters*  codec_params;
};

class AudioStream: public BaseStream
{
  using BaseStream::BaseStream;
};

class VideoStream: public BaseStream
{
  using BaseStream::BaseStream;
};
