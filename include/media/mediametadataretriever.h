/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef MEDIAMETADATARETRIEVER_H
#define MEDIAMETADATARETRIEVER_H

#include <utils/Errors.h>  // for status_t
#include <utils/threads.h>
#include <binder/IMemory.h>
#include <media/IMediaMetadataRetriever.h>

namespace android {

class IMediaPlayerService;
class IMediaMetadataRetriever;

// Keep these in synch with the constants defined in MediaMetadataRetriever.java
// class.
enum {
    METADATA_KEY_CD_TRACK_NUMBER = 0,
    METADATA_KEY_ALBUM           = 1,
    METADATA_KEY_ARTIST          = 2,
    METADATA_KEY_AUTHOR          = 3,
    METADATA_KEY_COMPOSER        = 4,
    METADATA_KEY_DATE            = 5,
    METADATA_KEY_GENRE           = 6,
    METADATA_KEY_TITLE           = 7,
    METADATA_KEY_YEAR            = 8,
    METADATA_KEY_DURATION        = 9,
    METADATA_KEY_NUM_TRACKS      = 10,
    METADATA_KEY_IS_DRM_CRIPPLED = 11,
    METADATA_KEY_CODEC           = 12,
    METADATA_KEY_RATING          = 13,
    METADATA_KEY_COMMENT         = 14,
    METADATA_KEY_COPYRIGHT       = 15,
    METADATA_KEY_BIT_RATE        = 16,
    METADATA_KEY_FRAME_RATE      = 17,
    METADATA_KEY_VIDEO_FORMAT    = 18,
    METADATA_KEY_VIDEO_HEIGHT    = 19,
    METADATA_KEY_VIDEO_WIDTH     = 20,
    METADATA_KEY_WRITER          = 21,
    METADATA_KEY_MIMETYPE        = 22,
    METADATA_KEY_DISC_NUMBER     = 23,
    METADATA_KEY_ALBUMARTIST     = 24,
    METADATA_KEY_COMPILATION     = 25,
    // Add more here...
};

// The intended mode of operations:$
// METADATA_MODE_NOOP: Experimental - just add and remove data source.$
// METADATA_MODE_FRAME_CAPTURE_ONLY: For capture frame/thumbnail only.$
// METADATA_MODE_METADATA_RETRIEVAL_ONLY: For meta data retrieval only.$
// METADATA_MODE_FRAME_CAPTURE_AND_METADATA_RETRIEVAL: For both frame capture
//   and meta data retrieval.$
enum {
    METADATA_MODE_NOOP                                 = 0x00,
    METADATA_MODE_METADATA_RETRIEVAL_ONLY              = 0x01,
    METADATA_MODE_FRAME_CAPTURE_ONLY                   = 0x02,
    METADATA_MODE_FRAME_CAPTURE_AND_METADATA_RETRIEVAL = 0x03
};

class MediaMetadataRetriever: public RefBase
{
public:
    MediaMetadataRetriever();
    ~MediaMetadataRetriever();
    void disconnect();
    status_t setDataSource(const char* dataSourceUrl);
    status_t setDataSource(int fd, int64_t offset, int64_t length);
    status_t setMode(int mode);
    status_t getMode(int* mode);
    sp<IMemory> captureFrame();
    sp<IMemory> extractAlbumArt();
    const char* extractMetadata(int keyCode);

private:
    static const sp<IMediaPlayerService>& getService();

    class DeathNotifier: public IBinder::DeathRecipient
    {
    public:
        DeathNotifier() {}
        virtual ~DeathNotifier();
        virtual void binderDied(const wp<IBinder>& who);
    };

    static sp<DeathNotifier>                  sDeathNotifier;
    static Mutex                              sServiceLock;
    static sp<IMediaPlayerService>            sService;

    Mutex                                     mLock;
    sp<IMediaMetadataRetriever>               mRetriever;

};

}; // namespace android

#endif // MEDIAMETADATARETRIEVER_H
