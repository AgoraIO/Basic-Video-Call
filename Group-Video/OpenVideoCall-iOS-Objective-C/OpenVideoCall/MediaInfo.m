//
//  MediaInfo.m
//  OpenVideoCall
//
//  Created by CavanSu on 2019/6/11.
//  Copyright © 2019 Agora. All rights reserved.
//

#import "MediaInfo.h"

@implementation MediaInfo
- (NSString *)description {
    return [NSString stringWithFormat:@"%ld x %ld, %ld fps", (NSInteger)self.dimension.width, (NSInteger)self.dimension.height, self.fps];
}

- (instancetype)initWithDimension:(CGSize)dimension fps:(NSInteger)fps {
    if (self = [super init]) {
        self.dimension = dimension;
        self.fps = fps;
    }
    return self;
}
@end
