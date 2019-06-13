//
//  Message.h
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef NS_ENUM(int, MessageType) {
    MessageTypeInfo = 0,
    MessageTypeError = 1
};

@interface Message: NSObject
@property (assign, nonatomic) MessageType type;
@property (copy, nonatomic) NSString *text;

+ (instancetype)messageWithText:(NSString *)text type:(MessageType)type;
@end
