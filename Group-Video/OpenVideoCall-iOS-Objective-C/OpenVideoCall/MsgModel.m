//
//  MsgModel.m
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import "MsgModel.h"

@implementation MsgModel

+ (instancetype)modelWithMessage:(NSString *)message type:(MsgType)type {
    
    MsgModel *model = [[MsgModel alloc] init];
    model.message = message;
    if (type == MsgTypeChat) {
        model.color = [UIColor colorWithRed:0 green:0 blue:1 alpha:0.5];
    }
    else {
        model.color = [UIColor colorWithRed:1 green:0.5 blue:0 alpha:0.5];
    }
    
    return model;
}

@end
