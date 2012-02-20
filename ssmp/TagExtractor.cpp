#include "TagExtractor.h"
std::vector<std::string> initFF()
{
	std::vector<std::string> v;
	v.push_back("mp3");
	v.push_back("flac");
	v.push_back("m4a");
	v.push_back("ogg");
	return v;
}
const std::vector<std::string> TagExtractor::supportedFileFormats = initFF();

void TagExtractor::extractID3v2Tag(TagLib::ID3v2::Tag* tag, QMap<QString, QString>* tagmap)
{
	//Somehow this means album artist / band. http://www.id3.org/id3v2.4.0-frames
	TagLib::ID3v2::FrameList l = tag->frameList("TPE2");
	if(!l.isEmpty())
		tagmap->insert("albumartist",l.front()->toString().toCString());
}

bool TagExtractor::extractTag(QString fpath, QMap<QString, QString>* stmap, QMap<QString, int>* itmap)
{
	wchar_t wname[250]; //TODO: Dynamic. Need to figure out wchar length from QStr length
	wname[fpath.toWCharArray(wname)] = 0;
	TagLib::FileName fname(wname);
				
	TagLib::File* file = NULL;
	//MP3 or FLAC Means we can check for additional info in ID3v2 tags
	if(fpath.endsWith("mp3"))
	{				
		TagLib::MPEG::File* fr = new TagLib::MPEG::File(fname, true, TagLib::AudioProperties::ReadStyle::Fast);				
		if(fr->ID3v2Tag())
			TagExtractor::extractID3v2Tag(fr->ID3v2Tag(), stmap);
		file = dynamic_cast<TagLib::File*>(fr);
	}
	else if(fpath.endsWith("flac"))
	{
		TagLib::FLAC::File* fr = new TagLib::FLAC::File(fname,true,TagLib::AudioProperties::ReadStyle::Fast);
		if(fr->ID3v2Tag())
			TagExtractor::extractID3v2Tag(fr->ID3v2Tag(), stmap);
		file = dynamic_cast<TagLib::File*>(fr);
	}
	else
		file = TagLib::FileRef::create(fname);
	if(file == NULL)
	{
		qDebug() << "ERR: " + fpath;
		return false;
	}
	//Try to get audio properties
	TagLib::AudioProperties* ap = file->audioProperties();

	TagLib::Tag* genTag = file->tag();			
	stmap->insert("name", genTag->title().toCString());
	stmap->insert("genre", genTag->genre().toCString());
	itmap->insert("year", genTag->year());
	itmap->insert("tracknum", genTag->track());
	stmap->insert("album", genTag->album().toCString());
	stmap->insert("artist", genTag->artist().toCString());
	if(ap != NULL)
		itmap->insert("length", ap->length());
	stmap->insert("path", fpath);	
	delete file;	
	return true;
}

bool TagExtractor::isAudioFile(QFileInfo f)
{
	QString type = f.suffix();
	return std::accumulate(supportedFileFormats.begin(), supportedFileFormats.end(), false,
		[type](bool a, std::string b) {return (a || b == type.toStdString());});
}

TagExtractor::TagExtractor(void)
{	
}

TagExtractor::~TagExtractor(void)
{
}
