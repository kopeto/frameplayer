#include "format.h"
#include "stream.h"
#include "packet.h"
#include <sstream>

Format::~Format()
{
  if(context)
    avformat_free_context(context);
}

Format::Format(const std::string filename)
{
  av_register_all();
  auto res = avformat_open_input(&context, filename.c_str(), NULL, NULL);
  if(res<0)
  {
    std::ostringstream strm;
    strm << "Error open file \""<< filename <<"\" error code "<<std::endl;
    throw std::runtime_error(strm.str());
  }

  avformat_find_stream_info(context,NULL);

  // FILL STREAM LIST
  for(auto i = 0u; i<context->nb_streams; i++)
  {
    switch(context->streams[i]->codecpar->codec_type)
    {
      case AVMEDIA_TYPE_VIDEO:
        streamsList.push_back(std::make_unique<VideoStream>(context->streams[i]));
        break;
      case AVMEDIA_TYPE_AUDIO:
        streamsList.push_back(std::make_unique<AudioStream>(context->streams[i]));
        break;
      default:
        streamsList.push_back(std::make_unique<BaseStream>(context->streams[i]));
        break;
    }
  }
}

AVFormatContext *Format::get_context() const
{
  return context;
}

const char *Format::get_filename() const
{
  return context->filename;
}


BaseStream *Format::streams(unsigned idx)
{
  return streamsList[idx].get();
}

VideoStream *Format::best_video_stream()
{
  int stream_index = av_find_best_stream 	(
    context,
  	AVMEDIA_TYPE_VIDEO,
  	-1,
  	-1,
  	NULL,
  	0
  );
  if(stream_index>=0)
  {
    return dynamic_cast<VideoStream*>(streams(stream_index));
  }
  else
  {
    return NULL;
  }
}
// av_find_best_stream 	( 	AVFormatContext *  	ic,
// 		enum AVMediaType  	type,
// 		int  	wanted_stream_nb,
// 		int  	related_stream,
// 		AVCodec **  	decoder_ret,
// 		int  	flags
// 	)


unsigned Format::nbStreams() const
{
  return context->nb_streams;
}

std::unique_ptr<Packet> Format::read()
{
  auto res = std::make_unique<Packet>();
  auto err = av_read_frame(context, &res->packet);
  if(err == AVERROR_EOF)
  {
    return NULL;
  }
  else if(err<0)
  {
    std::ostringstream strm;
    strm << "Error read frame error code "<< err << std::endl;
    throw std::runtime_error(strm.str());
  }
  return res;
}
