#pragma once

extern "C"
{
  #include <libavformat/avformat.h>
  #include <libavcodec/avcodec.h>

}

class Codec
{
public:
  Codec();
  virtual ~Codec() = default;
  void set_decoder(AVCodec*);
  void set_context(AVCodecContext*);
  void set_params(AVCodecParameters*);
private:
  AVCodec*        decoder; //AVCodec* avcodec_find_decoder 	( 	enum AVCodecID  	id	)
  AVCodecContext* context;
  AVCodecParameters* params;
};

class VideoCodec : public Codec
{

};

class AudioCodec : public Codec
{

};
