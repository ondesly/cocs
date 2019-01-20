//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "text_renderer_impl.h"

cc::float2 cc::text_renderer_impl::render(const std::string &value, const float font_size, float max_width, unsigned char *&data) {

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