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
    TagLib::PropertyMap props = tag->properties();
    if(!props.isEmpty())
    {
        if(props.contains("ALBUMARTIST"))
            tagmap->insert("albumartist", props["ALBUMARTIST"].toString().toCString());
    }
    else
    {
        //Fallback on this old way just in case
        //Somehow this means album artist / band. http://www.id3.org/id3v2.4.0-frames
        TagLib::ID3v2::FrameList l = tag->frameListMap()["TPE2"];
        if(!l.isEmpty())
            tagmap->insert("albumartist",l.front()->toString().toCString());
        else //Fallback on artist name
            tagmap->insert("albumartist",tag->artist().toCString());
    }
}

bool TagExtractor::extractTag(QString fpath, QMap<QString, QString>* stmap, QMap<QString, int>* itmap)
{
	QByteArray qb = fpath.toLocal8Bit();
	const char *wname = qb.data();
	TagLib::FileName fname(wname);

	bool suc = false;
					 
	//MP3 or FLAC Means we can check for additional info in ID3v2 tags
	if(fpath.endsWith("mp3"))
	{				
        TagLib::MPEG::File fr(fname, true, TagLib::AudioProperties::Fast);
        if(fr.ID3v2Tag())
			extractID3v2Tag(fr.ID3v2Tag(), stmap);
		suc = loadTagIntoMaps(&fr, stmap, itmap);		
	}
	else if(fpath.endsWith("flac"))
	{
		TagLib::FLAC::File fr(fname,true,TagLib::AudioProperties::Fast);
		if(fr.ID3v2Tag())
			extractID3v2Tag(fr.ID3v2Tag(), stmap);
		suc = loadTagIntoMaps(&fr, stmap, itmap);		
	}
	else
	{
        TagLib::File* file = TagLib::FileRef::create(fname);
		suc = loadTagIntoMaps(file, stmap, itmap);		
	}

	if(suc)
		stmap->insert("path", fpath);	
	return suc;
}

bool TagExtractor::loadTagIntoMaps(TagLib::File* file, QMap<QString, QString>* stmap, QMap<QString, int>* itmap)
{
	if(file == NULL)	
		return false;	
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
    return true;
}

bool TagExtractor::isVaildAudioFile(QFileInfo f)
{
    QString type = f.suffix();
    bool isaudio = std::accumulate(supportedFileFormats.begin(), supportedFileFormats.end(), false,
                                   [type](bool a, std::string b) {return (a || b == type.toStdString());});
    return isaudio;
}

TagExtractor::TagExtractor(void)
{	
}

TagExtractor::~TagExtractor(void)
{
}
