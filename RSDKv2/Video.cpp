#include "RetroEngine.hpp"

int CurrentVideoFrame = 0;
int VideoFrameCount   = 0;
int VideoWidth        = 0;
int VideoHeight       = 0;
int VideoSurface      = 0;
int VideoFilePos      = 0;
bool VideoPlaying     = false;

// FIX: Dedicated FileInfo for RSV video playback. LoadFile2 loads the entire
// RSV file into fileBuffer (malloc'd RAM) at startup. UpdateVideoFrame reads
// frame header data via FileRead2(..., true) from RAM, then sets VideoMemBuffer
// so FillFileBuffer in Reader.hpp feeds the GIF decoder from RAM as well.
// No file handle is ever open during playback, eliminating all file handle
// races that caused intermittent corruption on Adreno 650 and Mali-G52 GPUs.
FileInfo VideoFileInfo;

void UpdateVideoFrame() {
    if (VideoPlaying) {
        if (VideoFrameCount <= CurrentVideoFrame) {
            VideoPlaying = false;
            // Free the RSV memory buffer now that playback is complete
            if (VideoFileInfo.fileBuffer) {
                free(VideoFileInfo.fileBuffer);
                VideoFileInfo.fileBuffer = nullptr;
            }
            // FIX: Clear VideoMemBuffer so FillFileBuffer doesn't read from the
            // now-freed fileBuffer. If left set, any subsequent FillFileBuffer call
            // (e.g. from LoadMusic's ov_open_callbacks) reads dangling memory,
            // causing ov_open_callbacks to fail and music to never load.
            VideoMemBuffer     = nullptr;
            VideoMemBufferSize = 0;
            VideoMemBufferPos  = 0;
        } else {
            GFXSurface *surface = &GfxSurface[VideoSurface];
            byte fileBuffer     = 0;
            ushort fileBuffer2  = 0;

            // Read frame header data from RAM via FileRead2(..., true)
            FileRead2(&VideoFileInfo, &fileBuffer, 1, true);
            VideoFilePos += fileBuffer;
            FileRead2(&VideoFileInfo, &fileBuffer, 1, true);
            VideoFilePos += fileBuffer << 8;
            FileRead2(&VideoFileInfo, &fileBuffer, 1, true);
            VideoFilePos += fileBuffer << 16;
            FileRead2(&VideoFileInfo, &fileBuffer, 1, true);
            VideoFilePos += fileBuffer << 24;

            byte clr[3];
            for (int i = 0; i < 0x80; ++i) {
                FileRead2(&VideoFileInfo, &clr, 3, true);
                SetPaletteEntry(i, clr[0], clr[1], clr[2]);
            }
            SetPaletteEntry(0, 0x00, 0x00, 0x00);

            FileRead2(&VideoFileInfo, &fileBuffer, 1, true);
            while (fileBuffer != ',') FileRead2(&VideoFileInfo, &fileBuffer, 1, true);

            FileRead2(&VideoFileInfo, &fileBuffer2, 2, true); // IMAGE LEFT
            FileRead2(&VideoFileInfo, &fileBuffer2, 2, true); // IMAGE TOP
            FileRead2(&VideoFileInfo, &fileBuffer2, 2, true); // IMAGE WIDTH
            FileRead2(&VideoFileInfo, &fileBuffer2, 2, true); // IMAGE HEIGHT
            FileRead2(&VideoFileInfo, &fileBuffer, 1, true);  // PaletteType
            bool interlaced = (fileBuffer & 0x40) >> 6;
            if (fileBuffer >> 7 == 1) {
                int c = 0x80;
                do {
                    ++c;
                    FileRead2(&VideoFileInfo, &fileBuffer, 1, true);
                    FileRead2(&VideoFileInfo, &fileBuffer, 1, true);
                    FileRead2(&VideoFileInfo, &fileBuffer, 1, true);
                } while (c != 0x100);
            }

            // Point FillFileBuffer at our RAM buffer so ReadGifPictureData and
            // its internal ReadGifCode calls read from memory with no file handle.
            // VideoMemBufferPos tracks consumption within the remaining frame data.
            // The frame ends at VideoFilePos so remaining = VideoFilePos - readPos.
            int frameRemaining        = VideoFilePos - (int)VideoFileInfo.readPos;
            VideoMemBuffer            = VideoFileInfo.fileBuffer + VideoFileInfo.readPos;
            VideoMemBufferSize        = frameRemaining;
            VideoMemBufferPos         = 0;
            // Set global file state for ReadGifPictureData's FileRead calls
            FileSize       = frameRemaining;
            ReadPos        = 0;
            ReadSize       = 0;
            BufferPosition = 0;
            CFileHandle    = nullptr; // FillFileBuffer won't use this when VideoMemBuffer is set

            // fileBuffer was XOR-decrypted in LoadRSVFile so it contains plain bytes.
            // FileRead applies ^ 0xFF when Engine.UseBinFile=true, which would
            // double-decrypt and corrupt the GIF data. Set UseBinFile=false so
            // FileRead passes bytes through without XOR while VideoMemBuffer is active.
            bool savedUseBinFile = Engine.UseBinFile;
            Engine.UseBinFile    = false;

            ReadGifPictureData(surface->width, surface->height, interlaced, GraphicData, surface->dataPosition);

            // Restore Engine.UseBinFile and clear VideoMemBuffer
            Engine.UseBinFile  = savedUseBinFile;
            VideoMemBuffer     = nullptr;
            VideoMemBufferSize = 0;
            VideoMemBufferPos  = 0;

            // Advance to next frame in our RAM buffer
            VideoFileInfo.readPos = VideoFilePos;

            ++CurrentVideoFrame;
        }
    }
}
