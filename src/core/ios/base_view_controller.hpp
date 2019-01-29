//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface base_view_controller : GLKViewController

- (std::shared_ptr<cc::node>)make_scene;

@end