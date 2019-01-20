//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#import <Foundation/Foundation.h>

#include "files_impl.h"

std::string cc::files_impl::get_content(const std::string &path) {
    NSString *ns_path = [NSString stringWithUTF8String:path.c_str()];

    NSString *resource = [[ns_path lastPathComponent] stringByDeletingPathExtension];
    NSString *directory = [ns_path stringByDeletingLastPathComponent];
    NSString *type = [ns_path pathExtension];

    NSString *full_path = [[NSBundle mainBundle] pathForResource:resource ofType:type inDirectory:directory];

    NSData *fileData = [NSData dataWithContentsOfFile:full_path];
    return std::string((const char *) fileData.bytes, fileData.length);
}