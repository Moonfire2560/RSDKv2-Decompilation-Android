#ifndef AUDIO_H
#define AUDIO_H

#include <stdlib.h>

#include <vorbis/vorbisfile.h>

#if RETRO_PLATFORM != RETRO_VITA
#include "SDL.h"
#endif

#define TRACK_COUNT (0x10)
#define SFX_COUNT (0x100)
#define CHANNEL_COUNT (0x4)
#define SFXDATA_COUNT (0x400000)

#define MAX_VOLUME (100)

struct TrackInfo {
    char fileName[0x40];
    bool trackLoop;
};

struct MusicPlaybackInfo {
    OggVorbis_File vorbisFile;
    int vorbBitstream;
#if RETRO_USING_SDL1
    SDL_AudioSpec spec;
#endif
#if RETRO_USING_SDL2
    SDL_AudioStream *stream;
#endif
    Sint16 *buffer;
    FileInfo fileInfo;
    bool trackLoop;
    bool loaded;
};

struct SFXInfo {
    char name[0x40];
    Sint16 *buffer;
    size_t length;
    bool loaded;
};

struct ChannelInfo {
    size_t sampleLength;
    Sint16 *samplePtr;
    int sfxID;
    byte loopSFX;
    sbyte pan;
};

enum MusicStatuses {
    MUSIC_STOPPED = 0,
    MUSIC_PLAYING = 1,
    MUSIC_PAUSED  = 2,
    MUSIC_LOADING = 3,
    MUSIC_READY   = 4,
};

extern int NoGlobalSFX;
extern int NoStageSFX;

extern int MusicVolume;
extern int CurrentMusicTrack;
extern int sfxVolume;
extern int bgmVolume;
extern bool audioEnabled;
extern bool musicLoading; // FIX: true while a LoadMusic thread is running

extern int nextChannelPos;
extern bool musicEnabled;
extern int musicStatus;
extern TrackInfo musicTracks[TRACK_COUNT];
extern SFXInfo sfxList[SFX_COUNT];

extern ChannelInfo sfxChannels[CHANNEL_COUNT];

extern MusicPlaybackInfo musInfo;

#if RETRO_USING_SDL1 || RETRO_USING_SDL2
extern SDL_AudioSpec audioDeviceFormat;
extern SDL_AudioDeviceID audioDevice;

// FIX: Use SDL_LockAudioDevice/SDL_UnlockAudioDevice with the actual device ID.
// SDL_LockAudio() only locks device 1, but SDL_OpenAudioDevice may return a
// higher ID (common on UNISOC/Mali devices). Using the wrong lock causes
// freeMusInfo and LoadMusic to race, preventing music from restarting after
// StopMusic is called (e.g. re-entering dev menu). Defined here so Audio.hpp
// inlines (freeMusInfo) can use them without depending on Audio.cpp's macros.
#define LockAudioDevice()   SDL_LockAudioDevice(audioDevice)
#define UnlockAudioDevice() SDL_UnlockAudioDevice(audioDevice)
#else
#define LockAudioDevice()   ;
#define UnlockAudioDevice() ;
#endif

int InitSoundDevice();
void LoadGlobalSfx();

#if RETRO_USING_SDL1 || RETRO_USING_SDL2
void ProcessMusicStream(void *data, Sint16 *stream, int len);
void ProcessAudioPlayback(void *data, Uint8 *stream, int len);
void ProcessAudioMixing(Sint32 *dst, const Sint16 *src, int len, int volume, sbyte pan);


inline void freeMusInfo()
{
    if (musInfo.loaded) {
        // FIX: Use LockAudioDevice/UnlockAudioDevice (SDL_LockAudioDevice with
        // the correct device ID) instead of SDL_LockAudio/SDL_UnlockAudio.
        // On UNISOC and other devices where SDL_OpenAudioDevice returns ID > 1,
        // SDL_LockAudio locks the wrong device and the cleanup races with the
        // audio callback, leaving musInfo in a bad state that prevents music
        // from restarting after StopMusic (e.g. re-entering dev menu).
        LockAudioDevice();

        if (musInfo.buffer)
            delete[] musInfo.buffer;
#if RETRO_USING_SDL2
        if (musInfo.stream)
            SDL_FreeAudioStream(musInfo.stream);
#endif
        ov_clear(&musInfo.vorbisFile);
        musInfo.buffer       = nullptr;
#if RETRO_USING_SDL2
        musInfo.stream = nullptr;
#endif
        musInfo.trackLoop    = false;
        musInfo.loaded       = false;
        musicStatus          = MUSIC_STOPPED;

        UnlockAudioDevice();
    }
}
#else
void ProcessMusicStream() {}
void ProcessAudioPlayback() {}
void ProcessAudioMixing() {}

inline void freeMusInfo()
{
    if (musInfo.loaded) {
        LockAudioDevice(); // FIX: use correct device ID, not SDL_LockAudio

        if (musInfo.buffer)
            delete[] musInfo.buffer;
        ov_clear(&musInfo.vorbisFile);
        musInfo.buffer    = nullptr;
        musInfo.trackLoop = false;
        musInfo.loaded    = false;
        musicStatus       = MUSIC_STOPPED;

        UnlockAudioDevice(); // FIX: use correct device ID, not SDL_UnlockAudio
    }
}
#endif

void LoadMusic(void *userdata);
void SetMusicTrack(char *filePath, byte trackID, bool loop);
bool PlayMusic(int track);
inline void StopMusic()
{
    musicStatus = MUSIC_STOPPED;
    freeMusInfo();
}

void LoadSfx(char *filePath, byte sfxID);
void PlaySfx(int sfx, bool loop);
inline void StopSfx(int sfx)
{
    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        if (sfxChannels[i].sfxID == sfx) {
            MEM_ZERO(sfxChannels[i]);
            sfxChannels[i].sfxID = -1;
        }
    }
}
void SetSfxAttributes(int sfx, int loopCount, sbyte pan);

inline void SetMusicVolume(int volume)
{
    if (volume < 0)
        volume = 0;
    if (volume > MAX_VOLUME)
        volume = MAX_VOLUME;
    MusicVolume = volume;
}

inline void PauseSound()
{
    if (musicStatus == MUSIC_PLAYING)
        musicStatus = MUSIC_PAUSED;
}

inline void ResumeSound()
{
    if (musicStatus == MUSIC_PAUSED)
        musicStatus = MUSIC_PLAYING;
}


inline void StopAllSfx()
{
    for (int i = 0; i < CHANNEL_COUNT; ++i) sfxChannels[i].sfxID = -1;
}
inline void ReleaseGlobalSfx()
{
    StopAllSfx();
    for (int i = NoGlobalSFX - 1; i >= 0; --i) {
        if (sfxList[i].loaded) {
            StrCopy(sfxList[i].name, "");
            free(sfxList[i].buffer);
            sfxList[i].length = 0;
            sfxList[i].loaded = false;
        }
    }
    NoGlobalSFX = 0;
}
inline void ReleaseStageSfx()
{
    for (int i = NoStageSFX + NoGlobalSFX; i >= NoGlobalSFX; --i) {
        if (sfxList[i].loaded) {
            StrCopy(sfxList[i].name, "");
            free(sfxList[i].buffer);
            sfxList[i].length = 0;
            sfxList[i].loaded = false;
        }
    }
    NoStageSFX = 0;
}

inline void ReleaseSoundDevice()
{
    StopMusic();
    StopAllSfx();
    ReleaseStageSfx();
    ReleaseGlobalSfx();
}

#endif // !AUDIO_H
