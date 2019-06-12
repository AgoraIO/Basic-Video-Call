//
//  MediaInfo.h
//  OpenVideoCall
//
//  Created by CavanSu on 2019/6/11.
//  Copyright © 2019 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MediaInfo : NSObject
@property (assign, nonatomic) CGSize dimension;
@property (assign, nonatomic) NSInteger fps;
- (instancetype)initWithDimension:(CGSize)dimension fps:(NSInteger)fps;
@end
