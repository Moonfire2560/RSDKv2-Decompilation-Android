#ifndef VIDEO_H
#define VIDEO_H

extern int CurrentVideoFrame;
extern int VideoFrameCount;
extern int VideoWidth;
extern int VideoHeight;
extern int VideoSurface;
extern int VideoFilePos;
extern bool VideoPlaying;
// FIX: Persistent FileInfo for RSV video playback. LoadFile2 loads the entire
// RSV file into fileBuffer (RAM) via LoadRSVFile in Sprite.cpp. During playback
// UpdateVideoFrame reads from this RAM buffer — no file handle is ever open.
extern FileInfo VideoFileInfo;

void UpdateVideoFrame();

#endif // !VIDEO_H
