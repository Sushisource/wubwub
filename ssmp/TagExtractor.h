#ifndef TAGE_H
#define TAGE_H
#define TAGLIB_NO_CONFIG

#include <QObject>
#include <QMap>
#include <QDir>
#include <qdebug.h>
#include <taglib\tag.h>
#include <taglib\fileref.h>
#include <taglib\mpeg\id3v2\id3v2tag.h>
#include <taglib\mpeg\mpegfile.h>
#include <taglib\mp4\mp4file.h>
#include <taglib/ogg/vorbis/vorbisfile.h>
#include <taglib\ogg\oggfile.h>
#include <taglib\flac\flacfile.h>
#include <functional>
#include <numeric>

class TagExtractor
{
public:
	static void extractID3v2Tag(TagLib::ID3v2::Tag* tag, QMap<QString, QString>* tagmap);
	static bool extractTag(QString fpath, QMap<QString, QString>* stmap, QMap<QString, int>* itmap);
    static bool isVaildAudioFile(QFileInfo f);
	TagExtractor(void);
	~TagExtractor(void);
	static const std::vector<std::string> supportedFileFormats;
	static bool loadTagIntoMaps(TagLib::File*, QMap<QString, QString>*, QMap<QString, int>*);
    TagLib::File* createFile(TagLib::FileName fileName, bool readAudioProperties, TagLib::AudioProperties::ReadStyle audioPropertiesStyle);
};
#endif
