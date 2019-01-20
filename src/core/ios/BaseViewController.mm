//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <vector>

#include "core/engine.h"
#include "core/ios/files_impl.h"
#include "core/ios/audio_impl.h"
#include "core/touches.h"
#include "math/float2.h"
#include "textures/ios/text_renderer_impl.h"

#import "BaseViewController.h"

@interface BaseViewController ()

@property(strong, nonatomic) EAGLContext *context;

@end

@implementation BaseViewController

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

    new cc::engine({w, h}, new cc::files_impl, new cc::text_renderer_impl, new cc::audio_impl);
}

- (void)dealloc {
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }

    [super dealloc];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    cc::engine::i()->tick();
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    std::vector<cc::float2> points;

    for (UITouch *touch in touches) {
        points.push_back({
                float([touch locationInView:[touch view]].x * self.view.contentScaleFactor),
                float([touch locationInView:[touch view]].y * self.view.contentScaleFactor)
        });
    }

    cc::engine::i()->get_touches()->dispatch_touches_began(points);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    std::vector<cc::float2> points;

    for (UITouch *touch in touches) {
        points.push_back({
                float([touch locationInView:[touch view]].x * self.view.contentScaleFactor),
                float([touch locationInView:[touch view]].y * self.view.contentScaleFactor)
        });
    }

    cc::engine::i()->get_touches()->dispatch_touches_moved(points);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    std::vector<cc::float2> points;

    for (UITouch *touch in touches) {
        points.push_back({
                float([touch locationInView:[touch view]].x * self.view.contentScaleFactor),
                float([touch locationInView:[touch view]].y * self.view.contentScaleFactor)
        });
    }

    cc::engine::i()->get_touches()->dispatch_touches_ended(points);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    std::vector<cc::float2> points;

    for (UITouch *touch in touches) {
        points.push_back({
                float([touch locationInView:[touch view]].x * self.view.contentScaleFactor),
                float([touch locationInView:[touch view]].y * self.view.contentScaleFactor)
        });
    }

    cc::engine::i()->get_touches()->dispatch_touches_cancelled(points);
}

@end
