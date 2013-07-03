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
    if(!props.isEmpty() && props.contains("ALBUMARTIST"))
        tagmap->insert("albumartist", props["ALBUMARTIST"].toString().toCString(true));
    else //Fallback on artist name
        tagmap->insert("albumartist",tag->artist().toCString(true));
}

bool TagExtractor::extractTag(QString fpath, QMap<QString, QString>* stmap, QMap<QString, int>* itmap)
{
#ifdef Q_OS_WIN
    TagLib::FileName fname((wchar_t *)fpath.constData());
#else
    TagLib::FileName fname(QFile::encodeName(fpath).constData());
#endif

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
        delete file;
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
    stmap->insert("name", genTag->title().toCString(true));
    stmap->insert("genre", genTag->genre().toCString(true));
	itmap->insert("year", genTag->year());
	itmap->insert("tracknum", genTag->track());
    stmap->insert("album", genTag->album().toCString(true));
    stmap->insert("artist", genTag->artist().toCString(true));
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
