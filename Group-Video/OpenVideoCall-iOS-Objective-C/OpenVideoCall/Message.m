//
//  Message.m
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import "Message.h"

@implementation Message
+ (instancetype)messageWithText:(NSString *)text type:(MessageType)type {
    Message *message = [[Message alloc] init];
    message.text = text;
    message.type = type;
    return message;
}
@end
