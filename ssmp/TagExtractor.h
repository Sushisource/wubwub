#ifndef TAGE_H
#define TAGE_H
#include <QObject>
#include <QMap>
#include <QDir>
#include <qdebug.h>
#include <taglib\tag.h>
#include <taglib\fileref.h>
#include <id3v2tag.h>
#include <taglib\mpeg\mpegfile.h>
#include <taglib\mp4\mp4file.h>
#include <taglib\ogg\oggfile.h>
#include <functional>
#include <numeric>
#include <taglib\flac\flacfile.h>

class TagExtractor
{
public:
	static void extractID3v2Tag(TagLib::ID3v2::Tag* tag, QMap<QString, QString>* tagmap);
	static bool extractTag(QString fpath, QMap<QString, QString>* stmap, QMap<QString, int>* itmap);
	static bool isAudioFile(QFileInfo f);
	TagExtractor(void);
	~TagExtractor(void);
	static const std::vector<std::string> supportedFileFormats;
	static bool loadTagIntoMaps(TagLib::File*, QMap<QString, QString>*, QMap<QString, int>*);
};
#endif
