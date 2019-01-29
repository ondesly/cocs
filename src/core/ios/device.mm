//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <unordered_map>
#include <vector>

#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

#include "../device.hpp"

namespace {

    AVAudioPlayer *m_audio_player = nullptr;

    AVAudioEngine *m_audio_engine = nullptr;
    std::unordered_map<std::string, std::pair<AVAudioPlayerNode *, AVAudioFile *>> m_players;

}

cc::device::device() {

}

cc::device::~device() {
    clear_music();
    clear_sounds();
}

void cc::device::load_music(const std::string &path) {
    clear_music();

    NSString *ns_path = [NSString stringWithUTF8String:path.c_str()];
    NSURL *url = [[NSBundle mainBundle] URLForResource:ns_path withExtension:@"mp3"];

    m_audio_player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:nil];
    [m_audio_player prepareToPlay];
}

void cc::device::play_music() {
    if (m_audio_player) {
        [m_audio_player play];
    }
}

void cc::device::pause_music() {
    if (m_audio_player) {
        [m_audio_player pause];
    }
}

void cc::device::stop_music() {
    if (m_audio_player) {
        [m_audio_player stop];
    }
}

void cc::device::clear_music() {
    if (m_audio_player) {
        [m_audio_player stop];
        [m_audio_player release];

        m_audio_player = nullptr;
    }
}

void cc::device::load_sounds(const std::vector<std::string> &paths) {
    clear_sounds();

    m_audio_engine = [[AVAudioEngine alloc] init];

    for (auto &path : paths) {
        auto player = [[AVAudioPlayerNode alloc] init];
        [m_audio_engine attachNode:player];

        NSString *ns_path = [NSString stringWithUTF8String:path.c_str()];
        NSURL *url = [[NSBundle mainBundle] URLForResource:ns_path withExtension:@"mp3"];
        auto file = [[AVAudioFile alloc] initForReading:url error:nil];

        auto mixer = [m_audio_engine mainMixerNode];
        [m_audio_engine connect:player to:mixer format:file.processingFormat];

        m_players[path] = std::make_pair(player, file);
    }

    [m_audio_engine prepare];

    if (!m_audio_engine.isRunning) {
        [m_audio_engine startAndReturnError:nil];
    }
}

void cc::device::play_sound(const std::string &path) {
    auto it = m_players.find(path);
    if (it != m_players.end()) {
        auto player = (*it).second.first;
        auto file = (*it).second.second;

        [player scheduleFile:file atTime:nil completionHandler:nil];
        [player play];
    }
}

void cc::device::clear_sounds() {
    if (m_audio_engine) {
        [m_audio_engine stop];

        for (auto &pair : m_players) {
            auto player = pair.second.first;
            auto file = pair.second.second;

            [m_audio_engine detachNode:player];

            [file release];
            [player release];
        }

        [m_audio_engine release];
        m_audio_engine = nullptr;
    }
}

std::string cc::device::get_file_content(const std::string &path) {
    NSString *ns_path = [NSString stringWithUTF8String:path.c_str()];

    NSString *resource = [[ns_path lastPathComponent] stringByDeletingPathExtension];
    NSString *directory = [ns_path stringByDeletingLastPathComponent];
    NSString *type = [ns_path pathExtension];

    NSString *full_path = [[NSBundle mainBundle] pathForResource:resource ofType:type inDirectory:directory];

    NSData *fileData = [NSData dataWithContentsOfFile:full_path];
    return std::string((const char *) fileData.bytes, fileData.length);
}

cc::float2 cc::device::render_text(const std::string &value, const float font_size, float max_width, unsigned char *&data) {
    // Color space

    auto color_space = CGColorSpaceCreateDeviceRGB();

    if (!color_space) {
        return {0.f, 0.f};
    }

    // String

    auto ns_string = [NSString stringWithUTF8String:value.c_str()];

    NSMutableParagraphStyle *paragraph_style = [[[NSMutableParagraphStyle alloc] init] autorelease];
    [paragraph_style setAlignment:NSTextAlignmentCenter];
    auto attributes = @{NSFontAttributeName: [UIFont systemFontOfSize:font_size],
            NSForegroundColorAttributeName: [UIColor whiteColor],
            NSBackgroundColorAttributeName: [UIColor clearColor],
            NSParagraphStyleAttributeName: paragraph_style};

    auto rect = [ns_string boundingRectWithSize:CGSizeMake(max_width, 0.f) options:NSStringDrawingUsesLineFragmentOrigin attributes:attributes context:nil];

    // Context

    auto width = size_t(ceil(rect.size.width));
    auto height = size_t(ceil(rect.size.height));

    auto length = width * height * 4;
    data = new unsigned char[length];
    std::fill(data, data + length, '\0');

    auto context = CGBitmapContextCreate(data, width, height, 8, width * 4, color_space, kCGImageAlphaPremultipliedLast);

    if (!context) {
        return {0.f, 0.f};
    }

    // Draw

    UIGraphicsPushContext(context);
    CGContextTranslateCTM(context, 0.f, rect.size.height);
    CGContextScaleCTM(context, 1.f, -1.f);

    [ns_string drawInRect:CGRectMake(0.f, 0.f, rect.size.width, rect.size.height) withAttributes:attributes];

    UIGraphicsPopContext();

    // Cleanup

    CGColorSpaceRelease(color_space);
    CGContextRelease(context);

    return float2(width, height);
}
