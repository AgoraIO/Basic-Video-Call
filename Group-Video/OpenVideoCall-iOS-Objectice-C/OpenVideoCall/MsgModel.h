//
//  MsgModel.h
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MessageType.h"

@interface MsgModel : NSObject
@property (copy, nonatomic) NSString *message;
@property (assign, nonatomic) CGFloat height;
@property (strong, nonatomic) UIColor *color;
+ (instancetype)modelWithMessage:(NSString *)message type:(MsgType)type;
@end
