//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#import <AVFoundation/AVFoundation.h>

#include "audio_impl.h"

cc::audio_impl::~audio_impl() {
    for (auto &pair : m_players) {
        [pair.second stop];
        [pair.second release];
    }
}

void cc::audio_impl::preload(const std::vector<std::string> &paths) {
    for (auto &path : paths) {
        NSString *ns_path = [NSString stringWithUTF8String:path.c_str()];
        NSURL *url = [[NSBundle mainBundle] URLForResource:ns_path withExtension:@"mp3"];

        auto player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:nil];
        [player prepareToPlay];

        m_players[path] = player;
    }

}

void cc::audio_impl::play(const std::string &path) {
    auto it = m_players.find(path);
    if (it != m_players.end()) {
        [(*it).second play];
    }
}

void cc::audio_impl::stop(const std::string &path) {
    auto it = m_players.find(path);
    if (it != m_players.end() && [(*it).second isPlaying]) {
        [(*it).second stop];
    }
}