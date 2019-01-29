//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <vector>

#include "core/engine.hpp"
#include "core/events.hpp"
#include "core/value.hpp"

#import "base_view_controller.hpp"

@interface base_view_controller ()

@property(strong, nonatomic) EAGLContext *context;

@end

@implementation base_view_controller

@synthesize context = _context;

- (void)viewDidLoad {
    [super viewDidLoad];

    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    GLKView *view = (GLKView *) self.view;
    view.context = self.context;

    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    self.preferredFramesPerSecond = 60;

    [EAGLContext setCurrentContext:self.context];
    [((GLKView *) self.view) bindDrawable];

    //

    float w = self.view.bounds.size.width * self.view.contentScaleFactor;
    float h = self.view.bounds.size.height * self.view.contentScaleFactor;

    cc::engine::i()->set_screen_size({w, h});

    if (!cc::engine::i()->is_running()) {
        cc::engine::i()->run(self.make_scene);
    }
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id <UIViewControllerTransitionCoordinator>)coordinator {
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];

    float w = size.width * self.view.contentScaleFactor;
    float h = size.height * self.view.contentScaleFactor;

    cc::engine::i()->set_screen_size({w, h});
}

- (std::shared_ptr<cc::node>)make_scene {
    return nullptr;
}

- (void)dealloc {
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }

    cc::engine::destroy_instance();

    [super dealloc];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    cc::engine::i()->tick();
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    cc::value points;

    for (UITouch *touch in touches) {
        points.push_back(cc::float2(
                float([touch locationInView:[touch view]].x * self.view.contentScaleFactor),
                float([touch locationInView:[touch view]].y * self.view.contentScaleFactor)
        ));
    }

    cc::events::i()->dispatch_touches_began(points);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    cc::value points;

    for (UITouch *touch in touches) {
        points.push_back(cc::float2(
                float([touch locationInView:[touch view]].x * self.view.contentScaleFactor),
                float([touch locationInView:[touch view]].y * self.view.contentScaleFactor)
        ));
    }

    cc::events::i()->dispatch_touches_moved(points);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    cc::value points;

    for (UITouch *touch in touches) {
        points.push_back(cc::float2(
                float([touch locationInView:[touch view]].x * self.view.contentScaleFactor),
                float([touch locationInView:[touch view]].y * self.view.contentScaleFactor)
        ));
    }

    cc::events::i()->dispatch_touches_ended(points);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    cc::value points;

    for (UITouch *touch in touches) {
        points.push_back(cc::float2(
                float([touch locationInView:[touch view]].x * self.view.contentScaleFactor),
                float([touch locationInView:[touch view]].y * self.view.contentScaleFactor)
        ));
    }

    cc::events::i()->dispatch_touches_cancelled(points);
}

@end
