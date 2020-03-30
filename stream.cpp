#include "stream.h"
#include <sstream>

BaseStream::BaseStream(AVStream *stream): stream(stream), codec_params(stream->codecpar)
{
  codec =  avcodec_find_decoder(codec_params->codec_id);
  codec_context = avcodec_alloc_context3(codec);
  avcodec_parameters_to_context(codec_context, codec_params);
}

BaseStream::~BaseStream()
{
  if(codec_context)
    avcodec_free_context(&codec_context);
}

void BaseStream::open_codec()
{
  int ret = avcodec_open2(codec_context, codec, NULL);
  if(ret<0)
  {
    std::ostringstream strm;
    strm << "Error open codec \""<< codec->long_name <<"\" error code "<<ret<<std::endl;
    throw std::runtime_error(strm.str());
  }
}


int64_t BaseStream::duration() const
{
  return stream->duration;
}

double BaseStream::real_duration() const
{
  return (double)stream->duration*av_q2d(stream->time_base);
}

int64_t BaseStream::nb_frames() const
{
  return stream->nb_frames;
}

AVRational BaseStream::time_base() const
{
  return stream->time_base;
}

//+============================
// Get BASIC AV struct pointers
//+============================
AVStream* BaseStream::get_stream() const
{
  return stream;
}
AVCodec*  BaseStream::get_codec() const
{
  return  codec;
}
AVCodecContext* BaseStream::get_codec_context() const
{
  return  codec_context;
}
AVCodecParameters* BaseStream::get_codec_params() const
{
  return  codec_params;
}
//+============================
